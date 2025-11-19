#include<iostream>
#include<stdexcept>
#include <fstream>
#include <string>

template<typename T>
class TQueue{
public:
    TQueue();
    TQueue(size_t capacity_);
    TQueue(const TQueue& other); 
    TQueue(TQueue&& other) ;
    ~TQueue(); 

    size_t GetCapacity() const;
    size_t GetFront() const;
    size_t GetTail() const;
    size_t Size() const;

    void Put(T elem);
    T Get();
    bool IsEmpty() const;
    bool IsFull() const;

    TQueue& operator=(const TQueue& other);
    TQueue& operator=(TQueue&& other);
    bool operator==(const TQueue& other) const;
    bool operator!=(const TQueue& other) const;

    T FindMin() const;
    void SaveToFile(const std::string& filename) const;
    void LoadFromFile(const std::string& filename);

    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const TQueue<U>& queue);
    
    template<typename U>
    friend std::istream& operator>>(std::istream& is, TQueue<U>& queue);
private:
    T* data;
    size_t capacity;
    size_t front;
    size_t tail;
    void resize( size_t newCapacity);
};

template<typename T>
TQueue<T>::TQueue() : capacity(0), front(0), tail(0)
{
    data = nullptr;
}

template<class T>
inline TQueue<T>::TQueue(size_t capacity_) 
    : capacity(capacity_ + 1), front(0), tail(0)
{
    if (capacity_ == 0) 
    {
        throw std::invalid_argument("jopa1");
    }
    data = new T[capacity];
}
template<class T>
inline TQueue<T>::TQueue(const TQueue& other) 
    : capacity(other.capacity), front(other.front), tail(other.tail)
{
    data = new T[capacity];
    for (size_t i = 0; i < capacity; ++i) 
    {
        data[i] = other.data[i];
    }
}
template<class T>
inline TQueue<T>::TQueue(TQueue&& other) 
    : data(other.data), capacity(other.capacity), front(other.front), tail(other.tail) 
{
    other.data = nullptr;
    other.capacity = 0;
    other.front = 0;
    other.tail = 0;
}
template<class T>
inline TQueue<T>::~TQueue()
{
    if (data != nullptr)
        delete[] data;
    capacity = 0;
}
template<class T>
inline void TQueue<T>::Put(T elem)
{
    if (IsFull()) 
    {
        resize(capacity * 2);
    }
    data[tail] = elem;
    tail = (tail + 1) % capacity; // по кольцу
}

template<class T>
inline T TQueue<T>::Get()
{
    if (IsEmpty()) 
    {
        throw std::invalid_argument("jopa2");
    }
    T value = data[front];
    front = (front + 1) % capacity;
    return value;
}
template<class T>
inline bool TQueue<T>::IsEmpty() const
{
    return front == tail;
}
template<class T>
inline bool TQueue<T>::IsFull() const
{
    return Size() == capacity - 1;
}
template <class T>
inline size_t TQueue<T>::GetCapacity() const
{
  return capacity;
}
template<class T>
inline size_t TQueue<T>::GetFront() const
{
    return front;
}

template<class T>
inline size_t TQueue<T>::GetTail() const
{
    return tail;
}
template<class T>
inline size_t TQueue<T>::Size() const
{
    if (tail >= front) 
        return tail - front;
    else 
        return capacity - (front - tail);
}
template<class T>
inline TQueue<T>& TQueue<T>::operator=(const TQueue& other)
{
    if (this != &other) 
    {
        delete[] data;
        
        capacity = other.capacity;
        front = other.front;
        tail = other.tail;
        
        data = new T[capacity];
        for (size_t i = 0; i < capacity; ++i) 
            data[i] = other.data[i];
    }
    return *this;
}

template<class T>
inline TQueue<T>& TQueue<T>::operator=(TQueue&& other)
{
    if (this != &other) 
    {
        delete[] data;
        
        data = other.data;
        capacity = other.capacity;
        front = other.front;
        tail = other.tail;
        
        other.data = nullptr;
        other.capacity = 0;
        other.front = 0;
        other.tail = 0;
    }
    return *this;
}

template<class T>
inline bool TQueue<T>::operator==(const TQueue& other) const
{
    if (Size() != other.Size()) 
    {
        return false;
    }
    
    size_t i = front;
    size_t j = other.front;
    for (size_t k = 0; k < Size(); ++k) 
    {
        if (data[i] != other.data[j]) 
        {
            return false;
        }
        i = (i + 1) % capacity;
        j = (j + 1) % other.capacity;
    }
    return true;
}

template<class T>
inline bool TQueue<T>::operator!=(const TQueue& other) const
{
    return !(*this == other);
}
template<class T>
inline std::ostream& operator<<(std::ostream& os, const TQueue<T>& queue)
{
    os << "[";
    size_t current = queue.front;
    for (size_t i = 0; i < queue.Size(); ++i) 
    {
        os << queue.data[current];
        if (i < queue.Size() - 1) 
        {
            os << ", ";
        }
        current = (current + 1) % queue.capacity;
    }
    os << "]";
    return os;
}
template<class T>
inline std::istream& operator>>(std::istream& is, TQueue<T>& queue)
{
    T value;
    while (is >> value) 
    {
        queue.Put(value);
    }
    return is;
}
template<class T>
inline void TQueue<T>::resize(size_t newCapacity)
{
    if (newCapacity <= capacity) 
        return;
    
    T* newData = new T[newCapacity];
    size_t currentSize = Size();
    
    size_t current = front;
    for (size_t i = 0; i < currentSize; ++i) 
    {
        newData[i] = std::move(data[current]);
        current = (current + 1) % capacity;
    }
    
    delete[] data;
    data = newData;
    capacity = newCapacity;
    front = 0;
    tail = currentSize;
}

template<class T>
inline T TQueue<T>::FindMin() const
{
    if (IsEmpty())
        throw std::underflow_error("jopa 15");

    T minElem = data[front];
    size_t current = (front + 1) % capacity;
    
    for (size_t i = 1; i < Size(); ++i)
    {
        if (data[current] < minElem)
            minElem = data[current];
        current = (current + 1) % capacity;
    }
    
    return minElem;
}
template<class T>
inline void TQueue<T>::SaveToFile(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file");

    size_t currentSize = Size();
    file << currentSize << std::endl;

    size_t current = front;
    for (size_t i = 0; i < currentSize; ++i)
    {
        file << data[current] << std::endl;
        current = (current + 1) % capacity;
    }

    file.close();
}

template<class T>
inline void TQueue<T>::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file");

    
    size_t elementCount;
    file >> elementCount;

    
    TQueue<T> temp(elementCount + 1); 

    T element;
    for (size_t i = 0; i < elementCount; ++i)
    {
        file >> element;
        temp.Put(element);
    }

    *this = std::move(temp);
    file.close();
}
