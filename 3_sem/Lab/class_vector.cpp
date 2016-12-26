#include <iostream>
#include <memory>
#include <algorithm>
#include <new>
#include <cmath>

template < class T, class Alloc = std::allocator<T> >
class vector {
public:
    using value_type = T;
    using allocator_type = Alloc;
    using size_type = std::size_t;

    using reference = T&;
    using const_reference = const T&;

    using iterator = T*;
    using const_iterator = const T*;

    using a_traits = typename std::allocator_traits<Alloc>;
    using pointer = typename std::allocator_traits<Alloc>::pointer;

    explicit vector(size_type n = 0, const_reference value = T(),
                    const allocator_type& alloc = allocator_type()) :
                    _capacity(n), _alloc(alloc) {
         _data = a_traits::allocate(_alloc, _capacity);

        for (_size = 0; _size < n; _size++) {
            try {
                a_traits::construct(_alloc, _data + _size, value);
            } catch(...) {
                this->~vector();
                throw;
            }
        }
    }

    vector(const vector &x) : _capacity(x._capacity),
                              _alloc(x._alloc) {
        _data = a_traits::allocate(_alloc, _capacity);

        for (_size = 0; _size < x._size; _size++) {
            try {
                 a_traits::construct(_alloc, _data + _size, x[_size]);
            } catch(...) {
                this->~vector();
                throw;
            }
        }
    }

    vector(vector&& x) : _data(x._data),
                         _capacity(x._capacity),
                         _size(x._size), _alloc(x._alloc) {
        x._data = nullptr;
        x._size = 0;
        x._capacity = 0;
    }

    vector(std::initializer_list<value_type> il,
           const allocator_type& alloc = allocator_type()) {
        insert(begin(), il.begin(), il.end());
    }

    void clear() noexcept {
        for(auto i = begin(); i != end(); i++) {
            a_traits::destroy(_alloc, i);
        }
        _size = 0;
    }

    ~vector() {
        clear();
        a_traits::deallocate(_alloc, _data, _capacity);
        _data = nullptr;
        _size = 0;
        _capacity = 0;
    }

   vector& operator=(const vector &x) {
        if (this != &x) {
            vector temp(x);
            swap(*this, temp);
        }
        return *this;
   }

    vector& operator=(vector&& x) {
        vector temp(std::move(x));
        swap(*this, temp);
        return *this;
    }

    void reserve(size_type n) {
        if (n <= _capacity) return;

        const pointer _newData = a_traits::allocate(_alloc, n * 2);
        const size_t _newSize = _size;

        for (size_type i = 0; i < _size; i++) {
            try {
                a_traits::construct(_alloc, _newData + i,
                                    std::move_if_noexcept(_data[i]));
            } catch(...) {
                for (;i > 0; i--) {
                    a_traits::destroy(_alloc, _newData + i - 1);
                }
                a_traits::deallocate(_alloc, _newData, n * 2);

                throw;
            }
        }

        this->~vector();
        _data = _newData;
        _size = _newSize;
        _capacity = n * 2;
    }

    template <class... Args>
    void emplace_back(Args&&... args) {
        reserve(_size + 1);
        a_traits::construct(_alloc, _data + _size, std::forward<Args>(args)...);
        _size++;
    }

    template <class U>
    void push_back(U&& value) {
        emplace_back(std::forward<U>(value));
    }

    void pop_back() {
        if (!_size) {
            throw "Element is out of range\n";
        }

        _size--;
        a_traits::destroy(_alloc, _data + _size);
    }

    void resize(size_type n) {
        if(n < _size) {
            clear();
            _size = n;
        } else {
            resize(n, T());
        }
    }

    void resize(size_type n, const_reference value) {
        size_type newElemPos = _size;
        if (n < _size) {
            clear();
            newElemPos = 0;
        } else {
            reserve(n);
        }

        for (_size = newElemPos; _size < n; _size++) {
            try {
                a_traits::construct(_alloc, _data + _size, value);
            } catch(...) {
                this->~vector();
                throw;
            }
        }
    }

    void insert(iterator pos, std::initializer_list<value_type> il) {
        insert(pos, il.begin(), il.end());
    }

    void insert(iterator pos, const_reference value) {
        insert(pos, {value});
    }

    template <class InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last) {
        if (pos > end() || pos < begin()) {
            throw "Element is out of range\n";
        }

        size_type rangeSize = last - first;
        size_type curPos = _size - 1;
        size_type moveRange = end() - pos;

        reserve(_size + rangeSize);

        for (size_type i = 0; i < moveRange; i++, curPos--) {
           size_type newPos = curPos + rangeSize;
           if (newPos < _size) {
                _data[newPos] = _data[curPos];
           }
           else {
                a_traits::construct(_alloc, _data + newPos,
                                        std::move_if_noexcept(_data[curPos]));
           }
        }

        curPos++;
        for (size_type i = 0; i < rangeSize; i++, curPos++) {
            if (curPos < _size) {
                _data[curPos] = std::move(*(first + i));
            }
            else {
                a_traits::construct(_alloc, _data + curPos,
                                    std::move_if_noexcept( *(first + i) ));
            }
        }

        _size += rangeSize;
    }

    void erase(const_iterator pos) {
        iterator it = begin();
        std::advance(it, std::distance<const_iterator>(it, pos));
        erase(it, it + 1);
    }

    void erase(iterator first, iterator last) {
        for (size_type i = 0; first + i != end(); i++) {
            if (last + i != end()) {
                *(first + i) = std::move(*(last + i));
            } else {
                a_traits::destroy(_alloc, first + i);
            }
        }
        _size -= (last - first);
    }

    friend void swap(vector& lhs, vector &rhs) {
        using std::swap;
        swap(lhs._data, rhs._data);
        swap(lhs._alloc, rhs._alloc);
        swap(lhs._size, rhs._size);
        swap(lhs._capacity, rhs._capacity);
    }

    reference operator[](size_type pos) {
        return _data[pos];
    }

    const_reference operator[](size_type pos) const {
        return _data[pos];
    }

    size_type size() const noexcept {
        return _size;
    }

    iterator begin() noexcept {
        return _data;
    }

    iterator end() noexcept {
        return _data + _size;
    }

    const_iterator begin() const noexcept {
        return _data;
    }

    const_iterator end() const noexcept {
        return _data + _size;
    }

private:
    pointer _data = nullptr;
    allocator_type _alloc;
    size_type _size = 0;
    size_type _capacity = 0;
};

int main()
{
    return 0;
}

