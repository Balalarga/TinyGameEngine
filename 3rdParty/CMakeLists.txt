cmake_minimum_required(VERSION 3.30)

# need to setup prefix path for find_package
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_LIST_DIR}")


# ------------------------------------ SDL3 ------------------------------------
find_package(SDL3 REQUIRED)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/SDL3-3.2.2/lib/x64/SDL3.dll DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/SDL3-3.2.2/lib/x64/SDL3.pdb DESTINATION ${CMAKE_BINARY_DIR}/)
target_link_libraries(${GAME_TARGET} PUBLIC SDL3::SDL3-shared)


# ------------------------------------ SpdLog ----------------------------------
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/spdlog)
target_link_libraries(${GAME_TARGET} PUBLIC spdlog::spdlog)


# ------------------------------------ GTest -----------------------------------
include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
target_link_libraries(${GAME_TARGET} PUBLIC GTest::gtest_main)


# ------------------------------------ ImGui -----------------------------------
set(IMGUI_SOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/imgui/)
set(IMGUI_SOURCES
	${IMGUI_SOURCES_DIR}/imgui.h
	${IMGUI_SOURCES_DIR}/imconfig.h
	${IMGUI_SOURCES_DIR}/imgui_internal.h
	${IMGUI_SOURCES_DIR}/imgui.cpp
	${IMGUI_SOURCES_DIR}/imgui_draw.cpp
	${IMGUI_SOURCES_DIR}/imgui_demo.cpp
	${IMGUI_SOURCES_DIR}/imgui_tables.cpp
	${IMGUI_SOURCES_DIR}/imgui_widgets.cpp
	# Backend
	${IMGUI_SOURCES_DIR}/backends/imgui_impl_sdl3.h
	${IMGUI_SOURCES_DIR}/backends/imgui_impl_sdl3.cpp
	${IMGUI_SOURCES_DIR}/backends/imgui_impl_opengl3.h
	${IMGUI_SOURCES_DIR}/backends/imgui_impl_opengl3_loader.h
	${IMGUI_SOURCES_DIR}/backends/imgui_impl_opengl3.cpp
)

target_sources(${GAME_TARGET} PUBLIC ${IMGUI_SOURCES})
target_include_directories(${GAME_TARGET} PUBLIC ${IMGUI_SOURCES_DIR})


# ------------------------------------ Glm -------------------------------------
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/glm-1.0.1/glm)
target_include_directories(${GAME_TARGET} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/glm-1.0.1)
target_link_libraries(${GAME_TARGET} PUBLIC glm)

# ------------------------------------ Json -------------------------------------
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/nlohmann_json-3.11.3)
target_link_libraries(${GAME_TARGET} PUBLIC nlohmann_json)