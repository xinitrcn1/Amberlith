#include <numbers>
#include <cmath>

#include "system_state.hpp"
#include "gui_element_base.hpp"
#include "gui_element_types.hpp"
#include "gui_deserialize.hpp"
#include "alice_ui.hpp"
#include "uitemplate.hpp"

namespace alice_ui {

bool state_is_rtl(sys::state& state) {
	return state.world.locale_get_native_rtl(state.font_collection.get_current_locale());
}

ogl::animation::type to_ogl_type(animation_type type, bool forwards) {
	switch(type) {
	case animation_type::page_left:
		return forwards ? ogl::animation::type::page_flip_left : ogl::animation::type::page_flip_left_rev;
	case animation_type::page_right:
		return forwards ? ogl::animation::type::page_flip_right : ogl::animation::type::page_flip_right_rev;
	case animation_type::page_up:
		return forwards ? ogl::animation::type::page_flip_up : ogl::animation::type::page_flip_up_rev;
	case animation_type::none:
		return ogl::animation::type::page_flip_left;
	case animation_type::page_middle:
		return forwards ? ogl::animation::type::page_flip_mid : ogl::animation::type::page_flip_mid_rev;
	default:
		return ogl::animation::type::page_flip_left;
	}
}

void layout_level::change_page(sys::state& state, layout_window_element& container, int32_t new_page) {
	if(page_animation != animation_type::none) {
		bool lflip = new_page < current_page && current_page > 0;
		bool rflip = new_page > current_page && current_page < int32_t(page_starts.size() - 1);
		if(rflip) {
			auto pos = ui::get_absolute_location(state, container);
			state.ui_animation.start_animation(state, pos.x + resolved_x_pos, pos.y + resolved_y_pos, resolved_x_size, resolved_y_size, to_ogl_type(page_animation, true), 220);
		} else if(lflip) {
			auto pos = ui::get_absolute_location(state, container);
			state.ui_animation.start_animation(state, pos.x + resolved_x_pos, pos.y + resolved_y_pos, resolved_x_size, resolved_y_size, to_ogl_type(page_animation, false), 220);
		}

		current_page = int16_t(std::clamp(new_page, 0, int32_t(page_starts.size() - 1)));

		if(rflip || lflip) {
			container.remake_layout(state, false);
			state.ui_animation.post_update_frame(state);
		}
	} else if(new_page != current_page) {
		current_page = int16_t(std::clamp(new_page, 0, int32_t(page_starts.size() - 1)));
		container.remake_layout(state, false);
	}
}


void template_icon_graphic::update_tooltip(sys::state& state, int32_t x, int32_t y, text::columnar_layout& contents) noexcept {
	text::add_line(state, contents, default_tooltip);
}
void template_icon_graphic::render(sys::state& state, int32_t x, int32_t y) noexcept {
	if(template_id == -1)
		return;

	ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
		state.ui_templates.icons[template_id].renders.get_render(state, int32_t(base_data.size.x), int32_t(base_data.size.y), state.user_settings.ui_scale, color.r, color.g, color.b));
}


void template_bg_graphic::update_tooltip(sys::state& state, int32_t x, int32_t y, text::columnar_layout& contents) noexcept {
	text::add_line(state, contents, default_tooltip);
}
void template_bg_graphic::render(sys::state& state, int32_t x, int32_t y) noexcept {
	if(template_id == -1)
		return;

	grid_size_window* par = static_cast<grid_size_window*>(parent);
	ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
		state.ui_templates.backgrounds[template_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
}

void template_icon_button::update_tooltip(sys::state& state, int32_t x, int32_t y, text::columnar_layout& contents) noexcept {
	text::add_line(state, contents, default_tooltip);
}
void template_icon_button::render(sys::state& state, int32_t x, int32_t y) noexcept {
	if(template_id == -1)
		return;

	grid_size_window* par = static_cast<grid_size_window*>(parent);
	auto ms_after = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_activated);
	if(disabled) {
		auto bg_id = state.ui_templates.iconic_button_t[template_id].disabled.bg;
		if(bg_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}
		if(icon != -1) {
			auto ico_color = state.ui_templates.iconic_button_t[template_id].disabled.icon_color;
			auto l = state.ui_templates.iconic_button_t[template_id].disabled.icon_left.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
			auto t = state.ui_templates.iconic_button_t[template_id].disabled.icon_top.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
			auto r = state.ui_templates.iconic_button_t[template_id].disabled.icon_right.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
			auto b = state.ui_templates.iconic_button_t[template_id].disabled.icon_bottom.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
			ogl::render_textured_rect_direct(state, l, t, r - l, b - t,
				state.ui_templates.icons[icon].renders.get_render(state, int32_t(r - l), int32_t(b - t), state.user_settings.ui_scale, state.ui_templates.colors[ico_color].r, state.ui_templates.colors[ico_color].g, state.ui_templates.colors[ico_color].b));
		}
	} else if(ms_after.count() < mouse_over_animation_ms && state.ui_templates.iconic_button_t[template_id].animate_active_transition) {
		float percentage = float(ms_after.count()) / float(mouse_over_animation_ms);
		if(this == state.ui_state.under_mouse) {
			auto active_id = state.ui_templates.iconic_button_t[template_id].active.bg;
			if(active_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					0.0f, percentage);
			}
			auto bg_id = state.ui_templates.iconic_button_t[template_id].primary.bg;
			if(bg_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					percentage, 1.0f);
			}
			if(icon != -1) {
				auto ico_color = state.ui_templates.iconic_button_t[template_id].active.icon_color;
				auto l = state.ui_templates.iconic_button_t[template_id].active.icon_left.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
				auto t = state.ui_templates.iconic_button_t[template_id].active.icon_top.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
				auto r = state.ui_templates.iconic_button_t[template_id].active.icon_right.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
				auto b = state.ui_templates.iconic_button_t[template_id].active.icon_bottom.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
				ogl::render_textured_rect_direct(state, l, t, r - l, b - t,
					state.ui_templates.icons[icon].renders.get_render(state, int32_t(r - l), int32_t(b - t), state.user_settings.ui_scale, state.ui_templates.colors[ico_color].r, state.ui_templates.colors[ico_color].g, state.ui_templates.colors[ico_color].b));
			}
		} else {
			auto active_id = state.ui_templates.iconic_button_t[template_id].active.bg;
			if(active_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					percentage, 1.0f);
			}
			auto bg_id = state.ui_templates.iconic_button_t[template_id].primary.bg;
			if(bg_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					0.0f, percentage);
			}

			if(icon != -1) {
				auto ico_color = state.ui_templates.iconic_button_t[template_id].primary.icon_color;
				auto l = state.ui_templates.iconic_button_t[template_id].primary.icon_left.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
				auto t = state.ui_templates.iconic_button_t[template_id].primary.icon_top.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
				auto r = state.ui_templates.iconic_button_t[template_id].primary.icon_right.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
				auto b = state.ui_templates.iconic_button_t[template_id].primary.icon_bottom.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
				ogl::render_textured_rect_direct(state, l, t, r - l, b - t,
					state.ui_templates.icons[icon].renders.get_render(state, int32_t(r - l), int32_t(b - t), state.user_settings.ui_scale, state.ui_templates.colors[ico_color].r, state.ui_templates.colors[ico_color].g, state.ui_templates.colors[ico_color].b));
			}
		}
	} else if(this == state.ui_state.under_mouse) {
		auto active_id = state.ui_templates.iconic_button_t[template_id].active.bg;
		if(active_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}
		if(icon != -1) {
			auto ico_color = state.ui_templates.iconic_button_t[template_id].active.icon_color;
			auto l = state.ui_templates.iconic_button_t[template_id].active.icon_left.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
			auto t = state.ui_templates.iconic_button_t[template_id].active.icon_top.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
			auto r = state.ui_templates.iconic_button_t[template_id].active.icon_right.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
			auto b = state.ui_templates.iconic_button_t[template_id].active.icon_bottom.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
			ogl::render_textured_rect_direct(state, l, t, r - l, b - t,
				state.ui_templates.icons[icon].renders.get_render(state, int32_t(r - l), int32_t(b - t), state.user_settings.ui_scale, state.ui_templates.colors[ico_color].r, state.ui_templates.colors[ico_color].g, state.ui_templates.colors[ico_color].b));
		}
	} else {
		auto bg_id = state.ui_templates.iconic_button_t[template_id].primary.bg;
		if(bg_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}
		if(icon != -1) {
			auto ico_color = state.ui_templates.iconic_button_t[template_id].primary.icon_color;
			auto l = state.ui_templates.iconic_button_t[template_id].primary.icon_left.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
			auto t = state.ui_templates.iconic_button_t[template_id].primary.icon_top.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
			auto r = state.ui_templates.iconic_button_t[template_id].primary.icon_right.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
			auto b = state.ui_templates.iconic_button_t[template_id].primary.icon_bottom.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
			ogl::render_textured_rect_direct(state, l, t, r - l, b - t,
				state.ui_templates.icons[icon].renders.get_render(state, int32_t(r - l), int32_t(b - t), state.user_settings.ui_scale, state.ui_templates.colors[ico_color].r, state.ui_templates.colors[ico_color].g, state.ui_templates.colors[ico_color].b));
		}
	}
}

void template_icon_button::on_hover(sys::state& state) noexcept {
	last_activated = std::chrono::steady_clock::now();
	button_on_hover(state);
}
void template_icon_button::on_hover_end(sys::state& state) noexcept {
	last_activated = std::chrono::steady_clock::now();
	button_on_hover_end(state);
}

void template_icon_button_ci::render(sys::state& state, int32_t x, int32_t y) noexcept {
	if(template_id == -1)
		return;

	grid_size_window* par = static_cast<grid_size_window*>(parent);
	auto ms_after = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_activated);
	if(disabled) {
		auto bg_id = state.ui_templates.iconic_button_t[template_id].disabled.bg;
		if(bg_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}
		if(icon != -1) {
			auto ico_color = state.ui_templates.iconic_button_t[template_id].disabled.icon_color;
			auto l = state.ui_templates.iconic_button_t[template_id].disabled.icon_left.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
			auto t = state.ui_templates.iconic_button_t[template_id].disabled.icon_top.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
			auto r = state.ui_templates.iconic_button_t[template_id].disabled.icon_right.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
			auto b = state.ui_templates.iconic_button_t[template_id].disabled.icon_bottom.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
			ogl::render_textured_rect_direct(state, l, t, r - l, b - t,
				state.ui_templates.icons[icon].renders.get_render(state, int32_t(r - l), int32_t(b - t), state.user_settings.ui_scale, icon_color.r, icon_color.g, icon_color.b));
		}
	} else if(ms_after.count() < mouse_over_animation_ms && state.ui_templates.iconic_button_t[template_id].animate_active_transition) {
		float percentage = float(ms_after.count()) / float(mouse_over_animation_ms);
		if(this == state.ui_state.under_mouse) {
			auto active_id = state.ui_templates.iconic_button_t[template_id].active.bg;
			if(active_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					0.0f, percentage);
			}
			auto bg_id = state.ui_templates.iconic_button_t[template_id].primary.bg;
			if(bg_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					percentage, 1.0f);
			}
			if(icon != -1) {
				auto ico_color = state.ui_templates.iconic_button_t[template_id].active.icon_color;
				auto l = state.ui_templates.iconic_button_t[template_id].active.icon_left.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
				auto t = state.ui_templates.iconic_button_t[template_id].active.icon_top.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
				auto r = state.ui_templates.iconic_button_t[template_id].active.icon_right.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
				auto b = state.ui_templates.iconic_button_t[template_id].active.icon_bottom.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
				ogl::render_textured_rect_direct(state, l, t, r - l, b - t,
					state.ui_templates.icons[icon].renders.get_render(state, int32_t(r - l), int32_t(b - t), state.user_settings.ui_scale, icon_color.r, icon_color.g, icon_color.b));
			}
		} else {
			auto active_id = state.ui_templates.iconic_button_t[template_id].active.bg;
			if(active_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					percentage, 1.0f);
			}
			auto bg_id = state.ui_templates.iconic_button_t[template_id].primary.bg;
			if(bg_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					0.0f, percentage);
			}

			if(icon != -1) {
				auto ico_color = state.ui_templates.iconic_button_t[template_id].primary.icon_color;
				auto l = state.ui_templates.iconic_button_t[template_id].primary.icon_left.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
				auto t = state.ui_templates.iconic_button_t[template_id].primary.icon_top.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
				auto r = state.ui_templates.iconic_button_t[template_id].primary.icon_right.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
				auto b = state.ui_templates.iconic_button_t[template_id].primary.icon_bottom.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
				ogl::render_textured_rect_direct(state, l, t, r - l, b - t,
					state.ui_templates.icons[icon].renders.get_render(state, int32_t(r - l), int32_t(b - t), state.user_settings.ui_scale, icon_color.r, icon_color.g, icon_color.b));
			}
		}
	} else if(this == state.ui_state.under_mouse) {
		auto active_id = state.ui_templates.iconic_button_t[template_id].active.bg;
		if(active_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}
		if(icon != -1) {
			auto ico_color = state.ui_templates.iconic_button_t[template_id].active.icon_color;
			auto l = state.ui_templates.iconic_button_t[template_id].active.icon_left.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
			auto t = state.ui_templates.iconic_button_t[template_id].active.icon_top.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
			auto r = state.ui_templates.iconic_button_t[template_id].active.icon_right.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
			auto b = state.ui_templates.iconic_button_t[template_id].active.icon_bottom.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
			ogl::render_textured_rect_direct(state, l, t, r - l, b - t,
				state.ui_templates.icons[icon].renders.get_render(state, int32_t(r - l), int32_t(b - t), state.user_settings.ui_scale, icon_color.r, icon_color.g, icon_color.b));
		}
	} else {
		auto bg_id = state.ui_templates.iconic_button_t[template_id].primary.bg;
		if(bg_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}
		if(icon != -1) {
			auto ico_color = state.ui_templates.iconic_button_t[template_id].primary.icon_color;
			auto l = state.ui_templates.iconic_button_t[template_id].primary.icon_left.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
			auto t = state.ui_templates.iconic_button_t[template_id].primary.icon_top.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
			auto r = state.ui_templates.iconic_button_t[template_id].primary.icon_right.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
			auto b = state.ui_templates.iconic_button_t[template_id].primary.icon_bottom.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
			ogl::render_textured_rect_direct(state, l, t, r - l, b - t,
				state.ui_templates.icons[icon].renders.get_render(state, int32_t(r - l), int32_t(b - t), state.user_settings.ui_scale, icon_color.r, icon_color.g, icon_color.b));
		}
	}
}

