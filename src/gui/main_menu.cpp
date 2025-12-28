// BEGIN prelude
#include "alice_ui.hpp"
#include "system_state.hpp"
#include "gui_deserialize.hpp"
// END

namespace alice_ui {
struct main_menu_base_exit_t;
struct main_menu_base_graphics_options_t;
struct main_menu_base_sound_options_t;
struct main_menu_base_controls_options_t;
struct main_menu_base_end_icon_t;
struct main_menu_base_t;
struct main_menu_sound_master_volume_minimum_t;
struct main_menu_sound_master_volume_less_t;
struct main_menu_sound_master_voume_amount_t;
struct main_menu_sound_master_volume_more_t;
struct main_menu_sound_master_volume_maximum_t;
struct main_menu_sound_music_volume_minimum_t;
struct main_menu_sound_music_volume_less_t;
struct main_menu_sound_music_volume_amount_t;
struct main_menu_sound_music_volume_more_t;
struct main_menu_sound_music_volume_max_t;
struct main_menu_sound_interface_volume_min_t;
struct main_menu_sound_interface_volume_less_t;
struct main_menu_sound_interface_volume_amount_t;
struct main_menu_sound_interface_volume_more_t;
struct main_menu_sound_interface_volume_max_t;
struct main_menu_sound_mute_on_lose_focus_t;
struct main_menu_sound_t;
struct main_menu_misc_language_dropdown_t;
struct main_menu_misc_floating_tooltip_t;
struct main_menu_misc_edge_scrolling_t;
struct main_menu_misc_t;
struct main_menu_string_dropdown_choice_contents_t;
struct main_menu_string_dropdown_choice_t;
struct main_menu_graphics_ui_scale_dropdown_t;
struct main_menu_graphics_window_mode_label_t;
struct main_menu_graphics_t;
struct main_menu_base_exit_t : public alice_ui::template_text_button {
// BEGIN base::exit::variables
// END
	bool button_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_base_graphics_options_t : public alice_ui::template_text_button {
// BEGIN base::graphics_options::variables
// END
	bool button_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_base_sound_options_t : public alice_ui::template_text_button {
// BEGIN base::sound_options::variables
// END
	bool button_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_base_controls_options_t : public alice_ui::template_text_button {
// BEGIN base::controls_options::variables
// END
	bool button_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_base_end_icon_t : public ui::element_base {
// BEGIN base::end_icon::variables
// END
	std::string_view texture_key;
	dcon::texture_id background_texture;
	void on_create(sys::state& state) noexcept override;
	void render(sys::state & state, int32_t x, int32_t y) noexcept override;
	ui::tooltip_behavior has_tooltip(sys::state & state) noexcept override {
		return ui::tooltip_behavior::no_tooltip;
	}
	ui::message_result test_mouse(sys::state& state, int32_t x, int32_t y, ui::mouse_probe_type type) noexcept override {
		if(type == ui::mouse_probe_type::click) {
			return ui::message_result::unseen;
		} else if(type == ui::mouse_probe_type::tooltip) {
			return ui::message_result::unseen;
		} else if(type == ui::mouse_probe_type::scroll) {
			return ui::message_result::unseen;
		} else {
			return ui::message_result::unseen;
		}
	}
	ui::message_result on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override;
	ui::message_result on_rbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_sound_master_volume_minimum_t : public alice_ui::template_icon_button {
// BEGIN sound::master_volume_minimum::variables
// END
	bool button_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_sound_master_volume_less_t : public alice_ui::template_icon_button {
// BEGIN sound::master_volume_less::variables
// END
	bool button_action(sys::state& state) noexcept override;
	bool button_shift_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_sound_master_voume_amount_t : public alice_ui::template_label {
// BEGIN sound::master_voume_amount::variables
// END
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_sound_master_volume_more_t : public alice_ui::template_icon_button {
// BEGIN sound::master_volume_more::variables
// END
	bool button_action(sys::state& state) noexcept override;
	bool button_shift_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_sound_master_volume_maximum_t : public alice_ui::template_icon_button {
// BEGIN sound::master_volume_maximum::variables
// END
	bool button_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_sound_music_volume_minimum_t : public alice_ui::template_icon_button {
// BEGIN sound::music_volume_minimum::variables
// END
	bool button_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_sound_music_volume_less_t : public alice_ui::template_icon_button {
// BEGIN sound::music_volume_less::variables
// END
	bool button_action(sys::state& state) noexcept override;
	bool button_shift_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_sound_music_volume_amount_t : public alice_ui::template_label {
// BEGIN sound::music_volume_amount::variables
// END
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_sound_music_volume_more_t : public alice_ui::template_icon_button {
// BEGIN sound::music_volume_more::variables
// END
	bool button_action(sys::state& state) noexcept override;
	bool button_shift_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_sound_music_volume_max_t : public alice_ui::template_icon_button {
// BEGIN sound::music_volume_max::variables
// END
	bool button_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_sound_interface_volume_min_t : public alice_ui::template_icon_button {
// BEGIN sound::interface_volume_min::variables
// END
	bool button_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_sound_interface_volume_less_t : public alice_ui::template_icon_button {
// BEGIN sound::interface_volume_less::variables
// END
	bool button_action(sys::state& state) noexcept override;
	bool button_shift_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_sound_interface_volume_amount_t : public alice_ui::template_label {
// BEGIN sound::interface_volume_amount::variables
// END
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_sound_interface_volume_more_t : public alice_ui::template_icon_button {
// BEGIN sound::interface_volume_more::variables
// END
	bool button_action(sys::state& state) noexcept override;
	bool button_shift_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_sound_interface_volume_max_t : public alice_ui::template_icon_button {
// BEGIN sound::interface_volume_max::variables
// END
	bool button_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_sound_mute_on_lose_focus_t : public alice_ui::template_toggle_button {
// BEGIN sound::mute_on_lose_focus::variables
// END
	bool button_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_misc_language_dropdown_t : public alice_ui::template_drop_down_control {
// BEGIN misc::language_dropdown::variables
// END
	struct string_dropdown_choice_option { int32_t index; std::string display_value; };
	std::vector<string_dropdown_choice_option> list_contents;
	std::vector<std::unique_ptr<main_menu_string_dropdown_choice_t >> list_pool;
	std::unique_ptr<main_menu_string_dropdown_choice_t> label_window_internal;
	void add_item( int32_t index,  std::string display_value);
	ui::element_base* get_nth_item(sys::state& state, int32_t id, int32_t pool_id) override;
	void quiet_on_selection(sys::state& state, int32_t id);
	void on_selection(sys::state& state, int32_t id) override;
	void clear_list();
	void on_create(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_misc_floating_tooltip_t : public alice_ui::template_toggle_button {
// BEGIN misc::floating_tooltip::variables
// END
	bool button_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_misc_edge_scrolling_t : public alice_ui::template_toggle_button {
// BEGIN misc::edge_scrolling::variables
// END
	bool button_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_string_dropdown_choice_contents_t : public alice_ui::template_label {
// BEGIN string_dropdown_choice::contents::variables
// END
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_graphics_ui_scale_dropdown_t : public alice_ui::template_drop_down_control {
// BEGIN graphics::ui_scale_dropdown::variables
// END
	struct string_dropdown_choice_option { int32_t index; std::string display_value; };
	std::vector<string_dropdown_choice_option> list_contents;
	std::vector<std::unique_ptr<main_menu_string_dropdown_choice_t >> list_pool;
	std::unique_ptr<main_menu_string_dropdown_choice_t> label_window_internal;
	void add_item( int32_t index,  std::string display_value);
	ui::element_base* get_nth_item(sys::state& state, int32_t id, int32_t pool_id) override;
	void quiet_on_selection(sys::state& state, int32_t id);
	void on_selection(sys::state& state, int32_t id) override;
	void clear_list();
	void on_create(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_graphics_window_mode_label_t : public alice_ui::template_toggle_button {
// BEGIN graphics::window_mode_label::variables
// END
	bool button_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct main_menu_base_t : public layout_window_element {
// BEGIN base::variables
// END
	std::unique_ptr<main_menu_base_exit_t> exit;
	std::unique_ptr<main_menu_base_graphics_options_t> graphics_options;
	std::unique_ptr<main_menu_base_sound_options_t> sound_options;
	std::unique_ptr<main_menu_base_controls_options_t> controls_options;
	std::unique_ptr<main_menu_base_end_icon_t> end_icon;
	std::vector<std::unique_ptr<ui::element_base>> gui_inserts;
	void create_layout_level(sys::state& state, layout_level& lvl, char const* ldata, size_t sz);
	void on_create(sys::state& state) noexcept override;
	void on_hide(sys::state& state) noexcept override;
	ui::message_result on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override;
	ui::message_result on_rbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override;
	void on_drag(sys::state& state, int32_t oldx, int32_t oldy, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override {
		auto location_abs = get_absolute_location(state, *this);
		if(location_abs.x <= oldx && oldx < base_data.size.x + location_abs.x && location_abs.y <= oldy && oldy < base_data.size.y + location_abs.y) {
			ui::xy_pair new_abs_pos = location_abs;
			new_abs_pos.x += int16_t(x - oldx);
			new_abs_pos.y += int16_t(y - oldy);
			if(ui::ui_width(state) > base_data.size.x)
				new_abs_pos.x = int16_t(std::clamp(int32_t(new_abs_pos.x), 0, ui::ui_width(state) - base_data.size.x));
			if(ui::ui_height(state) > base_data.size.y)
				new_abs_pos.y = int16_t(std::clamp(int32_t(new_abs_pos.y), 0, ui::ui_height(state) - base_data.size.y));
			if(state_is_rtl(state)) {
				base_data.position.x -= int16_t(new_abs_pos.x - location_abs.x);
			} else {
				base_data.position.x += int16_t(new_abs_pos.x - location_abs.x);
			}
		base_data.position.y += int16_t(new_abs_pos.y - location_abs.y);
		}
	}
	void on_update(sys::state& state) noexcept override;
};
std::unique_ptr<ui::element_base> make_main_menu_base(sys::state& state);
struct main_menu_sound_t : public layout_window_element {
// BEGIN sound::variables
// END
	std::unique_ptr<template_label> window_title;
	std::unique_ptr<main_menu_sound_master_volume_minimum_t> master_volume_minimum;
	std::unique_ptr<main_menu_sound_master_volume_less_t> master_volume_less;
	std::unique_ptr<main_menu_sound_master_voume_amount_t> master_voume_amount;
	std::unique_ptr<main_menu_sound_master_volume_more_t> master_volume_more;
	std::unique_ptr<main_menu_sound_master_volume_maximum_t> master_volume_maximum;
	std::unique_ptr<template_label> music_volume_label;
	std::unique_ptr<template_label> master_volume_label;
	std::unique_ptr<main_menu_sound_music_volume_minimum_t> music_volume_minimum;
	std::unique_ptr<main_menu_sound_music_volume_less_t> music_volume_less;
	std::unique_ptr<main_menu_sound_music_volume_amount_t> music_volume_amount;
	std::unique_ptr<main_menu_sound_music_volume_more_t> music_volume_more;
	std::unique_ptr<main_menu_sound_music_volume_max_t> music_volume_max;
	std::unique_ptr<template_label> interface_volume_label;
	std::unique_ptr<main_menu_sound_interface_volume_min_t> interface_volume_min;
	std::unique_ptr<main_menu_sound_interface_volume_less_t> interface_volume_less;
	std::unique_ptr<main_menu_sound_interface_volume_amount_t> interface_volume_amount;
	std::unique_ptr<main_menu_sound_interface_volume_more_t> interface_volume_more;
	std::unique_ptr<main_menu_sound_interface_volume_max_t> interface_volume_max;
	std::unique_ptr<template_bg_graphic> divider;
	std::unique_ptr<main_menu_sound_mute_on_lose_focus_t> mute_on_lose_focus;
	std::vector<std::unique_ptr<ui::element_base>> gui_inserts;
	void create_layout_level(sys::state& state, layout_level& lvl, char const* ldata, size_t sz);
	void on_create(sys::state& state) noexcept override;
	void on_hide(sys::state& state) noexcept override;
	ui::message_result on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override;
	ui::message_result on_rbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override;
	void on_drag(sys::state& state, int32_t oldx, int32_t oldy, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override {
		auto location_abs = get_absolute_location(state, *this);
		if(location_abs.x <= oldx && oldx < base_data.size.x + location_abs.x && location_abs.y <= oldy && oldy < base_data.size.y + location_abs.y) {
			ui::xy_pair new_abs_pos = location_abs;
			new_abs_pos.x += int16_t(x - oldx);
			new_abs_pos.y += int16_t(y - oldy);
			if(ui::ui_width(state) > base_data.size.x)
				new_abs_pos.x = int16_t(std::clamp(int32_t(new_abs_pos.x), 0, ui::ui_width(state) - base_data.size.x));
			if(ui::ui_height(state) > base_data.size.y)
				new_abs_pos.y = int16_t(std::clamp(int32_t(new_abs_pos.y), 0, ui::ui_height(state) - base_data.size.y));
			if(state_is_rtl(state)) {
				base_data.position.x -= int16_t(new_abs_pos.x - location_abs.x);
			} else {
				base_data.position.x += int16_t(new_abs_pos.x - location_abs.x);
			}
		base_data.position.y += int16_t(new_abs_pos.y - location_abs.y);
		}
	}
	void on_update(sys::state& state) noexcept override;
};
std::unique_ptr<ui::element_base> make_main_menu_sound(sys::state& state);
struct main_menu_misc_t : public layout_window_element {
// BEGIN misc::variables
// END
	std::unique_ptr<template_label> window_title;
	std::unique_ptr<template_label> language_label;
	std::unique_ptr<main_menu_misc_language_dropdown_t> language_dropdown;
	std::unique_ptr<main_menu_misc_floating_tooltip_t> floating_tooltip;
	std::unique_ptr<main_menu_misc_edge_scrolling_t> edge_scrolling;
	std::vector<std::unique_ptr<ui::element_base>> gui_inserts;
	void create_layout_level(sys::state& state, layout_level& lvl, char const* ldata, size_t sz);
	void on_create(sys::state& state) noexcept override;
	void on_hide(sys::state& state) noexcept override;
	ui::message_result on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override;
	ui::message_result on_rbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override;
	void on_drag(sys::state& state, int32_t oldx, int32_t oldy, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override {
		auto location_abs = get_absolute_location(state, *this);
		if(location_abs.x <= oldx && oldx < base_data.size.x + location_abs.x && location_abs.y <= oldy && oldy < base_data.size.y + location_abs.y) {
			ui::xy_pair new_abs_pos = location_abs;
			new_abs_pos.x += int16_t(x - oldx);
			new_abs_pos.y += int16_t(y - oldy);
			if(ui::ui_width(state) > base_data.size.x)
				new_abs_pos.x = int16_t(std::clamp(int32_t(new_abs_pos.x), 0, ui::ui_width(state) - base_data.size.x));
			if(ui::ui_height(state) > base_data.size.y)
				new_abs_pos.y = int16_t(std::clamp(int32_t(new_abs_pos.y), 0, ui::ui_height(state) - base_data.size.y));
			if(state_is_rtl(state)) {
				base_data.position.x -= int16_t(new_abs_pos.x - location_abs.x);
			} else {
				base_data.position.x += int16_t(new_abs_pos.x - location_abs.x);
			}
		base_data.position.y += int16_t(new_abs_pos.y - location_abs.y);
		}
	}
	void on_update(sys::state& state) noexcept override;
};
std::unique_ptr<ui::element_base> make_main_menu_misc(sys::state& state);
struct main_menu_string_dropdown_choice_t : public layout_window_element {
// BEGIN string_dropdown_choice::variables
// END
	int32_t index;
	std::string display_value;
	std::unique_ptr<main_menu_string_dropdown_choice_contents_t> contents;
	std::vector<std::unique_ptr<ui::element_base>> gui_inserts;
	void create_layout_level(sys::state& state, layout_level& lvl, char const* ldata, size_t sz);
	void on_create(sys::state& state) noexcept override;
	ui::message_result on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override;
	ui::message_result on_rbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override;
	void on_update(sys::state& state) noexcept override;
	void* get_by_name(sys::state& state, std::string_view name_parameter) noexcept override {
		if(name_parameter == "index") {
			return (void*)(&index);
		}
		if(name_parameter == "display_value") {
			return (void*)(&display_value);
		}
		return nullptr;
	}
};
std::unique_ptr<ui::element_base> make_main_menu_string_dropdown_choice(sys::state& state);
struct main_menu_graphics_t : public layout_window_element {
// BEGIN graphics::variables
// END
	std::unique_ptr<template_label> title;
	std::unique_ptr<template_label> ui_scale_label;
	std::unique_ptr<main_menu_graphics_ui_scale_dropdown_t> ui_scale_dropdown;
	std::unique_ptr<main_menu_graphics_window_mode_label_t> window_mode_label;
	std::vector<std::unique_ptr<ui::element_base>> gui_inserts;
	void create_layout_level(sys::state& state, layout_level& lvl, char const* ldata, size_t sz);
	void on_create(sys::state& state) noexcept override;
	void on_hide(sys::state& state) noexcept override;
	ui::message_result on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override;
	ui::message_result on_rbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override;
	ui::message_result test_mouse(sys::state& state, int32_t x, int32_t y, ui::mouse_probe_type type) noexcept override {
		return ui::message_result::consumed;
	}
	void on_drag(sys::state& state, int32_t oldx, int32_t oldy, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override {
		auto location_abs = get_absolute_location(state, *this);
		if(location_abs.x <= oldx && oldx < base_data.size.x + location_abs.x && location_abs.y <= oldy && oldy < base_data.size.y + location_abs.y) {
			ui::xy_pair new_abs_pos = location_abs;
			new_abs_pos.x += int16_t(x - oldx);
			new_abs_pos.y += int16_t(y - oldy);
			if(ui::ui_width(state) > base_data.size.x)
				new_abs_pos.x = int16_t(std::clamp(int32_t(new_abs_pos.x), 0, ui::ui_width(state) - base_data.size.x));
			if(ui::ui_height(state) > base_data.size.y)
				new_abs_pos.y = int16_t(std::clamp(int32_t(new_abs_pos.y), 0, ui::ui_height(state) - base_data.size.y));
			if(state_is_rtl(state)) {
				base_data.position.x -= int16_t(new_abs_pos.x - location_abs.x);
			} else {
				base_data.position.x += int16_t(new_abs_pos.x - location_abs.x);
			}
		base_data.position.y += int16_t(new_abs_pos.y - location_abs.y);
		}
	}
	void on_update(sys::state& state) noexcept override;
};
std::unique_ptr<ui::element_base> make_main_menu_graphics(sys::state& state);
void main_menu_base_exit_t::on_update(sys::state& state) noexcept {
	main_menu_base_t& base = *((main_menu_base_t*)(parent)); 
// BEGIN base::exit::update
// END
}
bool main_menu_base_exit_t::button_action(sys::state& state) noexcept {
	main_menu_base_t& base = *((main_menu_base_t*)(parent)); 
// BEGIN base::exit::lbutton_action
// END
	return true;
}
void main_menu_base_graphics_options_t::on_update(sys::state& state) noexcept {
	main_menu_base_t& base = *((main_menu_base_t*)(parent)); 
// BEGIN base::graphics_options::update
// END
}
bool main_menu_base_graphics_options_t::button_action(sys::state& state) noexcept {
	main_menu_base_t& base = *((main_menu_base_t*)(parent)); 
// BEGIN base::graphics_options::lbutton_action
	alice_ui::display_at_front<alice_ui::make_main_menu_graphics>(state);
	parent->set_visible(state, false);
// END
	return true;
}
void main_menu_base_sound_options_t::on_update(sys::state& state) noexcept {
	main_menu_base_t& base = *((main_menu_base_t*)(parent)); 
// BEGIN base::sound_options::update
// END
}
bool main_menu_base_sound_options_t::button_action(sys::state& state) noexcept {
	main_menu_base_t& base = *((main_menu_base_t*)(parent)); 
// BEGIN base::sound_options::lbutton_action
	alice_ui::display_at_front<alice_ui::make_main_menu_sound>(state);
	parent->set_visible(state, false);
// END
	return true;
}
void main_menu_base_controls_options_t::on_update(sys::state& state) noexcept {
	main_menu_base_t& base = *((main_menu_base_t*)(parent)); 
// BEGIN base::controls_options::update
// END
}
bool main_menu_base_controls_options_t::button_action(sys::state& state) noexcept {
	main_menu_base_t& base = *((main_menu_base_t*)(parent)); 
// BEGIN base::controls_options::lbutton_action
	alice_ui::display_at_front<alice_ui::make_main_menu_misc>(state);
	parent->set_visible(state, false);
// END
	return true;
}
ui::message_result main_menu_base_end_icon_t::on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	return ui::message_result::unseen;
}
ui::message_result main_menu_base_end_icon_t::on_rbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	return ui::message_result::unseen;
}
void main_menu_base_end_icon_t::render(sys::state & state, int32_t x, int32_t y) noexcept {
	ogl::render_textured_rect(state, ui::get_color_modification(this == state.ui_state.under_mouse, false, false), float(x), float(y), float(base_data.size.x), float(base_data.size.y), ogl::get_late_load_texture_handle(state, background_texture, texture_key), base_data.get_rotation(), false, state_is_rtl(state));
}
void main_menu_base_end_icon_t::on_update(sys::state& state) noexcept {
	main_menu_base_t& base = *((main_menu_base_t*)(parent)); 
// BEGIN base::end_icon::update
// END
}
void main_menu_base_end_icon_t::on_create(sys::state& state) noexcept {
// BEGIN base::end_icon::create
// END
}
ui::message_result main_menu_base_t::on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	state.ui_state.drag_target = this;
	return ui::message_result::consumed;
}
ui::message_result main_menu_base_t::on_rbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	return ui::message_result::consumed;
}
void main_menu_base_t::on_hide(sys::state& state) noexcept {
// BEGIN base::on_hide
	if(state.user_setting_changed)
		state.save_user_settings();
	state.user_setting_changed = false;
// END
}
void main_menu_base_t::on_update(sys::state& state) noexcept {
// BEGIN base::update
// END
	remake_layout(state, true);
}
void main_menu_base_t::create_layout_level(sys::state& state, layout_level& lvl, char const* ldata, size_t sz) {
	serialization::in_buffer buffer(ldata, sz);
	buffer.read(lvl.size_x); 
	buffer.read(lvl.size_y); 
	buffer.read(lvl.margin_top); 
	buffer.read(lvl.margin_bottom); 
	buffer.read(lvl.margin_left); 
	buffer.read(lvl.margin_right); 
	buffer.read(lvl.line_alignment); 
	buffer.read(lvl.line_internal_alignment); 
	buffer.read(lvl.type); 
	buffer.read(lvl.page_animation); 
	buffer.read(lvl.interline_spacing); 
	buffer.read(lvl.paged); 
	if(lvl.paged) {
		lvl.page_controls = std::make_unique<page_buttons>();
		lvl.page_controls->for_layout = &lvl;
		lvl.page_controls->parent = this;
		lvl.page_controls->base_data.size.x = int16_t(grid_size * 10);
		lvl.page_controls->base_data.size.y = int16_t(grid_size * 2);
	}
	auto expansion_section = buffer.read_section();
	if(expansion_section)
		expansion_section.read(lvl.template_id);
	if(lvl.template_id == -1 && window_template != -1)
		lvl.template_id = int16_t(state.ui_templates.window_t[window_template].layout_region_definition);
	while(buffer) {
		layout_item_types t;
		buffer.read(t);
		switch(t) {
			case layout_item_types::texture_layer:
			{
				texture_layer temp;
				buffer.read(temp.texture_type);
				buffer.read(temp.texture);
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::control2:
			{
				layout_control temp;
				std::string_view cname = buffer.read<std::string_view>();
				buffer.read(temp.abs_x);
				buffer.read(temp.abs_y);
				buffer.read(temp.absolute_position);
				buffer.read(temp.fill_x);
				buffer.read(temp.fill_y);
				temp.ptr = nullptr;
				if(cname == "exit") {
					temp.ptr = exit.get();
				} else
				if(cname == "graphics_options") {
					temp.ptr = graphics_options.get();
				} else
				if(cname == "sound_options") {
					temp.ptr = sound_options.get();
				} else
				if(cname == "controls_options") {
					temp.ptr = controls_options.get();
				} else
				if(cname == "end_icon") {
					temp.ptr = end_icon.get();
				}
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::window2:
			{
				layout_window temp;
				std::string_view cname = buffer.read<std::string_view>();
				buffer.read(temp.abs_x);
				buffer.read(temp.abs_y);
				buffer.read(temp.absolute_position);
				buffer.read(temp.fill_x);
				buffer.read(temp.fill_y);
				if(cname == "base") {
					temp.ptr = make_main_menu_base(state);
				}
				if(cname == "sound") {
					temp.ptr = make_main_menu_sound(state);
				}
				if(cname == "misc") {
					temp.ptr = make_main_menu_misc(state);
				}
				if(cname == "string_dropdown_choice") {
					temp.ptr = make_main_menu_string_dropdown_choice(state);
				}
				if(cname == "graphics") {
					temp.ptr = make_main_menu_graphics(state);
				}
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::glue:
			{
				layout_glue temp;
				buffer.read(temp.type);
				buffer.read(temp.amount);
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::generator2:
			{
				generator_instance temp;
				std::string_view cname = buffer.read<std::string_view>();
				auto gen_details = buffer.read_section(); // ignored
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::layout:
			{
				sub_layout temp;
				temp.layout = std::make_unique<layout_level>();
				auto layout_section = buffer.read_section();
				create_layout_level(state, *temp.layout, layout_section.view_data() + layout_section.view_read_position(), layout_section.view_size() - layout_section.view_read_position());
				lvl.contents.emplace_back(std::move(temp));
			} break;
		}
	}
}
void main_menu_base_t::on_create(sys::state& state) noexcept {
	auto window_bytes = state.ui_state.new_ui_windows.find(std::string("main_menu::base"));
	if(window_bytes == state.ui_state.new_ui_windows.end()) std::abort();
	std::vector<sys::aui_pending_bytes> pending_children;
	auto win_data = read_window_bytes(window_bytes->second.data, window_bytes->second.size, pending_children);
	base_data.position.x = win_data.x_pos;
	base_data.position.y = win_data.y_pos;
	base_data.size.x = win_data.x_size;
	base_data.size.y = win_data.y_size;
	base_data.flags = uint8_t(win_data.orientation);
	layout_window_element::initialize_template(state, win_data.template_id, win_data.grid_size, win_data.auto_close_button);
	while(!pending_children.empty()) {
		auto child_data = read_child_bytes(pending_children.back().data, pending_children.back().size);
		if(child_data.name == "exit") {
			exit = std::make_unique<main_menu_base_exit_t>();
			exit->parent = this;
			auto cptr = exit.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "graphics_options") {
			graphics_options = std::make_unique<main_menu_base_graphics_options_t>();
			graphics_options->parent = this;
			auto cptr = graphics_options.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "sound_options") {
			sound_options = std::make_unique<main_menu_base_sound_options_t>();
			sound_options->parent = this;
			auto cptr = sound_options.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "controls_options") {
			controls_options = std::make_unique<main_menu_base_controls_options_t>();
			controls_options->parent = this;
			auto cptr = controls_options.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "end_icon") {
			end_icon = std::make_unique<main_menu_base_end_icon_t>();
			end_icon->parent = this;
			auto cptr = end_icon.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->texture_key = child_data.texture;
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} 
		
		pending_children.pop_back();
	}
	page_left_texture_key = win_data.page_left_texture;
	page_right_texture_key = win_data.page_right_texture;
	page_text_color = win_data.page_text_color;
	create_layout_level(state, layout, win_data.layout_data, win_data.layout_data_size);
// BEGIN base::create
// END
}
std::unique_ptr<ui::element_base> make_main_menu_base(sys::state& state) {
	auto ptr = std::make_unique<main_menu_base_t>();
	ptr->on_create(state);
	return ptr;
}
void main_menu_sound_master_volume_minimum_t::on_update(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::master_volume_minimum::update
// END
}
bool main_menu_sound_master_volume_minimum_t::button_action(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::master_volume_minimum::lbutton_action
	bool music_was_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;

	state.user_settings.master_volume = 0.0f;
	sound::change_music_volume(state, state.user_settings.music_volume * state.user_settings.master_volume);
	sound::change_effect_volume(state, state.user_settings.effects_volume * state.user_settings.master_volume);
	sound::change_interface_volume(state, state.user_settings.interface_volume * state.user_settings.master_volume);

	bool music_is_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;
	if(music_was_playing != music_is_playing) {
		if(music_is_playing)
			sound::start_music(state, state.user_settings.music_volume * state.user_settings.master_volume);
		else
			sound::stop_music(state);
	}
	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
void main_menu_sound_master_volume_less_t::on_update(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::master_volume_less::update
// END
}
bool main_menu_sound_master_volume_less_t::button_action(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::master_volume_less::lbutton_action
	bool music_was_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;

	state.user_settings.master_volume = std::clamp(state.user_settings.master_volume - 0.1f, 0.0f, 1.0f);
	sound::change_music_volume(state, state.user_settings.music_volume * state.user_settings.master_volume);
	sound::change_effect_volume(state, state.user_settings.effects_volume * state.user_settings.master_volume);
	sound::change_interface_volume(state, state.user_settings.interface_volume * state.user_settings.master_volume);

	bool music_is_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;
	if(music_was_playing != music_is_playing) {
		if(music_is_playing)
			sound::start_music(state, state.user_settings.music_volume * state.user_settings.master_volume);
		else
			sound::stop_music(state);
	}
	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
bool main_menu_sound_master_volume_less_t::button_shift_action(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::master_volume_less::lbutton_shift_action
	bool music_was_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;

	state.user_settings.master_volume = std::clamp(state.user_settings.master_volume - 0.01f, 0.0f, 1.0f);
	sound::change_music_volume(state, state.user_settings.music_volume * state.user_settings.master_volume);
	sound::change_effect_volume(state, state.user_settings.effects_volume * state.user_settings.master_volume);
	sound::change_interface_volume(state, state.user_settings.interface_volume * state.user_settings.master_volume);

	bool music_is_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;
	if(music_was_playing != music_is_playing) {
		if(music_is_playing)
			sound::start_music(state, state.user_settings.music_volume * state.user_settings.master_volume);
		else
			sound::stop_music(state);
	}
	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
void main_menu_sound_master_voume_amount_t::on_update(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::master_voume_amount::update
	set_text(state, std::to_string(int32_t(state.user_settings.master_volume * 100)));
// END
}
void main_menu_sound_master_volume_more_t::on_update(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::master_volume_more::update
// END
}
bool main_menu_sound_master_volume_more_t::button_action(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::master_volume_more::lbutton_action
	bool music_was_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;

	state.user_settings.master_volume = std::clamp(state.user_settings.master_volume + 0.1f, 0.0f, 1.0f);
	sound::change_music_volume(state, state.user_settings.music_volume * state.user_settings.master_volume);
	sound::change_effect_volume(state, state.user_settings.effects_volume * state.user_settings.master_volume);
	sound::change_interface_volume(state, state.user_settings.interface_volume * state.user_settings.master_volume);

	bool music_is_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;
	if(music_was_playing != music_is_playing) {
		if(music_is_playing)
			sound::start_music(state, state.user_settings.music_volume * state.user_settings.master_volume);
		else
			sound::stop_music(state);
	}
	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
bool main_menu_sound_master_volume_more_t::button_shift_action(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::master_volume_more::lbutton_shift_action
	bool music_was_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;

	state.user_settings.master_volume = std::clamp(state.user_settings.master_volume + 0.01f, 0.0f, 1.0f);
	sound::change_music_volume(state, state.user_settings.music_volume * state.user_settings.master_volume);
	sound::change_effect_volume(state, state.user_settings.effects_volume * state.user_settings.master_volume);
	sound::change_interface_volume(state, state.user_settings.interface_volume * state.user_settings.master_volume);

	bool music_is_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;
	if(music_was_playing != music_is_playing) {
		if(music_is_playing)
			sound::start_music(state, state.user_settings.music_volume * state.user_settings.master_volume);
		else
			sound::stop_music(state);
	}
	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
void main_menu_sound_master_volume_maximum_t::on_update(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::master_volume_maximum::update
// END
}
bool main_menu_sound_master_volume_maximum_t::button_action(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::master_volume_maximum::lbutton_action
	bool music_was_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;

	state.user_settings.master_volume = 1.0f;
	sound::change_music_volume(state, state.user_settings.music_volume * state.user_settings.master_volume);
	sound::change_effect_volume(state, state.user_settings.effects_volume * state.user_settings.master_volume);
	sound::change_interface_volume(state, state.user_settings.interface_volume * state.user_settings.master_volume);

	bool music_is_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;
	if(music_was_playing != music_is_playing) {
		if(music_is_playing)
			sound::start_music(state, state.user_settings.music_volume * state.user_settings.master_volume);
		else
			sound::stop_music(state);
	}
	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
void main_menu_sound_music_volume_minimum_t::on_update(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::music_volume_minimum::update
// END
}
bool main_menu_sound_music_volume_minimum_t::button_action(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::music_volume_minimum::lbutton_action
	bool music_was_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;

	state.user_settings.music_volume = 0.0f;
	sound::change_music_volume(state, state.user_settings.music_volume * state.user_settings.master_volume);

	bool music_is_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;
	if(music_was_playing != music_is_playing) {
		if(music_is_playing)
			sound::start_music(state, state.user_settings.music_volume * state.user_settings.master_volume);
		else
			sound::stop_music(state);
	}

	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
void main_menu_sound_music_volume_less_t::on_update(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::music_volume_less::update
// END
}
bool main_menu_sound_music_volume_less_t::button_action(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::music_volume_less::lbutton_action
	bool music_was_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;

	state.user_settings.music_volume = std::clamp(state.user_settings.music_volume - 0.1f, 0.0f, 1.0f);
	sound::change_music_volume(state, state.user_settings.music_volume * state.user_settings.master_volume);

	bool music_is_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;
	if(music_was_playing != music_is_playing) {
		if(music_is_playing)
			sound::start_music(state, state.user_settings.music_volume * state.user_settings.master_volume);
		else
			sound::stop_music(state);
	}

	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
bool main_menu_sound_music_volume_less_t::button_shift_action(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::music_volume_less::lbutton_shift_action
	bool music_was_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;

	state.user_settings.music_volume = std::clamp(state.user_settings.music_volume - 0.01f, 0.0f, 1.0f);
	sound::change_music_volume(state, state.user_settings.music_volume * state.user_settings.master_volume);

	bool music_is_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;
	if(music_was_playing != music_is_playing) {
		if(music_is_playing)
			sound::start_music(state, state.user_settings.music_volume * state.user_settings.master_volume);
		else
			sound::stop_music(state);
	}

	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
void main_menu_sound_music_volume_amount_t::on_update(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::music_volume_amount::update
	set_text(state, std::to_string(int32_t(state.user_settings.music_volume * 100.0f)));
// END
}
void main_menu_sound_music_volume_more_t::on_update(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::music_volume_more::update
// END
}
bool main_menu_sound_music_volume_more_t::button_action(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::music_volume_more::lbutton_action
	bool music_was_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;

	state.user_settings.music_volume = std::clamp(state.user_settings.music_volume + 0.1f, 0.0f, 1.0f);
	sound::change_music_volume(state, state.user_settings.music_volume * state.user_settings.master_volume);

	bool music_is_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;
	if(music_was_playing != music_is_playing) {
		if(music_is_playing)
			sound::start_music(state, state.user_settings.music_volume * state.user_settings.master_volume);
		else
			sound::stop_music(state);
	}

	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
bool main_menu_sound_music_volume_more_t::button_shift_action(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::music_volume_more::lbutton_shift_action
	bool music_was_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;

	state.user_settings.music_volume = std::clamp(state.user_settings.music_volume + 0.01f, 0.0f, 1.0f);
	sound::change_music_volume(state, state.user_settings.music_volume * state.user_settings.master_volume);

	bool music_is_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;
	if(music_was_playing != music_is_playing) {
		if(music_is_playing)
			sound::start_music(state, state.user_settings.music_volume * state.user_settings.master_volume);
		else
			sound::stop_music(state);
	}

	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
void main_menu_sound_music_volume_max_t::on_update(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::music_volume_max::update
// END
}
bool main_menu_sound_music_volume_max_t::button_action(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::music_volume_max::lbutton_action
	bool music_was_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;

	state.user_settings.music_volume = 1.0f;
	sound::change_music_volume(state, state.user_settings.music_volume * state.user_settings.master_volume);

	bool music_is_playing = state.user_settings.music_volume * state.user_settings.master_volume > 0;
	if(music_was_playing != music_is_playing) {
		if(music_is_playing)
			sound::start_music(state, state.user_settings.music_volume * state.user_settings.master_volume);
		else
			sound::stop_music(state);
	}

	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
void main_menu_sound_interface_volume_min_t::on_update(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::interface_volume_min::update
// END
}
bool main_menu_sound_interface_volume_min_t::button_action(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::interface_volume_min::lbutton_action
	state.user_settings.interface_volume = 0.0f;
	sound::change_interface_volume(state, state.user_settings.interface_volume * state.user_settings.master_volume);
	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
void main_menu_sound_interface_volume_less_t::on_update(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::interface_volume_less::update
// END
}
bool main_menu_sound_interface_volume_less_t::button_action(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::interface_volume_less::lbutton_action
	state.user_settings.interface_volume = std::clamp(state.user_settings.interface_volume - 0.1f, 0.0f, 1.0f);
	sound::change_interface_volume(state, state.user_settings.interface_volume * state.user_settings.master_volume);
	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
bool main_menu_sound_interface_volume_less_t::button_shift_action(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::interface_volume_less::lbutton_shift_action
	state.user_settings.interface_volume = std::clamp(state.user_settings.interface_volume - 0.01f, 0.0f, 1.0f);
	sound::change_interface_volume(state, state.user_settings.interface_volume * state.user_settings.master_volume);
	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
void main_menu_sound_interface_volume_amount_t::on_update(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::interface_volume_amount::update
	set_text(state, std::to_string(int32_t(state.user_settings.interface_volume * 100.0f)));
// END
}
void main_menu_sound_interface_volume_more_t::on_update(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::interface_volume_more::update
// END
}
bool main_menu_sound_interface_volume_more_t::button_action(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::interface_volume_more::lbutton_action
	state.user_settings.interface_volume = std::clamp(state.user_settings.interface_volume + 0.1f, 0.0f, 1.0f);
	sound::change_interface_volume(state, state.user_settings.interface_volume * state.user_settings.master_volume);
	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
bool main_menu_sound_interface_volume_more_t::button_shift_action(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::interface_volume_more::lbutton_shift_action
	state.user_settings.interface_volume = std::clamp(state.user_settings.interface_volume + 0.01f, 0.0f, 1.0f);
	sound::change_interface_volume(state, state.user_settings.interface_volume * state.user_settings.master_volume);
	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
void main_menu_sound_interface_volume_max_t::on_update(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::interface_volume_max::update
// END
}
bool main_menu_sound_interface_volume_max_t::button_action(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::interface_volume_max::lbutton_action
	state.user_settings.interface_volume = 1.0f;
	sound::change_interface_volume(state, state.user_settings.interface_volume * state.user_settings.master_volume);
	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
void main_menu_sound_mute_on_lose_focus_t::on_update(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::mute_on_lose_focus::update
	set_active(state, state.user_settings.mute_on_focus_lost);
// END
}
bool main_menu_sound_mute_on_lose_focus_t::button_action(sys::state& state) noexcept {
	main_menu_sound_t& sound = *((main_menu_sound_t*)(parent)); 
// BEGIN sound::mute_on_lose_focus::lbutton_action
	state.user_settings.mute_on_focus_lost = !state.user_settings.mute_on_focus_lost;
	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
ui::message_result main_menu_sound_t::on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	state.ui_state.drag_target = this;
	return ui::message_result::consumed;
}
ui::message_result main_menu_sound_t::on_rbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	return ui::message_result::consumed;
}
void main_menu_sound_t::on_hide(sys::state& state) noexcept {
// BEGIN sound::on_hide
	alice_ui::display_at_front<alice_ui::make_main_menu_base>(state);
// END
}
void main_menu_sound_t::on_update(sys::state& state) noexcept {
// BEGIN sound::update
// END
	remake_layout(state, true);
}
void main_menu_sound_t::create_layout_level(sys::state& state, layout_level& lvl, char const* ldata, size_t sz) {
	serialization::in_buffer buffer(ldata, sz);
	buffer.read(lvl.size_x); 
	buffer.read(lvl.size_y); 
	buffer.read(lvl.margin_top); 
	buffer.read(lvl.margin_bottom); 
	buffer.read(lvl.margin_left); 
	buffer.read(lvl.margin_right); 
	buffer.read(lvl.line_alignment); 
	buffer.read(lvl.line_internal_alignment); 
	buffer.read(lvl.type); 
	buffer.read(lvl.page_animation); 
	buffer.read(lvl.interline_spacing); 
	buffer.read(lvl.paged); 
	if(lvl.paged) {
		lvl.page_controls = std::make_unique<page_buttons>();
		lvl.page_controls->for_layout = &lvl;
		lvl.page_controls->parent = this;
		lvl.page_controls->base_data.size.x = int16_t(grid_size * 10);
		lvl.page_controls->base_data.size.y = int16_t(grid_size * 2);
	}
	auto expansion_section = buffer.read_section();
	if(expansion_section)
		expansion_section.read(lvl.template_id);
	if(lvl.template_id == -1 && window_template != -1)
		lvl.template_id = int16_t(state.ui_templates.window_t[window_template].layout_region_definition);
	while(buffer) {
		layout_item_types t;
		buffer.read(t);
		switch(t) {
			case layout_item_types::texture_layer:
			{
				texture_layer temp;
				buffer.read(temp.texture_type);
				buffer.read(temp.texture);
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::control2:
			{
				layout_control temp;
				std::string_view cname = buffer.read<std::string_view>();
				buffer.read(temp.abs_x);
				buffer.read(temp.abs_y);
				buffer.read(temp.absolute_position);
				buffer.read(temp.fill_x);
				buffer.read(temp.fill_y);
				temp.ptr = nullptr;
				if(cname == "window_title") {
					temp.ptr = window_title.get();
				} else
				if(cname == "master_volume_minimum") {
					temp.ptr = master_volume_minimum.get();
				} else
				if(cname == "master_volume_less") {
					temp.ptr = master_volume_less.get();
				} else
				if(cname == "master_voume_amount") {
					temp.ptr = master_voume_amount.get();
				} else
				if(cname == "master_volume_more") {
					temp.ptr = master_volume_more.get();
				} else
				if(cname == "master_volume_maximum") {
					temp.ptr = master_volume_maximum.get();
				} else
				if(cname == "music_volume_label") {
					temp.ptr = music_volume_label.get();
				} else
				if(cname == "master_volume_label") {
					temp.ptr = master_volume_label.get();
				} else
				if(cname == "music_volume_minimum") {
					temp.ptr = music_volume_minimum.get();
				} else
				if(cname == "music_volume_less") {
					temp.ptr = music_volume_less.get();
				} else
				if(cname == "music_volume_amount") {
					temp.ptr = music_volume_amount.get();
				} else
				if(cname == "music_volume_more") {
					temp.ptr = music_volume_more.get();
				} else
				if(cname == "music_volume_max") {
					temp.ptr = music_volume_max.get();
				} else
				if(cname == "interface_volume_label") {
					temp.ptr = interface_volume_label.get();
				} else
				if(cname == "interface_volume_min") {
					temp.ptr = interface_volume_min.get();
				} else
				if(cname == "interface_volume_less") {
					temp.ptr = interface_volume_less.get();
				} else
				if(cname == "interface_volume_amount") {
					temp.ptr = interface_volume_amount.get();
				} else
				if(cname == "interface_volume_more") {
					temp.ptr = interface_volume_more.get();
				} else
				if(cname == "interface_volume_max") {
					temp.ptr = interface_volume_max.get();
				} else
				if(cname == "divider") {
					temp.ptr = divider.get();
				} else
				if(cname == "mute_on_lose_focus") {
					temp.ptr = mute_on_lose_focus.get();
				} 
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::window2:
			{
				layout_window temp;
				std::string_view cname = buffer.read<std::string_view>();
				buffer.read(temp.abs_x);
				buffer.read(temp.abs_y);
				buffer.read(temp.absolute_position);
				buffer.read(temp.fill_x);
				buffer.read(temp.fill_y);
				if(cname == "base") {
					temp.ptr = make_main_menu_base(state);
				}
				if(cname == "sound") {
					temp.ptr = make_main_menu_sound(state);
				}
				if(cname == "misc") {
					temp.ptr = make_main_menu_misc(state);
				}
				if(cname == "string_dropdown_choice") {
					temp.ptr = make_main_menu_string_dropdown_choice(state);
				}
				if(cname == "graphics") {
					temp.ptr = make_main_menu_graphics(state);
				}
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::glue:
			{
				layout_glue temp;
				buffer.read(temp.type);
				buffer.read(temp.amount);
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::generator2:
			{
				generator_instance temp;
				std::string_view cname = buffer.read<std::string_view>();
				auto gen_details = buffer.read_section(); // ignored
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::layout:
			{
				sub_layout temp;
				temp.layout = std::make_unique<layout_level>();
				auto layout_section = buffer.read_section();
				create_layout_level(state, *temp.layout, layout_section.view_data() + layout_section.view_read_position(), layout_section.view_size() - layout_section.view_read_position());
				lvl.contents.emplace_back(std::move(temp));
			} break;
		}
	}
}
void main_menu_sound_t::on_create(sys::state& state) noexcept {
	auto window_bytes = state.ui_state.new_ui_windows.find(std::string("main_menu::sound"));
	if(window_bytes == state.ui_state.new_ui_windows.end()) std::abort();
	std::vector<sys::aui_pending_bytes> pending_children;
	auto win_data = read_window_bytes(window_bytes->second.data, window_bytes->second.size, pending_children);
	base_data.position.x = win_data.x_pos;
	base_data.position.y = win_data.y_pos;
	base_data.size.x = win_data.x_size;
	base_data.size.y = win_data.y_size;
	base_data.flags = uint8_t(win_data.orientation);
	layout_window_element::initialize_template(state, win_data.template_id, win_data.grid_size, win_data.auto_close_button);
	while(!pending_children.empty()) {
		auto child_data = read_child_bytes(pending_children.back().data, pending_children.back().size);
		if(child_data.name == "window_title") {
			window_title = std::make_unique<template_label>();
			window_title->parent = this;
			auto cptr = window_title.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "master_volume_minimum") {
			master_volume_minimum = std::make_unique<main_menu_sound_master_volume_minimum_t>();
			master_volume_minimum->parent = this;
			auto cptr = master_volume_minimum.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			cptr->icon = child_data.icon_id;
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "master_volume_less") {
			master_volume_less = std::make_unique<main_menu_sound_master_volume_less_t>();
			master_volume_less->parent = this;
			auto cptr = master_volume_less.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			cptr->icon = child_data.icon_id;
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "master_voume_amount") {
			master_voume_amount = std::make_unique<main_menu_sound_master_voume_amount_t>();
			master_voume_amount->parent = this;
			auto cptr = master_voume_amount.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "master_volume_more") {
			master_volume_more = std::make_unique<main_menu_sound_master_volume_more_t>();
			master_volume_more->parent = this;
			auto cptr = master_volume_more.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			cptr->icon = child_data.icon_id;
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "master_volume_maximum") {
			master_volume_maximum = std::make_unique<main_menu_sound_master_volume_maximum_t>();
			master_volume_maximum->parent = this;
			auto cptr = master_volume_maximum.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			cptr->icon = child_data.icon_id;
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "music_volume_label") {
			music_volume_label = std::make_unique<template_label>();
			music_volume_label->parent = this;
			auto cptr = music_volume_label.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "master_volume_label") {
			master_volume_label = std::make_unique<template_label>();
			master_volume_label->parent = this;
			auto cptr = master_volume_label.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "music_volume_minimum") {
			music_volume_minimum = std::make_unique<main_menu_sound_music_volume_minimum_t>();
			music_volume_minimum->parent = this;
			auto cptr = music_volume_minimum.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			cptr->icon = child_data.icon_id;
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "music_volume_less") {
			music_volume_less = std::make_unique<main_menu_sound_music_volume_less_t>();
			music_volume_less->parent = this;
			auto cptr = music_volume_less.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			cptr->icon = child_data.icon_id;
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "music_volume_amount") {
			music_volume_amount = std::make_unique<main_menu_sound_music_volume_amount_t>();
			music_volume_amount->parent = this;
			auto cptr = music_volume_amount.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "music_volume_more") {
			music_volume_more = std::make_unique<main_menu_sound_music_volume_more_t>();
			music_volume_more->parent = this;
			auto cptr = music_volume_more.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			cptr->icon = child_data.icon_id;
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "music_volume_max") {
			music_volume_max = std::make_unique<main_menu_sound_music_volume_max_t>();
			music_volume_max->parent = this;
			auto cptr = music_volume_max.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			cptr->icon = child_data.icon_id;
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "interface_volume_label") {
			interface_volume_label = std::make_unique<template_label>();
			interface_volume_label->parent = this;
			auto cptr = interface_volume_label.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "interface_volume_min") {
			interface_volume_min = std::make_unique<main_menu_sound_interface_volume_min_t>();
			interface_volume_min->parent = this;
			auto cptr = interface_volume_min.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			cptr->icon = child_data.icon_id;
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "interface_volume_less") {
			interface_volume_less = std::make_unique<main_menu_sound_interface_volume_less_t>();
			interface_volume_less->parent = this;
			auto cptr = interface_volume_less.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			cptr->icon = child_data.icon_id;
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "interface_volume_amount") {
			interface_volume_amount = std::make_unique<main_menu_sound_interface_volume_amount_t>();
			interface_volume_amount->parent = this;
			auto cptr = interface_volume_amount.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "interface_volume_more") {
			interface_volume_more = std::make_unique<main_menu_sound_interface_volume_more_t>();
			interface_volume_more->parent = this;
			auto cptr = interface_volume_more.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			cptr->icon = child_data.icon_id;
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "interface_volume_max") {
			interface_volume_max = std::make_unique<main_menu_sound_interface_volume_max_t>();
			interface_volume_max->parent = this;
			auto cptr = interface_volume_max.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			cptr->icon = child_data.icon_id;
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "divider") {
			divider = std::make_unique<template_bg_graphic>();
			divider->parent = this;
			auto cptr = divider.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "mute_on_lose_focus") {
			mute_on_lose_focus = std::make_unique<main_menu_sound_mute_on_lose_focus_t>();
			mute_on_lose_focus->parent = this;
			auto cptr = mute_on_lose_focus.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} 
		pending_children.pop_back();
	}
	page_left_texture_key = win_data.page_left_texture;
	page_right_texture_key = win_data.page_right_texture;
	page_text_color = win_data.page_text_color;
	create_layout_level(state, layout, win_data.layout_data, win_data.layout_data_size);
// BEGIN sound::create
// END
}
std::unique_ptr<ui::element_base> make_main_menu_sound(sys::state& state) {
	auto ptr = std::make_unique<main_menu_sound_t>();
	ptr->on_create(state);
	return ptr;
}
void main_menu_misc_language_dropdown_t::add_item( int32_t index,  std::string display_value) {
	list_contents.emplace_back(string_dropdown_choice_option{index, display_value});
	++total_items;
}
ui::element_base* main_menu_misc_language_dropdown_t::get_nth_item(sys::state& state, int32_t id, int32_t pool_id) {
	while(pool_id >= int32_t(list_pool.size())) {
		list_pool.emplace_back(static_cast<main_menu_string_dropdown_choice_t*>(make_main_menu_string_dropdown_choice(state).release()));
	}
	list_pool[pool_id]->index = list_contents[id].index; 
	list_pool[pool_id]->display_value = list_contents[id].display_value; 
	return list_pool[pool_id].get();
}
void main_menu_misc_language_dropdown_t::quiet_on_selection(sys::state& state, int32_t id) {
	if(id < 0 || id >= int32_t(list_contents.size())) return;
	selected_item = id;
	label_window_internal->index = list_contents[id].index; 
	label_window_internal->display_value = list_contents[id].display_value; 
	label_window_internal->impl_on_update(state); 
}
void main_menu_misc_language_dropdown_t::on_selection(sys::state& state, int32_t id) {
	quiet_on_selection(state, id);
	main_menu_misc_t& misc = *((main_menu_misc_t*)(parent)); 
// BEGIN misc::language_dropdown::on_selection
	window::change_cursor(state, window::cursor_type::busy);

	dcon::locale_id new_locale = dcon::locale_id{ dcon::locale_id::value_base_t(list_contents[id].index) };

	auto length = std::min(state.world.locale_get_locale_name(new_locale).size(), uint32_t(15));
	std::memcpy(state.user_settings.locale, state.world.locale_get_locale_name(new_locale).begin(), length);
	state.user_settings.locale[length] = 0;
	state.font_collection.change_locale(state, new_locale);


	state.ui_state.for_each_root([&](ui::element_base& elm) {
		elm.impl_on_reset_text(state);
	});

	state.game_state_updated.store(true, std::memory_order::release); //update ui

	state.user_setting_changed = true;
	window::change_cursor(state, window::cursor_type::normal);
// END
}
void main_menu_misc_language_dropdown_t::clear_list() {	list_contents.clear();
	total_items = 0;
}
void main_menu_misc_language_dropdown_t::on_update(sys::state& state) noexcept {
	main_menu_misc_t& misc = *((main_menu_misc_t*)(parent)); 
// BEGIN misc::language_dropdown::update
	clear_list();
	int32_t i = 0;
	for(auto loc : state.world.in_locale) {
		auto ln = state.world.locale_get_locale_name(loc);
		auto ln_sv = std::string_view{ (char const*)ln.begin(), ln.size() };
		add_item(loc.id.index(), std::string(ln_sv));
		if(state.font_collection.get_current_locale() == loc)
			quiet_on_selection(state, i);
		++i;
	}
// END
}
void main_menu_misc_language_dropdown_t::on_create(sys::state& state) noexcept {
	template_drop_down_control::on_create(state);
// BEGIN misc::language_dropdown::create
// END
}
void main_menu_misc_floating_tooltip_t::on_update(sys::state& state) noexcept {
	main_menu_misc_t& misc = *((main_menu_misc_t*)(parent)); 
// BEGIN misc::floating_tooltip::update
	set_active(state, state.user_settings.bind_tooltip_mouse);
// END
}
bool main_menu_misc_floating_tooltip_t::button_action(sys::state& state) noexcept {
	main_menu_misc_t& misc = *((main_menu_misc_t*)(parent)); 
// BEGIN misc::floating_tooltip::lbutton_action
	state.user_settings.bind_tooltip_mouse = !state.user_settings.bind_tooltip_mouse;
	state.user_setting_changed = true;
	parent->impl_on_update(state);
// END
	return true;
}
void main_menu_misc_edge_scrolling_t::on_update(sys::state& state) noexcept {
	main_menu_misc_t& misc = *((main_menu_misc_t*)(parent)); 
// BEGIN misc::edge_scrolling::update
	//TODO
	//set_active(state, state.user_settings.mouse_edge_scrolling);
// END
}
bool main_menu_misc_edge_scrolling_t::button_action(sys::state& state) noexcept {
	main_menu_misc_t& misc = *((main_menu_misc_t*)(parent)); 
// BEGIN misc::edge_scrolling::lbutton_action
	//state.user_settings.mouse_edge_scrolling = !state.user_settings.mouse_edge_scrolling;
	state.user_setting_changed = true;
	parent->impl_on_update(state);
// END
	return true;
}
ui::message_result main_menu_misc_t::on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	state.ui_state.drag_target = this;
	return ui::message_result::consumed;
}
ui::message_result main_menu_misc_t::on_rbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	return ui::message_result::consumed;
}
void main_menu_misc_t::on_hide(sys::state& state) noexcept {
// BEGIN misc::on_hide
	alice_ui::display_at_front<alice_ui::make_main_menu_base>(state);
// END
}
void main_menu_misc_t::on_update(sys::state& state) noexcept {
// BEGIN misc::update
// END
	remake_layout(state, true);
}
void main_menu_misc_t::create_layout_level(sys::state& state, layout_level& lvl, char const* ldata, size_t sz) {
	serialization::in_buffer buffer(ldata, sz);
	buffer.read(lvl.size_x); 
	buffer.read(lvl.size_y); 
	buffer.read(lvl.margin_top); 
	buffer.read(lvl.margin_bottom); 
	buffer.read(lvl.margin_left); 
	buffer.read(lvl.margin_right); 
	buffer.read(lvl.line_alignment); 
	buffer.read(lvl.line_internal_alignment); 
	buffer.read(lvl.type); 
	buffer.read(lvl.page_animation); 
	buffer.read(lvl.interline_spacing); 
	buffer.read(lvl.paged); 
	if(lvl.paged) {
		lvl.page_controls = std::make_unique<page_buttons>();
		lvl.page_controls->for_layout = &lvl;
		lvl.page_controls->parent = this;
		lvl.page_controls->base_data.size.x = int16_t(grid_size * 10);
		lvl.page_controls->base_data.size.y = int16_t(grid_size * 2);
	}
	auto expansion_section = buffer.read_section();
	if(expansion_section)
		expansion_section.read(lvl.template_id);
	if(lvl.template_id == -1 && window_template != -1)
		lvl.template_id = int16_t(state.ui_templates.window_t[window_template].layout_region_definition);
	while(buffer) {
		layout_item_types t;
		buffer.read(t);
		switch(t) {
			case layout_item_types::texture_layer:
			{
				texture_layer temp;
				buffer.read(temp.texture_type);
				buffer.read(temp.texture);
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::control2:
			{
				layout_control temp;
				std::string_view cname = buffer.read<std::string_view>();
				buffer.read(temp.abs_x);
				buffer.read(temp.abs_y);
				buffer.read(temp.absolute_position);
				buffer.read(temp.fill_x);
				buffer.read(temp.fill_y);
				temp.ptr = nullptr;
				if(cname == "window_title") {
					temp.ptr = window_title.get();
				} else
				if(cname == "language_label") {
					temp.ptr = language_label.get();
				} else
				if(cname == "language_dropdown") {
					temp.ptr = language_dropdown.get();
				} else
				if(cname == "floating_tooltip") {
					temp.ptr = floating_tooltip.get();
				} else
				if(cname == "edge_scrolling") {
					temp.ptr = edge_scrolling.get();
				}
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::window2:
			{
				layout_window temp;
				std::string_view cname = buffer.read<std::string_view>();
				buffer.read(temp.abs_x);
				buffer.read(temp.abs_y);
				buffer.read(temp.absolute_position);
				buffer.read(temp.fill_x);
				buffer.read(temp.fill_y);
				if(cname == "base") {
					temp.ptr = make_main_menu_base(state);
				}
				if(cname == "sound") {
					temp.ptr = make_main_menu_sound(state);
				}
				if(cname == "misc") {
					temp.ptr = make_main_menu_misc(state);
				}
				if(cname == "string_dropdown_choice") {
					temp.ptr = make_main_menu_string_dropdown_choice(state);
				}
				if(cname == "graphics") {
					temp.ptr = make_main_menu_graphics(state);
				}
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::glue:
			{
				layout_glue temp;
				buffer.read(temp.type);
				buffer.read(temp.amount);
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::generator2:
			{
				generator_instance temp;
				std::string_view cname = buffer.read<std::string_view>();
				auto gen_details = buffer.read_section(); // ignored
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::layout:
			{
				sub_layout temp;
				temp.layout = std::make_unique<layout_level>();
				auto layout_section = buffer.read_section();
				create_layout_level(state, *temp.layout, layout_section.view_data() + layout_section.view_read_position(), layout_section.view_size() - layout_section.view_read_position());
				lvl.contents.emplace_back(std::move(temp));
			} break;
		}
	}
}
void main_menu_misc_t::on_create(sys::state& state) noexcept {
	auto window_bytes = state.ui_state.new_ui_windows.find(std::string("main_menu::misc"));
	if(window_bytes == state.ui_state.new_ui_windows.end()) std::abort();
	std::vector<sys::aui_pending_bytes> pending_children;
	auto win_data = read_window_bytes(window_bytes->second.data, window_bytes->second.size, pending_children);
	base_data.position.x = win_data.x_pos;
	base_data.position.y = win_data.y_pos;
	base_data.size.x = win_data.x_size;
	base_data.size.y = win_data.y_size;
	base_data.flags = uint8_t(win_data.orientation);
	layout_window_element::initialize_template(state, win_data.template_id, win_data.grid_size, win_data.auto_close_button);
	while(!pending_children.empty()) {
		auto child_data = read_child_bytes(pending_children.back().data, pending_children.back().size);
		if(child_data.name == "window_title") {
			window_title = std::make_unique<template_label>();
			window_title->parent = this;
			auto cptr = window_title.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "language_label") {
			language_label = std::make_unique<template_label>();
			language_label->parent = this;
			auto cptr = language_label.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "language_dropdown") {
			language_dropdown = std::make_unique<main_menu_misc_language_dropdown_t>();
			language_dropdown->parent = this;
			auto cptr = language_dropdown.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			cptr->two_columns = (child_data.text_type != aui_text_type::body);
			cptr->target_page_height = child_data.border_size;
			cptr->label_window_internal = std::unique_ptr<main_menu_string_dropdown_choice_t>(static_cast<main_menu_string_dropdown_choice_t*>(make_main_menu_string_dropdown_choice(state).release()));
			cptr->element_x_size = cptr->label_window_internal->base_data.size.x;
			cptr->element_y_size = cptr->label_window_internal->base_data.size.y;
			cptr->label_window = cptr->label_window_internal.get();
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "floating_tooltip") {
			floating_tooltip = std::make_unique<main_menu_misc_floating_tooltip_t>();
			floating_tooltip->parent = this;
			auto cptr = floating_tooltip.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "edge_scrolling") {
			edge_scrolling = std::make_unique<main_menu_misc_edge_scrolling_t>();
			edge_scrolling->parent = this;
			auto cptr = edge_scrolling.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		}
		pending_children.pop_back();
	}
	page_left_texture_key = win_data.page_left_texture;
	page_right_texture_key = win_data.page_right_texture;
	page_text_color = win_data.page_text_color;
	create_layout_level(state, layout, win_data.layout_data, win_data.layout_data_size);
// BEGIN misc::create
// END
}
std::unique_ptr<ui::element_base> make_main_menu_misc(sys::state& state) {
	auto ptr = std::make_unique<main_menu_misc_t>();
	ptr->on_create(state);
	return ptr;
}
void main_menu_string_dropdown_choice_contents_t::on_update(sys::state& state) noexcept {
	main_menu_string_dropdown_choice_t& string_dropdown_choice = *((main_menu_string_dropdown_choice_t*)(parent)); 
// BEGIN string_dropdown_choice::contents::update
	set_text(state, string_dropdown_choice.display_value);
// END
}
ui::message_result main_menu_string_dropdown_choice_t::on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	return ui::message_result::consumed;
}
ui::message_result main_menu_string_dropdown_choice_t::on_rbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	return ui::message_result::consumed;
}
void main_menu_string_dropdown_choice_t::on_update(sys::state& state) noexcept {
// BEGIN string_dropdown_choice::update
// END
	remake_layout(state, true);
}
void main_menu_string_dropdown_choice_t::create_layout_level(sys::state& state, layout_level& lvl, char const* ldata, size_t sz) {
	serialization::in_buffer buffer(ldata, sz);
	buffer.read(lvl.size_x); 
	buffer.read(lvl.size_y); 
	buffer.read(lvl.margin_top); 
	buffer.read(lvl.margin_bottom); 
	buffer.read(lvl.margin_left); 
	buffer.read(lvl.margin_right); 
	buffer.read(lvl.line_alignment); 
	buffer.read(lvl.line_internal_alignment); 
	buffer.read(lvl.type); 
	buffer.read(lvl.page_animation); 
	buffer.read(lvl.interline_spacing); 
	buffer.read(lvl.paged); 
	if(lvl.paged) {
		lvl.page_controls = std::make_unique<page_buttons>();
		lvl.page_controls->for_layout = &lvl;
		lvl.page_controls->parent = this;
		lvl.page_controls->base_data.size.x = int16_t(grid_size * 10);
		lvl.page_controls->base_data.size.y = int16_t(grid_size * 2);
	}
	auto expansion_section = buffer.read_section();
	if(expansion_section)
		expansion_section.read(lvl.template_id);
	if(lvl.template_id == -1 && window_template != -1)
		lvl.template_id = int16_t(state.ui_templates.window_t[window_template].layout_region_definition);
	while(buffer) {
		layout_item_types t;
		buffer.read(t);
		switch(t) {
			case layout_item_types::texture_layer:
			{
				texture_layer temp;
				buffer.read(temp.texture_type);
				buffer.read(temp.texture);
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::control2:
			{
				layout_control temp;
				std::string_view cname = buffer.read<std::string_view>();
				buffer.read(temp.abs_x);
				buffer.read(temp.abs_y);
				buffer.read(temp.absolute_position);
				buffer.read(temp.fill_x);
				buffer.read(temp.fill_y);
				temp.ptr = nullptr;
				if(cname == "contents") {
					temp.ptr = contents.get();
				}
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::window2:
			{
				layout_window temp;
				std::string_view cname = buffer.read<std::string_view>();
				buffer.read(temp.abs_x);
				buffer.read(temp.abs_y);
				buffer.read(temp.absolute_position);
				buffer.read(temp.fill_x);
				buffer.read(temp.fill_y);
				if(cname == "base") {
					temp.ptr = make_main_menu_base(state);
				}
				if(cname == "sound") {
					temp.ptr = make_main_menu_sound(state);
				}
				if(cname == "misc") {
					temp.ptr = make_main_menu_misc(state);
				}
				if(cname == "string_dropdown_choice") {
					temp.ptr = make_main_menu_string_dropdown_choice(state);
				}
				if(cname == "graphics") {
					temp.ptr = make_main_menu_graphics(state);
				}
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::glue:
			{
				layout_glue temp;
				buffer.read(temp.type);
				buffer.read(temp.amount);
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::generator2:
			{
				generator_instance temp;
				std::string_view cname = buffer.read<std::string_view>();
				auto gen_details = buffer.read_section(); // ignored
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::layout:
			{
				sub_layout temp;
				temp.layout = std::make_unique<layout_level>();
				auto layout_section = buffer.read_section();
				create_layout_level(state, *temp.layout, layout_section.view_data() + layout_section.view_read_position(), layout_section.view_size() - layout_section.view_read_position());
				lvl.contents.emplace_back(std::move(temp));
			} break;
		}
	}
}
void main_menu_string_dropdown_choice_t::on_create(sys::state& state) noexcept {
	auto window_bytes = state.ui_state.new_ui_windows.find(std::string("main_menu::string_dropdown_choice"));
	if(window_bytes == state.ui_state.new_ui_windows.end()) std::abort();
	std::vector<sys::aui_pending_bytes> pending_children;
	auto win_data = read_window_bytes(window_bytes->second.data, window_bytes->second.size, pending_children);
	base_data.position.x = win_data.x_pos;
	base_data.position.y = win_data.y_pos;
	base_data.size.x = win_data.x_size;
	base_data.size.y = win_data.y_size;
	base_data.flags = uint8_t(win_data.orientation);
	layout_window_element::initialize_template(state, win_data.template_id, win_data.grid_size, win_data.auto_close_button);
	while(!pending_children.empty()) {
		auto child_data = read_child_bytes(pending_children.back().data, pending_children.back().size);
		if(child_data.name == "contents") {
			contents = std::make_unique<main_menu_string_dropdown_choice_contents_t>();
			contents->parent = this;
			auto cptr = contents.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		}
		pending_children.pop_back();
	}
	page_left_texture_key = win_data.page_left_texture;
	page_right_texture_key = win_data.page_right_texture;
	page_text_color = win_data.page_text_color;
	create_layout_level(state, layout, win_data.layout_data, win_data.layout_data_size);
// BEGIN string_dropdown_choice::create
// END
}
std::unique_ptr<ui::element_base> make_main_menu_string_dropdown_choice(sys::state& state) {
	auto ptr = std::make_unique<main_menu_string_dropdown_choice_t>();
	ptr->on_create(state);
	return ptr;
}
void main_menu_graphics_ui_scale_dropdown_t::add_item( int32_t index,  std::string display_value) {
	list_contents.emplace_back(string_dropdown_choice_option{index, display_value});
	++total_items;
}
ui::element_base* main_menu_graphics_ui_scale_dropdown_t::get_nth_item(sys::state& state, int32_t id, int32_t pool_id) {
	while(pool_id >= int32_t(list_pool.size())) {
		list_pool.emplace_back(static_cast<main_menu_string_dropdown_choice_t*>(make_main_menu_string_dropdown_choice(state).release()));
	}
	list_pool[pool_id]->index = list_contents[id].index; 
	list_pool[pool_id]->display_value = list_contents[id].display_value; 
	return list_pool[pool_id].get();
}
void main_menu_graphics_ui_scale_dropdown_t::quiet_on_selection(sys::state& state, int32_t id) {
	if(id < 0 || id >= int32_t(list_contents.size())) return;
	selected_item = id;
	label_window_internal->index = list_contents[id].index; 
	label_window_internal->display_value = list_contents[id].display_value; 
	label_window_internal->impl_on_update(state); 
}
void main_menu_graphics_ui_scale_dropdown_t::on_selection(sys::state& state, int32_t id) {
	quiet_on_selection(state, id);
	main_menu_graphics_t& graphics = *((main_menu_graphics_t*)(parent)); 
// BEGIN graphics::ui_scale_dropdown::on_selection
	state.update_ui_scale(sys::ui_scales[list_contents[id].index]);
	state.user_setting_changed = true;
// END
}
void main_menu_graphics_ui_scale_dropdown_t::clear_list() {	list_contents.clear();
	total_items = 0;
}
void main_menu_graphics_ui_scale_dropdown_t::on_update(sys::state& state) noexcept {
	main_menu_graphics_t& graphics = *((main_menu_graphics_t*)(parent)); 
// BEGIN graphics::ui_scale_dropdown::update
	for(size_t i = 0; i < sys::ui_scales_count; ++i) {
		if(state.user_settings.ui_scale >= sys::ui_scales[i] && (i + 1 == sys::ui_scales_count || state.user_settings.ui_scale < sys::ui_scales[i + 1])) {
			quiet_on_selection(state, int32_t(i));
		}
	}
// END
}
void main_menu_graphics_ui_scale_dropdown_t::on_create(sys::state& state) noexcept {
	template_drop_down_control::on_create(state);
// BEGIN graphics::ui_scale_dropdown::create
	for(size_t i = 0; i < sys::ui_scales_count; ++i) {
		add_item(int32_t(i), text::format_float(sys::ui_scales[i], 2));
	}
// END
}
void main_menu_graphics_window_mode_label_t::on_update(sys::state& state) noexcept {
	main_menu_graphics_t& graphics = *((main_menu_graphics_t*)(parent)); 
// BEGIN graphics::window_mode_label::update
	set_active(state, state.user_settings.prefer_fullscreen);
// END
}
bool main_menu_graphics_window_mode_label_t::button_action(sys::state& state) noexcept {
	main_menu_graphics_t& graphics = *((main_menu_graphics_t*)(parent)); 
// BEGIN graphics::window_mode_label::lbutton_action
	state.user_settings.prefer_fullscreen = !state.user_settings.prefer_fullscreen;
	window::set_borderless_full_screen(state, state.user_settings.prefer_fullscreen);
	parent->impl_on_update(state);
	state.user_setting_changed = true;
// END
	return true;
}
ui::message_result main_menu_graphics_t::on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	state.ui_state.drag_target = this;
	return ui::message_result::consumed;
}
ui::message_result main_menu_graphics_t::on_rbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	return ui::message_result::consumed;
}
void main_menu_graphics_t::on_hide(sys::state& state) noexcept {
// BEGIN graphics::on_hide
	alice_ui::display_at_front<alice_ui::make_main_menu_base>(state);
// END
}
void main_menu_graphics_t::on_update(sys::state& state) noexcept {
// BEGIN graphics::update
// END
	remake_layout(state, true);
}
void main_menu_graphics_t::create_layout_level(sys::state& state, layout_level& lvl, char const* ldata, size_t sz) {
	serialization::in_buffer buffer(ldata, sz);
	buffer.read(lvl.size_x); 
	buffer.read(lvl.size_y); 
	buffer.read(lvl.margin_top); 
	buffer.read(lvl.margin_bottom); 
	buffer.read(lvl.margin_left); 
	buffer.read(lvl.margin_right); 
	buffer.read(lvl.line_alignment); 
	buffer.read(lvl.line_internal_alignment); 
	buffer.read(lvl.type); 
	buffer.read(lvl.page_animation); 
	buffer.read(lvl.interline_spacing); 
	buffer.read(lvl.paged); 
	if(lvl.paged) {
		lvl.page_controls = std::make_unique<page_buttons>();
		lvl.page_controls->for_layout = &lvl;
		lvl.page_controls->parent = this;
		lvl.page_controls->base_data.size.x = int16_t(grid_size * 10);
		lvl.page_controls->base_data.size.y = int16_t(grid_size * 2);
	}
	auto expansion_section = buffer.read_section();
	if(expansion_section)
		expansion_section.read(lvl.template_id);
	if(lvl.template_id == -1 && window_template != -1)
		lvl.template_id = int16_t(state.ui_templates.window_t[window_template].layout_region_definition);
	while(buffer) {
		layout_item_types t;
		buffer.read(t);
		switch(t) {
			case layout_item_types::texture_layer:
			{
				texture_layer temp;
				buffer.read(temp.texture_type);
				buffer.read(temp.texture);
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::control2:
			{
				layout_control temp;
				std::string_view cname = buffer.read<std::string_view>();
				buffer.read(temp.abs_x);
				buffer.read(temp.abs_y);
				buffer.read(temp.absolute_position);
				buffer.read(temp.fill_x);
				buffer.read(temp.fill_y);
				temp.ptr = nullptr;
				if(cname == "title") {
					temp.ptr = title.get();
				} else
				if(cname == "ui_scale_label") {
					temp.ptr = ui_scale_label.get();
				} else
				if(cname == "ui_scale_dropdown") {
					temp.ptr = ui_scale_dropdown.get();
				} else
				if(cname == "window_mode_label") {
					temp.ptr = window_mode_label.get();
				}
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::window2:
			{
				layout_window temp;
				std::string_view cname = buffer.read<std::string_view>();
				buffer.read(temp.abs_x);
				buffer.read(temp.abs_y);
				buffer.read(temp.absolute_position);
				buffer.read(temp.fill_x);
				buffer.read(temp.fill_y);
				if(cname == "base") {
					temp.ptr = make_main_menu_base(state);
				}
				if(cname == "sound") {
					temp.ptr = make_main_menu_sound(state);
				}
				if(cname == "misc") {
					temp.ptr = make_main_menu_misc(state);
				}
				if(cname == "string_dropdown_choice") {
					temp.ptr = make_main_menu_string_dropdown_choice(state);
				}
				if(cname == "graphics") {
					temp.ptr = make_main_menu_graphics(state);
				}
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::glue:
			{
				layout_glue temp;
				buffer.read(temp.type);
				buffer.read(temp.amount);
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::generator2:
			{
				generator_instance temp;
				std::string_view cname = buffer.read<std::string_view>();
				auto gen_details = buffer.read_section(); // ignored
				lvl.contents.emplace_back(std::move(temp));
			} break;
			case layout_item_types::layout:
			{
				sub_layout temp;
				temp.layout = std::make_unique<layout_level>();
				auto layout_section = buffer.read_section();
				create_layout_level(state, *temp.layout, layout_section.view_data() + layout_section.view_read_position(), layout_section.view_size() - layout_section.view_read_position());
				lvl.contents.emplace_back(std::move(temp));
			} break;
		}
	}
}
void main_menu_graphics_t::on_create(sys::state& state) noexcept {
	auto window_bytes = state.ui_state.new_ui_windows.find(std::string("main_menu::graphics"));
	if(window_bytes == state.ui_state.new_ui_windows.end()) std::abort();
	std::vector<sys::aui_pending_bytes> pending_children;
	auto win_data = read_window_bytes(window_bytes->second.data, window_bytes->second.size, pending_children);
	base_data.position.x = win_data.x_pos;
	base_data.position.y = win_data.y_pos;
	base_data.size.x = win_data.x_size;
	base_data.size.y = win_data.y_size;
	base_data.flags = uint8_t(win_data.orientation);
	layout_window_element::initialize_template(state, win_data.template_id, win_data.grid_size, win_data.auto_close_button);
	while(!pending_children.empty()) {
		auto child_data = read_child_bytes(pending_children.back().data, pending_children.back().size);
		if(child_data.name == "title") {
			title = std::make_unique<template_label>();
			title->parent = this;
			auto cptr = title.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "ui_scale_label") {
			ui_scale_label = std::make_unique<template_label>();
			ui_scale_label->parent = this;
			auto cptr = ui_scale_label.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "ui_scale_dropdown") {
			ui_scale_dropdown = std::make_unique<main_menu_graphics_ui_scale_dropdown_t>();
			ui_scale_dropdown->parent = this;
			auto cptr = ui_scale_dropdown.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			cptr->two_columns = (child_data.text_type != aui_text_type::body);
			cptr->target_page_height = child_data.border_size;
			cptr->label_window_internal = std::unique_ptr<main_menu_string_dropdown_choice_t>(static_cast<main_menu_string_dropdown_choice_t*>(make_main_menu_string_dropdown_choice(state).release()));
			cptr->element_x_size = cptr->label_window_internal->base_data.size.x;
			cptr->element_y_size = cptr->label_window_internal->base_data.size.y;
			cptr->label_window = cptr->label_window_internal.get();
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		if(child_data.name == "window_mode_label") {
			window_mode_label = std::make_unique<main_menu_graphics_window_mode_label_t>();
			window_mode_label->parent = this;
			auto cptr = window_mode_label.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			if(child_data.text_key.length() > 0)
				cptr->default_text = state.lookup_key(child_data.text_key);
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		}
		pending_children.pop_back();
	}
	page_left_texture_key = win_data.page_left_texture;
	page_right_texture_key = win_data.page_right_texture;
	page_text_color = win_data.page_text_color;
	create_layout_level(state, layout, win_data.layout_data, win_data.layout_data_size);
// BEGIN graphics::create
// END
}
std::unique_ptr<ui::element_base> make_main_menu_graphics(sys::state& state) {
	auto ptr = std::make_unique<main_menu_graphics_t>();
	ptr->on_create(state);
	return ptr;
}
// LOST-CODE
}
