#include <algorithm>
#include <functional>
#include <thread>
#include "system_state.hpp"
#include "opengl_wrapper.hpp"
#include "window.hpp"
#include "blake2.h"
#include "gui_element_base.hpp"
#include "gui_deserialize.hpp"
#include "user_interactions.hpp"
#include "game_scene.hpp"
#include "alice_ui.hpp"
#include "parsers.hpp"

namespace sys {


//
// window event functions
//

void state::on_rbutton_down(int32_t x, int32_t y, key_modifiers mod) {
	game_scene::on_rbutton_down(*this, x, y, mod);
}

void state::on_mbutton_down(int32_t x, int32_t y, key_modifiers mod) {
	// Lose focus on text
	ui_state.set_focus_target(*this, nullptr);
}

void state::on_lbutton_down(int32_t x, int32_t y, key_modifiers mod) {
	if(ui_state.current_drag_and_drop_data_type != ui::drag_and_drop_data::none) {
		if(!current_scene.get_root)
			return;

		auto root = current_scene.get_root(*this);

		auto qresult = root->impl_drag_and_drop_query(*this, int32_t(x / user_settings.ui_scale), int32_t(y / user_settings.ui_scale), ui_state.current_drag_and_drop_data_type);
		if(qresult.under_mouse) {
			//TODO: implement targets other than center depending on qresult return
			auto finished = qresult.under_mouse->recieve_drag_and_drop(*this, ui_state.current_drag_and_drop_data, ui_state.current_drag_and_drop_data_type, ui::drag_and_drop_target::center, ui_state.shift_held_down);
			if(finished) {
				ui_state.current_drag_and_drop_data_type = ui::drag_and_drop_data::none;
				ui_state.current_drag_and_drop_data.reset();
			}
			return;
		}
	} 
	
	game_scene::on_lbutton_down(*this, x, y, mod);
	
}

void state::on_rbutton_up(int32_t x, int32_t y, key_modifiers mod) { }
void state::on_mbutton_up(int32_t x, int32_t y, key_modifiers mod) {
	
}
void state::on_lbutton_up(int32_t x, int32_t y, key_modifiers mod) {
	game_scene::on_lbutton_up(*this, x, y, mod);
}
void state::on_mouse_move(int32_t x, int32_t y, key_modifiers mod) {
	
	if(ui_state.under_mouse != nullptr) {
		auto r = ui_state.under_mouse->impl_on_mouse_move(*this, ui_state.relative_mouse_location.x,
				ui_state.relative_mouse_location.y, mod);
	}
	if(ui_state.mouse_sensitive_target) {
		auto mx = int32_t(x / user_settings.ui_scale);
		auto my = int32_t(y / user_settings.ui_scale);

		auto x_distance = std::max(std::max(ui_state.target_ul_bounds.x - mx, 0), std::max(mx - ui_state.target_lr_bounds.x, 0));
		auto y_distance = std::max(std::max(ui_state.target_ul_bounds.y - my, 0), std::max(my - ui_state.target_lr_bounds.y, 0));
		auto new_target_distance = std::max(x_distance, y_distance);
		if(new_target_distance > ui_state.target_distance + 5) {
			ui_state.mouse_sensitive_target->set_visible(*this, false);
			ui_state.mouse_sensitive_target = nullptr;
		} else {
			ui_state.target_distance = std::min(ui_state.target_distance, new_target_distance);
		}
	}
}
void state::on_mouse_drag(int32_t x, int32_t y, key_modifiers mod) { // called when the left button is held down
	is_dragging = true;
	if(ui_state.drag_target) {
		ui_state.drag_target->on_drag(*this, int32_t(mouse_x_position / user_settings.ui_scale),
			int32_t(mouse_y_position / user_settings.ui_scale), int32_t(x / user_settings.ui_scale),
			int32_t(y / user_settings.ui_scale), mod);
	}
}
void state::on_drag_finished(int32_t x, int32_t y, key_modifiers mod) { // called when the left button is released after one or more drag events
	if(ui_state.drag_target) {
		ui_state.drag_target->on_drag_finish(*this);
		ui_state.drag_target = nullptr;
	}
}
void state::on_resize(int32_t x, int32_t y, window::window_state win_state) {
	ogl::deinitialize_msaa(*this);
	ogl::initialize_msaa(*this, x, y);

	ogl::deinitialize_framebuffer_for_province_indices(*this);
	ogl::initialize_framebuffer_for_province_indices(*this, x, y);

	

	if(win_state != window::window_state::minimized) {
		ui_state.for_each_root([&](ui::element_base& elm) {
			elm.base_data.size.x = int16_t(x / user_settings.ui_scale);
			elm.base_data.size.y = int16_t(y / user_settings.ui_scale);
		});
	}
}


void state::on_key_down(virtual_key keycode, key_modifiers mod) {
	if(keycode == virtual_key::CONTROL)
		ui_state.ctrl_held_down = true;
	if(keycode == virtual_key::SHIFT || keycode == virtual_key::LSHIFT || keycode == virtual_key::RSHIFT)
		ui_state.shift_held_down = true;
	if(keycode == virtual_key::ESCAPE && ui_state.current_drag_and_drop_data_type != ui::drag_and_drop_data::none) {
		ui_state.current_drag_and_drop_data_type = ui::drag_and_drop_data::none;
		return;
	}

	game_scene::on_key_down(*this, keycode, mod);
}

void state::on_key_up(virtual_key keycode, key_modifiers mod) {
	if(keycode == virtual_key::CONTROL)
		ui_state.ctrl_held_down = false;
	if(keycode == virtual_key::SHIFT || keycode == virtual_key::LSHIFT || keycode == virtual_key::RSHIFT)
		ui_state.shift_held_down = false;

}
void state::on_text(char32_t c) { // c is win1250 codepage value
	if(ui_state.edit_target_internal)
		ui_state.edit_target_internal->on_text(*this, c);
}
bool state::filter_tso_mouse_events(int32_t x, int32_t y, uint32_t buttons) {
	if(ui_state.edit_target_internal && ui_state.edit_target_internal->edit_consume_mouse_event(*this, x, y, buttons))
		return true;
	return false;
}
void state::pass_edit_command(ui::edit_command command, sys::key_modifiers mod) {
	if(ui_state.edit_target_internal)
		ui_state.edit_target_internal->on_edit_command(*this, command, mod);
}
bool state::send_edit_mouse_move(int32_t x, int32_t y, bool extend_selection) {
	if(ui_state.edit_target_internal) {
		auto abs_pos = ui::get_absolute_location(*this, *ui_state.edit_target_internal);
		auto posx = int32_t(x / user_settings.ui_scale);
		auto posy = int32_t(y / user_settings.ui_scale);
		if(posx < abs_pos.x || posy < abs_pos.y || posx > abs_pos.x + ui_state.edit_target_internal->base_data.size.x || posy > abs_pos.y + ui_state.edit_target_internal->base_data.size.y)
			return false;

		ui_state.edit_target_internal->edit_move_cursor_to_screen_point(*this, posx - abs_pos.x, posy - abs_pos.y, extend_selection);
		return true;
	}
	return false;
}
text_mouse_test_result state::detailed_text_mouse_test(int32_t x, int32_t y) {
	if(ui_state.edit_target_internal) {
		auto abs_pos = ui::get_absolute_location(*this, *ui_state.edit_target_internal);
		auto posx = int32_t(x / user_settings.ui_scale);
		auto posy = int32_t(y / user_settings.ui_scale);
		if(posx < abs_pos.x || posy < abs_pos.y || posx > abs_pos.x + ui_state.edit_target_internal->base_data.size.x || posy > abs_pos.y + ui_state.edit_target_internal->base_data.size.y)
			return text_mouse_test_result{ 0,0 };

		return ui_state.edit_target_internal->detailed_text_mouse_test(*this, posx - abs_pos.x, posy - abs_pos.y);
	}
	return text_mouse_test_result{0,0};
}


int state::get_edit_x() {
	if (ui_state.edit_target_internal) {
		return ui::get_absolute_location(*this, *ui_state.edit_target_internal).x;
	}
	return 0;
}
int state::get_edit_y(){
	if (ui_state.edit_target_internal) {
		return ui::get_absolute_location(*this, *ui_state.edit_target_internal).y;
	}
	return 0;
}


GLuint request_query(std::vector<GLuint>& ids, std::vector<bool>& free_ids) {
	unsigned int first_free_index = 0;
	while(first_free_index < free_ids.size() && !free_ids[first_free_index]) first_free_index++;
	if(first_free_index >= free_ids.size()) {
		GLuint query_id;
		glGenQueries(1, &query_id);
		ids.push_back(query_id);
		free_ids.push_back(true);
	}
	free_ids[first_free_index] = false;
	return ids[first_free_index];
}

void state::render() { // called to render the frame may (and should) delay returning until the frame is rendered, including
	if(!current_scene.get_root)
		return;


	//if(ui_state.fps_counter) {
	//	if(ui_state.fps_counter->is_visible()) {
	//		glBeginQuery(GL_TIME_ELAPSED, request_query(query_frame_time_other, query_frame_time_other_free));
	//	}
	//}


	auto game_state_was_updated = game_state_updated.exchange(false, std::memory_order::acq_rel);

	if(game_state_was_updated) {
		//
	}

	ui::element_base* root_elm = current_scene.get_root(*this);

	root_elm->base_data.size.x = ui_state.root->base_data.size.x;
	root_elm->base_data.size.y = ui_state.root->base_data.size.y;

	auto mouse_probe = root_elm->impl_probe_mouse(*this, int32_t(mouse_x_position / user_settings.ui_scale),
		int32_t(mouse_y_position / user_settings.ui_scale), ui::mouse_probe_type::click);
	auto tooltip_probe = root_elm->impl_probe_mouse(*this, int32_t(mouse_x_position / user_settings.ui_scale),
		int32_t(mouse_y_position / user_settings.ui_scale), ui::mouse_probe_type::tooltip);

	if(!mouse_probe.under_mouse) {
		mouse_probe = current_scene.recalculate_mouse_probe(*this, mouse_probe, tooltip_probe);
		tooltip_probe = current_scene.recalculate_tooltip_probe(*this, mouse_probe, tooltip_probe);
	}

	ui::urect tooltip_bounds;
	int32_t tooltip_sub_index = -1;
	if(tooltip_probe.under_mouse) {
		tooltip_probe.under_mouse->tooltip_position(
			*this,
			tooltip_probe.relative_location.x,
			tooltip_probe.relative_location.y,
			tooltip_sub_index,
			tooltip_bounds
		);
	}

	if(game_state_was_updated) {
		root_elm->impl_on_update(*this);
		current_scene.on_game_state_update(*this);
		ui_state.update_tooltip(*this, tooltip_probe, tooltip_sub_index, int16_t(root_elm->base_data.size.y - 20));
	} // END game state was updated

	ui_state.populate_tooltip(*this, tooltip_probe, tooltip_sub_index, int16_t(root_elm->base_data.size.y - 20));
	ui_state.reposition_tooltip(tooltip_bounds, root_elm->base_data.size.y, root_elm->base_data.size.x);

	if(ui_state.under_mouse != mouse_probe.under_mouse) {
		if(ui_state.under_mouse)
			ui_state.under_mouse->on_hover_end(*this);
		ui_state.under_mouse = mouse_probe.under_mouse;
		if(ui_state.under_mouse)
			ui_state.under_mouse->on_hover(*this);
	}

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/// render any whole screen background here

	current_scene.render_screen(*this);

	
	//UI rendering
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(open_gl.ui_shader_program);
	glUniform1i(open_gl.ui_shader_texture_sampler_uniform, 0);
	glUniform1i(open_gl.ui_shader_secondary_texture_sampler_uniform, 1);
	glUniform1f(open_gl.ui_shader_screen_width_uniform, float(x_size) / user_settings.ui_scale);
	glUniform1f(open_gl.ui_shader_screen_height_uniform, float(y_size) / user_settings.ui_scale);
	glUniform1f(open_gl.ui_shader_gamma_uniform, 1.0f);
	glViewport(0, 0, x_size, y_size);
	glDepthRange(-1.0f, 1.0f);

	ui_state.relative_mouse_location = mouse_probe.relative_location;

	root_elm->impl_render(*this, 0, 0);
	ui_animation.render(*this);
	ui_state.render_tooltip(*this, user_settings.bind_tooltip_mouse, mouse_x_position, mouse_y_position, x_size, y_size, user_settings.ui_scale);


	if(ui_state.current_drag_and_drop_data_type != ui::drag_and_drop_data::none) {
		auto win_x_size = 18 + 10 + ui_state.drag_and_drop_image.cap_width;
		auto win_y_size = std::max(18, ui_state.drag_and_drop_image.cap_height) + 10;

		static auto popup_bg = template_project::background_by_name(ui_templates, "outset_region.asvg");
		static auto dad_icon = template_project::icon_by_name(ui_templates, "ic_fluent_document_briefcase_32_regular.svg");
		static auto dad_color = template_project::color_by_name(ui_templates, "med red");

		ogl::render_textured_rect_direct(*this, float((x_size / user_settings.ui_scale) / 2 - win_x_size/2), float((y_size / user_settings.ui_scale) - win_y_size), float(win_x_size), float(win_y_size), ui_templates.backgrounds[popup_bg].renders.get_render(*this, float(win_x_size) / float(9), float(win_y_size) / float(9), int32_t(9), user_settings.ui_scale));

		ogl::render_textured_rect_direct(*this, float((x_size / user_settings.ui_scale) / 2 - win_x_size / 2 + 5), float((y_size / user_settings.ui_scale) - win_y_size + 5), float(18), float(18),
			ui_templates.icons[dad_icon].renders.get_render(*this, 18, 18, user_settings.ui_scale, ui_templates.colors[dad_color].r, ui_templates.colors[dad_color].g, ui_templates.colors[dad_color].b));

		ui_state.drag_and_drop_image.render(*this, int32_t((x_size / user_settings.ui_scale) / 2) - win_x_size / 2 + 5 + 18, int32_t(y_size / user_settings.ui_scale) - win_y_size + 5);
	}

	//if(ui_state.fps_counter) {
	//	if(ui_state.fps_counter->is_visible()) {
	//		glEndQuery(GL_TIME_ELAPSED);
	//	}
	//}
	
}

void state::on_create() {
	// lua

	ui_state.default_header_font = text::name_into_font_id(*this, "vic_22");
	ui_state.default_body_font = text::name_into_font_id(*this, "vic_18");

	// Load late ui defs
	auto root = get_root(common_fs);
	auto assets = simple_fs::open_directory(root, NATIVE("assets"));

	auto uitemplates = simple_fs::open_file(assets, NATIVE("the.tui"));
	if(uitemplates) {
		auto content = view_contents(*uitemplates);
		serialization::in_buffer buffer(content.data, content.file_size);
		ui_templates = template_project::bytes_to_project(buffer);
		ui_templates.svg_directory.pop_back();
		svg_image_files.root_directory = simple_fs::utf16_to_native(ui_templates.svg_directory);
		auto svgdir = simple_fs::open_directory(assets, simple_fs::utf16_to_native(ui_templates.svg_directory));
		for(auto& i : ui_templates.icons) {
			auto f = simple_fs::open_file(svgdir, simple_fs::utf8_to_native(i.file_name));
			if(f) {
				auto contents = simple_fs::view_contents(*f);
				i.renders = asvg::simple_svg(contents.data, size_t(contents.file_size));
			}
		}
		for(auto& b : ui_templates.backgrounds) {
			auto f = simple_fs::open_file(svgdir, simple_fs::utf8_to_native(b.file_name));
			if(f) {
				auto contents = simple_fs::view_contents(*f);
				b.renders = asvg::svg(contents.data, size_t(contents.file_size), b.base_x, b.base_y);
			}
		}
	}

	for(auto gui_file : list_files(assets, NATIVE(".aui"))) {
		auto file_name = simple_fs::get_file_name(gui_file);
		auto opened_file = open_file(gui_file);
		if(opened_file) {
			file_name.pop_back(); file_name.pop_back(); file_name.pop_back(); file_name.pop_back();
			auto afile_name = simple_fs::native_to_utf8(file_name);
			auto content = view_contents(*opened_file);
			bytes_to_windows(content.data, content.file_size, afile_name, ui_state.new_ui_windows);
			ui_state.held_open_ui_files.emplace_back(std::move(*opened_file));
		}
	}

}
//
// string pool functions
//

std::string_view state::to_string_view(dcon::text_key tag) const {
	if(!tag)
		return std::string_view();
	assert(size_t(tag.index()) < key_data.size());
	auto start_position = key_data.data() + tag.index();
	auto data_size = key_data.size();
	auto end_position = start_position;
	for(; end_position < key_data.data() + data_size; ++end_position) {
		if(*end_position == 0)
			break;
	}
	return std::string_view(key_data.data() + tag.index(), size_t(end_position - start_position));
}

std::string_view state::locale_string_view(uint32_t tag) const {
	assert(size_t(tag) < locale_text_data.size());
	auto start_position = locale_text_data.data() + tag;
	auto data_size = locale_text_data.size();
	auto end_position = start_position;
	for(; end_position < locale_text_data.data() + data_size; ++end_position) {
		if(*end_position == 0)
			break;
	}
	return std::string_view(locale_text_data.data() + tag, size_t(end_position - start_position));
}

void state::reset_locale_pool() {
	locale_text_data.clear();
	locale_key_to_text_sequence.clear();
	locale_text_data.push_back(0);
}

void state::load_locale_strings(std::string_view locale_name) {
	auto root_dir = get_root(common_fs);
	auto assets_dir = open_directory(root_dir, NATIVE("assets/localization"));

	auto locale_dir = open_directory(assets_dir, simple_fs::utf8_to_native(locale_name));
	for(auto& file : list_files(locale_dir, NATIVE(".csv"))) {
		if(auto ofile = open_file(file); ofile) {
			auto content = view_contents(*ofile);
			text::consume_csv_file(*this, content.data, content.file_size, 1);
		}
	}
}

bool state::key_is_localized(dcon::text_key tag) const {
	if(!tag)
		return false;
	assert(size_t(tag.index()) < key_data.size());
	return locale_key_to_text_sequence.find(tag) != locale_key_to_text_sequence.end();
}
bool state::key_is_localized(std::string_view key) const {
	return locale_key_to_text_sequence.find(key) != locale_key_to_text_sequence.end();
}
dcon::text_key state::lookup_key(std::string_view text) const {
	if(auto it = untrans_key_to_text_sequence.find(text); it != untrans_key_to_text_sequence.end()) {
		return *it;
	}
	return dcon::text_key{};
}

dcon::text_key state::add_key_utf8(std::string const& new_text) {
	return add_key_utf8(std::string_view(new_text.data()));
}
dcon::text_key state::add_key_utf8(std::string_view new_text) {
	auto ekey = lookup_key(new_text);
	if(ekey)
		return ekey;

	auto start = key_data.size();
	auto length = new_text.length();
	if(length == 0)
		return dcon::text_key();
	key_data.resize(start + length + 1, char(0));
	std::copy_n(new_text.data(), length, key_data.data() + start);
	key_data.back() = 0;

	auto ret = dcon::text_key(dcon::text_key::value_base_t(start));
	untrans_key_to_text_sequence.insert(ret);
	return ret;
}

uint32_t state::add_locale_data_utf8(std::string const& new_text) {
	return add_locale_data_utf8(std::string_view(new_text));
}
uint32_t state::add_locale_data_utf8(std::string_view new_text) {
	auto start = locale_text_data.size();
	auto length = new_text.length();
	if(length == 0)
		return 0;
	locale_text_data.resize(start + length + 1, char(0));
	std::copy_n(new_text.data(), length, locale_text_data.data() + start);
	locale_text_data.back() = 0;
	return uint32_t(start);
}


struct locale_parser {
	bool rtl = false;
	std::string display_name;
	std::string script = "Latn";
	std::string body_font;
	std::string header_font;
	std::string fallback;
	std::vector<uint32_t> body_features;
	std::vector<uint32_t> header_features;

