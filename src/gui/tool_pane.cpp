// BEGIN prelude
#include "alice_ui.hpp"
#include "system_state.hpp"
#include "gui_deserialize.hpp"
// END

namespace alice_ui {
struct tool_pane_main_panel_t;
struct tool_pane_basic_component_button_t;
struct tool_pane_basic_component_t;
struct tool_pane_wire_component_button_t;
struct tool_pane_wire_component_t;
struct tool_pane_main_panel_basic_component_list_t : public layout_generator {
// BEGIN main_panel::basic_component_list::variables
// END
	struct basic_component_option { sys::basic_component_type type; };
	std::vector<std::unique_ptr<ui::element_base>> basic_component_pool;
	int32_t basic_component_pool_used = 0;
	void add_basic_component( sys::basic_component_type type);
	struct wire_component_option { };
	std::vector<std::unique_ptr<ui::element_base>> wire_component_pool;
	int32_t wire_component_pool_used = 0;
	void add_wire_component();
	std::vector<std::variant<std::monostate, basic_component_option, wire_component_option>> values;
	void on_create(sys::state& state, layout_window_element* container);
	void update(sys::state& state, layout_window_element* container);
	measure_result place_item(sys::state& state, ui::non_owning_container_base* destination, size_t index, int32_t x, int32_t y, bool first_in_section, bool& alternate) override;
	size_t item_count() override { return values.size(); };
	void reset_pools() override;
};
struct tool_pane_basic_component_button_t : public alice_ui::template_icon_button_ci {
// BEGIN basic_component::button::variables
// END
	ui::tooltip_behavior has_tooltip(sys::state & state) noexcept override {
		return ui::tooltip_behavior::variable_tooltip;
	}
	void update_tooltip(sys::state& state, int32_t x, int32_t y, text::columnar_layout& contents) noexcept override;
	bool button_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct tool_pane_wire_component_button_t : public alice_ui::template_icon_button_ci {
// BEGIN wire_component::button::variables
// END
	bool button_action(sys::state& state) noexcept override;
	bool button_right_action(sys::state& state) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
struct tool_pane_main_panel_t : public layout_window_element {
// BEGIN main_panel::variables
// END
	std::unique_ptr<template_label> basic_label;
	tool_pane_main_panel_basic_component_list_t basic_component_list;
	std::vector<std::unique_ptr<ui::element_base>> gui_inserts;
	void create_layout_level(sys::state& state, layout_level& lvl, char const* ldata, size_t sz);
	void on_create(sys::state& state) noexcept override;
	ui::message_result on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override;
	ui::message_result on_rbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
std::unique_ptr<ui::element_base> make_tool_pane_main_panel(sys::state& state);
struct tool_pane_basic_component_t : public layout_window_element {
// BEGIN basic_component::variables
// END
	sys::basic_component_type type;
	std::unique_ptr<tool_pane_basic_component_button_t> button;
	std::vector<std::unique_ptr<ui::element_base>> gui_inserts;
	void create_layout_level(sys::state& state, layout_level& lvl, char const* ldata, size_t sz);
	void on_create(sys::state& state) noexcept override;
	void set_alternate(bool alt) noexcept;
	ui::message_result on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override;
	ui::message_result on_rbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override;
	void on_update(sys::state& state) noexcept override;
	void* get_by_name(sys::state& state, std::string_view name_parameter) noexcept override {
		if(name_parameter == "type") {
			return (void*)(&type);
		}
		return nullptr;
	}
};
std::unique_ptr<ui::element_base> make_tool_pane_basic_component(sys::state& state);
struct tool_pane_wire_component_t : public layout_window_element {
// BEGIN wire_component::variables
// END
	std::unique_ptr<tool_pane_wire_component_button_t> button;
	std::vector<std::unique_ptr<ui::element_base>> gui_inserts;
	void create_layout_level(sys::state& state, layout_level& lvl, char const* ldata, size_t sz);
	void on_create(sys::state& state) noexcept override;
	void set_alternate(bool alt) noexcept;
	ui::message_result on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override;
	ui::message_result on_rbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept override;
	void on_update(sys::state& state) noexcept override;
};
std::unique_ptr<ui::element_base> make_tool_pane_wire_component(sys::state& state);
void tool_pane_main_panel_basic_component_list_t::add_basic_component(sys::basic_component_type type) {
	values.emplace_back(basic_component_option{type});
}
void tool_pane_main_panel_basic_component_list_t::add_wire_component() {
	values.emplace_back(wire_component_option{});
}
void  tool_pane_main_panel_basic_component_list_t::on_create(sys::state& state, layout_window_element* parent) {
	tool_pane_main_panel_t& main_panel = *((tool_pane_main_panel_t*)(parent)); 
// BEGIN main_panel::basic_component_list::on_create
	add_basic_component(sys::basic_component_type::diode);
	add_basic_component(sys::basic_component_type::enable_high_transistor);
	add_basic_component(sys::basic_component_type::enable_low_transistor);
	add_wire_component();
// END
}
void  tool_pane_main_panel_basic_component_list_t::update(sys::state& state, layout_window_element* parent) {
	tool_pane_main_panel_t& main_panel = *((tool_pane_main_panel_t*)(parent)); 
// BEGIN main_panel::basic_component_list::update
// END
}
measure_result  tool_pane_main_panel_basic_component_list_t::place_item(sys::state& state, ui::non_owning_container_base* destination, size_t index, int32_t x, int32_t y, bool first_in_section, bool& alternate) {
	if(index >= values.size()) return measure_result{0,0,measure_result::special::none};
	if(std::holds_alternative<basic_component_option>(values[index])) {
		if(basic_component_pool.empty()) basic_component_pool.emplace_back(make_tool_pane_basic_component(state));
		if(destination) {
			if(basic_component_pool.size() <= size_t(basic_component_pool_used)) basic_component_pool.emplace_back(make_tool_pane_basic_component(state));
			basic_component_pool[basic_component_pool_used]->base_data.position.x = int16_t(x);
			basic_component_pool[basic_component_pool_used]->base_data.position.y = int16_t(y);
			basic_component_pool[basic_component_pool_used]->parent = destination;
			destination->children.push_back(basic_component_pool[basic_component_pool_used].get());
			((tool_pane_basic_component_t*)(basic_component_pool[basic_component_pool_used].get()))->type = std::get<basic_component_option>(values[index]).type;
			((tool_pane_basic_component_t*)(basic_component_pool[basic_component_pool_used].get()))->set_alternate(alternate);
			basic_component_pool[basic_component_pool_used]->impl_on_update(state);
			basic_component_pool_used++;
		}
		alternate = !alternate;
	 	 	bool stick_to_next = false;
		return measure_result{ basic_component_pool[0]->base_data.size.x, basic_component_pool[0]->base_data.size.y + 0, stick_to_next ? measure_result::special::no_break : measure_result::special::none};
	}
	if(std::holds_alternative<wire_component_option>(values[index])) {
		if(wire_component_pool.empty()) wire_component_pool.emplace_back(make_tool_pane_wire_component(state));
		if(destination) {
			if(wire_component_pool.size() <= size_t(wire_component_pool_used)) wire_component_pool.emplace_back(make_tool_pane_wire_component(state));
			wire_component_pool[wire_component_pool_used]->base_data.position.x = int16_t(x);
			wire_component_pool[wire_component_pool_used]->base_data.position.y = int16_t(y);
			wire_component_pool[wire_component_pool_used]->parent = destination;
			destination->children.push_back(wire_component_pool[wire_component_pool_used].get());
			((tool_pane_wire_component_t*)(wire_component_pool[wire_component_pool_used].get()))->set_alternate(alternate);
			wire_component_pool[wire_component_pool_used]->impl_on_update(state);
			wire_component_pool_used++;
		}
		alternate = !alternate;
	 	 	bool stick_to_next = false;
		return measure_result{ wire_component_pool[0]->base_data.size.x, wire_component_pool[0]->base_data.size.y + 0, stick_to_next ? measure_result::special::no_break : measure_result::special::none};
	}
	return measure_result{0,0,measure_result::special::none};
}
void  tool_pane_main_panel_basic_component_list_t::reset_pools() {
	basic_component_pool_used = 0;
	wire_component_pool_used = 0;
}
ui::message_result tool_pane_main_panel_t::on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	return ui::message_result::consumed;
}
ui::message_result tool_pane_main_panel_t::on_rbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	return ui::message_result::consumed;
}
void tool_pane_main_panel_t::on_update(sys::state& state) noexcept {
// BEGIN main_panel::update
// END
	basic_component_list.update(state, this);
	remake_layout(state, true);
}
void tool_pane_main_panel_t::create_layout_level(sys::state& state, layout_level& lvl, char const* ldata, size_t sz) {
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
				if(cname == "basic_label") {
					temp.ptr = basic_label.get();
				} else
				{
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
				if(cname == "main_panel") {
					temp.ptr = make_tool_pane_main_panel(state);
				}
				if(cname == "basic_component") {
					temp.ptr = make_tool_pane_basic_component(state);
				}
				if(cname == "wire_component") {
					temp.ptr = make_tool_pane_wire_component(state);
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
				if(cname == "basic_component_list") {
					temp.generator = &basic_component_list;
				}
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
void tool_pane_main_panel_t::on_create(sys::state& state) noexcept {
	auto window_bytes = state.ui_state.new_ui_windows.find(std::string("tool_pane::main_panel"));
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
		if(child_data.name == "basic_label") {
			basic_label = std::make_unique<template_label>();
			basic_label->parent = this;
			auto cptr = basic_label.get();
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
		{ } 
		pending_children.pop_back();
	}
	basic_component_list.on_create(state, this);
	page_left_texture_key = win_data.page_left_texture;
	page_right_texture_key = win_data.page_right_texture;
	page_text_color = win_data.page_text_color;
	create_layout_level(state, layout, win_data.layout_data, win_data.layout_data_size);
// BEGIN main_panel::create
// END
}
std::unique_ptr<ui::element_base> make_tool_pane_main_panel(sys::state& state) {
	auto ptr = std::make_unique<tool_pane_main_panel_t>();
	ptr->on_create(state);
	return ptr;
}
void tool_pane_basic_component_button_t::update_tooltip(sys::state& state, int32_t x, int32_t y, text::columnar_layout& contents) noexcept {
	tool_pane_basic_component_t& basic_component = *((tool_pane_basic_component_t*)(parent)); 
	tool_pane_main_panel_t& main_panel = *((tool_pane_main_panel_t*)(parent->parent)); 
// BEGIN basic_component::button::tooltip
	text::add_line(state, contents, basic_component_to_name(basic_component.type));
// END
}
void tool_pane_basic_component_button_t::on_update(sys::state& state) noexcept {
	tool_pane_basic_component_t& basic_component = *((tool_pane_basic_component_t*)(parent)); 
	tool_pane_main_panel_t& main_panel = *((tool_pane_main_panel_t*)(parent->parent)); 
// BEGIN basic_component::button::update
	icon = basic_component_to_icon(state, basic_component.type);
	icon_color = state.ui_templates.colors[basic_component_to_color(state, basic_component.type)];
// END
}
bool tool_pane_basic_component_button_t::button_action(sys::state& state) noexcept {
	tool_pane_basic_component_t& basic_component = *((tool_pane_basic_component_t*)(parent)); 
	tool_pane_main_panel_t& main_panel = *((tool_pane_main_panel_t*)(parent->parent)); 
// BEGIN basic_component::button::lbutton_action
	if(std::holds_alternative<sys::basic_component_type>(state.active_component_tool) && std::get<sys::basic_component_type>(state.active_component_tool) == basic_component.type) {
		state.active_component_tool = std::monostate{};
	} else {
		state.active_component_tool = basic_component.type;
	}
	state.game_state_updated.store(true, std::memory_order_release);
// END
	return true;
}
void  tool_pane_basic_component_t::set_alternate(bool alt) noexcept {
	window_template = alt ? -1 : 2;
	button->template_id = alt ? 3 : 2;
}
ui::message_result tool_pane_basic_component_t::on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	return ui::message_result::consumed;
}
ui::message_result tool_pane_basic_component_t::on_rbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	return ui::message_result::consumed;
}
void tool_pane_basic_component_t::on_update(sys::state& state) noexcept {
	tool_pane_main_panel_t& main_panel = *((tool_pane_main_panel_t*)(parent->parent)); 
// BEGIN basic_component::update
	if(std::holds_alternative<sys::basic_component_type>(state.active_component_tool) && std::get<sys::basic_component_type>(state.active_component_tool) == type) {
		set_alternate(true);
	} else {
		set_alternate(false);
	}
// END
	remake_layout(state, true);
}
void tool_pane_basic_component_t::create_layout_level(sys::state& state, layout_level& lvl, char const* ldata, size_t sz) {
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
				if(cname == "button") {
					temp.ptr = button.get();
				} else
				{
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
				if(cname == "main_panel") {
					temp.ptr = make_tool_pane_main_panel(state);
				}
				if(cname == "basic_component") {
					temp.ptr = make_tool_pane_basic_component(state);
				}
				if(cname == "wire_component") {
					temp.ptr = make_tool_pane_wire_component(state);
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
void tool_pane_basic_component_t::on_create(sys::state& state) noexcept {
	auto window_bytes = state.ui_state.new_ui_windows.find(std::string("tool_pane::basic_component"));
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
		if(child_data.name == "button") {
			button = std::make_unique<tool_pane_basic_component_button_t>();
			button->parent = this;
			auto cptr = button.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			cptr->icon = child_data.icon_id;
			cptr->icon_color = child_data.table_divider_color;
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		{ } 
		pending_children.pop_back();
	}
	page_left_texture_key = win_data.page_left_texture;
	page_right_texture_key = win_data.page_right_texture;
	page_text_color = win_data.page_text_color;
	create_layout_level(state, layout, win_data.layout_data, win_data.layout_data_size);
// BEGIN basic_component::create
// END
}
std::unique_ptr<ui::element_base> make_tool_pane_basic_component(sys::state& state) {
	auto ptr = std::make_unique<tool_pane_basic_component_t>();
	ptr->on_create(state);
	return ptr;
}
void tool_pane_wire_component_button_t::on_update(sys::state& state) noexcept {
	tool_pane_wire_component_t& wire_component = *((tool_pane_wire_component_t*)(parent)); 
	tool_pane_main_panel_t& main_panel = *((tool_pane_main_panel_t*)(parent->parent)); 
// BEGIN wire_component::button::update
	icon_color = state.ui_templates.colors[int32_t(state.current_wire_color) * 2 + 1];
// END
}
bool tool_pane_wire_component_button_t::button_action(sys::state& state) noexcept {
	tool_pane_wire_component_t& wire_component = *((tool_pane_wire_component_t*)(parent)); 
	tool_pane_main_panel_t& main_panel = *((tool_pane_main_panel_t*)(parent->parent)); 
// BEGIN wire_component::button::lbutton_action
	if(std::holds_alternative<sys::wire>(state.active_component_tool)) {
		if(uint8_t(state.current_wire_color) < sys::max_wire_color) {
			state.current_wire_color = sys::wire_colors(int32_t(state.current_wire_color) + 1);
		} else {
			state.current_wire_color = sys::wire_colors(0);
		}
	} else {
		state.active_component_tool = sys::wire{};
	}
	state.game_state_updated.store(true, std::memory_order_release);
// END
	return true;
}
bool tool_pane_wire_component_button_t::button_right_action(sys::state& state) noexcept {
	tool_pane_wire_component_t& wire_component = *((tool_pane_wire_component_t*)(parent)); 
	tool_pane_main_panel_t& main_panel = *((tool_pane_main_panel_t*)(parent->parent)); 
// BEGIN wire_component::button::rbutton_action
	if(int32_t(state.current_wire_color) > 0) {
		state.current_wire_color = sys::wire_colors(int32_t(state.current_wire_color) - 1);
	} else {
		state.current_wire_color = sys::wire_colors(sys::max_wire_color);
	}
	state.game_state_updated.store(true, std::memory_order_release);
// END
	return true;
}
void  tool_pane_wire_component_t::set_alternate(bool alt) noexcept {
	window_template = alt ? -1 : 2;
	button->template_id = alt ? 3 : 2;
}
ui::message_result tool_pane_wire_component_t::on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	return ui::message_result::consumed;
}
ui::message_result tool_pane_wire_component_t::on_rbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	return ui::message_result::consumed;
}
void tool_pane_wire_component_t::on_update(sys::state& state) noexcept {
	tool_pane_main_panel_t& main_panel = *((tool_pane_main_panel_t*)(parent->parent)); 
// BEGIN wire_component::update
	if(std::holds_alternative<sys::wire>(state.active_component_tool)) {
		set_alternate(true);
	} else {
		set_alternate(false);
	}
// END
	remake_layout(state, true);
}
void tool_pane_wire_component_t::create_layout_level(sys::state& state, layout_level& lvl, char const* ldata, size_t sz) {
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
				if(cname == "button") {
					temp.ptr = button.get();
				} else
				{
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
				if(cname == "main_panel") {
					temp.ptr = make_tool_pane_main_panel(state);
				}
				if(cname == "basic_component") {
					temp.ptr = make_tool_pane_basic_component(state);
				}
				if(cname == "wire_component") {
					temp.ptr = make_tool_pane_wire_component(state);
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
void tool_pane_wire_component_t::on_create(sys::state& state) noexcept {
	auto window_bytes = state.ui_state.new_ui_windows.find(std::string("tool_pane::wire_component"));
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
		if(child_data.name == "button") {
			button = std::make_unique<tool_pane_wire_component_button_t>();
			button->parent = this;
			auto cptr = button.get();
			cptr->base_data.position.x = child_data.x_pos;
			cptr->base_data.position.y = child_data.y_pos;
			cptr->base_data.size.x = child_data.x_size;
			cptr->base_data.size.y = child_data.y_size;
			cptr->template_id = child_data.template_id;
			cptr->icon = child_data.icon_id;
			cptr->icon_color = child_data.table_divider_color;
			if(child_data.tooltip_text_key.length() > 0)
				cptr->default_tooltip = state.lookup_key(child_data.tooltip_text_key);
			cptr->parent = this;
			cptr->on_create(state);
			children.push_back(cptr);
			pending_children.pop_back(); continue;
		} else 
		{ } 
		pending_children.pop_back();
	}
	page_left_texture_key = win_data.page_left_texture;
	page_right_texture_key = win_data.page_right_texture;
	page_text_color = win_data.page_text_color;
	create_layout_level(state, layout, win_data.layout_data, win_data.layout_data_size);
// BEGIN wire_component::create
// END
}
std::unique_ptr<ui::element_base> make_tool_pane_wire_component(sys::state& state) {
	auto ptr = std::make_unique<tool_pane_wire_component_t>();
	ptr->on_create(state);
	return ptr;
}
// LOST-CODE
}