bool auto_close_button::button_action(sys::state& state) noexcept {
	state.ui_state.set_focus_target(state, nullptr);
	parent->set_visible(state, false);
	return true;
}
ui::message_result auto_close_button::on_key_down(sys::state& state, sys::virtual_key key, sys::key_modifiers mods) noexcept {
	if(key == sys::virtual_key::ESCAPE) {
		sound::play_interface_sound(state, sound::get_click_sound(state), state.user_settings.interface_volume * state.user_settings.master_volume);
		state.ui_state.set_focus_target(state, nullptr);
		parent->set_visible(state, false);
		return ui::message_result::consumed;
	}
	return ui::message_result::unseen;
}

text::alignment convert_align(template_project::aui_text_alignment a) {
	switch(a) {
	case template_project::aui_text_alignment::center: return text::alignment::center;
	case template_project::aui_text_alignment::left: return text::alignment::left;
	case template_project::aui_text_alignment::right: return text::alignment::right;
	}
	return text::alignment::left;
}

void template_label::set_text(sys::state& state, std::string_view new_text) {
	if(new_text != cached_text) {
		template_project::text_region_template region;
		grid_size_window* par = static_cast<grid_size_window*>(parent);

		if(template_id != -1) {
			region = state.ui_templates.label_t[template_id].primary;
		}


		cached_text = new_text;
		{
			internal_layout.contents.clear();
			internal_layout.number_of_lines = 0;

			text::single_line_layout sl{ internal_layout,
				text::layout_parameters{
					0, 0, static_cast<int16_t>(base_data.size.x - region.h_text_margins * par->grid_size * 2), static_cast<int16_t>(base_data.size.y - region.v_text_margins * 2),
					text::make_font_id(state, region.font_choice == 1, region.font_scale * par->grid_size * 2), 0,
					convert_align(region.h_text_alignment), text::text_color::black, true, true
				},
				state.world.locale_get_native_rtl(state.font_collection.get_current_locale()) ? text::layout_base::rtl_status::rtl : text::layout_base::rtl_status::ltr };
			sl.add_text(state, cached_text);
		}
	}
}

void template_label::on_reset_text(sys::state& state) noexcept {
	cached_text.clear();
	if(default_text)
		set_text(state, text::produce_simple_string(state, default_text));
}
void template_label::update_tooltip(sys::state& state, int32_t x, int32_t y, text::columnar_layout& contents) noexcept {
	text::add_line(state, contents, default_tooltip);
}
void template_label::on_create(sys::state& state) noexcept {
	on_reset_text(state);
}

void template_label::render(sys::state& state, int32_t x, int32_t y) noexcept {
	template_project::text_region_template region;
	grid_size_window* par = static_cast<grid_size_window*>(parent);

	if(template_id != -1) {
		region = state.ui_templates.label_t[template_id].primary;
	}
	auto color = state.ui_templates.colors[region.text_color];

	auto bg_id = region.bg;
	if(bg_id != -1) {
		ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
			state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
	}

	auto fh = text::make_font_id(state, region.font_choice == 1, region.font_scale * par->grid_size * 2);
	auto linesz = state.font_collection.line_height(state, fh);
	if(linesz == 0.f)
		return;

	int32_t yoff = 0;
	switch(region.v_text_alignment) {
	case template_project::aui_text_alignment::center: yoff = int32_t((base_data.size.y - linesz) / 2); break;
	case template_project::aui_text_alignment::left: yoff = int32_t(region.v_text_margins * par->grid_size); break;
	case template_project::aui_text_alignment::right: yoff = int32_t(base_data.size.y - linesz - region.v_text_margins * par->grid_size); break;
	}

	for(auto& t : internal_layout.contents) {
		ui::render_text_chunk(
			state,
			t,
			float(x) + t.x + region.h_text_margins * par->grid_size,
			float(y + t.y + yoff),
			fh,
			ogl::color3f{ color.r, color.g, color.b },
			ogl::color_modification::none
		);
	}
}



void template_mixed_button::set_text(sys::state& state, std::string_view new_text) {
	if(new_text != cached_text) {
		template_project::mixed_region_template region;
		grid_size_window* par = static_cast<grid_size_window*>(parent);

		if(template_id != -1) {
			region = state.ui_templates.mixed_button_t[template_id].primary;
		}

		cached_text = new_text;
		{
			internal_layout.contents.clear();
			internal_layout.number_of_lines = 0;
			auto icon_space = region.icon_right.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size));

			text::single_line_layout sl{ internal_layout,
				text::layout_parameters{
					0, 0, static_cast<int16_t>(base_data.size.x - icon_space - region.h_text_margins * par->grid_size * 2), static_cast<int16_t>(base_data.size.y - region.v_text_margins * 2),
					text::make_font_id(state, region.font_choice == 1, region.font_scale * par->grid_size * 2), 0,
					convert_align(region.h_text_alignment), text::text_color::black, true, true
				},
				state.world.locale_get_native_rtl(state.font_collection.get_current_locale()) ? text::layout_base::rtl_status::rtl : text::layout_base::rtl_status::ltr };
			sl.add_text(state, cached_text);
		}
	}
}
void template_mixed_button::on_hover(sys::state& state) noexcept {
	last_activated = std::chrono::steady_clock::now();
	button_on_hover(state);
}
void template_mixed_button::on_hover_end(sys::state& state) noexcept {
	last_activated = std::chrono::steady_clock::now();
	button_on_hover_end(state);
}
void template_mixed_button::on_reset_text(sys::state& state) noexcept {
	cached_text.clear();
	if(default_text)
		set_text(state, text::produce_simple_string(state, default_text));
}
void template_mixed_button::update_tooltip(sys::state& state, int32_t x, int32_t y, text::columnar_layout& contents) noexcept {
	text::add_line(state, contents, default_tooltip);
}
void template_mixed_button::on_create(sys::state& state) noexcept {
	on_reset_text(state);
}

void template_mixed_button::render(sys::state& state, int32_t x, int32_t y) noexcept {
	template_project::mixed_region_template region;
	grid_size_window* par = static_cast<grid_size_window*>(parent);

	if(template_id == -1)
		return;

	auto ms_after = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_activated);
	if(disabled) {
		region = state.ui_templates.mixed_button_t[template_id].disabled;
		auto bg_id = region.bg;
		if(bg_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}
	} else if(ms_after.count() < mouse_over_animation_ms && state.ui_templates.mixed_button_t[template_id].animate_active_transition) {
		float percentage = float(ms_after.count()) / float(mouse_over_animation_ms);
		if(this == state.ui_state.under_mouse) {
			region = state.ui_templates.mixed_button_t[template_id].active;
			auto active_id = state.ui_templates.mixed_button_t[template_id].active.bg;
			if(active_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					0.0f, percentage);
			}
			auto bg_id = state.ui_templates.mixed_button_t[template_id].primary.bg;
			if(bg_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					percentage, 1.0f);
			}
			
		} else {
			region = state.ui_templates.mixed_button_t[template_id].primary;
			auto active_id = state.ui_templates.mixed_button_t[template_id].active.bg;
			if(active_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					percentage, 1.0f);
			}
			auto bg_id = state.ui_templates.mixed_button_t[template_id].primary.bg;
			if(bg_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					0.0f, percentage);
			}
		}
	} else if(this == state.ui_state.under_mouse) {
		region = state.ui_templates.mixed_button_t[template_id].active;
		auto active_id = region.bg;
		if(active_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}
		
	} else {
		region = state.ui_templates.mixed_button_t[template_id].primary;
		auto bg_id = region.bg;
		if(bg_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}
	}

	auto color = state.ui_templates.colors[region.shared_color];
	auto icon_space = region.icon_right.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size));

	{ // icon
		if(icon_id != -1) {
			auto l = region.icon_left.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
			auto t = region.icon_top.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
			auto r = region.icon_right.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
			auto b = region.icon_bottom.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
			ogl::render_textured_rect_direct(state, l, t, r - l, b - t,
				state.ui_templates.icons[icon_id].renders.get_render(state, int32_t(r - l), int32_t(b - t), state.user_settings.ui_scale, color.r, color.g, color.b));
		}
	}
	{ // text
		auto fh = text::make_font_id(state, region.font_choice == 1, region.font_scale * par->grid_size * 2);
		auto linesz = state.font_collection.line_height(state, fh);
		if(linesz == 0.f)
			return;

		int32_t yoff = 0;
		switch(region.v_text_alignment) {
		case template_project::aui_text_alignment::center: yoff = int32_t((base_data.size.y - linesz) / 2); break;
		case template_project::aui_text_alignment::left: yoff = int32_t(region.v_text_margins * par->grid_size); break;
		case template_project::aui_text_alignment::right: yoff = int32_t(base_data.size.y - linesz - region.v_text_margins * par->grid_size); break;
		}

		for(auto& t : internal_layout.contents) {
			ui::render_text_chunk(
				state,
				t,
				float(x) + t.x + region.h_text_margins * par->grid_size + icon_space,
				float(y + t.y + yoff),
				fh,
				ogl::color3f{ color.r, color.g, color.b },
				ogl::color_modification::none
			);
		}
	}
}


void template_mixed_button_ci::render(sys::state& state, int32_t x, int32_t y) noexcept {
	template_project::mixed_region_template region;
	grid_size_window* par = static_cast<grid_size_window*>(parent);

	if(template_id == -1)
		return;

	auto ms_after = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_activated);
	if(disabled) {
		region = state.ui_templates.mixed_button_t[template_id].disabled;
		auto bg_id = region.bg;
		if(bg_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}
	} else if(ms_after.count() < mouse_over_animation_ms && state.ui_templates.mixed_button_t[template_id].animate_active_transition) {
		float percentage = float(ms_after.count()) / float(mouse_over_animation_ms);
		if(this == state.ui_state.under_mouse) {
			region = state.ui_templates.mixed_button_t[template_id].active;
			auto active_id = state.ui_templates.mixed_button_t[template_id].active.bg;
			if(active_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					0.0f, percentage);
			}
			auto bg_id = state.ui_templates.mixed_button_t[template_id].primary.bg;
			if(bg_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					percentage, 1.0f);
			}

		} else {
			region = state.ui_templates.mixed_button_t[template_id].primary;
			auto active_id = state.ui_templates.mixed_button_t[template_id].active.bg;
			if(active_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					percentage, 1.0f);
			}
			auto bg_id = state.ui_templates.mixed_button_t[template_id].primary.bg;
			if(bg_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					0.0f, percentage);
			}
		}
	} else if(this == state.ui_state.under_mouse) {
		region = state.ui_templates.mixed_button_t[template_id].active;
		auto active_id = region.bg;
		if(active_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}

	} else {
		region = state.ui_templates.mixed_button_t[template_id].primary;
		auto bg_id = region.bg;
		if(bg_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}
	}

	auto color = state.ui_templates.colors[region.shared_color];
	auto icon_space = region.icon_right.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size));

	{ // icon
		if(icon_id != -1) {
			auto l = region.icon_left.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
			auto t = region.icon_top.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
			auto r = region.icon_right.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + x;
			auto b = region.icon_bottom.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size)) + y;
			ogl::render_textured_rect_direct(state, l, t, r - l, b - t,
				state.ui_templates.icons[icon_id].renders.get_render(state, int32_t(r - l), int32_t(b - t), state.user_settings.ui_scale, icon_color.r, icon_color.g, icon_color.b));
		}
	}
	{ // text
		auto fh = text::make_font_id(state, region.font_choice == 1, region.font_scale * par->grid_size * 2);
		auto linesz = state.font_collection.line_height(state, fh);
		if(linesz == 0.f)
			return;

		int32_t yoff = 0;
		switch(region.v_text_alignment) {
		case template_project::aui_text_alignment::center: yoff = int32_t((base_data.size.y - linesz) / 2); break;
		case template_project::aui_text_alignment::left: yoff = int32_t(region.v_text_margins * par->grid_size); break;
		case template_project::aui_text_alignment::right: yoff = int32_t(base_data.size.y - linesz - region.v_text_margins * par->grid_size); break;
		}

		for(auto& t : internal_layout.contents) {
			ui::render_text_chunk(
				state,
				t,
				float(x) + t.x + region.h_text_margins * par->grid_size + icon_space,
				float(y + t.y + yoff),
				fh,
				ogl::color3f{ color.r, color.g, color.b },
				ogl::color_modification::none
			);
		}
	}
}


