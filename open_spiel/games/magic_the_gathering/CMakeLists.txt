cmake_minimum_required(VERSION 3.12)
project(mtg_test)

set(CMAKE_CXX_STANDARD 17)

# Include OpenSpiel headers
include_directories(/mnt/c/users/Alex/Desktop/open_spiel)

# Add the OpenSpiel library
link_directories(/mnt/c/users/Alex/Desktop/open_spiel/build)

add_executable(mtg magic_the_gathering_test.cc)
target_link_libraries(mtg open_spiel)
