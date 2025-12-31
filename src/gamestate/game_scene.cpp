#include "game_scene.hpp"
#include "simple_fs.hpp"
#include "user_interactions.hpp"
#include "alice_ui.hpp"
#include "opengl_wrapper.hpp"

namespace game_scene {

void in_game_hotkeys(sys::state& state, sys::virtual_key keycode, sys::key_modifiers mod);
void do_nothing_hotkeys(sys::state& state, sys::virtual_key keycode, sys::key_modifiers mod);
void render_module_contents(sys::state& state);
void in_game_scroll(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mod, float amount);

sys::virtual_key replace_keycodes_identity(sys::state& state, sys::virtual_key keycode, sys::key_modifiers mod) {
	return keycode;
}


void switch_scene(sys::state& state, scene_id ui_scene) {
	switch(ui_scene) {
	case scene_id::loading:
	{
		state.current_scene = scene_properties{
			.id = scene_id::loading,

			.get_root = [](sys::state& state) { return state.ui_state.root.get(); },
			.allow_drag_selection = false,
			.on_drag_start = do_nothing_screen,
			.drag_selection = do_nothing_screen,
			.lbutton_up = do_nothing,
			.keycode_mapping = replace_keycodes_identity,
			.handle_hotkeys = do_nothing_hotkeys,
			.render_screen = do_nothing,
			.recalculate_mouse_probe = recalculate_mouse_probe_identity,
			.recalculate_tooltip_probe = recalculate_mouse_probe_identity,
			.on_game_state_update = do_nothing,
		};
	} break;
	case scene_id::in_game_basic:
	{
		state.current_scene = scene_properties{
			.id = scene_id::in_game_basic,

			.get_root = [](sys::state& state) { return state.ui_state.root.get(); },
			.allow_drag_selection = false,
			.on_drag_start = do_nothing_screen,
			.drag_selection = do_nothing_screen,
			.lbutton_up = do_nothing,
			.keycode_mapping = replace_keycodes_identity,
			.handle_hotkeys = in_game_hotkeys,
			.render_screen = render_module_contents,
			.recalculate_mouse_probe = recalculate_mouse_probe_identity,
			.recalculate_tooltip_probe = recalculate_mouse_probe_identity,
			.on_scroll = in_game_scroll,
			.on_game_state_update = do_nothing,
		};
		// move scene ui windows to front here

		{
			auto ptr = alice_ui::display_at_front<alice_ui::make_tool_pane_main_panel>(state);
			ptr->base_data.size.y = int16_t(state.ui_state.root->base_data.size.y - ptr->base_data.position.y);
		}
	} break;
	case scene_id::count: // this should never happen
		assert(false);
		break;
	}
	
	state.game_state_updated.store(true, std::memory_order_release);
}

void do_nothing(sys::state& state) { }
void do_nothing_screen(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mod) { }

void render_module_contents(sys::state& state) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(state.open_gl.ui_shader_program);
	glUniform1i(state.open_gl.ui_shader_texture_sampler_uniform, 0);
	glUniform1i(state.open_gl.ui_shader_secondary_texture_sampler_uniform, 1);
	glUniform1f(state.open_gl.ui_shader_screen_width_uniform, float(state.x_size) / state.user_settings.ui_scale);
	glUniform1f(state.open_gl.ui_shader_screen_height_uniform, float(state.y_size) / state.user_settings.ui_scale);
	glUniform1f(state.open_gl.ui_shader_gamma_uniform, 1.0f);
	glViewport(0, 0, state.x_size, state.y_size);
	glDepthRange(-1.0f, 1.0f);


	glBindVertexArray(state.open_gl.global_square_vao);
	glBindVertexBuffer(0, state.open_gl.global_square_buffer, 0, sizeof(GLfloat) * 4);
	glUniform4f(state.open_gl.ui_shader_d_rect_uniform, 0.0f, 0.0f, float(state.x_size) / state.user_settings.ui_scale, float(state.y_size) / state.user_settings.ui_scale);
	GLuint subroutines[2] = { 0, 27 };
	glUniform2ui(state.open_gl.ui_shader_subroutines_index_uniform, subroutines[0], subroutines[1]);

	auto half_x = int32_t(float(state.x_size) / state.user_settings.ui_scale) / 2;
	auto half_y = int32_t(float(state.y_size) / state.user_settings.ui_scale) / 2;

