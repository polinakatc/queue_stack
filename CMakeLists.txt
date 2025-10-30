cmake_minimum_required(VERSION 3.10)
project(QUEUE_STACK)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)

# Добавление поддиректорий
add_subdirectory(includes)
add_subdirectory(src)
add_subdirectory(tests)