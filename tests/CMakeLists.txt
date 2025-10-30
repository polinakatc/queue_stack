# Компилируем gtest
add_library(gtest STATIC ../gtest/gtest-all.cc)
target_include_directories(gtest PUBLIC ../gtest)

# Создаем тесты
add_executable(test 
    test_main.cpp
    queue_test.cpp 
    stack_test.cpp
)

# Линкуем с gtest и нашей библиотекой
target_link_libraries(test 
    queue_stack_lib 
    gtest
    pthread  # gtest требует pthread
)

target_include_directories(test PRIVATE 
    ../includes
    ../gtest
)

enable_testing()
add_test(NAME tests COMMAND test)