	glUniform3f(state.open_gl.ui_shader_inner_color_uniform, float(state.x_offset - half_x), float(state.y_offset - half_y), 0.0f);
	glUniform1f(state.open_gl.ui_shader_border_size_uniform, float(state.zoom));

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void in_game_scroll(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mod, float amount) {
	//auto old_x_center = ;
	if(amount < 0 && state.zoom > 4) {
		state.zoom /= 2;
		state.x_offset /= 2;
		state.y_offset /= 2;
	}
	if(amount > 0 && state.zoom < 128) {
		state.zoom *= 2;
		state.x_offset *= 2;
		state.y_offset *= 2;
	}
}

float get_effects_volume(sys::state& state) {
	return state.user_settings.effects_volume * state.user_settings.master_volume;
}


void start_dragging(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mod) {
	state.x_drag_start = x;
	state.y_drag_start = y;
	state.drag_selecting = true;
	//window::change_cursor(state, window::cursor_type::normal);
}

void stop_dragging(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mod) {
	state.is_dragging = false;
	if(state.ui_state.drag_target) {
		state.on_drag_finished(x, y, mod);
	}
}

void ui_rbutton(sys::state& state, sys::key_modifiers mod) {
	state.ui_state.under_mouse->impl_on_rbutton_down(
		state,
		state.ui_state.relative_mouse_location.x,
		state.ui_state.relative_mouse_location.y,
		mod
	);
}
void ui_lbutton(sys::state& state, sys::key_modifiers mod) {
	auto result = state.ui_state.under_mouse->impl_on_lbutton_down(
		state,
		state.ui_state.relative_mouse_location.x,
		state.ui_state.relative_mouse_location.y,
		mod
	);
	if(result != ui::message_result::unseen) {
		state.ui_state.set_focus_target(state, state.ui_state.under_mouse);
	}
	state.ui_state.left_mouse_hold_target = state.ui_state.under_mouse;
}


void on_rbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mod) {
	// Lose focus on text
	state.ui_state.set_focus_target(state, nullptr);

	//if we clicked on UI element, handle it
	if(state.ui_state.under_mouse != nullptr) {
		ui_rbutton(state, mod);
		return;
	}
	// otherwise ...
}

void on_lbutton_down(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mod) {
	if(state.ui_state.under_mouse != nullptr) {
		ui_lbutton(state, mod);
		return;
	}

	// otherwise ...
	state.ui_state.set_focus_target(state, nullptr);
}

void on_lbutton_up_ui_click_hold_and_release(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mod) {
	auto rel_x = state.ui_state.relative_mouse_location.x;
	auto rel_y = state.ui_state.relative_mouse_location.y;

	bool mouse_over_target = state.ui_state.under_mouse == state.ui_state.left_mouse_hold_target;

	if(!state.current_scene.allow_drag_selection) {
		if(mouse_over_target) {
			state.ui_state.under_mouse->impl_on_lbutton_up(state, rel_x, rel_y, mod, true);
		} else {
			state.ui_state.left_mouse_hold_target->impl_on_lbutton_up(state, rel_x, rel_y, mod, false);
		}
	} else {
		if(mouse_over_target) {
			state.ui_state.left_mouse_hold_target = nullptr;
			state.ui_state.under_mouse->impl_on_lbutton_up(state, rel_x, rel_y, mod, true);
		} else if(!mouse_over_target && !state.drag_selecting) {
			state.ui_state.left_mouse_hold_target->impl_on_lbutton_up(state, rel_x, rel_y, mod, false);
			ui::element_base* temp_hold_target = state.ui_state.left_mouse_hold_target;
			state.ui_state.left_mouse_hold_target = nullptr;
		}
	}
}



void handle_drag_stop(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mod) {
	bool insignificant_movement =
		std::abs(x - state.x_drag_start) <= int32_t(std::ceil(state.x_size * 0.0025))
		&& std::abs(y - state.y_drag_start) <= int32_t(std::ceil(state.x_size * 0.0025));

	if(state.ui_state.under_mouse != nullptr || !state.drag_selecting) {
		state.drag_selecting = false;
		if(state.ui_state.edit_target_internal == nullptr)
			window::change_cursor(state, window::cursor_type::normal);
	} else if(insignificant_movement) {
		// we assume that user wanted to click
		state.drag_selecting = false;
		window::change_cursor(state, window::cursor_type::normal);

		state.game_state_updated.store(true, std::memory_order_release);
	} else {
		// stop dragging and select units
		state.drag_selecting = false;
		window::change_cursor(state, window::cursor_type::normal);
		if(x < state.x_drag_start)
			std::swap(x, state.x_drag_start);
		if(y < state.y_drag_start)
			std::swap(y, state.y_drag_start);

		state.current_scene.drag_selection(state, x, y, mod);
	}
}