void template_text_button::set_text(sys::state& state, std::string_view new_text) {
	if(new_text != cached_text) {
		template_project::text_region_template region;
		grid_size_window* par = static_cast<grid_size_window*>(parent);

		if(template_id != -1) {
			region = state.ui_templates.button_t[template_id].primary;
		}

		cached_text = new_text;
		{
			internal_layout.contents.clear();
			internal_layout.number_of_lines = 0;

			text::single_line_layout sl{ internal_layout,
				text::layout_parameters{
					0, 0, static_cast<int16_t>(base_data.size.x - region.h_text_margins * par->grid_size* 2), static_cast<int16_t>(base_data.size.y - region.v_text_margins * 2),
					text::make_font_id(state, region.font_choice == 1, region.font_scale * par->grid_size * 2), 0,
					convert_align(region.h_text_alignment), text::text_color::black, true, true
				},
				state.world.locale_get_native_rtl(state.font_collection.get_current_locale()) ? text::layout_base::rtl_status::rtl : text::layout_base::rtl_status::ltr };
			sl.add_text(state, cached_text);
		}
	}
}
void template_text_button::on_hover(sys::state& state) noexcept {
	last_activated = std::chrono::steady_clock::now();
	button_on_hover(state);
}
void template_text_button::on_hover_end(sys::state& state) noexcept {
	last_activated = std::chrono::steady_clock::now();
	button_on_hover_end(state);
}
void template_text_button::on_reset_text(sys::state& state) noexcept {
	cached_text.clear();
	if(default_text)
		set_text(state, text::produce_simple_string(state, default_text));
}
void template_text_button::update_tooltip(sys::state& state, int32_t x, int32_t y, text::columnar_layout& contents) noexcept {
	text::add_line(state, contents, default_tooltip);
}
void template_text_button::on_create(sys::state& state) noexcept {
	on_reset_text(state);
}

void template_text_button::render(sys::state& state, int32_t x, int32_t y) noexcept {
	template_project::text_region_template region;
	grid_size_window* par = static_cast<grid_size_window*>(parent);

	if(template_id == -1)
		return;

	auto ms_after = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_activated);
	if(disabled) {
		region = state.ui_templates.button_t[template_id].disabled;
		auto bg_id = region.bg;
		if(bg_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}
	} else if(ms_after.count() < mouse_over_animation_ms && state.ui_templates.button_t[template_id].animate_active_transition) {
		float percentage = float(ms_after.count()) / float(mouse_over_animation_ms);
		if(this == state.ui_state.under_mouse) {
			region = state.ui_templates.button_t[template_id].active;
			auto active_id = state.ui_templates.button_t[template_id].active.bg;
			if(active_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					0.0f, percentage);
			}
			auto bg_id = state.ui_templates.button_t[template_id].primary.bg;
			if(bg_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					percentage, 1.0f);
			}

		} else {
			region = state.ui_templates.button_t[template_id].primary;
			auto active_id = state.ui_templates.button_t[template_id].active.bg;
			if(active_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					percentage, 1.0f);
			}
			auto bg_id = state.ui_templates.button_t[template_id].primary.bg;
			if(bg_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					0.0f, percentage);
			}
		}
	} else if(this == state.ui_state.under_mouse) {
		region = state.ui_templates.button_t[template_id].active;
		auto active_id = region.bg;
		if(active_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}

	} else {
		region = state.ui_templates.button_t[template_id].primary;
		auto bg_id = region.bg;
		if(bg_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}
	}

	auto color = state.ui_templates.colors[region.text_color];

	auto fh = text::make_font_id(state, region.font_choice == 1, region.font_scale * par->grid_size * 2);
	auto linesz = state.font_collection.line_height(state, fh);
	if(linesz == 0.f)
		return;

	int32_t yoff = 0;
	switch(region.v_text_alignment) {
	case template_project::aui_text_alignment::center: yoff = int32_t((base_data.size.y - linesz) / 2); break;
	case template_project::aui_text_alignment::left: yoff = int32_t(region.v_text_margins * par->grid_size);  break;
	case template_project::aui_text_alignment::right: yoff = int32_t(base_data.size.y - linesz - region.v_text_margins * par->grid_size); break;
	}

	for(auto& t : internal_layout.contents) {
		ui::render_text_chunk(
			state,
			t,
			float(x) + t.x + region.h_text_margins * par->grid_size,
			float(y + t.y + yoff),
			fh,
			ogl::color3f{ color.r, color.g, color.b },
			ogl::color_modification::none
		);
	}
}


void template_toggle_button::set_text(sys::state& state, std::string_view new_text) {
	if(new_text != cached_text) {
		template_project::toggle_region region;
		grid_size_window* par = static_cast<grid_size_window*>(parent);

		if(template_id != -1) {
			region = is_active ? state.ui_templates.toggle_button_t[template_id].on_region : state.ui_templates.toggle_button_t[template_id].off_region;
		}

		auto l = region.text_margin_left.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size));
		auto r = region.text_margin_right.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size));

		cached_text = new_text;
		{
			internal_layout.contents.clear();
			internal_layout.number_of_lines = 0;

			text::single_line_layout sl{ internal_layout,
				text::layout_parameters{
					0, 0, static_cast<int16_t>(base_data.size.x - (l + r)), static_cast<int16_t>(base_data.size.y),
					text::make_font_id(state, region.font_choice == 1, region.font_scale * par->grid_size * 2), 0,
					convert_align(region.h_text_alignment), text::text_color::black, true, true
				},
				state.world.locale_get_native_rtl(state.font_collection.get_current_locale()) ? text::layout_base::rtl_status::rtl : text::layout_base::rtl_status::ltr };
			sl.add_text(state, cached_text);
		}
	}
}
void template_toggle_button::on_hover(sys::state& state) noexcept {
	last_activated = std::chrono::steady_clock::now();
	button_on_hover(state);
}
void template_toggle_button::on_hover_end(sys::state& state) noexcept {
	last_activated = std::chrono::steady_clock::now();
	button_on_hover_end(state);
}
void template_toggle_button::on_reset_text(sys::state& state) noexcept {
	cached_text.clear();
	if(default_text)
		set_text(state, text::produce_simple_string(state, default_text));
}
void template_toggle_button::set_active(sys::state& state, bool active) {
	if(active != is_active) {
		is_active = active;
		auto temp = std::move(cached_text);
		cached_text.clear();
		set_text(state, temp);
	}
}
void template_toggle_button::update_tooltip(sys::state& state, int32_t x, int32_t y, text::columnar_layout& contents) noexcept {
	text::add_line(state, contents, default_tooltip);
}
void template_toggle_button::on_create(sys::state& state) noexcept {
	on_reset_text(state);
}

void template_toggle_button::render(sys::state& state, int32_t x, int32_t y) noexcept {
	template_project::toggle_region mainregion;
	template_project::color_region region;
	grid_size_window* par = static_cast<grid_size_window*>(parent);

	if(template_id == -1)
		return;

	mainregion = is_active ? state.ui_templates.toggle_button_t[template_id].on_region : state.ui_templates.toggle_button_t[template_id].off_region;

	auto ms_after = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_activated);
	if(disabled) {
		region = mainregion.disabled;
		auto bg_id = region.bg;
		if(bg_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}
	} else if(ms_after.count() < mouse_over_animation_ms && state.ui_templates.toggle_button_t[template_id].animate_active_transition) {
		float percentage = float(ms_after.count()) / float(mouse_over_animation_ms);
		if(this == state.ui_state.under_mouse) {
			region = mainregion.active;
			auto active_id = region.bg;
			if(active_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					0.0f, percentage);
			}
			auto bg_id = mainregion.primary.bg;
			if(bg_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					percentage, 1.0f);
			}

		} else {
			region = mainregion.primary;
			auto active_id = mainregion.active.bg;
			if(active_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					percentage, 1.0f);
			}
			auto bg_id = mainregion.primary.bg;
			if(bg_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					0.0f, percentage);
			}
		}
	} else if(this == state.ui_state.under_mouse) {
		region = mainregion.active;
		auto active_id = region.bg;
		if(active_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}

	} else {
		region = mainregion.primary;
		auto bg_id = region.bg;
		if(bg_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}
	}

	auto color = state.ui_templates.colors[region.color];

	auto l = mainregion.text_margin_left.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size));
	auto top = mainregion.text_margin_top.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size));
	auto b = mainregion.text_margin_bottom.resolve(float(base_data.size.x), float(base_data.size.y), float(par->grid_size));

	auto fh = text::make_font_id(state, mainregion.font_choice == 1, mainregion.font_scale * par->grid_size * 2);
	auto linesz = state.font_collection.line_height(state, fh);
	if(linesz == 0.f)
		return;

	int32_t yoff = 0;
	switch(mainregion.v_text_alignment) {
	case template_project::aui_text_alignment::center: yoff = int32_t((base_data.size.y - linesz - (top + b)) / 2); break;
	case template_project::aui_text_alignment::left: yoff = int32_t(top);  break;
	case template_project::aui_text_alignment::right: yoff = int32_t(base_data.size.y - linesz - b); break;
	}

	for(auto& t : internal_layout.contents) {
		ui::render_text_chunk(
			state,
			t,
			float(x) + t.x + l,
			float(y + t.y + yoff),
			fh,
			ogl::color3f{ color.r, color.g, color.b },
			ogl::color_modification::none
		);
	}
}


void page_buttons::render(sys::state& state, int32_t x, int32_t y) noexcept {
	int16_t p = for_layout->current_page;
	int16_t z = int16_t(for_layout->page_starts.size());

	if(p != last_page || z != last_size) {
		text_layout.contents.clear();
		text_layout.number_of_lines = 0;
		last_page = p;
		last_size = z;
		if(z > 1) {
			std::string display = std::to_string(p + 1) + "/" + std::to_string(z);
			text::single_line_layout sl{ text_layout, text::layout_parameters{ 0, 0, static_cast<int16_t>(base_data.size.x), static_cast<int16_t>(base_data.size.y), text::make_font_id(state, false, base_data.size.y), 0, text::alignment::center, text::text_color::black, true, true }, state.world.locale_get_native_rtl(state.font_collection.get_current_locale()) ? text::layout_base::rtl_status::rtl : text::layout_base::rtl_status::ltr };
			sl.add_text(state, display);
		}
	}

	if(last_size <= 1)
		return;


		int32_t rel_mouse_x = int32_t(state.mouse_x_position / state.user_settings.ui_scale) - ui::get_absolute_location(state, *this).x;
		grid_size_window* par = static_cast<grid_size_window*>(parent);

		if(auto button_template = state.ui_templates.layout_region_t[for_layout->template_id].left_button; button_template != -1) { // left button
			auto icon = state.ui_templates.layout_region_t[for_layout->template_id].left_button_icon;
			auto x_pos = x;
			auto y_pos = y;
			template_project::icon_region_template region;
			if(for_layout->current_page == 0) {
				region = state.ui_templates.iconic_button_t[button_template].disabled;
			} else if(this == state.ui_state.under_mouse && rel_mouse_x <= base_data.size.y) {
				region = state.ui_templates.iconic_button_t[button_template].active;
			} else {
				region = state.ui_templates.iconic_button_t[button_template].primary;
			}
			auto bg_id = region.bg;
			if(bg_id != -1) {
				ogl::render_textured_rect_direct(state, float(x_pos), float(y_pos), float(base_data.size.y), float(base_data.size.y),
					state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.y) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
			}
			if(icon != -1) {
				auto ico_color = region.icon_color;
				auto l = region.icon_left.resolve(float(base_data.size.y), float(base_data.size.y), float(par->grid_size)) + x_pos;
				auto t = region.icon_top.resolve(float(base_data.size.y), float(base_data.size.y), float(par->grid_size)) + y_pos;
				auto r = region.icon_right.resolve(float(base_data.size.y), float(base_data.size.y), float(par->grid_size)) + x_pos;
				auto b = region.icon_bottom.resolve(float(base_data.size.y), float(base_data.size.y), float(par->grid_size)) + y_pos;
				ogl::render_textured_rect_direct(state, l, t, r - l, b - t,
					state.ui_templates.icons[icon].renders.get_render(state, int32_t(r - l), int32_t(b - t), state.user_settings.ui_scale, state.ui_templates.colors[ico_color].r, state.ui_templates.colors[ico_color].g, state.ui_templates.colors[ico_color].b));
			}
		}
		if(auto button_template = state.ui_templates.layout_region_t[for_layout->template_id].right_button; button_template != -1) { // right button
			auto icon = state.ui_templates.layout_region_t[for_layout->template_id].right_button_icon;
			auto x_pos = x + base_data.size.x - base_data.size.y;
			auto y_pos = y;
			template_project::icon_region_template region;
			if(for_layout->current_page >= int32_t(for_layout->page_starts.size()) - 1) {
				region = state.ui_templates.iconic_button_t[button_template].disabled;
			} else if(this == state.ui_state.under_mouse && (base_data.size.x - base_data.size.y) <= rel_mouse_x) {
				region = state.ui_templates.iconic_button_t[button_template].active;
			} else {
				region = state.ui_templates.iconic_button_t[button_template].primary;
			}
			auto bg_id = region.bg;
			if(bg_id != -1) {
				ogl::render_textured_rect_direct(state, float(x_pos), float(y_pos), float(base_data.size.y), float(base_data.size.y),
					state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.y) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
			}
			if(icon != -1) {
				auto ico_color = region.icon_color;
				auto l = region.icon_left.resolve(float(base_data.size.y), float(base_data.size.y), float(par->grid_size)) + x_pos;
				auto t = region.icon_top.resolve(float(base_data.size.y), float(base_data.size.y), float(par->grid_size)) + y_pos;
				auto r = region.icon_right.resolve(float(base_data.size.y), float(base_data.size.y), float(par->grid_size)) + x_pos;
				auto b = region.icon_bottom.resolve(float(base_data.size.y), float(base_data.size.y), float(par->grid_size)) + y_pos;
				ogl::render_textured_rect_direct(state, l, t, r - l, b - t,
					state.ui_templates.icons[icon].renders.get_render(state, int32_t(r - l), int32_t(b - t), state.user_settings.ui_scale, state.ui_templates.colors[ico_color].r, state.ui_templates.colors[ico_color].g, state.ui_templates.colors[ico_color].b));
			}
		}
		{ // text
			auto region = state.ui_templates.layout_region_t[for_layout->template_id].page_number_text;

			auto bg_id = region.bg;
			if(bg_id != -1) {
				ogl::render_textured_rect_direct(state, float(base_data.size.y), float(y), float(base_data.size.x - 2 * base_data.size.y), float(base_data.size.y),
					state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x -  2 * base_data.size.y) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
			}

			auto fh = text::make_font_id(state, region.font_choice == 1, region.font_scale * par->grid_size * 2);
			auto linesz = state.font_collection.line_height(state, fh);
			if(linesz == 0.0f) return;

			auto ycentered = (base_data.size.y - linesz) / 2;
			auto color = state.ui_templates.colors[region.text_color];

			for(auto& t : text_layout.contents) {
				ui::render_text_chunk(
					state,
					t,
					float(x) + t.x,
					float(y + t.y + ycentered),
					fh,
					ogl::color3f{ color.r, color.g, color.b },
					ogl::color_modification::none
				);
			}
	}
}
ui::message_result page_buttons::on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	if(last_size <= 1)
		return ui::message_result::unseen;

	if(x <= base_data.size.y) { // left button
		if(for_layout->current_page > 0) {
			sound::play_interface_sound(state, sound::get_click_sound(state), state.user_settings.interface_volume * state.user_settings.master_volume);
			for_layout->change_page(state, *((layout_window_element*)parent), for_layout->current_page - 1);
		}
	} else if(base_data.size.x - base_data.size.y <= x) { // right button
		if(for_layout->current_page < int32_t(for_layout->page_starts.size()) - 1) {
			sound::play_interface_sound(state, sound::get_click_sound(state), state.user_settings.interface_volume * state.user_settings.master_volume);
			for_layout->change_page(state, *((layout_window_element*)parent), for_layout->current_page + 1);
		}
	}

	return ui::message_result::consumed;
}
void page_buttons::on_update(sys::state& state) noexcept {

}

