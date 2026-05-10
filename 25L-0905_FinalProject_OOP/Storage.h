#pragma once

template <class T>
class Storage {
private:
    T* data;      
    int count;
    int capacity;

public:
    Storage()  
    {
        count = 0;
        capacity = 100;
        data = new T[capacity];
    }

    ~Storage() 
    {
        delete[] data;
    }


    void add(const T& item) 
    {
        if (count < capacity) 
        {
            data[count] = item;
            count++;
        }
    }

    T& get(int index) 
    {
        return data[index];
    }

  
    int size() 
    {
        return count;
    }


    T* findbyid(int id) 
    {
        for (int i = 0; i < count; i++)
        {
            if (data[i].getid() == id)
            {
                return &data[i];
            }
        }
        return nullptr;
    }

    bool removebyid(int id)
    {
        for (int i = 0; i < count; i++)
        {
            if (data[i].getid() == id) 
            {
                for (int j = i; j < count - 1; j++)
                {
                    data[j] = data[j + 1];
                }
                count = count - 1;
                return true;
            }
        }
        return false;
    }


    T* getAll() 
    {
        return data;
    }


    void clear()
    {
        count = 0;
    }
};