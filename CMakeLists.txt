cmake_minimum_required(VERSION 3.0)

project(PROG2-projet-deepdiep)
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Wreturn-type -Werror=return-type -Wunreachable-code -Werror")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("On need, see https://www.sfml-dev.org/tutorials/2.5/ to install SFML on your system.")
find_package(SFML 2 REQUIRED system window graphics)

add_executable(deepdiep src/Entity.cpp src/Entity.hpp src/Tank.cpp src/Tank.hpp src/Bullet.cpp src/Bullet.hpp src/Asteroid.cpp src/Asteroid.hpp
                        src/Game.cpp src/Game.hpp src/GameCmd.cpp src/GameCmd.hpp
                        src/View.cpp src/View.hpp
                        src/World.cpp src/World.hpp
                        )

target_link_libraries(deepdiep sfml-graphics sfml-window sfml-system)

file(COPY assets/Monospace.ttf DESTINATION ${CMAKE_BINARY_DIR})