ui::drag_and_drop_query_result drag_and_drop_target_control::impl_drag_and_drop_query(sys::state& state, int32_t x, int32_t y, ui::drag_and_drop_data data_type) noexcept {
	ui::drag_and_drop_query_result result;
	if(data_type != supported_data_type)
		return result;
	if(x < 0 || y < 0 || x > base_data.size.x || y > base_data.size.y)
		return result;

	result.directions = supported_directions;
	result.under_mouse = this;
	return result;
}
void drag_and_drop_target_control::render(sys::state& state, int32_t x, int32_t y) noexcept {
	if(state.ui_state.current_drag_and_drop_data_type != supported_data_type)
		return;

	grid_size_window* par = static_cast<grid_size_window*>(parent);

	// TODO: pick distinct graphics, render appropriate direction targets instead of just center only
	// TODO: highlight correct side if non-center targets are present

	static auto bg_sprite = template_project::icon_by_name(state.ui_templates, "map_location_bottom.svg");
	static auto outline_color = template_project::color_by_name(state.ui_templates, "med red");
	static auto active_color = template_project::color_by_name(state.ui_templates, "light orange");

	if(this == state.ui_state.under_mouse) {
		ogl::render_colored_rect(state,
			float(x), float(y),
			float(base_data.size.x), float(1),
			state.ui_templates.colors[active_color].r, state.ui_templates.colors[active_color].g, state.ui_templates.colors[active_color].b,
			ui::rotation::upright, false, false);

		ogl::render_colored_rect(state,
			float(x), float(y + base_data.size.y - 1),
			float(base_data.size.x), float(1),
			state.ui_templates.colors[active_color].r, state.ui_templates.colors[active_color].g, state.ui_templates.colors[active_color].b,
			ui::rotation::upright, false, false);

		ogl::render_colored_rect(state,
			float(x), float(y),
			float(1), float(base_data.size.y),
			state.ui_templates.colors[active_color].r, state.ui_templates.colors[active_color].g, state.ui_templates.colors[active_color].b,
			ui::rotation::upright, false, false);

		ogl::render_colored_rect(state,
			float(x + base_data.size.x - 1), float(y),
			float(1), float(base_data.size.y),
			state.ui_templates.colors[active_color].r, state.ui_templates.colors[active_color].g, state.ui_templates.colors[active_color].b,
			ui::rotation::upright, false, false);

		ogl::render_textured_rect_direct(state, float(x + base_data.size.x / 2 - par->grid_size / 2), float(y + base_data.size.y / 2 - par->grid_size / 2), float(par->grid_size), float(par->grid_size ),
			state.ui_templates.icons[bg_sprite].renders.get_render(state, par->grid_size, par->grid_size, state.user_settings.ui_scale, state.ui_templates.colors[active_color].r, state.ui_templates.colors[active_color].g, state.ui_templates.colors[active_color].b));
	} else {
		ogl::render_colored_rect(state,
		float(x), float(y),
		float(base_data.size.x), float(1),
		state.ui_templates.colors[outline_color].r, state.ui_templates.colors[outline_color].g, state.ui_templates.colors[outline_color].b,
		ui::rotation::upright, false, false);

		ogl::render_colored_rect(state,
			float(x), float(y + base_data.size.y - 1),
			float(base_data.size.x), float(1),
			state.ui_templates.colors[outline_color].r, state.ui_templates.colors[outline_color].g, state.ui_templates.colors[outline_color].b,
			ui::rotation::upright, false, false);

		ogl::render_colored_rect(state,
			float(x), float(y),
			float(1), float(base_data.size.y),
			state.ui_templates.colors[outline_color].r, state.ui_templates.colors[outline_color].g, state.ui_templates.colors[outline_color].b,
			ui::rotation::upright, false, false);

		ogl::render_colored_rect(state,
			float(x + base_data.size.x - 1), float(y),
			float(1), float(base_data.size.y),
			state.ui_templates.colors[outline_color].r, state.ui_templates.colors[outline_color].g, state.ui_templates.colors[outline_color].b,
			ui::rotation::upright, false, false);

		ogl::render_textured_rect_direct(state, float(x + base_data.size.x / 2 - par->grid_size / 2), float(y + base_data.size.y / 2 - par->grid_size / 2), float(par->grid_size), float(par->grid_size),
			state.ui_templates.icons[bg_sprite].renders.get_render(state, par->grid_size, par->grid_size, state.user_settings.ui_scale, state.ui_templates.colors[outline_color].r, state.ui_templates.colors[outline_color].g, state.ui_templates.colors[outline_color].b));
	}
}

void drop_down_list_page_buttons::render(sys::state& state, int32_t x, int32_t y) noexcept {
	if(!owner_control)
		return;

	auto tid = owner_control->template_id;
	if(tid == -1)
		return;
	auto lt = state.ui_templates.drop_down_t[tid].layout_region_base;

	auto total_pages = (owner_control->total_items + owner_control->items_per_page  -1) / owner_control->items_per_page;

	if(owner_control->page_text_out_of_date) {
		text_layout.contents.clear();
		text_layout.number_of_lines = 0;
		if(total_pages > 1) {
			std::string display = std::to_string(owner_control->list_page + 1) + "/" + std::to_string(total_pages);
			text::single_line_layout sl{ text_layout, text::layout_parameters{ 0, 0, static_cast<int16_t>(base_data.size.x), static_cast<int16_t>(base_data.size.y), text::make_font_id(state, false, base_data.size.y), 0, text::alignment::center, text::text_color::black, true, true }, state.world.locale_get_native_rtl(state.font_collection.get_current_locale()) ? text::layout_base::rtl_status::rtl : text::layout_base::rtl_status::ltr };
			sl.add_text(state, display);
		}
		owner_control->page_text_out_of_date = false;
	}

	if(total_pages <= 1)
		return;


	int32_t rel_mouse_x = int32_t(state.mouse_x_position / state.user_settings.ui_scale) - ui::get_absolute_location(state, *this).x;
	grid_size_window* par = static_cast<grid_size_window*>(parent);

	if(auto button_template = state.ui_templates.layout_region_t[lt].left_button; button_template != -1) { // left button
		auto icon = state.ui_templates.layout_region_t[lt].left_button_icon;
		auto x_pos = x;
		auto y_pos = y;
		template_project::icon_region_template region;
		if(owner_control->list_page == 0) {
			region = state.ui_templates.iconic_button_t[button_template].disabled;
		} else if(this == state.ui_state.under_mouse && rel_mouse_x <= base_data.size.y) {
			region = state.ui_templates.iconic_button_t[button_template].active;
		} else {
			region = state.ui_templates.iconic_button_t[button_template].primary;
		}
		auto bg_id = region.bg;
		if(bg_id != -1) {
			ogl::render_textured_rect_direct(state, float(x_pos), float(y_pos), float(base_data.size.y), float(base_data.size.y),
				state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.y) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}
		if(icon != -1) {
			auto ico_color = region.icon_color;
			auto l = region.icon_left.resolve(float(base_data.size.y), float(base_data.size.y), float(par->grid_size)) + x_pos;
			auto t = region.icon_top.resolve(float(base_data.size.y), float(base_data.size.y), float(par->grid_size)) + y_pos;
			auto r = region.icon_right.resolve(float(base_data.size.y), float(base_data.size.y), float(par->grid_size)) + x_pos;
			auto b = region.icon_bottom.resolve(float(base_data.size.y), float(base_data.size.y), float(par->grid_size)) + y_pos;
			ogl::render_textured_rect_direct(state, l, t, r - l, b - t,
				state.ui_templates.icons[icon].renders.get_render(state, int32_t(r - l), int32_t(b - t), state.user_settings.ui_scale, state.ui_templates.colors[ico_color].r, state.ui_templates.colors[ico_color].g, state.ui_templates.colors[ico_color].b));
		}
	}
	if(auto button_template = state.ui_templates.layout_region_t[lt].right_button; button_template != -1) { // right button
		auto icon = state.ui_templates.layout_region_t[lt].right_button_icon;
		auto x_pos = x + base_data.size.x - base_data.size.y;
		auto y_pos = y;
		template_project::icon_region_template region;
		if(owner_control->list_page + 1 >= total_pages) {
			region = state.ui_templates.iconic_button_t[button_template].disabled;
		} else if(this == state.ui_state.under_mouse && (base_data.size.x - base_data.size.y) <= rel_mouse_x) {
			region = state.ui_templates.iconic_button_t[button_template].active;
		} else {
			region = state.ui_templates.iconic_button_t[button_template].primary;
		}
		auto bg_id = region.bg;
		if(bg_id != -1) {
			ogl::render_textured_rect_direct(state, float(x_pos), float(y_pos), float(base_data.size.y), float(base_data.size.y),
				state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.y) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}
		if(icon != -1) {
			auto ico_color = region.icon_color;
			auto l = region.icon_left.resolve(float(base_data.size.y), float(base_data.size.y), float(par->grid_size)) + x_pos;
			auto t = region.icon_top.resolve(float(base_data.size.y), float(base_data.size.y), float(par->grid_size)) + y_pos;
			auto r = region.icon_right.resolve(float(base_data.size.y), float(base_data.size.y), float(par->grid_size)) + x_pos;
			auto b = region.icon_bottom.resolve(float(base_data.size.y), float(base_data.size.y), float(par->grid_size)) + y_pos;
			ogl::render_textured_rect_direct(state, l, t, r - l, b - t,
				state.ui_templates.icons[icon].renders.get_render(state, int32_t(r - l), int32_t(b - t), state.user_settings.ui_scale, state.ui_templates.colors[ico_color].r, state.ui_templates.colors[ico_color].g, state.ui_templates.colors[ico_color].b));
		}
	}
	{ // text
		auto region = state.ui_templates.layout_region_t[lt].page_number_text;

		auto bg_id = region.bg;
		if(bg_id != -1) {
			ogl::render_textured_rect_direct(state, float(base_data.size.y), float(y), float(base_data.size.x - 2 * base_data.size.y), float(base_data.size.y),
				state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x - 2 * base_data.size.y) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}

		auto fh = text::make_font_id(state, region.font_choice == 1, region.font_scale * par->grid_size * 2);
		auto linesz = state.font_collection.line_height(state, fh);
		if(linesz == 0.0f) return;

		auto ycentered = (base_data.size.y - linesz) / 2;
		auto color = state.ui_templates.colors[region.text_color];

		for(auto& t : text_layout.contents) {
			ui::render_text_chunk(
				state,
				t,
				float(x) + t.x,
				float(y + t.y + ycentered),
				fh,
				ogl::color3f{ color.r, color.g, color.b },
				ogl::color_modification::none
			);
		}
	}
}

ui::message_result drop_down_list_page_buttons::test_mouse(sys::state& state, int32_t x, int32_t y, ui::mouse_probe_type type) noexcept {
	if(owner_control && type == ui::mouse_probe_type::click && owner_control->total_items > owner_control->items_per_page) {
		return ui::message_result::consumed;
	} else {
		return ui::message_result::unseen;
	}
}

ui::message_result drop_down_list_page_buttons::on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mods) noexcept {
	if(!owner_control)
		return ui::message_result::unseen;

	if(x <= base_data.size.y) { // left button
		if(owner_control->list_page > 0) {
			sound::play_interface_sound(state, sound::get_click_sound(state), state.user_settings.interface_volume * state.user_settings.master_volume);
			owner_control->change_page(state, owner_control->list_page - 1);
		}
	} else if(base_data.size.x - base_data.size.y <= x) { // right button
		if(owner_control->list_page < (owner_control->total_items - 1) / owner_control->items_per_page) {
			sound::play_interface_sound(state, sound::get_click_sound(state), state.user_settings.interface_volume * state.user_settings.master_volume);
			owner_control->change_page(state, owner_control->list_page + 1);
		}
	}

	return ui::message_result::consumed;
}
void drop_down_list_page_buttons::on_update(sys::state& state) noexcept {

}

void template_drop_down_control::on_hover(sys::state& state) noexcept {
	last_activated = std::chrono::steady_clock::now();
}
void template_drop_down_control::on_hover_end(sys::state& state) noexcept {
	last_activated = std::chrono::steady_clock::now();
}

