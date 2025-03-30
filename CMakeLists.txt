cmake_minimum_required(VERSION 3.30)
project(TinyGameEngine)

set(CMAKE_CXX_STANDARD 23)

set(GAME_TARGET TinyGameEngineLib)

add_library(${GAME_TARGET} STATIC
	Source/Graphics/GameApp.cpp
	Source/Graphics/GameApp.h
	Source/Utils/NonCopyable.h
	Source/Utils/ArgsParser.cpp
	Source/Utils/ArgsParser.h
	Source/Containers/TSparseArray.cpp
	Source/Containers/TSparseArray.h
	Source/Containers/BitArray.cpp
	Source/Containers/BitArray.h
	Source/Containers/ChunkStorage.cpp
	Source/Containers/ChunkStorage.h
	Source/Containers/ChunkStorage.test.cpp
	Source/Events/ISDLEventHandler.cpp
	Source/Events/ISDLEventHandler.h
	Source/Events/InputSystem.cpp
	Source/Events/InputSystem.h
	Source/Graphics/ImGuiLayers.cpp
	Source/Graphics/ImGuiLayers.h
)

target_include_directories(${GAME_TARGET} PUBLIC Source)

add_executable(TinyGameApp
	Source/Main/main.cpp
)
target_link_libraries(TinyGameApp PRIVATE ${GAME_TARGET})

add_subdirectory(3rdParty)