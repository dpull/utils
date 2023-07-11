/****************************************************************************
Copyright (c) 2019      dpull.com
http://www.dpull.com
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#pragma once
#include <functional>
#include <tuple>
#include <type_traits>

template <typename T, std::size_t N = std::tuple_size<T>::value> struct tuple_util {
    template <typename U> static bool for_each(T* t, U* f)
    {
        if (!tuple_util<T, N - 1>::for_each(t, f))
            return false;
        return (*f)(std::get<N - 1>(*t));
    }

    template <typename U> constexpr static int index(typename std::enable_if<std::is_same<typename std::remove_reference<typename std::tuple_element<N - 1, T>::type>::type, U>::value>::type*) { return N - 1; }

    template <typename U> constexpr static int index(...) { return tuple_util<T, N - 1>::template index<U>(nullptr); }
};

template <typename T> struct tuple_util<T, 0> {
    static bool for_each(...) { return true; }

    template <typename U> constexpr static int index(...) { return -1; }
};
