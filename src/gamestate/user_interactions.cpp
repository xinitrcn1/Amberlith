#include "system_state.hpp"
#include "gui_element_base.hpp"

namespace sys {

void on_mouse_wheel(sys::state& state, int32_t x, int32_t y, key_modifiers mod, float amount) { // an amount of 1.0 is one "click" of the wheel
	ui::element_base* root_elm = state.current_scene.get_root(state);
	auto probe_result = root_elm->impl_probe_mouse(state,
		int32_t(state.mouse_x_position / state.user_settings.ui_scale),
		int32_t(state.mouse_y_position / state.user_settings.ui_scale),
		ui::mouse_probe_type::scroll);

	state.ui_state.scroll_target = probe_result.under_mouse;

	if(state.ui_state.scroll_target != nullptr) {
		state.ui_state.scroll_target->impl_on_scroll(state, probe_result.relative_location.x, probe_result.relative_location.y, amount, mod);
	} else if(state.ui_state.under_mouse == nullptr) {
		if(state.ui_state.mouse_sensitive_target) {
			state.ui_state.mouse_sensitive_target->set_visible(state, false);
			state.ui_state.mouse_sensitive_target = nullptr;
		}
		state.current_scene.on_scroll(state, x, y, mod, amount);
	}
}

}
