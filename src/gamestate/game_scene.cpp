#include "game_scene.hpp"
#include "simple_fs.hpp"
#include "user_interactions.hpp"
#include "alice_ui.hpp"

namespace game_scene {

void in_game_hotkeys(sys::state& state, sys::virtual_key keycode, sys::key_modifiers mod);

sys::virtual_key replace_keycodes_identity(sys::state& state, sys::virtual_key keycode, sys::key_modifiers mod) {
	return keycode;
}


void switch_scene(sys::state& state, scene_id ui_scene) {
	switch(ui_scene) {
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
			.render_screen = do_nothing,
			.recalculate_mouse_probe = recalculate_mouse_probe_identity,
			.recalculate_tooltip_probe = recalculate_mouse_probe_identity,
			.on_game_state_update = do_nothing,
		};
		// move scene ui windows to front here
	} break;
	case scene_id::count: // this should never happen
		assert(false);
		break;
	}
	
	state.game_state_updated.store(true, std::memory_order_release);
}

void do_nothing(sys::state& state) { }
void do_nothing_screen(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mod) { }

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
		if(keycode == sys::virtual_key::LEFT || keycode == sys::virtual_key::RIGHT || keycode == sys::virtual_key::UP || keycode == sys::virtual_key::DOWN) {
			if(state.ui_state.mouse_sensitive_target) {
				state.ui_state.mouse_sensitive_target->set_visible(state, false);
				state.ui_state.mouse_sensitive_target = nullptr;
			}
		}
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
