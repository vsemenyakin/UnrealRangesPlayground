cmake_minimum_required(VERSION 3.2)
project(UnrealRangesPlayerground)

set(CMAKE_CXX_STANDARD 14)

add_executable(UnrealRangesPlayerground)

include(./UnrealRanges/UnrealRanges.cmake)

target_sources(UnrealRangesPlayerground
	PRIVATE
		main.cpp
		TinyAsciiGameEngine/TinyAsciiGameEngine.h
		TinyAsciiGameEngine/TinyAsciiGameEngine.cpp
		${UnrealRanges}
)
