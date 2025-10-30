#ifndef TMULTISTACK_H
#define TMULTISTACK_H

#include <iostream>
#include <stdexcept>

template<typename T>
class TMultiStack {
private:
    T* data;                   
    size_t totalSize;          
    size_t stackCount;         
    size_t* tops;              
    size_t* capacities;        
    size_t* starts;            

    void Repack();  

    size_t GetStackEnd(size_t stackIndex) const;  
    void ShiftStacks(size_t fromStack);           

public:
    TMultiStack();
    TMultiStack(size_t totalCapacity, size_t numStacks);
    TMultiStack(const TMultiStack& other);
    TMultiStack(TMultiStack&& other) noexcept;
    ~TMultiStack();


    void Push(size_t stackIndex, const T& value);
    void Push(size_t stackIndex, T&& value);
    T Pop(size_t stackIndex);
    T& Top(size_t stackIndex) const;
    bool IsEmpty(size_t stackIndex) const;
    bool IsFull(size_t stackIndex) const;
    
    
    size_t GetTotalSize() const;
    size_t GetStackCount() const;
    size_t GetStackSize(size_t stackIndex) const;
    size_t GetStackCapacity(size_t stackIndex) const;
    size_t GetStackStart(size_t stackIndex) const;
    size_t GetStackTop(size_t stackIndex) const;

    
    TMultiStack& operator=(const TMultiStack& other);
    TMultiStack& operator=(TMultiStack&& other) noexcept;
    bool operator==(const TMultiStack& other) const;
    bool operator!=(const TMultiStack& other) const;
    TStack<T> operator[](size_t stackIndex) const;  // Возвращает копию стека

   
    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const TMultiStack<U>& multistack);
    
    template<typename U>
    friend std::istream& operator>>(std::istream& is, TMultiStack<U>& multistack);

    
    class Iterator {
    private:
        TMultiStack<T>* multistack;
        size_t currentStack;
        size_t currentIndex;
        
    public:
        Iterator(TMultiStack<T>* ms, size_t stack, size_t index);
        
        // Базовые операции итератора
        T& operator*();
        Iterator& operator++();
        Iterator operator++(int);
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
        
        // Дополнительные методы для навигации
        size_t GetCurrentStack() const;
        size_t GetCurrentIndex() const;
    };

    // Методы для работы с итератором
    Iterator Begin();                          // Начало всех стеков
    Iterator End();                            // Конец всех стеков
    Iterator StackBegin(size_t stackIndex);    // Начало конкретного стека
    Iterator StackEnd(size_t stackIndex);      // Конец конкретного стека

private:
    // Вспомогательные методы
    void ValidateStackIndex(size_t stackIndex) const;
    void InitializeArrays();
    void RedistributeMemory();
};

#endif // TMULTISTACK_H