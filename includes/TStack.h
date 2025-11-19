#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>

template<typename T>
class TStack
{
public:
    TStack();
    TStack(size_t capacity_);
    TStack(const TStack& other); 
    TStack(TStack&& other);
    ~TStack(); 

    size_t GetCapacity() const;
    size_t GetTop() const;
    size_t Size() const;

    void Push(T elem);  
    T Pop();           
    bool IsEmpty() const;
    bool IsFull() const;

    TStack& operator=(const TStack& other);
    TStack& operator=(TStack&& other);
    bool operator==(const TStack& other) const;
    bool operator!=(const TStack& other) const;

    T FindMin() const;
    void SaveToFile(const std::string& filename) const;
    void LoadFromFile(const std::string& filename);

    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const TStack<U>& stack);
    
    template<typename U>
    friend std::istream& operator>>(std::istream& is, TStack<U>& stack);

private:
    T* data;
    size_t capacity;
    size_t top;        // Индекс верхнего элемента
    void resize(size_t newCapacity);
};

template<typename T>
TStack<T>::TStack() : capacity(0), top(0)
{
    data = nullptr;
}

template<typename T>
TStack<T>::TStack(size_t capacity_) : capacity(capacity_), top(0)
{
    if (capacity_ == 0) 
    {
        throw std::invalid_argument("jopa3");
    }
    data = new T[capacity];
}

template<typename T>
TStack<T>::TStack(const TStack& other) : capacity(other.capacity), top(other.top)
{
    data = new T[capacity];
    for (size_t i = 0; i < top; ++i) 
    {
        data[i] = other.data[i];
    }
}

template<typename T>
TStack<T>::TStack(TStack&& other) : data(other.data), capacity(other.capacity), top(other.top)
{
    other.data = nullptr;
    other.capacity = 0;
    other.top = 0;
}

template<typename T>
TStack<T>::~TStack()
{
    if (data != nullptr)
        delete[] data;
    top=0;
    capacity =0;
}
template<typename T>
void TStack<T>::Push(T elem)
{
    if (IsFull()) 
    {
        resize(capacity * 2);
    }
    data[top] = elem;
    top++;
}
template<typename T>
T TStack<T>::Pop()
{
    if (IsEmpty()) 
    {
        throw std::invalid_argument("jopa4");
    }
    top--;
    return data[top];
}
template<typename T>
bool TStack<T>::IsEmpty() const
{
    return top == 0;
}

template<typename T>
bool TStack<T>::IsFull() const
{
    return top == capacity;
}

template<typename T>
size_t TStack<T>::GetCapacity() const
{
    return capacity;
}

template<typename T>
size_t TStack<T>::GetTop() const
{
    return top;
}

template<typename T>
size_t TStack<T>::Size() const
{
    return top;
}
template<typename T>
TStack<T>& TStack<T>::operator=(const TStack& other)
{
    if (this != &other) 
    {
        delete[] data;
        
        capacity = other.capacity;
        top = other.top;
        
        data = new T[capacity];
        for (size_t i = 0; i < top; ++i) 
        {
            data[i] = other.data[i];
        }
    }
    return *this;
}

template<typename T>
TStack<T>& TStack<T>::operator=(TStack&& other)
{
    if (this != &other) 
    {
        delete[] data;
        
        data = other.data;
        capacity = other.capacity;
        top = other.top;
        
        other.data = nullptr;
        other.capacity = 0;
        other.top = 0;
    }
    return *this;
}

template<typename T>
bool TStack<T>::operator==(const TStack& other) const
{
    if (top != other.top) 
    {
        return false;
    }
    
    for (size_t i = 0; i < top; ++i) 
    {
        if (data[i] != other.data[i]) 
        {
            return false;
        }
    }
    return true;
}

template<typename T>
bool TStack<T>::operator!=(const TStack& other) const
{
    return !(*this == other);
}
template<typename T>
std::ostream& operator<<(std::ostream& os, const TStack<T>& stack)
{
    os << "[";
    for (size_t i = 0; i < stack.top; ++i) 
    {
        os << stack.data[i];
        if (i < stack.top - 1) 
        {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

template<typename T>
std::istream& operator>>(std::istream& is, TStack<T>& stack)
{
    T value;
    while (is >> value) 
    {
        stack.Put(value);
    }
    return is;
}
template<typename T>
void TStack<T>::resize(size_t newCapacity)
{
    if (newCapacity <= capacity) 
        return;
    
    T* newData = new T[newCapacity];
    
    for (size_t i = 0; i < top; ++i) 
    {
        newData[i] = std::move(data[i]);
    }
    
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

template<class T>
inline void TStack<T>::SaveToFile(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file");

    // Сохраняем только размер и элементы
    file << top << std::endl;

    for (size_t i = 0; i < top; ++i)
    {
        file << data[i] << std::endl;
    }

    file.close();
}

template<class T>
inline void TStack<T>::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file");

    // Читаем количество элементов
    size_t elementCount;
    file >> elementCount;

    // Создаем новый стек
    TStack<T> temp(elementCount);

    // Читаем и добавляем элементы
    T element;
    for (size_t i = 0; i < elementCount; ++i)
    {
        file >> element;
        temp.Push(element);
    }

    // Заменяем текущий стек
    *this = std::move(temp);
    file.close();
}

template<class T>
inline T TStack<T>::FindMin() const
{
    if (IsEmpty())
        throw std::underflow_error("Stack is empty");

    T minElem = data[0];
    for (size_t i = 1; i < top; ++i)
    {
        if (data[i] < minElem)
            minElem = data[i];
    }
    
    return minElem;
}
