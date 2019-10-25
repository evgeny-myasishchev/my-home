#ifndef V2_ARRAY_PTR_BUS_H
#define V2_ARRAY_PTR_BUS_H

#include <stdint.h>
#include <stddef.h>

namespace v2
{

template <typename T>
class ArrayPtr
{
private:
    size_t _size;
    T *_array;

public:
    ArrayPtr()
    {
        _size = 0;
        _array = NULL;
    };

    ArrayPtr(size_t size, T *array)
    {
        _size = size;
        _array = array;
    };

    size_t size() const
    {
        return _size;
    }

    T operator[](size_t index) const
    {
        // simple error handling
        if (index >= _size || index < 0)
        {
            return 0;
        }
        else
        {
            return _array[index];
        }
    }
};

} // namespace v2
#endif