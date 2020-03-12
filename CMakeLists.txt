##
## wolf, 2019
## CMakeLists.txt
##

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
cmake_policy(SET CMP0076 NEW)

project(wolf VERSION 0.1.0 LANGUAGES CXX)
add_executable(wolf)

target_compile_features(wolf
PRIVATE
	cxx_std_17
)

target_compile_options(wolf
PRIVATE
	-fPIC
	-W -Wall -Wextra
	-g3
)

target_include_directories(wolf
PUBLIC
	./sources
)

target_sources(wolf
PUBLIC
	sources/main.cpp
	sources/GameStates/InGame.cpp
	sources/GameStates/PauseMenu.cpp
)

target_link_libraries(wolf
PUBLIC
	SDL2
)