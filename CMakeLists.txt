cmake_minimum_required(VERSION 3.30)
project(TinyGame)

set(CMAKE_CXX_STANDARD 23)

set(GAME_TARGET TinyGame)

add_library(TinyGame STATIC
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
	Source/Events/IEventHandler.cpp
	Source/Events/IEventHandler.h
)

target_include_directories(TinyGame PUBLIC Source)

add_executable(TinyGameApp
	Source/Main/main.cpp
)
target_link_libraries(TinyGameApp PRIVATE TinyGame)

add_subdirectory(3rdParty)