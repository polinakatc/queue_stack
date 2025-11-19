#include <iostream>
#include <stdexcept>
#include <string>

template<class T>
class TMultiStack
{
private:
    size_t capacity;
    size_t count;
    T* data;
    size_t* stacksBegin;
    size_t* starts;

    void Repack();

public:
    TMultiStack();
    TMultiStack(size_t count_, size_t size);
    TMultiStack(const TMultiStack& other);
    TMultiStack(TMultiStack&& other) noexcept;
    ~TMultiStack();

    void Push(size_t stackIndex, const T& value);
    T Pop(size_t stackIndex);
    bool IsEmpty(size_t stackIndex) const;
    bool IsFull(size_t stackIndex) const;
    
    size_t GetCapacity() const;
    size_t GetStackCount() const;
    size_t GetStackSize(size_t stackIndex) const;

    TMultiStack& operator=(const TMultiStack& other);
    TMultiStack& operator=(TMultiStack&& other) noexcept;
    bool operator==(const TMultiStack& other) const;
    bool operator!=(const TMultiStack& other) const;
    T operator()(size_t stackIndex, size_t pos) const;

    template<class U>
    friend std::ostream& operator<<(std::ostream& os, const TMultiStack<U>& stack);
    
    template<class U>
    friend std::istream& operator>>(std::istream& is, TMultiStack<U>& stack);
};

template<class T>
inline void TMultiStack<T>::Repack()
{
    if (capacity == 0 || count == 0) return;
    
    //тут узнаем размеры стекоы
    size_t totalElements = 0;
    size_t* sizes = new size_t[count];
    for (size_t i = 0; i < count; i++) {
        sizes[i] = GetStackSize(i);
        totalElements += sizes[i];
    }
    
    if (totalElements >= capacity) {
        //здесь ищем самый смачный стек, уменьшаем на 1
        size_t maxStack = 0;
        for (size_t i = 1; i < count; i++) {
            if (sizes[i] > sizes[maxStack]) {
                maxStack = i;
            }
        }
        if (sizes[maxStack] > 0) {
            sizes[maxStack]--;
            totalElements--;
        }
    }
    
    //раскидываем оставшееся место
    size_t freeSpace = capacity - totalElements;
    size_t extraPerStack = freeSpace / count;
    size_t remainder = freeSpace % count;
    
    //новые начала
    size_t* newBegins = new size_t[count];
    size_t* newEnds = new size_t[count];
    
    newBegins[0] = 0;
    newEnds[0] = newBegins[0] + sizes[0] + extraPerStack + (remainder > 0 ? 1 : 0);
    if (remainder > 0) remainder--;
    
    for (size_t i = 1; i < count; i++) {
        newBegins[i] = newEnds[i - 1];
        newEnds[i] = newBegins[i] + sizes[i] + extraPerStack + (remainder > 0 ? 1 : 0);
        if (remainder > 0) remainder--;
    }
    
    //копируем
    T* newData = new T[capacity];
    for (size_t i = 0; i < count; i++) {
        for (size_t j = 0; j < sizes[i]; j++) {
            newData[newBegins[i] + j] = data[stacksBegin[i] + j];
        }
    }
    
    //upate указателей
    delete[] data;
    data = newData;
    
    for (size_t i = 0; i < count; i++) {
        stacksBegin[i] = newBegins[i];
        starts[i] = newBegins[i] + sizes[i];
    }
    
    delete[] sizes;
    delete[] newBegins;
    delete[] newEnds;
}
template<class T>
inline TMultiStack<T>::TMultiStack() 
    : capacity(0), count(0), data(nullptr), stacksBegin(nullptr), starts(nullptr)
{
}

template<class T>
inline TMultiStack<T>::TMultiStack(size_t count_, size_t size)
    : capacity(count_ * size), count(count_)
{
    if (count == 0 || capacity == 0)
    {
        capacity = 0;
        count = 0;
        data = nullptr;
        stacksBegin = nullptr;
        starts = nullptr;
        return;
    }
    
    data = new T[capacity];
    stacksBegin = new size_t[count];
    starts = new size_t[count];
    
    for (size_t i = 0; i < count; ++i)
    {
        stacksBegin[i] = i * size;
        starts[i] = i * size;
    }
}

template<class T>
inline TMultiStack<T>::TMultiStack(const TMultiStack& other)
    : capacity(other.capacity), count(other.count)
{
    data = new T[capacity];
    stacksBegin = new size_t[count];
    starts = new size_t[count];
    
    for (size_t i = 0; i < capacity; ++i)
        data[i] = other.data[i];
    
    for (size_t i = 0; i < count; ++i)
    {
        stacksBegin[i] = other.stacksBegin[i];
        starts[i] = other.starts[i];
    }
}

template<class T>
inline TMultiStack<T>::TMultiStack(TMultiStack&& other) noexcept
    : capacity(other.capacity), count(other.count),
      data(other.data), stacksBegin(other.stacksBegin), starts(other.starts)
{
    other.capacity = 0;
    other.count = 0;
    other.data = nullptr;
    other.stacksBegin = nullptr;
    other.starts = nullptr;
}
template<class T>
inline TMultiStack<T>::~TMultiStack()
{
    if (capacity){
        delete[] data;
        delete[] stacksBegin;
        delete[] starts;
    }
    capacity = 0;
    count = 0;
} 
template<class T>
inline size_t TMultiStack<T>::GetCapacity() const
{
    return capacity;
}

