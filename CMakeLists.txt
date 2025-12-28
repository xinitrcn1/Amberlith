# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#

cmake_minimum_required (VERSION 3.15)

project (BlankProject LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Turn on compile_commands.json export for wider editor/IDE support.
# This option is ignored on anything but Unix makefiles/Ninja.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

list(APPEND PROGRAM_CORE_SOURCES_LIST
	"src/main.cpp"
	"src/gui/alice_ui.cpp"
	"src/lunasvg/graphics.cpp"
	"src/lunasvg/lunasvg.cpp"
	"src/lunasvg/svgelement.cpp"
	"src/lunasvg/svggeometryelement.cpp"
	"src/lunasvg/svglayoutstate.cpp"
	"src/lunasvg/svgpaintelement.cpp"
	"src/lunasvg/svgparser.cpp"
	"src/lunasvg/svgproperty.cpp"
	"src/lunasvg/svgrenderstate.cpp"
	"src/lunasvg/svgtextelement.cpp"
	"src/lunasvg/plutovg-blend.c"
	"src/lunasvg/plutovg-canvas.c"
	"src/lunasvg/plutovg-font.c"
	"src/lunasvg/plutovg-ft-math.c"
	"src/lunasvg/plutovg-ft-raster.c"
	"src/lunasvg/plutovg-ft-stroker.c"
	"src/lunasvg/plutovg-matrix.c"
	"src/lunasvg/plutovg-paint.c"
	"src/lunasvg/plutovg-path.c"
	"src/lunasvg/plutovg-rasterize.c"
	"src/lunasvg/plutovg-surface.c"
	"src/gui/main_menu.cpp"
)
set_source_files_properties(src/lunasvg/plutovg-blend.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(src/lunasvg/plutovg-canvas.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(src/lunasvg/plutovg-font.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(src/lunasvg/plutovg-ft-math.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(src/lunasvg/plutovg-ft-raster.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(src/lunasvg/plutovg-ft-stroker.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(src/lunasvg/plutovg-matrix.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(src/lunasvg/plutovg-paint.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(src/lunasvg/plutovg-path.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(src/lunasvg/plutovg-rasterize.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(src/lunasvg/plutovg-surface.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

list(APPEND PROGRAM_INCREMENTAL_SOURCES_LIST
	${PROGRAM_CORE_SOURCES_LIST}
	"src/gamestate/user_interactions.cpp"
	"src/gamestate/system_state.cpp"
	"src/gui/ui_state.cpp"
	"src/gui/gui_element_base.cpp"
	"src/gui/gui_element_types.cpp"
	"src/common_types/blake2.cpp"
	"src/common_types/prng.cpp"
	"src/gamestate/commands.cpp"
	"src/graphics/opengl_wrapper.cpp"
	"src/graphics/texture.cpp"
	"src/gui/gui_graphics.cpp"
	"src/text/fonts.cpp"
	"src/text/text.cpp"
	"src/text/parsers.cpp"
	"src/gamestate/game_scene.cpp"
	"src/lunasvg/graphics.cpp"
	"src/lunasvg/lunasvg.cpp"
	"src/lunasvg/svgelement.cpp"
	"src/lunasvg/svggeometryelement.cpp"
	"src/lunasvg/svglayoutstate.cpp"
	"src/lunasvg/svgpaintelement.cpp"
	"src/lunasvg/svgparser.cpp"
	"src/lunasvg/svgproperty.cpp"
	"src/lunasvg/svgrenderstate.cpp"
	"src/lunasvg/svgtextelement.cpp"
	"src/graphics/asvg.cpp"
	"src/gamestate/uitemplate_serialization.cpp"
)

if(WIN32)
add_executable(Main WIN32
	${PROGRAM_CORE_SOURCES_LIST}
	"src/resource_file.rc"
	${ASSET_FILES})
add_executable(MainIncremental WIN32 EXCLUDE_FROM_ALL
	${PROGRAM_INCREMENTAL_SOURCES_LIST}
	"src/resource_file.rc"
	${ASSET_FILES})
else()
add_executable(Main WIN32
	${PROGRAM_CORE_SOURCES_LIST}
	${ASSET_FILES})
add_executable(MainIncremental WIN32 EXCLUDE_FROM_ALL
	${PROGRAM_INCREMENTAL_SOURCES_LIST}
	${ASSET_FILES})
endif()

target_compile_definitions(MainIncremental PRIVATE INCREMENTAL=1)
target_compile_definitions(Main PRIVATE GLM_ENABLE_EXPERIMENTAL)
target_compile_definitions(MainIncremental PRIVATE GLM_ENABLE_EXPERIMENTAL)
if(NOT WIN32)
	add_compile_definitions(PREFER_ONE_TBB)
endif()

# Include sub-projects.
add_subdirectory(dependencies EXCLUDE_FROM_ALL)

add_library(MainCommon INTERFACE)
target_compile_definitions(MainCommon INTERFACE GLM_ENABLE_EXPERIMENTAL)
target_compile_definitions(MainCommon INTERFACE "PROJECT_ROOT=\"${PROJECT_SOURCE_DIR}\"")
if(WIN32)
	# To build Non-AVX version : change all "/arch:AVX2" to "/d2archSSE42"
	# -march=nehalem for clang release
	set(CMAKE_CXX_FLAGS "")
	set(CMAKE_CXX_FLAGS_DEBUG "")
	set(CMAKE_CXX_FLAGS_RELEASE "")
	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		if(OPTIMIZE_MODE STREQUAL "On")
			message(STATUS "Optimizing with PGO data")
			target_compile_options(MainCommon INTERFACE
				/bigobj /wd4100 /wd4189 /wd4065 /wd4201 /wd4324 /GR- /W4 /permissive- /WX /arch:AVX2 /GF /w34388 /w34389 -Wno-unused-macros -Wno-zero-length-array -Wno-switch-default -Wno-reserved-identifier -Wno-unused-parameter -Wno-unused-variable -Wno-unused-private-field /Z7 -Wno-invalid-offsetof -Wno-deprecated-volatile -Wno-missing-prototypes -Wno-reserved-identifier -Wno-implicit-int-float-conversion -Wno-unsafe-buffer-usage -Wno-float-equal -Wno-shadow-field-in-constructor -Wno-gnu-anonymous-struct -Wno-nested-anon-types -Wno-double-promotion -Wno-shadow-uncaptured-local -Wno-nonportable-system-include-path -Wno-format-nonliteral -Wno-shorten-64-to-32 -Wno-ctad-maybe-unsupported -Wno-implicit-int-conversion -Wno-disabled-macro-expansion -Wno-comma -Wno-cast-function-type-strict -Wno-cast-function-type -Wno-invalid-offsetof -Wno-microsoft-cast -Wno-reserved-identifier -Wno-unused-but-set-variable -Wno-padded -Wno-unique-object-duplication -Wno-declaration-after-statement -Wno-undefined-func-template -Wno-bad-function-cast /DNDEBUG /wd4530 /MT /O2 /Oi /sdl- /GS- /Gy /Gw /Zc:inline -Wno-profile-instr-missing -Wno-profile-instr-out-of-date -Wno-nrvo -Wno-unused-function -Wno-cast-qual -Wno-format-security -fprofile-instr-use=code.profdata)
			target_link_options(MainCommon INTERFACE /OPT:REF /OPT:ICF /LTCG -fprofile-instr-use=code.profdata)
		elseif(PROFILE_MODE STREQUAL "On")
			message(STATUS "Compiling for PGO instrumentation")
			target_compile_options(MainCommon INTERFACE
				/bigobj /wd4100 /wd4189 /wd4065 /wd4201 /wd4324 /GR- /W4 /permissive- /WX /arch:AVX2 /GF /w34388 /w34389 -Wno-unused-macros -Wno-zero-length-array -Wno-switch-default -Wno-reserved-identifier -Wno-unused-parameter -Wno-unused-variable -Wno-unused-private-field /Z7 -Wno-invalid-offsetof -Wno-deprecated-volatile -Wno-missing-prototypes -Wno-reserved-identifier -Wno-implicit-int-float-conversion -Wno-unsafe-buffer-usage -Wno-float-equal -Wno-shadow-field-in-constructor -Wno-gnu-anonymous-struct -Wno-nested-anon-types -Wno-double-promotion -Wno-shadow-uncaptured-local -Wno-nonportable-system-include-path -Wno-format-nonliteral -Wno-shorten-64-to-32 -Wno-ctad-maybe-unsupported -Wno-implicit-int-conversion -Wno-disabled-macro-expansion -Wno-comma -Wno-cast-function-type-strict -Wno-cast-function-type -Wno-invalid-offsetof -Wno-microsoft-cast -Wno-reserved-identifier -Wno-unused-but-set-variable -Wno-padded -Wno-unique-object-duplication -Wno-nrvo -Wno-declaration-after-statement  -Wno-undefined-func-template -Wno-bad-function-cast  -Wno-unused-function -Wno-cast-qual -Wno-format-security /DNDEBUG /wd4530 /MT /O2 /Oi /sdl- /GS- /Gy /Gw /Zc:inline -fprofile-instr-generate)
			target_link_options(MainCommon INTERFACE /DEBUG:FULL /OPT:REF /OPT:ICF /LTCG -fprofile-instr-generate)
		else()
			target_compile_options(MainCommon INTERFACE
											/bigobj /wd4100 /wd4189 /wd4065 /wd4201 /wd4324 /GR- /W4 /permissive- /WX /GF /w34388 /w34389 -Wno-unused-macros -Wno-unused-parameter -Wno-unused-variable -Wno-unused-private-field /Z7 -Wno-invalid-offsetof -Wno-deprecated-volatile -Wno-missing-prototypes -Wno-reserved-identifier -Wno-implicit-int-float-conversion -Wno-unsafe-buffer-usage -Wno-float-equal -Wno-shadow-field-in-constructor -Wno-gnu-anonymous-struct -Wno-nested-anon-types -Wno-double-promotion -Wno-shadow-uncaptured-local -Wno-nonportable-system-include-path -Wno-format-nonliteral -Wno-shorten-64-to-32 -Wno-ctad-maybe-unsupported -Wno-implicit-int-conversion -Wno-disabled-macro-expansion -Wno-comma -Wno-cast-function-type-strict -Wno-cast-function-type -Wno-invalid-offsetof -Wno-switch-default -Wno-zero-length-array -Wno-microsoft-cast -Wno-reserved-identifier -Wno-unused-but-set-variable -Wno-padded -Wno-unique-object-duplication -Wno-nrvo -Wno-declaration-after-statement  -Wno-undefined-func-template -Wno-cast-qual -Wno-bad-function-cast -Wno-unused-function -Wno-format-security -Wno-nontrivial-memcall /arch:AVX2
				$<$<CONFIG:Debug>:			/RTC1 /EHsc /MTd /Od>
				$<$<NOT:$<CONFIG:Debug>>: 	/DNDEBUG /wd4530 /MT /O2 /Oi /sdl- /GS- /Gy /Gw /Zc:inline>)
			target_link_options(MainCommon INTERFACE
				$<$<CONFIG:Debug>: 			/DEBUG:FULL >
				$<$<NOT:$<CONFIG:Debug>>: 	/OPT:REF /OPT:ICF /LTCG>)
		endif()
	else()
		set_target_properties(MainCommon PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR})

		# /arch:AVX512
		# /d2archSSE42
		if(OPTIMIZE_MODE STREQUAL "On")
			message(STATUS "Optimizing with PGO data")
			target_compile_options(MainCommon INTERFACE
				/bigobj /wd4100 /wd4189 /wd4065 /wd4201 /wd4324 /wd4723 /GR- /W4 /permissive- /Zc:preprocessor /WX /d2archSSE42 /GF /w34388 /w34389 /DNDEBUG /wd4530 /MT /O2 /Oi /GL /sdl- /GS- /Gy /Gw /Zc:preprocessor /Zc:inline)
			target_link_options(MainCommon INTERFACE /OPT:REF /OPT:ICF /LTCG /USEPROFILE)
		elseif(PROFILE_MODE STREQUAL "On")
			message(STATUS "Compiling for PGO instrumentation")
			target_compile_options(MainCommon INTERFACE
				/bigobj /wd4100 /wd4189 /wd4065 /wd4201 /wd4324 /wd4723 /GR- /W4 /permissive- /Zc:preprocessor /WX /d2archSSE42 /GF /w34388 /w34389 /Z7 /DNDEBUG /wd4530 /MT /O2 /Oi /GL /sdl- /GS- /Gy /Gw /Zc:preprocessor /Zc:inline)
			target_link_options(MainCommon INTERFACE /DEBUG:FULL /OPT:REF /OPT:ICF /LTCG /GENPROFILE)
		else()
			message(STATUS "Normal, not PGO, build")
			target_compile_options(MainCommon INTERFACE
				/bigobj /wd4100 /wd4189 /wd4065 /wd4201 /wd4324 /GR- /W4 /permissive- /Zc:preprocessor /WX /d2archSSE42 /GF /w34388 /w34389 /Z7
				  $<$<CONFIG:Debug>: /EHsc /MTd /RTC1 /Od>
# for faster debug builds, replace /RTC1 /Od with /O1 -- should be able to use /Ox, but for some reason that crashes ZSTD
				  $<$<NOT:$<CONFIG:Debug>>: 	/DNDEBUG /wd4530 /MT /O2 /Oi /GL /sdl- /GS- /Gy /Gw /Zc:preprocessor /Zc:inline>)
			target_link_options(MainCommon INTERFACE
				$<$<CONFIG:Debug>: 			/DEBUG:FULL >
				$<$<NOT:$<CONFIG:Debug>>: 	/DEBUG:FULL /OPT:REF /OPT:ICF /LTCG>)
		endif()
	endif()
else() # GCC or CLANG
	target_link_options(Main INTERFACE -rdynamic -Wl,-E)
	target_link_options(MainIncremental INTERFACE -rdynamic -Wl,-E)
	target_link_options(MainCommon INTERFACE -rdynamic -Wl,-E)
	# See for FMA support: https://stackoverflow.com/questions/21001388/fma3-in-gcc-how-to-enable
	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		target_compile_options(MainCommon INTERFACE
			-Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-unused-variable -Wno-switch -Wdangling-else -Wno-unused-private-field -Wno-invalid-offsetof -Wno-unused-but-set-variable -Wno-microsoft-cast -Wno-declaration-after-statement #-Werror
			$<$<CONFIG:Debug>:			-mfpmath=sse -mavx2 -mfma -g -pipe>
			$<$<NOT:$<CONFIG:Debug>>: 	-mfpmath=sse -mavx2 -mfma -O3 -pipe -fno-rtti -DNDEBUG>)
	else()
		target_compile_options(MainCommon INTERFACE
										# -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-switch -Wno-unused-private-field -Wno-unused-but-set-variable -Wno-parentheses
			$<$<CONFIG:Debug>:			-mfpmath=sse -mavx2 -mfma -g -pipe>
			$<$<NOT:$<CONFIG:Debug>>: 	-mfpmath=sse -mavx2 -mfma -O3 -pipe -fno-rtti -DNDEBUG>)
	endif()
	target_link_libraries(MainCommon INTERFACE dl)
endif()
target_link_libraries(MainCommon INTERFACE dependency_DataContainer)
target_link_libraries(MainCommon INTERFACE dependency_random123)
target_link_libraries(MainCommon INTERFACE dependency_zstd)
target_link_libraries(MainCommon INTERFACE dependency_stb)
target_link_libraries(MainCommon INTERFACE libglew_static)
target_link_libraries(MainCommon INTERFACE freetype)
target_link_libraries(MainCommon INTERFACE harfbuzz)
target_link_libraries(MainCommon INTERFACE glm)
if (WIN32)

else()
	target_link_libraries(MainCommon INTERFACE glfw)
	target_link_libraries(MainCommon INTERFACE dependency_tbb)
	target_link_libraries(MainCommon INTERFACE dependency_miniaudio)
	target_link_libraries(MainCommon INTERFACE dependency_icu)
endif()
target_include_directories(MainCommon INTERFACE
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/src/ai
	${PROJECT_SOURCE_DIR}/src/common_types
	${PROJECT_SOURCE_DIR}/src/filesystem
	${PROJECT_SOURCE_DIR}/src/gamestate
	${PROJECT_SOURCE_DIR}/src/gui
	${PROJECT_SOURCE_DIR}/src/graphics
	${PROJECT_SOURCE_DIR}/src/parsing
	${PROJECT_SOURCE_DIR}/src/window
	${PROJECT_SOURCE_DIR}/src/text
	${PROJECT_SOURCE_DIR}/src/sound
	${PROJECT_SOURCE_DIR}/src/map
	${PROJECT_SOURCE_DIR}/src/lunasvg
	"${glew_SOURCE_DIR}/include/GL"
	"ankerl")

target_link_libraries(Main PRIVATE MainCommon)
target_link_libraries(MainIncremental PRIVATE MainCommon)

# System headers
target_precompile_headers(Main
	PRIVATE <stddef.h>
	PRIVATE <stdint.h>
	PRIVATE <assert.h>
	PRIVATE <stdarg.h>
	PRIVATE <stdlib.h>
	PRIVATE <math.h>
	PRIVATE <signal.h>
	PRIVATE <ctype.h>
)
target_precompile_headers(MainIncremental
	PRIVATE <stddef.h>
	PRIVATE <stdint.h>
	PRIVATE <assert.h>
	PRIVATE <stdarg.h>
	PRIVATE <stdlib.h>
	PRIVATE <math.h>
	PRIVATE <signal.h>
	PRIVATE <ctype.h>
)
# Standard C++ library headers
target_precompile_headers(Main
	PRIVATE <vector>
	PRIVATE <array>
	PRIVATE <optional>
	PRIVATE <memory>
	PRIVATE <variant>
	PRIVATE <string>
	PRIVATE <string_view>
	PRIVATE <charconv>
	PRIVATE <algorithm>
	PRIVATE <iterator>
	PRIVATE <functional>
	PRIVATE <atomic>
	PRIVATE <chrono>
	PRIVATE <tuple>
	PRIVATE <type_traits>
	PRIVATE <new>
	PRIVATE <limits>
	PRIVATE <iterator>
	PRIVATE <utility>
	PRIVATE <any>
)
target_precompile_headers(MainIncremental
	PRIVATE <vector>
	PRIVATE <array>
	PRIVATE <optional>
	PRIVATE <memory>
	PRIVATE <variant>
	PRIVATE <string>
	PRIVATE <string_view>
	PRIVATE <charconv>
	PRIVATE <algorithm>
	PRIVATE <iterator>
	PRIVATE <functional>
	PRIVATE <atomic>
	PRIVATE <chrono>
	PRIVATE <tuple>
	PRIVATE <type_traits>
	PRIVATE <new>
	PRIVATE <limits>
	PRIVATE <iterator>
	PRIVATE <utility>
)

# Aliases for C library
target_precompile_headers(Main
	PRIVATE <cstdlib>
	PRIVATE <cstddef>
	PRIVATE <cstdint>
	PRIVATE <cassert>
	PRIVATE <cstdarg>
	PRIVATE <cmath>
)
target_precompile_headers(MainIncremental
	PRIVATE <cstdlib>
	PRIVATE <cstddef>
	PRIVATE <cstdint>
	PRIVATE <cassert>
	PRIVATE <cstdarg>
	PRIVATE <cmath>
)

# GENERATE CONTAINER
set(CONTAINER_PATH ${PROJECT_SOURCE_DIR}/src/gamestate/dcon_generated)

# The command to build the generated file
add_custom_command(
	OUTPUT ${CONTAINER_PATH}.hpp ${CONTAINER_PATH}_ids.hpp
	COMMAND DCONGENERATOR ${CONTAINER_PATH}.txt
	DEPENDS ${CONTAINER_PATH}.txt
	VERBATIM)

add_custom_target(GENERATE_CONTAINER DEPENDS ${CONTAINER_PATH}.hpp)
add_dependencies(Main GENERATE_CONTAINER)
add_dependencies(MainIncremental GENERATE_CONTAINER)

target_precompile_headers(Main
	PRIVATE [["simple_fs.hpp"]]
	PRIVATE [["constants.hpp"]]
	PRIVATE [["unordered_dense.h"]]
	PRIVATE [["dcon_generated.hpp"]]
	PRIVATE [["container_types.hpp"]]
	PRIVATE [["stb_image.h"]]
)
target_precompile_headers(MainIncremental
	PRIVATE [["simple_fs.hpp"]]
	PRIVATE [["constants_dcon.hpp"]]
	PRIVATE [["unordered_dense.h"]]
	PRIVATE [["dcon_generated.hpp"]]
	PRIVATE [["dcon_generated_ids.hpp"]]
	PRIVATE [["container_types_dcon.hpp"]]
	PRIVATE [["container_types.hpp"]]
#	PRIVATE [["gui_templates.hpp"]]
	PRIVATE [["stb_image.h"]]
	PRIVATE [["glm/glm.hpp"]]
)

if(WIN32)
else()
	target_precompile_headers(Main
		PRIVATE [["miniaudio.h"]])
	target_precompile_headers(MainIncremental
		PRIVATE [["miniaudio.h"]])
endif()




