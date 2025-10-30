# Создаем библиотеку Google Test
add_library(gtest_lib gtest-all.cc)

# Подключаем заголовки
target_include_directories(gtest_lib PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Настройка стандарта C++
set_target_properties(gtest_lib PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)