void on_lbutton_up(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mod) {
	stop_dragging(state, x, y, mod);
	if(state.user_settings.left_mouse_click_hold_and_release && state.ui_state.left_mouse_hold_target) {
		on_lbutton_up_ui_click_hold_and_release(state, x, y, mod);
	}


	if(!state.ui_state.under_mouse) {
		state.current_scene.lbutton_up(state);
	}

	// if we were holding some "button" and this scene doesn't allow drag selection, then we can safely return
	if(state.user_settings.left_mouse_click_hold_and_release
		&& state.ui_state.left_mouse_hold_target
		&& !state.current_scene.allow_drag_selection
	) {
		return;
	}

	handle_drag_stop(state, x, y, mod);
}


sys::virtual_key replace_keycodes(sys::state& state, sys::virtual_key keycode, sys::key_modifiers mod) {
	if(keycode == sys::virtual_key::MINUS)
		return sys::virtual_key::SUBTRACT;
	else if(keycode == sys::virtual_key::PLUS)
		return sys::virtual_key::ADD;

	return state.current_scene.keycode_mapping(state, keycode, mod);
}


void in_game_hotkeys(sys::state& state, sys::virtual_key keycode, sys::key_modifiers mod) {
	if(state.ui_state.root->impl_on_key_down(state, keycode, mod) != ui::message_result::consumed) {
		uint32_t ctrl_group = 0;
		if(keycode == sys::virtual_key::ESCAPE) {
			alice_ui::display_at_front<alice_ui::make_main_menu_base>(state);
		}
		if(keycode == sys::virtual_key::A)
			keycode = sys::virtual_key::LEFT;
		if(keycode == sys::virtual_key::W)
			keycode = sys::virtual_key::UP;
		if(keycode == sys::virtual_key::S)
			keycode = sys::virtual_key::DOWN;
		if(keycode == sys::virtual_key::D)
			keycode = sys::virtual_key::RIGHT;

		if(keycode == sys::virtual_key::LEFT || keycode == sys::virtual_key::HOME || keycode == sys::virtual_key::RIGHT || keycode == sys::virtual_key::UP || keycode == sys::virtual_key::DOWN) {
			if(state.ui_state.mouse_sensitive_target) {
				state.ui_state.mouse_sensitive_target->set_visible(state, false);
				state.ui_state.mouse_sensitive_target = nullptr;
			}
		}

		if(keycode == sys::virtual_key::LEFT) {
			auto xamount = int32_t(float(state.x_size) / state.user_settings.ui_scale) / 4;
			state.x_offset -= xamount;
		}
		if(keycode == sys::virtual_key::RIGHT) {
			auto xamount = int32_t(float(state.x_size) / state.user_settings.ui_scale) / 4;
			state.x_offset += xamount;
		}
		if(keycode == sys::virtual_key::UP) {
			auto yamount = int32_t(float(state.y_size) / state.user_settings.ui_scale) / 4;
			state.y_offset -= yamount;
		}
		if(keycode == sys::virtual_key::DOWN) {
			auto yamount = int32_t(float(state.y_size) / state.user_settings.ui_scale) / 4;
			state.y_offset += yamount;
		}
		if(keycode == sys::virtual_key::HOME) {
			state.y_offset = 0;
			state.x_offset = 0;
		}

		sound::play_interface_sound(state, sound::get_click_sound(state), state.user_settings.interface_volume * state.user_settings.master_volume);
	}
}

void do_nothing_hotkeys(sys::state& state, sys::virtual_key keycode, sys::key_modifiers mod) {
	return;
}


void on_key_down(sys::state& state, sys::virtual_key keycode, sys::key_modifiers mod) {
	if(state.ui_state.edit_target_internal && keycode != sys::virtual_key::ESCAPE) {
		state.ui_state.edit_target_internal->impl_on_key_down(state, keycode, mod);
	} else {
		keycode = replace_keycodes(state, keycode, mod);
		state.current_scene.handle_hotkeys(state, keycode, mod);
	}
}


ui::mouse_probe recalculate_mouse_probe_identity(sys::state& state, ui::mouse_probe mouse_probe, ui::mouse_probe tooltip_probe) {
	return mouse_probe;
}

}
