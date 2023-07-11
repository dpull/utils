/****************************************************************************
Copyright (c) 2013      dpull.com
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
#include <algorithm>
#include <vector>

template <class Compare> struct _comp_warpper {
    Compare& _comp;
    _comp_warpper(Compare& comp) : _comp(comp) { }
    template <class T1, class T2> bool operator()(T1& i1, T2& i2) { return _comp(i1, i2) < 0; }
};

// comp(element, value) like strcmp <=====> Returns < 0 if element is less than value; > 0 if element is greater than value, and 0 if they are equal.
// return:
//  first: an iterator pointing to the first element in the range [first, last)
//      that does not satisfy comp(element, value), (i.e. greater or equal to),
//      or last if no such element is found.
//  second: true if an element equal to value is found, false otherwise.
template <class ForwardIt, class T, class Compare> std::pair<ForwardIt, bool> binary_find(ForwardIt first, ForwardIt last, const T& value, Compare comp)
{
    auto ret = std::make_pair(std::lower_bound(first, last, value, _comp_warpper<Compare>(comp)), false);
    ret.second = ret.first != last && !comp(*ret.first, value);
    return ret;
}

// Returns a pair consisting of an iterator to the inserted element
//  (or to the element that prevented the insertion)
//  and a bool denoting whether the insertion took place.
template <class T, class Compare> auto add_unique(std::vector<T>& vec, T&& value, Compare comp) -> std::pair<decltype(vec.insert(vec.begin(), value)), bool>
{
    auto ret = binary_find(vec.begin(), vec.end(), value, comp);
    ret.second = !ret.second;
    if (!ret.second)
        return ret;
    ret.first = vec.insert(ret.first, value);
    return ret;
}