void drop_down_list_button::on_update(sys::state& state) noexcept {
	if(owner_control) {
		if(owner_control->selected_item == list_id)
			icon_id = int16_t(state.ui_templates.drop_down_t[owner_control->template_id].selection_icon);
		else
			icon_id = -1;
	}
}
bool drop_down_list_button::button_action(sys::state& state) noexcept {
	if(owner_control) {
		owner_control->on_selection(state, list_id);
		owner_control->hide_list(state);
	}
	return true;
}

void template_drop_down_control::open_list(sys::state& state) {
	if(template_id == -1)
		return;
	if(state.ui_templates.drop_down_t[template_id].list_button == -1)
		return;
	if(total_items <= 0)
		return;

	auto current_root = state.current_scene.get_root(state);
	if(!state.ui_state.popup_menu) {
		auto new_menu = std::make_unique<alice_ui::pop_up_menu_container>();
		state.ui_state.popup_menu = new_menu.get();
		current_root->add_child_to_back(std::move(new_menu));
	}

	// resize and position list window TODO
	grid_size_window* par = static_cast<grid_size_window*>(parent);
	auto self_pos = ui::get_absolute_location(state, *this);

	auto total_vert_margin = state.ui_templates.drop_down_t[template_id].dropdown_window_margin * par->grid_size * 2;
	auto max_vert_elm_count = int32_t((state.ui_state.root->base_data.size.y - (self_pos.y + base_data.size.y + total_vert_margin + par->grid_size * 2)) / element_y_size);
	bool position_below = true;
	if(max_vert_elm_count < target_page_height || (target_page_height <= 0 && max_vert_elm_count < 3)) {
		max_vert_elm_count = int32_t((self_pos.y - (total_vert_margin + par->grid_size * 2)) / element_y_size);
		position_below = false;
	}
	if(max_vert_elm_count <= 0)
		return;

	bool one_page = false;
	if(target_page_height <= 0) {
		items_per_page = max_vert_elm_count;
	} else {
		items_per_page = std::min(max_vert_elm_count, target_page_height);
	}

	if(two_columns && items_per_page * 2 >= total_items) {
		items_per_page = (total_items + 1) / 2;
		one_page = true;
	}
	if(!two_columns && items_per_page >= total_items) {
		items_per_page = total_items;
		one_page = true;
	}
	

	auto lb = state.ui_templates.drop_down_t[template_id].list_button;
	auto elm_h_size = int32_t(state.ui_templates.mixed_button_t[lb].primary.h_text_margins * par->grid_size + state.ui_templates.mixed_button_t[lb].primary.icon_right.resolve(float(element_x_size), float(element_y_size), float(par->grid_size)) + element_x_size);

	state.ui_state.popup_menu->base_data.size.x = int16_t(state.ui_templates.drop_down_t[template_id].dropdown_window_margin * par->grid_size * 2 + elm_h_size + (two_columns ? elm_h_size : 0));
	state.ui_state.popup_menu->base_data.size.y = int16_t(state.ui_templates.drop_down_t[template_id].dropdown_window_margin * par->grid_size * 2 + items_per_page * element_y_size + (one_page ? 0 : par->grid_size * 2));

	state.ui_state.popup_menu->base_data.position.x = int16_t(self_pos.x + base_data.size.x / 2 - state.ui_state.popup_menu->base_data.size.x / 2);
	if(state.ui_state.popup_menu->base_data.position.x < 0) {
		state.ui_state.popup_menu->base_data.position.x = 0;
	}
	if(state.ui_state.popup_menu->base_data.position.x + state.ui_state.popup_menu->base_data.size.x > state.ui_state.root->base_data.size.x) {
		state.ui_state.popup_menu->base_data.position.x = int16_t(state.ui_state.root->base_data.size.x - state.ui_state.popup_menu->base_data.size.x);
	}
	if(position_below) {
		state.ui_state.popup_menu->base_data.position.y = int16_t(self_pos.y + base_data.size.y);
	} else {
		state.ui_state.popup_menu->base_data.position.y = int16_t(self_pos.y - state.ui_state.popup_menu->base_data.size.y);
	}

	if(two_columns)
		items_per_page *= 2;

	state.ui_state.popup_menu->scroll_redirect = this;
	change_page(state, 0);

	state.ui_state.popup_menu->grid_size = par->grid_size;
	state.ui_state.popup_menu->bg_template = state.ui_templates.drop_down_t[template_id].dropdown_window_bg;

	state.ui_state.popup_menu->flags = uint8_t(state.ui_state.popup_menu->flags & ~is_invisible_mask);
	if(state.ui_state.popup_menu->parent != current_root) {
		auto take_child = state.ui_state.popup_menu->parent->remove_child(state.ui_state.popup_menu);
		current_root->add_child_to_front(std::move(take_child));
	} else {
		current_root->move_child_to_front(state.ui_state.popup_menu);
	}
	state.ui_state.set_mouse_sensitive_target(state, state.ui_state.popup_menu);
}
void template_drop_down_control::hide_list(sys::state& state) {
	if(state.ui_state.popup_menu) {
		state.ui_state.popup_menu->set_visible(state, false);
	}
}
void template_drop_down_control::change_page(sys::state& state, int32_t to_page) {
	if(template_id == -1)
		return;
	if(!state.ui_state.popup_menu || state.ui_state.popup_menu->scroll_redirect != this) // menu not open
		return;

	state.ui_state.popup_menu->children.clear();
	page_text_out_of_date = true;

	grid_size_window* par = static_cast<grid_size_window*>(parent);
	auto lb = state.ui_templates.drop_down_t[template_id].list_button;
	auto elm_h_size = int32_t(state.ui_templates.mixed_button_t[lb].primary.h_text_margins * par->grid_size + state.ui_templates.mixed_button_t[lb].primary.icon_right.resolve(float(element_x_size), float(element_y_size), float(par->grid_size)) + element_x_size);

	list_page = to_page;

	if(total_items > items_per_page) { // add list buttons
		// create if non existing
		if(!page_controls) {
			page_controls = std::make_unique<drop_down_list_page_buttons>();
			page_controls->owner_control = this;
			page_controls->base_data.size.x = int16_t(par->grid_size * 8);
			page_controls->base_data.size.y = int16_t(par->grid_size * 2);
		}

		state.ui_state.popup_menu->children.push_back(page_controls.get());
		page_controls->base_data.position.x = int16_t(state.ui_state.popup_menu->base_data.size.x / 2 - page_controls->base_data.size.x / 2);
		page_controls->base_data.position.y = int16_t(state.ui_state.popup_menu->base_data.size.y - page_controls->base_data.size.y - (state.ui_templates.drop_down_t[template_id].dropdown_window_margin * par->grid_size));
		page_controls->parent = state.ui_state.popup_menu;
	}

	auto index = 0;
	while(index < items_per_page) { // place elements
		auto effective_index = to_page * items_per_page + index;
		if(effective_index >= total_items)
			break;

		auto x_offset = (two_columns && index >= items_per_page / 2 ? elm_h_size : 0) + (elm_h_size - element_x_size)  + state.ui_templates.drop_down_t[template_id].dropdown_window_margin * par->grid_size;
		auto y_offset = (two_columns && index >= items_per_page / 2 ? ((index - items_per_page / 2) * element_y_size) : index * element_y_size) + state.ui_templates.drop_down_t[template_id].dropdown_window_margin * par->grid_size;

		auto child = get_nth_item(state, effective_index, index);
		state.ui_state.popup_menu->children.push_back(child);
		child->base_data.position.x = int16_t(x_offset);
		child->base_data.position.y = int16_t(y_offset);
		child->parent = state.ui_state.popup_menu;
		child->impl_on_update(state);
		++index;
	}

	index = 0;
	bool alt = false;
	while(index < items_per_page) { // place buttons
		if(two_columns && index == items_per_page / 2)
			alt = false;

		auto effective_index = to_page * items_per_page + index;
		if(effective_index >= total_items)
			break;

		auto x_offset = (two_columns && index >= items_per_page / 2 ? elm_h_size : 0) + state.ui_templates.drop_down_t[template_id].dropdown_window_margin * par->grid_size;
		auto y_offset = (two_columns && index >= items_per_page / 2 ? ((index - items_per_page / 2) * element_y_size) : index * element_y_size) + state.ui_templates.drop_down_t[template_id].dropdown_window_margin * par->grid_size;
		if(index >= int32_t(list_buttons_pool.size())) {
			list_buttons_pool.emplace_back(std::make_unique<drop_down_list_button>());
			list_buttons_pool.back()->owner_control = this;
		}
		state.ui_state.popup_menu->children.push_back(list_buttons_pool[index].get());
		list_buttons_pool[index]->list_id = effective_index;
		list_buttons_pool[index]->template_id = (alt && state.ui_templates.drop_down_t[template_id].list_button_alt != -1) ? state.ui_templates.drop_down_t[template_id].list_button_alt : state.ui_templates.drop_down_t[template_id].list_button;
		list_buttons_pool[index]->base_data.position.x = int16_t(x_offset);
		list_buttons_pool[index]->base_data.position.y = int16_t(y_offset);
		list_buttons_pool[index]->base_data.size.x = int16_t(elm_h_size);
		list_buttons_pool[index]->base_data.size.y = int16_t(element_y_size);
		list_buttons_pool[index]->parent = state.ui_state.popup_menu;
		list_buttons_pool[index]->impl_on_update(state);
		alt = !alt;
		++index;
	}

}

void pop_up_menu_container::render(sys::state& state, int32_t x, int32_t y) noexcept {
	if(bg_template != -1) {
		ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[bg_template].renders.get_render(state, float(base_data.size.x) / float(grid_size), float(base_data.size.y) / float(grid_size), grid_size, state.user_settings.ui_scale));
	}
}

void template_drop_down_control::on_create(sys::state& state) noexcept {
	grid_size_window* par = static_cast<grid_size_window*>(parent);
	label_window->base_data.position.y = int16_t(base_data.size.y / 2 - label_window->base_data.size.y / 2);
	label_window->base_data.position.x = int16_t((base_data.size.x - base_data.size.y) / 2 - label_window->base_data.size.x / 2);
	if(template_id != -1) {
		label_window->base_data.position.y += int16_t(state.ui_templates.drop_down_t[template_id].vertical_nudge);
	}
}

void template_drop_down_control::render(sys::state& state, int32_t x, int32_t y) noexcept {
	grid_size_window* par = static_cast<grid_size_window*>(parent);

	if(template_id == -1)
		return;

	auto ms_after = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_activated);
	if(disabled) {
		auto bg_id = state.ui_templates.drop_down_t[template_id].disabled_bg;
		if(bg_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}
	} else if(ms_after.count() < mouse_over_animation_ms && state.ui_templates.drop_down_t[template_id].animate_active_transition) {
		float percentage = float(ms_after.count()) / float(mouse_over_animation_ms);
		if(this == state.ui_state.under_mouse) {
			auto active_id = state.ui_templates.drop_down_t[template_id].active_bg;
			if(active_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					0.0f, percentage);
			}
			auto bg_id = state.ui_templates.drop_down_t[template_id].primary_bg;
			if(bg_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					percentage, 1.0f);
			}

		} else {
			auto active_id = state.ui_templates.drop_down_t[template_id].active_bg;
			if(active_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					percentage, 1.0f);
			}
			auto bg_id = state.ui_templates.drop_down_t[template_id].primary_bg;
			if(bg_id != -1) {
				ogl::render_rect_slice(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
					state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale),
					0.0f, percentage);
			}
		}
	} else if(this == state.ui_state.under_mouse) {
		auto active_id = state.ui_templates.drop_down_t[template_id].active_bg;
		if(active_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[active_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}

	} else {
		auto bg_id = state.ui_templates.drop_down_t[template_id].primary_bg;
		if(bg_id != -1) {
			ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[bg_id].renders.get_render(state, float(base_data.size.x) / float(par->grid_size), float(base_data.size.y) / float(par->grid_size), int32_t(par->grid_size), state.user_settings.ui_scale));
		}
	}
}

void layout_window_element::initialize_template(sys::state& state, int32_t id, int32_t gs, bool ac) {
	window_template = id;
	grid_size = gs;
	if(ac) {
		auto_close = std::make_unique<auto_close_button>();
		auto_close->base_data.size.x = int16_t(grid_size * 3);
		auto_close->base_data.size.y = int16_t(grid_size * 3);
		auto_close->base_data.flags = uint8_t(ui::orientation::upper_right);
		if(window_template != -1) {
			auto_close->base_data.position.y = int16_t(state.ui_templates.window_t[window_template].v_close_button_margin * gs);
			auto_close->base_data.position.x = int16_t(-grid_size * 3 - state.ui_templates.window_t[window_template].h_close_button_margin * gs);
			auto_close->template_id = state.ui_templates.window_t[window_template].close_button_definition;
			auto_close->icon = state.ui_templates.window_t[window_template].close_button_icon;
		} else {
			auto_close->base_data.position.y = int16_t(0);
			auto_close->base_data.position.x = int16_t(-grid_size * 3);
		}
		auto_close->parent = this;
	}
}
void layout_window_element::render(sys::state& state, int32_t x, int32_t y) noexcept {
	if(window_template != -1 && state.ui_templates.window_t[window_template].bg != -1) {
		auto bgid = state.ui_templates.window_t[window_template].bg;
		ogl::render_textured_rect_direct(state, float(x), float(y), float(base_data.size.x), float(base_data.size.y),
				state.ui_templates.backgrounds[bgid].renders.get_render(state, float(base_data.size.x) / float(grid_size), float(base_data.size.y) / float(grid_size), grid_size, state.user_settings.ui_scale));
	}
	render_layout_internal(layout, state, x, y);
}

layout_level* innermost_scroll_level(layout_level& current, int32_t x, int32_t y) {
	if(x < current.resolved_x_pos || y < current.resolved_y_pos || current.resolved_x_pos + current.resolved_x_size < x || current.resolved_y_pos + current.resolved_y_size < y)
		return nullptr;

	for(auto& m : current.contents) {
		if(std::holds_alternative< sub_layout>(m)) {
			auto r = innermost_scroll_level(*std::get<sub_layout>(m).layout, x, y);
			if(r)
				return r;
		}
	}

	if(current.paged)
		return &current;

	return nullptr;
}