	void body_feature(parsers::association_type, std::string_view value, parsers::error_handler& err, int32_t line, sys::state&) {
		body_features.push_back(hb_tag_from_string(value.data(), int(value.length())));
	}
	void header_feature(parsers::association_type, std::string_view value, parsers::error_handler& err, int32_t line, sys::state&) {
		header_features.push_back(hb_tag_from_string(value.data(), int(value.length())));
	}

	void finish(sys::state& context) {
	}
};


locale_parser parse_locale_parser(parsers::token_generator& gen, parsers::error_handler& err, sys::state& context) {
	locale_parser cobj;
	for(parsers::token_and_type cur = gen.get(); cur.type != parsers::token_type::unknown && cur.type != parsers::token_type::close_brace; cur = gen.get()) {
		if(cur.type == parsers::token_type::open_brace) {
			err.unhandled_free_group(cur); gen.discard_group();
			continue;
		}
		auto peek_result = gen.next();
		if(peek_result.type == parsers::token_type::special_identifier) {
			auto peek2_result = gen.next_next();
			if(peek2_result.type == parsers::token_type::open_brace) {
				gen.get(); gen.get();
				switch(int32_t(cur.content.length())) {
				default:
					err.unhandled_group_key(cur); gen.discard_group();
					break;
				}
			} else {
				auto const assoc_token = gen.get();
				auto const assoc_type = parse_association_type(assoc_token.content, assoc_token.line, err);
				auto const rh_token = gen.get();
				switch(int32_t(cur.content.length())) {
				case 3:
					// rtl
					if((true && (*(uint16_t const*)(&cur.content[0]) | 0x2020) == 0x7472 && (cur.content[2] | 0x20) == 0x6C)) {
						cobj.rtl = parse_bool(rh_token.content, rh_token.line, err);
					} else {
						err.unhandled_association_key(cur);
					}
					break;
				case 6:
					// script
					if((true && (*(uint32_t const*)(&cur.content[0]) | uint32_t(0x20202020)) == uint32_t(0x69726373) && (*(uint16_t const*)(&cur.content[4]) | 0x2020) == 0x7470)) {
						cobj.script = parse_text(rh_token.content, rh_token.line, err);
					} else {
						err.unhandled_association_key(cur);
					}
					break;
				case 8:
					switch(0x20 | int32_t(cur.content[0])) {
					case 0x66:
						// fallback
						if((true && (*(uint32_t const*)(&cur.content[1]) | uint32_t(0x20202020)) == uint32_t(0x626C6C61) && (*(uint16_t const*)(&cur.content[5]) | 0x2020) == 0x6361 && (cur.content[7] | 0x20) == 0x6B)) {
							cobj.fallback = parse_text(rh_token.content, rh_token.line, err);
						} else {
							err.unhandled_association_key(cur);
						}
						break;
					default:
						err.unhandled_association_key(cur);
						break;
					}
					break;
				case 9:
					// body_font
					if((true && (*(uint64_t const*)(&cur.content[0]) | uint64_t(0x2020202020202020)) == uint64_t(0x6E6F667F79646F62) && (cur.content[8] | 0x20) == 0x74)) {
						cobj.body_font = parse_text(rh_token.content, rh_token.line, err);
					} else {
						err.unhandled_association_key(cur);
					}
					break;
				case 11:
					switch(0x20 | int32_t(cur.content[0])) {
					case 0x68:
						// header_font
						if((true && (*(uint64_t const*)(&cur.content[1]) | uint64_t(0x2020202020202020)) == uint64_t(0x6F667F7265646165) && (*(uint16_t const*)(&cur.content[9]) | 0x2020) == 0x746E)) {
							cobj.header_font = parse_text(rh_token.content, rh_token.line, err);
						} else {
							err.unhandled_association_key(cur);
						}
						break;
					default:
						err.unhandled_association_key(cur);
						break;
					}
					break;
				case 12:
					switch(0x20 | int32_t(cur.content[0])) {
					case 0x62:
						// body_feature
						if((true && (*(uint64_t const*)(&cur.content[1]) | uint64_t(0x2020202020202020)) == uint64_t(0x746165667F79646F) && (*(uint16_t const*)(&cur.content[9]) | 0x2020) == 0x7275 && (cur.content[11] | 0x20) == 0x65)) {
							cobj.body_feature(assoc_type, parse_text(rh_token.content, rh_token.line, err), err, cur.line, context);
						} else {
							err.unhandled_association_key(cur);
						}
						break;
					case 0x64:
						// display_name
						if((true && (*(uint64_t const*)(&cur.content[1]) | uint64_t(0x2020202020202020)) == uint64_t(0x6E7F79616C707369) && (*(uint16_t const*)(&cur.content[9]) | 0x2020) == 0x6D61 && (cur.content[11] | 0x20) == 0x65)) {
							cobj.display_name = parse_text(rh_token.content, rh_token.line, err);
						} else {
							err.unhandled_association_key(cur);
						}
						break;
					default:
						err.unhandled_association_key(cur);
						break;
					}
					break;
				case 14:
					// header_feature
					if((true && (*(uint64_t const*)(&cur.content[0]) | uint64_t(0x2020202020202020)) == uint64_t(0x667F726564616568) && (*(uint32_t const*)(&cur.content[8]) | uint32_t(0x20202020)) == uint32_t(0x75746165) && (*(uint16_t const*)(&cur.content[12]) | 0x2020) == 0x6572)) {
						cobj.header_feature(assoc_type, parse_text(rh_token.content, rh_token.line, err), err, cur.line, context);
					} else {
						err.unhandled_association_key(cur);
					}
					break;
				default:
					err.unhandled_association_key(cur);
					break;
				}
			}
		} else {
			err.unhandled_free_value(cur);
		}
	}
	cobj.finish(context);
	return cobj;
}

void add_locale(sys::state& state, std::string_view locale_name, char const* data_start, char const* data_end) {
	parsers::token_generator gen(data_start, data_end);
	parsers::error_handler err("");

	locale_parser new_locale = parse_locale_parser(gen, err, state);
	hb_language_t lang = nullptr;

	auto new_locale_id = state.world.create_locale();
	auto new_locale_obj = fatten(state.world, new_locale_id);
	new_locale_obj.set_hb_script(hb_script_from_string(new_locale.script.c_str(), int(new_locale.script.length())));
	new_locale_obj.set_native_rtl(new_locale.rtl);

	{
		auto f = new_locale_obj.get_body_font();
		f.resize(uint32_t(new_locale.body_font.length()));
		f.load_range((uint8_t const*)new_locale.body_font.c_str(), (uint8_t const*)new_locale.body_font.c_str() + new_locale.body_font.length());
	}
	{
		auto f = new_locale_obj.get_header_font();
		f.resize(uint32_t(new_locale.header_font.length()));
		f.load_range((uint8_t const*)new_locale.header_font.c_str(), (uint8_t const*)new_locale.header_font.c_str() + new_locale.header_font.length());
	}
	
	{
		auto f = new_locale_obj.get_body_font_features();
		f.resize(uint32_t(new_locale.body_features.size()));
		f.load_range(new_locale.body_features.data(), new_locale.body_features.data() + new_locale.body_features.size());
	}
	{
		auto f = new_locale_obj.get_header_font_features();
		f.resize(uint32_t(new_locale.header_features.size()));
		f.load_range(new_locale.header_features.data(), new_locale.header_features.data() + new_locale.header_features.size());
	}
	
	{
		auto f = new_locale_obj.get_locale_name();
		f.resize(uint32_t(locale_name.length()));
		f.load_range((uint8_t const*)locale_name.data(), (uint8_t const*)locale_name.data() + locale_name.length());
	}
	{
		auto f = new_locale_obj.get_fallback();
		f.resize(uint32_t(new_locale.fallback.length()));
		f.load_range((uint8_t const*)new_locale.fallback.data(), (uint8_t const*)new_locale.fallback.data() + new_locale.fallback.length());
	}
	{
		auto f = new_locale_obj.get_display_name();
		f.resize(uint32_t(new_locale.display_name.length()));
		f.load_range((uint8_t const*)new_locale.display_name.data(), (uint8_t const*)new_locale.display_name.data() + new_locale.display_name.length());
	}
}


void state::save_user_settings() const {
	auto settings_location = simple_fs::get_or_create_settings_directory();

	char buffer[sizeof(user_settings_s)];
	char* ptr = &buffer[0];

#define US_SAVE(x) \
		std::memcpy(ptr, &user_settings.x, sizeof(user_settings.x)); \
		ptr += sizeof(user_settings.x);
	US_SAVE(ui_scale);
	US_SAVE(master_volume);
	US_SAVE(music_volume);
	US_SAVE(effects_volume);
	US_SAVE(interface_volume);
	US_SAVE(prefer_fullscreen);
	US_SAVE(bind_tooltip_mouse);
	US_SAVE(left_mouse_click_hold_and_release);
	US_SAVE(zoom_speed);
	US_SAVE(mute_on_focus_lost);
	US_SAVE(locale);
#undef US_SAVE

	simple_fs::write_file(settings_location, NATIVE("user_settings.dat"), &buffer[0], uint32_t(ptr - buffer));
}
void state::load_user_settings() {
	auto settings_location = simple_fs::get_or_create_settings_directory();
	auto settings_file = open_file(settings_location, NATIVE("user_settings.dat"));
	if(settings_file) {
		auto content = view_contents(*settings_file);
		auto ptr = content.data;

#define US_LOAD(x) \
		if(ptr > content.data + content.file_size - sizeof(user_settings.x)) break; \
		std::memcpy(&user_settings.x, ptr, sizeof(user_settings.x)); \
		ptr += sizeof(user_settings.x);

		do {
			US_LOAD(ui_scale);
			US_LOAD(master_volume);
			US_LOAD(music_volume);
			US_LOAD(effects_volume);
			US_LOAD(interface_volume);
			US_LOAD(prefer_fullscreen);
			US_LOAD(bind_tooltip_mouse);
			US_LOAD(left_mouse_click_hold_and_release);
			US_LOAD(zoom_speed);
			US_LOAD(mute_on_focus_lost);
			US_LOAD(locale);
#undef US_LOAD
		} while(false);

		//NaN will not get clamped, so use special std::isfinite test to set to reasonable values
		if(!std::isfinite(user_settings.interface_volume)) user_settings.interface_volume = 0.0f;
		user_settings.interface_volume = std::clamp(user_settings.interface_volume, 0.0f, 1.0f);

		if(!std::isfinite(user_settings.music_volume)) user_settings.music_volume = 0.0f;
		user_settings.music_volume = std::clamp(user_settings.music_volume, 0.0f, 1.0f);

		if(!std::isfinite(user_settings.effects_volume)) user_settings.effects_volume = 0.0f;
		user_settings.effects_volume = std::clamp(user_settings.effects_volume, 0.0f, 1.0f);

		if(!std::isfinite(user_settings.master_volume)) user_settings.master_volume = 0.0f;
		user_settings.master_volume = std::clamp(user_settings.master_volume, 0.0f, 1.0f);

		if(!std::isfinite(user_settings.zoom_speed)) user_settings.zoom_speed = 15.0f;
		user_settings.zoom_speed = std::clamp(user_settings.zoom_speed, 15.f, 25.f);

	}

	user_settings.locale[15] = 0;
	std::string lname(user_settings.locale);
	bool locale_loaded = false;

	auto rt = get_root(common_fs);
	auto assets = simple_fs::open_directory(rt, NATIVE("assets"));
	auto loc = simple_fs::open_directory(assets, NATIVE("localization"));
	for(auto& ld : simple_fs::list_subdirectories(loc)) {
		auto def_file = simple_fs::open_file(ld, NATIVE("locale.txt"));
		if(def_file) {
			auto contents = simple_fs::view_contents(*def_file);
			auto ld_name = simple_fs::get_full_name(ld);
			auto dir_lname = ld_name.substr(ld_name.find_last_of(NATIVE_DIR_SEPARATOR) + 1);
			add_locale(*this, simple_fs::native_to_utf8(dir_lname), contents.data, contents.data + contents.file_size);
		}
	}

	for(auto l : world.in_locale) {
		auto ln = l.get_locale_name();
		auto ln_sv = std::string_view{ (char const*)ln.begin(), ln.size() };
		if(ln_sv == lname) {
			font_collection.change_locale(*this, l);
			locale_loaded = true;
			break;
		}
	}

	if(!locale_loaded) {
		for(auto l : world.in_locale) {
			auto ln = l.get_locale_name();
			auto ln_sv = std::string_view{ (char const*)ln.begin(), ln.size() };
			if(ln_sv == "en-US") {
				font_collection.change_locale(*this, l);
				locale_loaded = true;
				break;
			}
		}
	}

	if(!locale_loaded) {
		font_collection.change_locale(*this, dcon::locale_id{ 0 });
	}
}

void state::update_ui_scale(float new_scale) {
	user_settings.ui_scale = new_scale;
	ui_state.for_each_root([&](ui::element_base& elm) {
		elm.base_data.size.x = int16_t(x_size / user_settings.ui_scale);
		elm.base_data.size.y = int16_t(y_size / user_settings.ui_scale);
	});

	for(auto& s : ui_templates.backgrounds) {
		s.renders.release_renders();
	}
	for(auto& s : ui_templates.icons) {
		s.renders.release_renders();
	}
	//font_collection.reset_fonts();

	ui_state.for_each_root([&](ui::element_base& elm) {
		elm.impl_on_reset_text(*this);
	});

	game_state_updated.store(true, std::memory_order::release); //update ui

	// TODO move windows
}

void state::single_game_tick() {
	// do update logic

	tick_start_counter.fetch_add(1, std::memory_order::seq_cst);

	// do business

	tick_end_counter.fetch_add(1, std::memory_order::seq_cst);
	game_state_updated.store(true, std::memory_order::release);
}


void state::game_loop() {
	static int32_t game_speed[] = {
		0,		// speed 0
		2000,	// speed 1 -- 2 seconds
		750,		// speed 2 -- 0.75 seconds
		250, 	// speed 3 -- 0.25 seconds
		125,		// speed 4 -- 0.125 seconds
	};

	while(quit_signaled.load(std::memory_order::acquire) == false) {
		{
			command::execute_pending_commands(*this);
		}

		auto speed = actual_game_speed.load(std::memory_order::acquire);
		auto upause = ui_pause.load(std::memory_order::acquire);
			
		if(speed <= 0 || upause || internally_paused || current_scene.enforced_pause) {
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
		} else {
			auto entry_time = std::chrono::steady_clock::now();
			auto ms_count = std::chrono::duration_cast<std::chrono::milliseconds>(entry_time - last_update).count();
			if(speed >= 5 || ms_count >= game_speed[speed]) { /*enough time has passed*/
				last_update = entry_time;
				single_game_tick();
			} else {
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}

	}
}

} // namespace sys
