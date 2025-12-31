#pragma once

#include <memory>
#include <stdint.h>
#include <atomic>
#include <chrono>
#include <condition_variable>

#include "window.hpp"
#include "sound.hpp"
#include "dcon_generated.hpp"
#include "constants_dcon.hpp"
#include "constants.hpp"
#include "asvg.hpp"
#include "uitemplate.hpp"
#include "SPSCQueue.h"
#include "text.hpp"
#include "game_scene.hpp"
#include "graphics/opengl_wrapper.hpp"
#include "gui/ui_state.hpp"
#include "commands.hpp"


// this header will eventually contain the highest-level objects
// that represent the overall state of the program
// it will also include the game state itself eventually as a member

namespace sys {
struct user_settings_s {
	float ui_scale = 1.0f;
	float master_volume = 0.5f;
	float music_volume = 1.0f;
	float effects_volume = 1.0f;
	float interface_volume = 1.0f;
	bool prefer_fullscreen = false;
	bool bind_tooltip_mouse = true;
	bool left_mouse_click_hold_and_release = false;
	float zoom_speed = 20.f;
	bool mute_on_focus_lost = true;
	char locale[16] = "en-US";
};

struct alignas(64) state {
	dcon::data_container world; // Holds data regarding the game world. Also contains user locales.

	// scenario data
	std::vector<char> key_data;
	std::vector<char> locale_text_data;
	ankerl::unordered_dense::set<dcon::text_key, text::vector_backed_ci_hash, text::vector_backed_ci_eq> untrans_key_to_text_sequence;
	ankerl::unordered_dense::map<dcon::text_key, uint32_t, text::vector_backed_ci_hash, text::vector_backed_ci_eq> locale_key_to_text_sequence;

#ifdef USE_LLVM
	std::unique_ptr<fif::environment> jit_environment;
#endif

	
	//
	// persistent user settings
	//

	user_settings_s user_settings;
	bool user_setting_changed = false;

	//
	// current program / ui state
	//
	uint32_t game_seed = 0;

	//current ui
	game_scene::scene_properties current_scene;
	component_type active_component_tool = std::monostate{};
	wire_colors current_wire_color = wire_colors::amber;
	int32_t x_offset = 0;
	int32_t y_offset = 0;
	int32_t zoom = 8;

	simple_fs::file_system common_fs;                                // file system for looking up graphics assets, etc
	std::unique_ptr<window::window_data_impl> win_ptr = nullptr;     // platform-dependent window information
	std::unique_ptr<sound::sound_impl> sound_ptr = nullptr;          // platform-dependent sound information
	ui::state ui_state;                                              // transient information for the state of the ui
	ogl::animation ui_animation;
	text::font_manager font_collection;
	asvg::file_bank svg_image_files;
	template_project::project ui_templates;

	// synchronization data (between main update logic and ui thread)
	std::atomic<bool> game_state_updated = false;                    // game state -> ui signal
	std::atomic<int32_t> actual_game_speed = 0;                      // ui -> game state message
	std::atomic<bool> quit_signaled = false;                         // ui -> game state signal
	rigtorp::SPSCQueue<command::command_data> incoming_commands;          // ui or network -> local gamestate
	std::atomic<bool> ui_pause = false;                              // force pause by an important message being open

	std::atomic<int64_t> tick_start_counter;
	std::atomic<int64_t> tick_end_counter;


	// internal game timer / update logic
	std::chrono::time_point<std::chrono::steady_clock> last_update = std::chrono::steady_clock::now();
	bool internally_paused = false; // should NOT be set from the ui context (but may be read)

	// common data for the window
	int32_t x_size = 0;
	int32_t y_size = 0;
	bool drag_selecting = false;
	int32_t mouse_x_position = 0;
	int32_t mouse_y_position = 0;
	bool is_dragging = false;
	int32_t x_drag_start = 0;
	int32_t y_drag_start = 0;

	// graphics data
	ogl::data open_gl;

#ifdef DIRECTX_11
	directx::data directx;
#endif

	std::mutex ui_lock; // lock for rendering the ui, when this is locked no rendering updates will occur
	std::condition_variable ui_lock_cv;
	bool yield_ui_lock = false;

	// the following functions will be invoked by the window subsystem

	void on_create(); // called once after the window is created and opengl is ready
	void on_rbutton_down(int32_t x, int32_t y, key_modifiers mod);
	void on_mbutton_down(int32_t x, int32_t y, key_modifiers mod);
	void on_lbutton_down(int32_t x, int32_t y, key_modifiers mod);
	void on_rbutton_up(int32_t x, int32_t y, key_modifiers mod);
	void on_mbutton_up(int32_t x, int32_t y, key_modifiers mod);
	void on_lbutton_up(int32_t x, int32_t y, key_modifiers mod);
	void on_mouse_move(int32_t x, int32_t y, key_modifiers mod);
	void on_mouse_drag(int32_t x, int32_t y, key_modifiers mod); // called when the left button is held down
	void on_drag_finished(int32_t x, int32_t y, key_modifiers mod); // called when the left button is released after one or more drag events
	void on_resize(int32_t x, int32_t y, window::window_state win_state);
	void on_key_down(virtual_key keycode, key_modifiers mod);
	void on_key_up(virtual_key keycode, key_modifiers mod);
	void on_text(char32_t c); // c is a win1250 codepage value

	bool filter_tso_mouse_events(int32_t x, int32_t y, uint32_t buttons);
	void pass_edit_command(ui::edit_command command, sys::key_modifiers mod);
	bool send_edit_mouse_move(int32_t x, int32_t y, bool extend_selection);
	text_mouse_test_result detailed_text_mouse_test(int32_t x, int32_t y);
	void render(); // called to render the frame may (and should) delay returning until the frame is rendered, including waiting for vsync

	void single_game_tick();
	// this function runs the internal logic of the game. It will return *only* after a quit notification is sent to it
	void game_loop();


	std::string_view to_string_view(dcon::text_key tag) const;
	std::string_view locale_string_view(uint32_t tag) const;
	bool key_is_localized(dcon::text_key tag) const;
	bool key_is_localized(std::string_view key) const;
	dcon::text_key lookup_key(std::string_view text) const;

	void reset_locale_pool();
	void load_locale_strings(std::string_view locale_name);

	dcon::text_key add_key_utf8(std::string const& text);
	dcon::text_key add_key_utf8(std::string_view text);
	uint32_t add_locale_data_utf8(std::string const& text);
	uint32_t add_locale_data_utf8(std::string_view text);

	state() : untrans_key_to_text_sequence(0, text::vector_backed_ci_hash(key_data), text::vector_backed_ci_eq(key_data)), locale_key_to_text_sequence(0, text::vector_backed_ci_hash(key_data), text::vector_backed_ci_eq(key_data)), incoming_commands(4096) {
		game_scene::switch_scene(*this, game_scene::scene_id::loading);
		key_data.push_back(0);
	}

	~state() = default;

	void save_user_settings() const;
	void load_user_settings();
	void update_ui_scale(float new_scale);

	int get_edit_x();
	int get_edit_y();
};

} // namespace sys