ui::message_result layout_window_element::on_scroll(sys::state& state, int32_t x, int32_t y, float amount, sys::key_modifiers mods) noexcept {
	auto sub_layout = innermost_scroll_level(layout, x, y);
	if(sub_layout) {
		sub_layout->change_page(state, *this, sub_layout->current_page + ((amount < 0) ? 1 : -1));
	}
	return ui::message_result::consumed;
}

struct layout_item_position {
	int32_t index = 0;
	int32_t sub_index = 0;

	bool operator==(layout_item_position const& o) const noexcept {
		return index == o.index && sub_index == o.sub_index;
	}
	bool operator!=(layout_item_position const& o) const noexcept {
		return !(*this == o);
	}
	bool operator<=(layout_item_position const& o) const noexcept {
		return (index < o.index) || (index == o.index && sub_index <= o.sub_index);
	}
	bool operator>=(layout_item_position const& o) const noexcept {
		return (index > o.index) || (index == o.index && sub_index >= o.sub_index);
	}
	bool operator<(layout_item_position const& o) const noexcept {
		return !(*this >= o);
	}
	bool operator>(layout_item_position const& o) const noexcept {
		return !(*this <= o);
	}
};

struct layout_iterator {
	std::vector<layout_item>& backing;
	layout_item_position position;

	layout_iterator(std::vector<layout_item>& backing) : backing(backing) {
	}

	bool current_is_glue() {
		return has_more() && std::holds_alternative<layout_glue>(backing[position.index]);
	}
	measure_result measure_current(sys::state& state, bool glue_horizontal, int32_t max_crosswise, bool first_in_section) {
		if(!has_more())
			return measure_result{ 0, 0, measure_result::special::none };
		auto& m = backing[position.index];

		if(std::holds_alternative<layout_control>(m)) {
			auto& i = std::get<layout_control>(m);
			if(i.absolute_position) {
				return  measure_result{ 0, 0, measure_result::special::none };
			}
			measure_result res;
			res.other = measure_result::special::none;
			res.x_space = i.ptr->base_data.size.x;
			res.y_space = i.ptr->base_data.size.y;
			if(i.fill_x) {
				if(glue_horizontal) {
					res.other = measure_result::special::space_consumer;
					res.x_space = 0;
				} else {
					res.x_space = int16_t(max_crosswise);
				}
			}
			if(i.fill_y) {
				if(!glue_horizontal) {
					res.other = measure_result::special::space_consumer;
					res.y_space = 0;
				} else {
					res.y_space = int16_t(max_crosswise);
				}
			}
			return res;
		} else if(std::holds_alternative<layout_window>(m)) {
			auto& i = std::get<layout_window>(m);
			if(i.absolute_position) {
				return  measure_result{ 0, 0, measure_result::special::none };
			}
			measure_result res;
			res.other = measure_result::special::none;
			res.x_space = i.ptr->base_data.size.x;
			res.y_space = i.ptr->base_data.size.y;
			if(i.fill_x) {
				if(glue_horizontal) {
					res.other = measure_result::special::space_consumer;
					res.x_space = 0;
				} else {
					res.x_space = int16_t(max_crosswise);
				}
			}
			if(i.fill_y) {
				if(!glue_horizontal) {
					res.other = measure_result::special::space_consumer;
					res.y_space = 0;
				} else {
					res.y_space = int16_t(max_crosswise);
				}
			}
			return res;
		} else if(std::holds_alternative<layout_glue>(m)) {
			auto& i = std::get<layout_glue>(m);
			if(glue_horizontal) {
				switch(i.type) {
				case glue_type::standard: return measure_result{ i.amount, 0, measure_result::special::none };
				case glue_type::at_least: return measure_result{ i.amount, 0, measure_result::special::space_consumer };
				case glue_type::line_break: return measure_result{ 0, 0, measure_result::special::end_line };
				case glue_type::page_break: return measure_result{ 0, 0, measure_result::special::end_page };
				case glue_type::glue_don_t_break: return measure_result{ i.amount, 0, measure_result::special::no_break };
				}
			} else {
				switch(i.type) {
				case glue_type::standard: return measure_result{ 0, i.amount, measure_result::special::none };
				case glue_type::at_least: return measure_result{ 0, i.amount, measure_result::special::space_consumer };
				case glue_type::line_break: return measure_result{ 0, 0, measure_result::special::end_line };
				case glue_type::page_break: return measure_result{ 0, 0, measure_result::special::end_page };
				case glue_type::glue_don_t_break: return measure_result{ 0, i.amount, measure_result::special::no_break };
				}
			}
		} else if(std::holds_alternative<generator_instance>(m)) {
			auto& i = std::get<generator_instance>(m);
			bool alt = false;
			return i.generator->place_item(state, nullptr, size_t(position.sub_index), 0, 0, first_in_section, alt);
		} else if(std::holds_alternative<sub_layout>(m)) {
			auto& i = std::get<sub_layout>(m);
			int32_t x = 0;
			int32_t y = 0;
			bool consume_fill = false;
			if(i.layout->size_x != -1)
				x = i.layout->size_x;
			else {
				if(glue_horizontal)
					consume_fill = true;
				else
					x = max_crosswise;
			}
			if(i.layout->size_y != -1)
				y = i.layout->size_y;
			else {
				if(!glue_horizontal)
					consume_fill = true;
				else
					y = max_crosswise;
			}

			return measure_result{ x, y, consume_fill ? measure_result::special::space_consumer : measure_result::special::none };
		}
		return measure_result{ 0, 0, measure_result::special::none };
	}
	void place_current(sys::state& state, layout_window_element* destination, int32_t x, int32_t y, int32_t width, int32_t height, bool first_in_section, bool& alternate, int32_t layout_x, int32_t layout_y, bool remake_pages) {
		if(!has_more())
			return;
		auto& m = backing[position.index];

		if(std::holds_alternative<layout_control>(m)) {
			auto& i = std::get<layout_control>(m);
			if(i.absolute_position) {
				i.ptr->base_data.position.x = int16_t(layout_x + i.abs_x);
				i.ptr->base_data.position.y = int16_t(layout_y + i.abs_y);
			} else {
				i.ptr->base_data.position.x = int16_t(x);
				i.ptr->base_data.position.y = int16_t(y);
			}
			if(i.fill_x)
				i.ptr->base_data.size.x = int16_t(width);
			if(i.fill_y)
				i.ptr->base_data.size.y = int16_t(height);
			destination->children.push_back(i.ptr);
			i.ptr->impl_on_update(state);
		} else if(std::holds_alternative<layout_window>(m)) {
			auto& i = std::get<layout_window>(m);
			if(i.absolute_position) {
				i.ptr->base_data.position.x = int16_t(layout_x + i.abs_x);
				i.ptr->base_data.position.y = int16_t(layout_y + i.abs_y);
			} else {
				i.ptr->base_data.position.x = int16_t(x);
				i.ptr->base_data.position.y = int16_t(y);
			}
			if(i.fill_x)
				i.ptr->base_data.size.x = int16_t(width);
			if(i.fill_y)
				i.ptr->base_data.size.y = int16_t(height);
			destination->children.push_back(i.ptr.get());
			i.ptr->impl_on_update(state);
		} else if(std::holds_alternative<layout_glue>(m)) {

		} else if(std::holds_alternative<generator_instance>(m)) {
			auto& i = std::get<generator_instance>(m);
			i.generator->place_item(state, destination, size_t(position.sub_index), x, y, first_in_section, alternate);
		} else if(std::holds_alternative<sub_layout>(m)) {
			auto& i = std::get<sub_layout>(m);
			destination->remake_layout_internal(*i.layout, state, x, y, width, height, remake_pages);
		}
	}
	void move_position(int32_t n) {
		while(n > 0 && has_more()) {
			if(std::holds_alternative<generator_instance>(backing[position.index])) {
				auto& g = std::get<generator_instance>(backing[position.index]);
				auto sub_count = g.generator->item_count();
				if(n >= int32_t(sub_count - position.sub_index)) {
					n -= int32_t(sub_count - position.sub_index);
					position.sub_index = 0;
					++position.index;
				} else {
					position.sub_index += n;
					n = 0;
				}
			} else {
				++position.index;
				--n;
			}
		}
		while(n < 0 && position.index >= 0) {
			if(position.index >= int32_t(backing.size())) {
				position.index = int32_t(backing.size()) - 1;
				if(backing.size() > 0 && std::holds_alternative<generator_instance>(backing[position.index])) {
					auto& g = std::get<generator_instance>(backing[position.index]);
					position.sub_index = std::max(int32_t(g.generator->item_count()) - 1, 0);
				}
				++n;
			} else if(std::holds_alternative<generator_instance>(backing[position.index])) {
				auto& g = std::get<generator_instance>(backing[position.index]);
				if(-n > position.sub_index) {
					n += (position.sub_index + 1);
					--position.index;
				} else {
					position.sub_index += n;
					n = 0;
					break; // don't reset sub index
				}
			} else {
				--position.index;
				++n;
			}

			if(position.index < 0) {
				position.sub_index = 0;
				position.index = 0; return;
			}
			if(std::holds_alternative<generator_instance>(backing[position.index])) {
				auto& g = std::get<generator_instance>(backing[position.index]);
				position.sub_index = std::max(int32_t(g.generator->item_count()) - 1, 0);
			}
		}
	}
	bool has_more() {
		return position.index < int32_t(backing.size());
	}
	void reset() {
		position.index = 0;
		position.sub_index = 0;
	}
};

void layout_window_element::impl_on_update(sys::state& state) noexcept {
	on_update(state);
}

void layout_window_element::clear_pages_internal(layout_level& lvl) {
	lvl.page_starts.clear();
	for(auto& m : lvl.contents) {
		if(holds_alternative<sub_layout>(m)) {
			auto& i = std::get<sub_layout>(m);
			clear_pages_internal(*i.layout);
		}
	}
}
void layout_window_element::render_layout_internal(layout_level& lvl, sys::state& state, int32_t x, int32_t y) {
	if(lvl.template_id != -1) {
		auto bg = state.ui_templates.layout_region_t[lvl.template_id].bg;
		if(bg != -1) {
			auto x_pos = lvl.resolved_x_pos;
			auto y_pos = lvl.resolved_y_pos;
			auto width = lvl.resolved_x_size;
			auto height = lvl.resolved_y_size;

			ogl::render_textured_rect_direct(state, float(x + x_pos), float(y + y_pos), float(width), float(height),
				state.ui_templates.backgrounds[bg].renders.get_render(state, float(width) / float(grid_size), float(height) / float(grid_size), int32_t(grid_size), state.user_settings.ui_scale));
		}
	}

	int32_t index_start = 0;
	if(lvl.current_page > 0 && lvl.page_starts.size() > 0) {
		index_start = lvl.page_starts[lvl.current_page - 1].last_index;
	}
	int32_t index_end = int32_t(lvl.contents.size());
	if(lvl.current_page < int32_t(lvl.page_starts.size())) {
		index_end = lvl.page_starts[lvl.current_page].last_index;
	}

	layout_iterator it(lvl.contents);
	it.move_position(index_start);
	for(int32_t j = index_start; j < index_end; ++j) {
		//sub_layout, texture_layer
		auto& c = lvl.contents[it.position.index];
		if(std::holds_alternative<texture_layer>(c)) {
			auto& i = std::get<texture_layer>(c);
			auto cmod = ogl::color_modification::none;
			ogl::render_textured_rect(state, cmod, float(x + lvl.resolved_x_pos), float(y + lvl.resolved_y_pos), float(lvl.resolved_x_size), float(lvl.resolved_y_size), ogl::get_late_load_texture_handle(state, i.texture_id, i.texture), base_data.get_rotation(), false, state_is_rtl(state));
		} else if(std::holds_alternative<sub_layout>(c)) {
			auto& i = std::get<sub_layout>(c);
			render_layout_internal(*i.layout, state, x, y);
		}
		it.move_position(1);
	}
}

struct layout_box {
	uint16_t x_dim = 0;
	uint16_t y_dim = 0;
	uint16_t item_count = 0;
	uint16_t space_conumer_count = 0;
	uint16_t non_glue_count = 0;
	bool end_page = false;
};

