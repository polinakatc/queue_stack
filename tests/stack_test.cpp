#include "gtest.h"
#include "TQueue.h"

// Тест 1: Базовая работа Put и Get
TEST(TQueueTest, BasicPutGet) {
    TQueue<int> queue(3);
    
    queue.Put(10);
    queue.Put(20);
    queue.Put(30);
    
    EXPECT_EQ(queue.Size(), 3);
    EXPECT_EQ(queue.Get(), 10);
    EXPECT_EQ(queue.Get(), 20);
    EXPECT_EQ(queue.Get(), 30);
    EXPECT_TRUE(queue.IsEmpty());
}

// Тест 2: Конструктор копирования
TEST(TQueueTest, CopyConstructor) {
    TQueue<int> original(2);
    original.Put(100);
    original.Put(200);
    
    TQueue<int> copy(original);
    
    EXPECT_EQ(original.Size(), 2);
    EXPECT_EQ(copy.Size(), 2);
    EXPECT_EQ(copy.Get(), 100);
    EXPECT_EQ(copy.Get(), 200);
}

// Тест 3: Автоматическое увеличение размера
TEST(TQueueTest, AutoResize) {
    TQueue<int> queue(2);
    
    queue.Put(1);
    queue.Put(2);
    queue.Put(3);  // Должно вызвать resize
    
    EXPECT_EQ(queue.Size(), 3);
    EXPECT_GE(queue.GetCapacity(), 3);
}

// Тест 4: Оператор присваивания
TEST(TQueueTest, AssignmentOperator) {
    TQueue<int> q1(3);
    q1.Put(5);
    q1.Put(6);
    
    TQueue<int> q2(1);
    q2 = q1;
    
    EXPECT_EQ(q2.Size(), 2);
    EXPECT_EQ(q2.Get(), 5);
    EXPECT_EQ(q2.Get(), 6);
}

// Тест 5: Исключения
TEST(TQueueTest, Exceptions) {
    TQueue<int> queue(2);
    
    EXPECT_ANY_THROW(queue.Get());  // Пустая очередь
    EXPECT_ANY_THROW(TQueue<int>(0));

}

