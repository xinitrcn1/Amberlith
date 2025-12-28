#ifndef UNICODE
#define UNICODE
#endif
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "system_state.hpp"
#include "game_scene.hpp"

#include <Windows.h>
#include <shellapi.h>
#include "Objbase.h"
#include "window.hpp"


#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "icu.lib")
#pragma comment(lib, "Imm32.lib")

static sys::state game_state; // too big for the stack


int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPWSTR /*commandline*/, int /*nCmdShow*/
) {

#ifdef _DEBUG
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
#endif

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	if(SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))) {
		// do everything here: create a window, read messages

		add_root(game_state.common_fs, NATIVE("."));

		int num_params = 0;
		auto parsed_cmd = CommandLineToArgvW(GetCommandLineW(), &num_params);

		if(num_params < 2) {
#ifdef NDEBUG
			// do you need params? if so, put an error here
#else
			// no params, debug only
#endif
		} else {
			for(int i = 1; i < num_params; ++i) {
				//if(native_string(parsed_cmd[i]) == NATIVE("-host")) {
				//} etc
			}
		}

		LocalFree(parsed_cmd);

		// scenario loading functions (would have to run these even when scenario is pre-built)
		game_state.load_user_settings();
		
		
		std::thread update_thread([&]() { game_state.game_loop(); });

		// entire game runs during this line
		window::create_window(game_state, window::creation_parameters{ 1024, 780, window::window_state::maximized, game_state.user_settings.prefer_fullscreen });
		game_state.quit_signaled.store(true, std::memory_order_release);

		update_thread.join();
		

		CoUninitialize();
	}
	return 0;
}
