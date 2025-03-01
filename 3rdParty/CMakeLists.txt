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