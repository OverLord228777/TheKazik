cmake_minimum_required(VERSION 4.0)
project(Project1)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML REQUIRED)

# Свяжите SFML с вашим исполняемым файлом
target_link_libraries(main.cpp ${SFML_LIBRARIES})

add_executable(Project1 main.cpp)