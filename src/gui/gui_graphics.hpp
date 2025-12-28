#pragma once

#include <vector>
#include <array>
#include <chrono>
#include <any>
#include "constants_dcon.hpp"
#include "constants.hpp"
#include "dcon_generated_ids.hpp"
#include "unordered_dense.h"
#include "container_types.hpp"
#include "simple_fs.hpp"
#include "constants_ui.hpp"
#include "container_types_ui.hpp"

namespace parsers {
struct building_gfx_context;
}

namespace alice_ui {
class pop_up_menu_container;
}

namespace ui {

class context_menu_window;
class factory_refit_window;



struct element_data {
	static constexpr uint8_t type_mask = 0x07;
	static constexpr uint8_t rotation_mask = (0x03 << rotation_bit_offset);
	static constexpr uint8_t orientation_mask = (0x07 << orientation_bit_offset);

	static constexpr uint8_t ex_is_top_level = 0x01;

	xy_pair position; // 4bytes
	xy_pair size; // 8bytes
	
	uint8_t flags = 0;
	uint8_t ex_flags = 0; 

	rotation get_rotation() const {
		return rotation(flags & rotation_mask);
	}
	orientation get_orientation() const {
		return orientation(flags & orientation_mask);
	}
	bool is_top_level() const {
		return (ex_flags & ex_is_top_level) != 0;
	}
};

class definitions {
public:
	static constexpr dcon::texture_id small_tiles_dialog = dcon::texture_id(0);
	static constexpr dcon::texture_id tiles_dialog = dcon::texture_id(1);
	static constexpr dcon::texture_id transparency = dcon::texture_id(2);

	tagged_vector<dcon::text_key, dcon::texture_id> textures;
	tagged_vector<element_data, dcon::gui_def_id> gui;
};


class element_base;

xy_pair child_relative_location(sys::state& state, element_base const& parent, element_base const& child);
xy_pair get_absolute_location(sys::state& state, element_base const& node);

xy_pair child_relative_non_mirror_location(sys::state& state, element_base const& parent, element_base const& child);
xy_pair get_absolute_non_mirror_location(sys::state& state, element_base const& node);

using ui_hook_fn = std::unique_ptr<element_base> (*)(sys::state&, dcon::gui_def_id);

struct element_target {
	ui_hook_fn generator = nullptr;
	dcon::gui_def_id definition;
};

class tool_tip;

template<class T>
class unit_details_window;

struct hash_text_key {
	using is_avalanching = void;
	using is_transparent = void;

	hash_text_key() { }

	auto operator()(dcon::text_key sv) const noexcept -> uint64_t {
		return ankerl::unordered_dense::detail::wyhash::hash(&sv, sizeof(sv));
	}
};

template<typename T>
constexpr ui_hook_fn hook() {
	return +[](sys::state&, dcon::gui_def_id) { return std::make_unique<T>(); };
}

std::unique_ptr<element_base> make_element(sys::state& state, std::string_view name);
std::unique_ptr<element_base> make_element_immediate(sys::state& state, dcon::gui_def_id id); // bypasses global map
void place_in_drag_and_drop(sys::state& state, element_base& elm, std::any const& data, drag_and_drop_data type);

int32_t ui_width(sys::state const& state);
int32_t ui_height(sys::state const& state);

inline ogl::color_modification get_color_modification(bool is_under_mouse, bool is_disabled, bool is_interactable) {
	if(!is_under_mouse || !is_interactable) {
		if(is_disabled) {
			return ogl::color_modification::disabled;
		} else {
			return ogl::color_modification::none;
		}
	} else {
		if(is_disabled) {
			return ogl::color_modification::interactable_disabled;
		} else {
			return ogl::color_modification::interactable;
		}
	}
}

//void create_in_game_windows(sys::state& state);

} // namespace ui