layout_box measure_horizontal_box(sys::state& state, layout_iterator& source, int32_t max_x, int32_t max_y) {
	layout_box result{};

	auto initial_pos = source.position;

	while(source.has_more()) {
		auto m_result = source.measure_current(state, true, max_y, source.position == initial_pos);
		bool is_glue = source.current_is_glue();
		int32_t xdtemp = result.x_dim;
		bool fits = ((m_result.x_space + result.x_dim) <= max_x) || (source.position == initial_pos) || is_glue;

		if(fits) {
			result.x_dim = std::min(uint16_t(m_result.x_space + result.x_dim), uint16_t(max_x));
			int32_t xdtemp2 = result.x_dim;
			result.y_dim = std::max(result.y_dim, uint16_t(m_result.y_space));
			if(m_result.other == measure_result::special::space_consumer) {
				++result.space_conumer_count;
			}
			++result.item_count;
			if(!is_glue)
				++result.non_glue_count;
			if(m_result.other == measure_result::special::end_page) {
				result.end_page = true;
				source.move_position(1);
				break;
			}
			if(m_result.other == measure_result::special::end_line) {
				source.move_position(1);
				break;
			}
		} else {
			break;
		}

		source.move_position(1);
	}

	if(source.has_more()) { // rollback only possible if we didn't run out of items before space
		int32_t rollback_count = 0;
		auto rollback_end_pos = source.position;

		// rollback loop -- drop any items that were glued to the preivous item
		while(source.position > initial_pos) {
			source.move_position(-1);
			auto m_result = source.measure_current(state, true, max_y, source.position == initial_pos);
			if(m_result.other != measure_result::special::no_break) {
				source.move_position(1);
				break;
			}
			if(source.current_is_glue()) // don't break just before no break glue
				source.move_position(-1);
		}

		if(source.position > initial_pos && rollback_end_pos != (source.position)) { // non trivial rollback
			result = layout_box{};
			auto new_end = source.position;
			source.position = initial_pos;

			// final measurement loop if rollback was non zero
			while(source.position < new_end) {
				auto m_result = source.measure_current(state, true, max_y, source.position == initial_pos);
				bool is_glue = source.current_is_glue();

				result.x_dim = std::min(uint16_t(m_result.x_space + result.x_dim), uint16_t(max_x));
				result.y_dim = std::max(result.y_dim, uint16_t(m_result.y_space));
				if(m_result.other == measure_result::special::space_consumer) {
					++result.space_conumer_count;
				}
				if(!is_glue)
					++result.non_glue_count;
				++result.item_count;

				if(m_result.other == measure_result::special::end_page) {
					result.end_page = true;
				}

				source.move_position(1);
			}
		} else { // if everything was rolled back, then ignore the break
			source.position = rollback_end_pos;
		}
	}
	return result;
}
layout_box measure_vertical_box(sys::state& state, layout_iterator& source, int32_t max_x, int32_t max_y) {
	layout_box result{};

	auto initial_pos = source.position;

	while(source.has_more()) {
		auto m_result = source.measure_current(state, false, max_x, source.position == initial_pos);
		bool is_glue = source.current_is_glue();
		bool fits = ((m_result.y_space + result.y_dim) <= max_y) || (source.position == initial_pos) || is_glue;

		if(fits) {
			result.y_dim = std::min(uint16_t(m_result.y_space + result.y_dim), uint16_t(max_y));
			result.x_dim = std::max(result.x_dim, uint16_t(m_result.x_space));
			if(m_result.other == measure_result::special::space_consumer) {
				++result.space_conumer_count;
			}
			++result.item_count;
			if(!is_glue)
				++result.non_glue_count;
			if(m_result.other == measure_result::special::end_page) {
				result.end_page = true;
				source.move_position(1);
				break;
			}
			if(m_result.other == measure_result::special::end_line) {
				source.move_position(1);
				break;
			}
		} else {
			break;
		}

		source.move_position(1);
	}

	if(source.has_more()) { // rollback only possible if we didn't run out of items before space
		int32_t rollback_count = 0;
		auto rollback_end_pos = source.position;

		// rollback loop -- drop any items that were glued to the preivous item

		while(source.position > initial_pos) {
			source.move_position(-1);
			auto m_result = source.measure_current(state, false, max_x, source.position == initial_pos);
			if(m_result.other != measure_result::special::no_break) {
				source.move_position(1);
				break;
			}
			if(source.current_is_glue()) // don't break just before no break glue
				source.move_position(-1);
		}

		if(source.position > initial_pos && rollback_end_pos != (source.position)) { // non trivial rollback
			result = layout_box{};
			auto new_end = source.position;
			source.position = initial_pos;

			// final measurement loop if rollback was non zero
			while(source.position < new_end) {
				auto m_result = source.measure_current(state, false, max_x, source.position == initial_pos);
				bool is_glue = source.current_is_glue();

				result.y_dim = std::min(uint16_t(m_result.y_space + result.y_dim), uint16_t(max_y));
				result.x_dim = std::max(result.x_dim, uint16_t(m_result.x_space));
				if(m_result.other == measure_result::special::space_consumer) {
					++result.space_conumer_count;
				}
				if(!is_glue)
					++result.non_glue_count;
				++result.item_count;

				if(m_result.other == measure_result::special::end_page) {
					result.end_page = true;
				}

				source.move_position(1);
			}
		}
	}
	return result;
}