template<class T>
inline size_t TMultiStack<T>::GetStackCount() const
{
    return count;
}

template<class T>
inline size_t TMultiStack<T>::GetStackSize(size_t stackIndex) const
{
    if (stackIndex >= count)
        throw std::out_of_range("jopa5");
    return starts[stackIndex] - stacksBegin[stackIndex];
}
template<class T>
inline T TMultiStack<T>::Pop(size_t stackIndex)
{
    if (stackIndex >= count)
        throw std::out_of_range("jopa6");
    
    if (IsEmpty(stackIndex))
        throw std::underflow_error("jopa7");
    
    starts[stackIndex]--;
    return data[starts[stackIndex]];
}

template<class T>
inline void TMultiStack<T>::Push(size_t stackIndex, const T& value)
{
    if (stackIndex >= count)
        throw std::out_of_range("jopa10");
    
    if (IsFull(stackIndex)) {
        Repack(); 
    }
    
    if (IsFull(stackIndex)) {
        throw std::overflow_error("jopa11");
    }
    
    data[starts[stackIndex]] = value;
    starts[stackIndex]++;
}

template<class T>
inline bool TMultiStack<T>::IsEmpty(size_t stackIndex) const
{
    if (stackIndex >= count)
        throw std::out_of_range("jopa8");
    return starts[stackIndex] == stacksBegin[stackIndex];
}

template<class T>
inline bool TMultiStack<T>::IsFull(size_t stackIndex) const
{
    
    if (stackIndex >= count)
        throw std::out_of_range("jopa9");
    
    if (stackIndex == count - 1) {
        return starts[stackIndex] >= capacity;
    }
    else {
        return starts[stackIndex] >= stacksBegin[stackIndex + 1];
    }
}

template<class T>
inline TMultiStack<T>& TMultiStack<T>::operator=(const TMultiStack& other)
{
    if (this != &other)
    {
        delete[] data;
        delete[] stacksBegin;
        delete[] starts;
        
        capacity = other.capacity;
        count = other.count;
        
        data = new T[capacity];
        stacksBegin = new size_t[count];
        starts = new size_t[count];
        
        for (size_t i = 0; i < capacity; ++i)
            data[i] = other.data[i];
        
        for (size_t i = 0; i < count; ++i)
        {
            stacksBegin[i] = other.stacksBegin[i];
            starts[i] = other.starts[i];
        }
    }
    return *this;
}
template<class T>
inline TMultiStack<T>& TMultiStack<T>::operator=(TMultiStack&& other) noexcept
{
    if (this != &other)
    {
        delete[] data;
        delete[] stacksBegin;
        delete[] starts;
        
        capacity = other.capacity;
        count = other.count;
        data = other.data;
        stacksBegin = other.stacksBegin;
        starts = other.starts;
        
        other.capacity = 0;
        other.count = 0;
        other.data = nullptr;
        other.stacksBegin = nullptr;
        other.starts = nullptr;
    }
    return *this;
}
template<class T>
inline bool TMultiStack<T>::operator==(const TMultiStack& other) const
{
    if (count != other.count || capacity != other.capacity)
        return false;
    
    for (size_t i = 0; i < count; ++i)
    {
        if (starts[i] != other.starts[i] || stacksBegin[i] != other.stacksBegin[i])
            return false;
        
        size_t stackSize = GetStackSize(i);
        for (size_t j = 0; j < stackSize; ++j)
        {
            if (data[stacksBegin[i] + j] != other.data[stacksBegin[i] + j])
                return false;
        }
    }
    return true;
}
template<class T>
inline bool TMultiStack<T>::operator!=(const TMultiStack& other) const
{
    return !(*this == other);
}
template<class T>
inline T TMultiStack<T>::operator()(size_t stackIndex, size_t pos) const
{
    if (stackIndex >= count)
        throw std::out_of_range("12");
    
    if (pos >= GetStackSize(stackIndex))
        throw std::out_of_range("13");
    
    return data[stacksBegin[stackIndex] + pos];
}
template<class U>
std::ostream& operator<<(std::ostream& os, const TMultiStack<U>& stack)
{
    os << "{";
    for (size_t i = 0; i < stack.count; ++i)
    {
        os << "[";
        for (size_t j = 0; j < stack.GetStackSize(i); ++j)
        {
            os << stack(i, j);
            if (j < stack.GetStackSize(i) - 1)
                os << ",";
        }
        os << "]";
        if (i < stack.count - 1)
            os << ",";
    }
    os << "}";
    return os;
}
template<class U>
std::istream& operator>>(std::istream& is, TMultiStack<U>& stack)
{
    size_t count, size;
    is >> count >> size;
    
    TMultiStack<U> temp(count, size);
    
    for (size_t i = 0; i < count; ++i)
    {
        size_t elementCount;
        is >> elementCount;
        
        for (size_t j = 0; j < elementCount; ++j)
        {
            U element;
            is >> element;
            temp.Push(i, element);
        }
    }
    
    stack = std::move(temp);
    return is;
}