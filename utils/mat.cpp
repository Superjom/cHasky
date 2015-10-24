#include "mat.h"
template<typename T>
void Mat<T>::set_shape(const shape_t& shape) {
    CHECK_GT(shape.size, 0);
    CHECK_GT(shape.width, 0);
    // init 
    if (_shape.size != shape.size) {
        _data.resize(shape.size);
        _shape.size = shape.size;
    } 
    if (_shape.width != shape.width) {
        for (int i = 0; i < _shape.size; i++)  {
            _data[i].init(shape.size);
        }
        _shape.width = shape.width;
    }
}

template<typename T>
inline const shape_t& Mat<T>::shape() const {
    return _shape;
}