void layout_window_element::remake_layout_internal(layout_level& lvl, sys::state& state, int32_t x, int32_t y, int32_t w, int32_t h, bool remake_pages) {
	auto base_x_size = lvl.size_x != -1 ? int32_t(lvl.size_x) : w;
	auto base_y_size = lvl.size_y != -1 ? int32_t(lvl.size_y) : h;
	auto top_margin = int32_t(lvl.margin_top);
	auto bottom_margin = lvl.margin_bottom != -1 ? int32_t(lvl.margin_bottom) : top_margin;
	auto left_margin = lvl.margin_left != -1 ? int32_t(lvl.margin_left) : bottom_margin;
	auto right_margin = lvl.margin_right != -1 ? int32_t(lvl.margin_right) : left_margin;
	auto effective_x_size = base_x_size - (left_margin + right_margin);
	auto effective_y_size = base_y_size - (top_margin + bottom_margin);
	if(lvl.paged) {
		effective_y_size -= lvl.page_controls->base_data.size.y;
		lvl.page_controls->base_data.position.x = int16_t(x + left_margin + (effective_x_size / 2) - (lvl.page_controls->base_data.size.x / 2));
		lvl.page_controls->base_data.position.y = int16_t(y + top_margin + effective_y_size);
		children.push_back(lvl.page_controls.get());
	}

	lvl.resolved_x_pos = int16_t(x);
	lvl.resolved_y_pos = int16_t(y );
	lvl.resolved_x_size = int16_t(base_x_size);
	lvl.resolved_y_size = int16_t(base_y_size);

	if(lvl.page_starts.empty())
		remake_pages = true;

	if(remake_pages) {
		switch(lvl.type) {
		case layout_type::single_horizontal:
		{
			layout_iterator it(lvl.contents);
			// measure loop
			int32_t running_count = 0;
			while(it.has_more()) {
				auto box = measure_horizontal_box(state, it, effective_x_size, effective_y_size);
				lvl.page_starts.push_back(page_info{ uint16_t(running_count + box.item_count) });
				running_count += box.item_count;
				assert(box.item_count > 0);
			}
			if(lvl.page_starts.empty()) {
				lvl.page_starts.push_back(page_info{ uint16_t(0) });
			}
		} break;
		case layout_type::single_vertical:
		{
			layout_iterator it(lvl.contents);
			// measure loop
			int32_t running_count = 0;
			while(it.has_more()) {
				auto box = measure_vertical_box(state, it, effective_x_size, effective_y_size);
				lvl.page_starts.push_back(page_info{ uint16_t(running_count + box.item_count) });
				running_count += box.item_count;
				assert(box.item_count > 0);
			}
			if(lvl.page_starts.empty()) {
				lvl.page_starts.push_back(page_info{ uint16_t(0) });
			}
		} break;
		case layout_type::overlapped_horizontal:
		{
			layout_iterator it(lvl.contents);
			auto box = measure_horizontal_box(state, it, std::numeric_limits<int32_t>::max(), effective_y_size);
			lvl.page_starts.push_back(page_info{ uint16_t(box.item_count) });
		} break;
		case layout_type::overlapped_vertical:
		{
			layout_iterator it(lvl.contents);
			auto box = measure_vertical_box(state, it, effective_x_size, std::numeric_limits<int32_t>::max());
			lvl.page_starts.push_back(page_info{ uint16_t(box.item_count) });
		} break;
		case layout_type::mulitline_horizontal:
		{
			layout_iterator it(lvl.contents);
			int32_t running_count = 0;

			while(it.has_more()) {
				int32_t y_remaining = effective_y_size;
				bool first = true;
				while(it.has_more()) {
					auto pre_pos = it.position;

					auto box = measure_horizontal_box(state, it, effective_x_size, y_remaining);
					assert(box.item_count > 0);
					if(box.y_dim > y_remaining && !first) { // end
						it.position = pre_pos;
						break;
					}
					running_count += box.item_count;
					y_remaining -= int32_t(box.y_dim + lvl.interline_spacing);
					if(y_remaining <= 0)
						break;
					if(box.end_page)
						break;
					first = false;
				}
				lvl.page_starts.push_back(page_info{ uint16_t(running_count)  });
			}
			if(lvl.page_starts.empty()) {
				lvl.page_starts.push_back(page_info{ uint16_t(0) });
			}
		} break;
		case layout_type::multiline_vertical:
		{
			layout_iterator it(lvl.contents);
			int32_t running_count = 0;

			while(it.has_more()) {
				int32_t x_remaining = effective_x_size;
				bool first = true;
				while(it.has_more()) {
					auto pre_pos = it.position;

					auto box = measure_vertical_box(state, it, x_remaining, effective_y_size);
					assert(box.item_count > 0);
					if(box.x_dim > x_remaining && !first) { // end
						it.position = pre_pos;
						break;
					}
					running_count += box.item_count;
					x_remaining -= int32_t(box.x_dim + lvl.interline_spacing);
					if(x_remaining <= 0)
						break;
					if(box.end_page)
						break;
					first = false;
				}
				lvl.page_starts.push_back(page_info{ uint16_t(running_count) });
			}
			if(lvl.page_starts.empty()) {
				lvl.page_starts.push_back(page_info{ uint16_t(0) });
			}
		} break;
		}
	} // end: remake lists

	for(auto& m : lvl.contents) {
		if(holds_alternative<generator_instance>(m)) {
			auto& i = std::get<generator_instance>(m);
			i.generator->reset_pools();
		}
	}

	// handle texture layers here: they do not depend on layout type

	if(window_template == -1) { // temporary until all ui windows are handled by templates
		layout_iterator place_it(lvl.contents);
		while(place_it.has_more()) {
			if(std::holds_alternative<texture_layer>(lvl.contents[place_it.position.index])) {
				auto& i = std::get<texture_layer>(lvl.contents[place_it.position.index]);
				positioned_texture temp{
					(int16_t)lvl.resolved_x_pos, (int16_t)lvl.resolved_y_pos, (int16_t)effective_x_size, (int16_t)effective_y_size,
					i.texture,
					dcon::texture_id{},
					i.texture_type
				};
				textures_to_render.push_back(temp);
			}
			place_it.move_position(1);
		}
	}

	switch(lvl.type) {
	case layout_type::single_horizontal:
	{
		lvl.current_page = std::clamp(lvl.current_page, int16_t(0), int16_t(lvl.page_starts.size() - 1));

		int32_t index_start = 0;
		if(lvl.current_page > 0) {
			index_start = lvl.page_starts[lvl.current_page - 1].last_index;
		}

		layout_iterator it(lvl.contents);
		it.move_position(index_start);

		auto start_pos = it.position;
		auto box = measure_horizontal_box(state, it, effective_x_size, effective_y_size);
		it.position = start_pos;

		int32_t space_used = box.x_dim;
		int32_t fill_consumer_count = box.space_conumer_count;
		// place / render

		int32_t extra_runlength = int32_t(effective_x_size - space_used);
		int32_t per_fill_consumer = fill_consumer_count != 0 ? (extra_runlength / fill_consumer_count) : 0;
		int32_t extra_lead = 0;
		switch(lvl.line_alignment) {
		case layout_line_alignment::leading: break;
		case layout_line_alignment::trailing: extra_lead = extra_runlength - fill_consumer_count * per_fill_consumer; break;
		case layout_line_alignment::centered: extra_lead = (extra_runlength - fill_consumer_count * per_fill_consumer) / 2;  break;
		}

		space_used = x + extra_lead + left_margin;
		bool alternate = true;
		for(uint16_t i = 0; i < box.item_count; ++i) {
			auto mr =  it.measure_current(state, true, effective_y_size, i == 0);
			int32_t yoff = 0;
			int32_t xoff = space_used;
			switch(lvl.line_internal_alignment) {
			case layout_line_alignment::leading: yoff = y + top_margin; break;
			case layout_line_alignment::trailing: yoff = y + top_margin + effective_y_size - mr.y_space; break;
			case layout_line_alignment::centered: yoff = y + top_margin + (effective_y_size - mr.y_space) / 2;  break;
			}

			it.place_current(state, this, xoff, yoff, mr.x_space + (mr.other == measure_result::special::space_consumer ? per_fill_consumer : 0), mr.y_space, i == 0, alternate, x, y, remake_pages);
			it.move_position(1);

			space_used += mr.x_space;
			if(mr.other == measure_result::special::space_consumer) {
				space_used += per_fill_consumer;
			}
		}
	} break;
	case layout_type::single_vertical:
	{
		lvl.current_page = std::clamp(lvl.current_page, int16_t(0), int16_t(lvl.page_starts.size() - 1));

		int32_t index_start = 0;
		if(lvl.current_page > 0) {
			index_start = lvl.page_starts[lvl.current_page - 1].last_index;
		}

		layout_iterator it(lvl.contents);
		it.move_position(index_start);

		auto start_pos = it.position;
		auto box = measure_vertical_box(state, it, effective_x_size, effective_y_size);
		it.position = start_pos;

		int32_t space_used = box.y_dim;
		int32_t fill_consumer_count = box.space_conumer_count;
		// place / render

		int32_t extra_runlength = int32_t(effective_y_size - space_used);
		int32_t per_fill_consumer = fill_consumer_count != 0 ? (extra_runlength / fill_consumer_count) : 0;
		int32_t extra_lead = 0;
		switch(lvl.line_alignment) {
		case layout_line_alignment::leading: break;
		case layout_line_alignment::trailing: extra_lead = extra_runlength - fill_consumer_count * per_fill_consumer; break;
		case layout_line_alignment::centered: extra_lead = (extra_runlength - fill_consumer_count * per_fill_consumer) / 2;  break;
		}

		space_used = y + extra_lead + top_margin;
		bool alternate = true;
		for(uint16_t i = 0; i < box.item_count; ++i) {
			auto mr = it.measure_current(state, false, effective_x_size, i == 0);

			int32_t xoff = 0;
			int32_t yoff = space_used;
			switch(lvl.line_internal_alignment) {
			case layout_line_alignment::leading: xoff = x + left_margin; break;
			case layout_line_alignment::trailing: xoff = x + left_margin + effective_x_size - mr.x_space; break;
			case layout_line_alignment::centered: xoff = x + left_margin + (effective_x_size - mr.x_space) / 2;  break;
			}

			it.place_current(state, this, xoff, yoff, mr.x_space, mr.y_space + (mr.other == measure_result::special::space_consumer ? per_fill_consumer : 0), i == 0, alternate, x, y, remake_pages);
			it.move_position(1);

			space_used += mr.y_space;
			if(mr.other == measure_result::special::space_consumer) {
				space_used += per_fill_consumer;
			}
		}
	} break;
	case layout_type::overlapped_horizontal:
	{
		lvl.current_page = std::clamp(lvl.current_page, int16_t(0), int16_t(lvl.page_starts.size() - 1));

		layout_iterator place_it(lvl.contents);
		int32_t index_start = 0;
		if(lvl.current_page > 0) {
			index_start = lvl.page_starts[lvl.current_page - 1].last_index;
		}
		place_it.move_position(index_start);

		auto pre_pos = place_it.position;
		auto box = measure_horizontal_box(state, place_it, std::numeric_limits<int32_t>::max(), effective_y_size);
		place_it.position = pre_pos;

		int32_t space_used = box.x_dim;
		int32_t fill_consumer_count = box.space_conumer_count;
		int32_t non_glue_count = box.non_glue_count;

		int32_t extra_runlength = std::max(0, int32_t(effective_x_size - space_used));
		int32_t per_fill_consumer = fill_consumer_count != 0 ? (extra_runlength / fill_consumer_count) : 0;
		int32_t extra_lead = 0;
		switch(lvl.line_alignment) {
		case layout_line_alignment::leading: break;
		case layout_line_alignment::trailing: extra_lead = extra_runlength - fill_consumer_count * per_fill_consumer; break;
		case layout_line_alignment::centered: extra_lead = (extra_runlength - fill_consumer_count * per_fill_consumer) / 2;  break;
		}
		int32_t overlap_subtraction = (non_glue_count > 1 && space_used > effective_x_size) ? int32_t(space_used - effective_x_size) / (non_glue_count - 1) : 0;
		space_used = x + extra_lead + left_margin;

		bool page_first = true;
		bool alternate = true;
		while(place_it.has_more()) {
			auto mr = place_it.measure_current(state, true, effective_y_size, page_first);
			int32_t yoff = 0;
			int32_t xoff = space_used;
			switch(lvl.line_internal_alignment) {
			case layout_line_alignment::leading: yoff = y + top_margin; break;
			case layout_line_alignment::trailing: yoff = y + top_margin + effective_y_size - mr.y_space; break;
			case layout_line_alignment::centered: yoff = y + top_margin + (effective_y_size - mr.y_space) / 2;  break;
			}
			bool was_abs = false;
			if(std::holds_alternative< layout_control>(lvl.contents[place_it.position.index])) {
				auto& i = std::get<layout_control>(lvl.contents[place_it.position.index]);
				was_abs = i.absolute_position;
			} else if(std::holds_alternative< layout_window>(lvl.contents[place_it.position.index])) {
				auto& i = std::get<layout_window>(lvl.contents[place_it.position.index]);
				was_abs = i.absolute_position;
			}
			place_it.place_current(state, this, xoff, yoff, mr.x_space + (mr.other == measure_result::special::space_consumer ? per_fill_consumer : 0), mr.y_space, page_first, alternate, x, y, remake_pages);

			if(!place_it.current_is_glue()) {
				page_first = false;
			}

			space_used += mr.x_space;
			if(mr.other == measure_result::special::space_consumer) {
				space_used += per_fill_consumer;
			}
			if(!place_it.current_is_glue() && !was_abs)
				space_used -= overlap_subtraction;

			place_it.move_position(1);
		}
	} break;
	case layout_type::overlapped_vertical:
	{
		lvl.current_page = std::clamp(lvl.current_page, int16_t(0), int16_t(lvl.page_starts.size() - 1));

		layout_iterator place_it(lvl.contents);
		int32_t index_start = 0;
		if(lvl.current_page > 0) {
			index_start = lvl.page_starts[lvl.current_page - 1].last_index;
		}
		place_it.move_position(index_start);

		auto pre_pos = place_it.position;
		auto box = measure_horizontal_box(state, place_it, effective_x_size, std::numeric_limits<int32_t>::max());
		place_it.position = pre_pos;

		int32_t space_used = box.y_dim;
		int32_t fill_consumer_count = box.space_conumer_count;
		int32_t non_glue_count = box.non_glue_count;

		int32_t extra_runlength = std::max(0, int32_t(effective_y_size - space_used));
		int32_t per_fill_consumer = fill_consumer_count != 0 ? (extra_runlength / fill_consumer_count) : 0;
		int32_t extra_lead = 0;
		switch(lvl.line_alignment) {
		case layout_line_alignment::leading: break;
		case layout_line_alignment::trailing: extra_lead = extra_runlength - fill_consumer_count * per_fill_consumer; break;
		case layout_line_alignment::centered: extra_lead = (extra_runlength - fill_consumer_count * per_fill_consumer) / 2;  break;
		}
		int32_t overlap_subtraction = (non_glue_count > 1 && space_used > effective_y_size) ? int32_t(space_used - effective_y_size) / (non_glue_count - 1) : 0;
		space_used = y + extra_lead + top_margin;

		bool page_first = true;
		bool alternate = true;
		while(place_it.has_more()) {
			auto mr = place_it.measure_current(state, false, effective_x_size, page_first);
			int32_t xoff = 0;
			int32_t yoff = space_used;
			switch(lvl.line_internal_alignment) {
			case layout_line_alignment::leading: xoff = x + left_margin; break;
			case layout_line_alignment::trailing: xoff = x + left_margin + effective_x_size - mr.x_space; break;
			case layout_line_alignment::centered: xoff = x + left_margin + (effective_x_size - mr.x_space) / 2;  break;
			}
			bool was_abs = false;
			if(std::holds_alternative< layout_control>(lvl.contents[place_it.position.index])) {
				auto& i = std::get<layout_control>(lvl.contents[place_it.position.index]);
				was_abs = i.absolute_position;
			} else if(std::holds_alternative< layout_window>(lvl.contents[place_it.position.index])) {
				auto& i = std::get<layout_window>(lvl.contents[place_it.position.index]);
				was_abs = i.absolute_position;
			}

			place_it.place_current(state, this, xoff, yoff, mr.x_space, mr.y_space + (mr.other == measure_result::special::space_consumer ? per_fill_consumer : 0), page_first, alternate, x, y, remake_pages);

			if(!place_it.current_is_glue()) {
				page_first = false;
			}

			space_used += mr.y_space;
			if(mr.other == measure_result::special::space_consumer) {
				space_used += per_fill_consumer;
			}
			if(!place_it.current_is_glue() && !was_abs)
				space_used -= overlap_subtraction;

			place_it.move_position(1);
		}
	} break;
	case layout_type::mulitline_horizontal:
	{
		layout_iterator place_it(lvl.contents);
		int32_t index_start = 0;
		lvl.current_page = std::clamp(lvl.current_page, int16_t(0), int16_t(lvl.page_starts.size() - 1));
		if(lvl.current_page > 0) {
			index_start = lvl.page_starts[lvl.current_page - 1].last_index;
		}
		place_it.move_position(index_start);

		int32_t y_remaining = effective_y_size;
		bool first = true;
		while(place_it.has_more()) {
			auto pre_pos = place_it.position;

			auto box = measure_horizontal_box(state, place_it, effective_x_size, y_remaining);
			assert(box.item_count > 0);
			if(box.y_dim > y_remaining && !first) { // end
				break;
			}

			place_it.position = pre_pos;
			bool alternate = true;

			int32_t extra_runlength = int32_t(effective_x_size - box.x_dim);
			int32_t per_fill_consumer = box.space_conumer_count != 0 ? (extra_runlength / box.space_conumer_count) : 0;
			int32_t extra_lead = 0;
			switch(lvl.line_alignment) {
			case layout_line_alignment::leading: break;
			case layout_line_alignment::trailing: extra_lead = extra_runlength - box.space_conumer_count * per_fill_consumer; break;
			case layout_line_alignment::centered: extra_lead = (extra_runlength - box.space_conumer_count * per_fill_consumer) / 2;  break;
			}
			auto space_used = x + extra_lead + left_margin;

			for(uint16_t i = 0; i < box.item_count; ++i) {
				auto mr = place_it.measure_current(state, false, effective_x_size, i == 0);

				int32_t yoff = 0;
				int32_t xoff = space_used;
				switch(lvl.line_internal_alignment) {
				case layout_line_alignment::leading: yoff = y + top_margin + (effective_y_size - y_remaining); break;
				case layout_line_alignment::trailing: yoff = y + top_margin + (effective_y_size - y_remaining) + box.y_dim - mr.y_space; break;
				case layout_line_alignment::centered: yoff = y + top_margin + (effective_y_size - y_remaining) + (box.y_dim - mr.y_space) / 2;  break;
				}
				place_it.place_current(state, this, xoff, yoff, mr.x_space, mr.y_space + (mr.other == measure_result::special::space_consumer ? per_fill_consumer : 0), i == 0, alternate, x, y, remake_pages);
				place_it.move_position(1);

				space_used += mr.x_space;
				if(mr.other == measure_result::special::space_consumer) {
					space_used += per_fill_consumer;
				}
			}

			y_remaining -= int32_t(box.y_dim + lvl.interline_spacing);
			if(y_remaining <= 0) {
				break;
			}
			if(box.end_page) {
				break;
			}
			first = false;
		}
		
	} break;
	case layout_type::multiline_vertical:
	{
		layout_iterator place_it(lvl.contents);
		int32_t index_start = 0;
		lvl.current_page = std::clamp(lvl.current_page, int16_t(0), int16_t(lvl.page_starts.size() - 1));
		if(lvl.current_page > 0) {
			index_start = lvl.page_starts[lvl.current_page - 1].last_index;
		}
		place_it.move_position(index_start);


		int32_t x_remaining = effective_x_size;
		bool first = true;
		while(place_it.has_more()) {
			auto pre_pos = place_it.position;

			auto box = measure_vertical_box(state, place_it, x_remaining, effective_y_size);
			assert(box.item_count > 0);
			if(box.x_dim > x_remaining && !first) { // end
				break;
			}

			place_it.position = pre_pos;
			bool alternate = true;

			int32_t extra_runlength = int32_t(effective_y_size - box.y_dim);
			int32_t per_fill_consumer = box.space_conumer_count != 0 ? (extra_runlength / box.space_conumer_count) : 0;
			int32_t extra_lead = 0;
			switch(lvl.line_alignment) {
			case layout_line_alignment::leading: break;
			case layout_line_alignment::trailing: extra_lead = extra_runlength - box.space_conumer_count * per_fill_consumer; break;
			case layout_line_alignment::centered: extra_lead = (extra_runlength - box.space_conumer_count * per_fill_consumer) / 2;  break;
			}
			auto space_used = y + extra_lead + top_margin;

			for(uint16_t i = 0; i < box.item_count; ++i) {
				auto mr = place_it.measure_current(state, false, effective_x_size, i == 0);

				int32_t xoff = 0;
				int32_t yoff = space_used;
				switch(lvl.line_internal_alignment) {
				case layout_line_alignment::leading: xoff = x + left_margin + (effective_x_size - x_remaining); break;
				case layout_line_alignment::trailing: xoff = x + left_margin + (effective_x_size - x_remaining) + box.x_dim - mr.x_space; break;
				case layout_line_alignment::centered: xoff = x + left_margin + (effective_x_size - x_remaining) + (box.x_dim - mr.x_space) / 2;  break;
				}
				place_it.place_current(state, this, xoff, yoff, mr.x_space + (mr.other == measure_result::special::space_consumer ? per_fill_consumer : 0), mr.y_space, i == 0, alternate, x, y, remake_pages);
				place_it.move_position(1);

				space_used += mr.y_space;
				if(mr.other == measure_result::special::space_consumer) {
					space_used += per_fill_consumer;
				}
			}

			x_remaining -= int32_t(box.x_dim + lvl.interline_spacing);
			if(x_remaining <= 0)
				break;
			if(box.end_page)
				break;
			first = false;
		}

	} break;
	}
}

std::string_view basic_component_to_name(sys::basic_component_type type) {
	switch(type) {
	case sys::basic_component_type::diode:
		return "diode";
	case sys::basic_component_type::enable_high_transistor:
		return "enable_high_transistor";
	case sys::basic_component_type::enable_low_transistor:
		return "enable_low_transistor";
	}
	return "";
}

int32_t basic_component_to_icon(sys::state& state, sys::basic_component_type type) {
	static int32_t dicon = template_project::icon_by_name(state.ui_templates, "diode.svg");
	static int32_t ehicon = template_project::icon_by_name(state.ui_templates, "npntransistor.svg");
	static int32_t elicon = template_project::icon_by_name(state.ui_templates, "pnptransistor.svg");

	switch(type) {
	case sys::basic_component_type::diode:
		return dicon;
	case sys::basic_component_type::enable_high_transistor:
		return ehicon;
	case sys::basic_component_type::enable_low_transistor:
		return elicon;
	}
	return 0;
}

int32_t basic_component_to_color(sys::state& state, sys::basic_component_type type) {
	static int32_t dclr = template_project::color_by_name(state.ui_templates, "blue");
	static int32_t ehclr = template_project::color_by_name(state.ui_templates, "sky blue");
	static int32_t elclr = template_project::color_by_name(state.ui_templates, "violet");

	switch(type) {
	case sys::basic_component_type::diode:
		return dclr;
	case sys::basic_component_type::enable_high_transistor:
		return ehclr;
	case sys::basic_component_type::enable_low_transistor:
		return elclr;
	}
	return 0;
}

}
