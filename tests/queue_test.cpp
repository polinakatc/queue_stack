#include "gtest.h"
#include "TStack.h"

// Тест 1: Конструкторы
TEST(TStackTest, Constructors) {
    TStack<int> stack1;                    // По умолчанию
    EXPECT_TRUE(stack1.IsEmpty());
    
    TStack<int> stack2(5);                 // С capacity
    EXPECT_EQ(stack2.GetCapacity(), 5);
    
    stack2.Push(100);
    TStack<int> stack3(stack2);            // Копирования
    EXPECT_EQ(stack3.Size(), 1);
    EXPECT_EQ(stack3.Pop(), 100);
    
    TStack<int> stack4(std::move(stack2)); // Перемещения
    EXPECT_EQ(stack4.Size(), 1);
    EXPECT_TRUE(stack2.IsEmpty());
}

// Тест 2: Основные методы Push/Pop
TEST(TStackTest, PushPop) {
    TStack<int> stack(3);
    
    stack.Push(10);
    stack.Push(20);
    stack.Push(30);
    
    EXPECT_EQ(stack.Size(), 3);
    EXPECT_EQ(stack.Pop(), 30);  // LIFO - последний вошел, первый вышел
    EXPECT_EQ(stack.Pop(), 20);
    EXPECT_EQ(stack.Pop(), 10);
    EXPECT_TRUE(stack.IsEmpty());
}

// Тест 3: Проверка состояний
TEST(TStackTest, StateChecks) {
    TStack<int> stack(2);
    
    EXPECT_TRUE(stack.IsEmpty());
    EXPECT_FALSE(stack.IsFull());
    
    stack.Push(1);
    EXPECT_FALSE(stack.IsEmpty());
    EXPECT_FALSE(stack.IsFull());
    
    stack.Push(2);
    EXPECT_FALSE(stack.IsEmpty());
    EXPECT_TRUE(stack.IsFull());
}

// Тест 4: Автоматическое увеличение
TEST(TStackTest, AutoResize) {
    TStack<int> stack(2);
    
    stack.Push(1);
    stack.Push(2);
    stack.Push(3);  // Должно увеличить capacity
    
    EXPECT_EQ(stack.Size(), 3);
    EXPECT_GE(stack.GetCapacity(), 3);
    EXPECT_EQ(stack.Pop(), 3);
}

// Тест 5: Операторы присваивания
TEST(TStackTest, AssignmentOperators) {
    TStack<int> stack1(3);
    stack1.Push(5);
    stack1.Push(6);
    
    TStack<int> stack2(1);
    stack2 = stack1;  // Копирующее присваивание
    
    EXPECT_EQ(stack2.Size(), 2);
    EXPECT_EQ(stack2.Pop(), 6);
    EXPECT_EQ(stack2.Pop(), 5);
    
    TStack<int> stack3 = std::move(stack1);  // Перемещающее присваивание
    EXPECT_EQ(stack3.Size(), 2);
    EXPECT_TRUE(stack1.IsEmpty());
}

// Тест 6: Операторы сравнения
TEST(TStackTest, ComparisonOperators) {
    TStack<int> stack1(3);
    stack1.Push(1);
    stack1.Push(2);
    
    TStack<int> stack2(3);
    stack2.Push(1);
    stack2.Push(2);
    
    TStack<int> stack3(3);
    stack3.Push(3);
    stack3.Push(4);
    
    EXPECT_TRUE(stack1 == stack2);
    EXPECT_FALSE(stack1 == stack3);
    EXPECT_TRUE(stack1 != stack3);
}

// Тест 7: Исключения
TEST(TStackTest, Exceptions) {
    TStack<int> stack(2);
    
    EXPECT_THROW(stack.Pop(), std::invalid_argument);  // Pop from empty
    
    EXPECT_THROW(TStack<int>(0), std::invalid_argument);  // Invalid capacity
}

// Тест 8: Потоковый вывод
TEST(TStackTest, StreamOutput) {
    TStack<int> stack(3);
    stack.Push(1);
    stack.Push(2);
    stack.Push(3);
    
    std::ostringstream oss;
    oss << stack;
    
    EXPECT_EQ(oss.str(), "[1, 2, 3]");
}