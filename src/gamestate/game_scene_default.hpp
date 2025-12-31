#pragma once

#include "game_scene_constants.hpp"
#include "constants_ui.hpp"
#include "constants.hpp"
#include "container_types_ui.hpp"

namespace game_scene {

void do_nothing(sys::state& state);
inline void on_scroll_do_nothing(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mod, float amount) { }

ui::mouse_probe recalculate_mouse_probe_identity(sys::state& state, ui::mouse_probe mouse_probe, ui::mouse_probe tooltip_probe);

struct scene_properties {
	scene_id id;

	std::function<ui::element_base* (sys::state& state)> get_root;

	bool enforced_pause = false;
	bool game_in_progress = true;

	//drag related
	bool allow_drag_selection;
	std::function<void(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mod)> on_drag_start;
	std::function<void(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mod)> drag_selection;

	std::function<void(sys::state& state)> lbutton_up;

	//key presses related
	std::function <sys::virtual_key(sys::state& state, sys::virtual_key keycode, sys::key_modifiers mod)> keycode_mapping;
	std::function <void(sys::state& state, sys::virtual_key keycode, sys::key_modifiers mod)> handle_hotkeys;

	//render
	std::function <void(sys::state& state)> render_screen = do_nothing;

	//mouse probing
	std::function <ui::mouse_probe(sys::state& state, ui::mouse_probe mouse_probe, ui::mouse_probe tooltip_probe)> recalculate_mouse_probe = recalculate_mouse_probe_identity;
	std::function <ui::mouse_probe(sys::state& state, ui::mouse_probe mouse_probe, ui::mouse_probe tooltip_probe)> recalculate_tooltip_probe = recalculate_mouse_probe_identity;
	std::function<void(sys::state& state, int32_t x, int32_t y, sys::key_modifiers mod, float amount)> on_scroll = on_scroll_do_nothing;

	std::function <void(sys::state& state)> on_game_state_update= do_nothing;

};
}
