/****************************************************************************
Copyright (c) 2018      dpull.com
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
#include <cstdint>
#include <cstring>
#include <list>
#include <map>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace serializer {

template <typename T> bool serialize_u(char*& buffer, size_t& buffer_size, T v)
{
    if (buffer_size < sizeof(v))
        return false;

    for (size_t i = 0; i < sizeof(v); i++) {
        buffer[i] = static_cast<char>(v & 0xFF);
        v >>= 8;
    }
    buffer += sizeof(v);
    buffer_size -= sizeof(v);
    return true;
}

template <typename T> bool deserialize_u(const char*& data, size_t& data_len, T& v)
{
    if (data_len < sizeof(v))
        return false;

    v = 0;
    for (int i = sizeof(v) - 1; i >= 0; i--) {
        v <<= 8;
        v |= static_cast<T>(static_cast<unsigned char>(data[i]));
    }
    data += sizeof(v);
    data_len -= sizeof(v);
    return true;
}

template <typename T> typename std::enable_if<std::is_integral<typename std::remove_reference<T>::type>::value && std::is_signed<T>::value, bool>::type serialize(char*& buffer, size_t& buffer_size, T v)
{
    typedef typename std::make_unsigned<T>::type UT;
    return serialize_u(buffer, buffer_size, (UT)v);
}
template <typename T> typename std::enable_if<std::is_integral<typename std::remove_reference<T>::type>::value && std::is_signed<T>::value, bool>::type deserialize(const char*& data, size_t& data_len, T& v)
{
    typedef typename std::make_unsigned<T>::type UT;
    return deserialize_u(data, data_len, *((UT*)&v));
}

template <typename T> typename std::enable_if<std::is_integral<T>::value && std::is_unsigned<T>::value, bool>::type serialize(char*& buffer, size_t& buffer_size, T v) { return serialize_u(buffer, buffer_size, v); }
template <typename T> typename std::enable_if<std::is_integral<T>::value && std::is_unsigned<T>::value, bool>::type deserialize(const char*& data, size_t& data_len, T& v) { return deserialize_u(data, data_len, v); }

template <typename T> typename std::enable_if<std::is_floating_point<T>::value, bool>::type serialize(char*& buffer, size_t& buffer_size, T v)
{
    if (buffer_size >= sizeof(v)) {
        memcpy(buffer, &v, sizeof(v));
        buffer += sizeof(v);
        buffer_size -= sizeof(v);
        return true;
    }
    return false;
}
template <typename T> typename std::enable_if<std::is_floating_point<T>::value, bool>::type deserialize(const char*& data, size_t& data_len, T& v)
{
    if (data_len >= sizeof(v)) {
        memcpy(&v, data, sizeof(v));
        data += sizeof(v);
        data_len -= sizeof(v);
        return true;
    }
    return false;
}
inline bool serialize(char*& buffer, size_t& buffer_size, const char* v)
{
    size_t len = strlen(v) + 1;
    if (buffer_size >= len) {
        memcpy(buffer, v, len);
        buffer += len;
        buffer_size -= len;
        return true;
    }
    return false;
}
inline bool serialize(char*& buffer, size_t& buffer_size, char* v) { return serialize(buffer, buffer_size, (const char*)v); }

inline bool deserialize(const char*& data, size_t& data_len, const char*& v)
{
    auto* end = data + data_len;
    for (auto* pos = data; pos < end; pos++) {
        if (*pos == '\0') {
            v = data;
            data = pos + 1;
            data_len = (size_t)(end - data);
            return true;
        }
    }
    return false;
}

inline bool serialize(char*& buffer, size_t& buffer_size, const std::string& v)
{
    size_t len = v.size() + 1;
    if (buffer_size >= len) {
        memcpy(buffer, v.c_str(), len);
        buffer += len;
        buffer_size -= len;
        return true;
    }
    return false;
}
inline bool deserialize(const char*& data, size_t& data_len, std::string& v)
{
    const char* str = nullptr;
    if (deserialize(data, data_len, str)) {
        v = str;
        return true;
    }
    return false;
}

template <int C> bool serialize(char*& buffer, size_t& buffer_size, const unsigned char (&v)[C])
{
    if (buffer_size >= C) {
        memcpy(buffer, v, C);
        buffer += C;
        buffer_size -= C;
        return true;
    }
    return false;
}
template <int C> bool deserialize(const char*& data, size_t& data_len, char (&v)[C])
{
    auto* end = data + data_len;
    for (auto* pos = data; pos < end; pos++) {
        if (*pos == '\0') {
            int len = (int)(pos - data) + 1;
            if (len <= C) {
                memcpy(v, data, len);
                data = pos + 1;
                data_len = (size_t)(end - data);
                return true;
            }
            return false;
        }
    }
    return false;
}

inline bool serialize(char*& data, size_t& data_len, bool v)
{
    unsigned char vchar = !!v;
    return serialize_u(data, data_len, vchar);
}
inline bool deserialize(const char*& data, size_t& data_len, bool& v)
{
    unsigned char vchar;
    if (deserialize_u(data, data_len, vchar)) {
        v = !!vchar;
        return true;
    }
    return false;
}

template <typename T, int C> bool serialize(char*& buffer, size_t& buffer_size, const T (&v)[C])
{
    for (int i = 0; i < C; i++) {
        if (!serialize(buffer, buffer_size, v[i]))
            return false;
    }
    return true;
}
template <typename T, int C> bool deserialize(const char*& data, size_t& data_len, T (&v)[C])
{
    for (int i = 0; i < C; i++) {
        if (!deserialize(data, data_len, v[i]))
            return false;
    }
    return true;
}

template <typename T, typename... Others> bool serialize(char*& buffer, size_t& buffer_size, const std::vector<T, Others...>& v)
{
    return serialize(buffer, buffer_size, (int32_t)v.size()) && std::all_of(v.begin(), v.end(), [&](const T& node) { return serialize(buffer, buffer_size, node); });
}
template <typename T, typename... Others> bool deserialize(const char*& data, size_t& data_len, std::vector<T, Others...>& v)
{
    int32_t count = 0;
    if (!deserialize(data, data_len, count))
        return false;

    v.reserve(count);
    for (int32_t i = 0; i < count; i++) {
        T tmp;
        if (!deserialize(data, data_len, tmp))
            return false;
        v.push_back(std::move(tmp));
    }
    return true;
}

template <typename T, typename... Others> bool serialize(char*& buffer, size_t& buffer_size, const std::list<T, Others...>& v)
{
    return serialize(buffer, buffer_size, (int32_t)v.size()) && std::all_of(v.begin(), v.end(), [&](const T& node) { return serialize(buffer, buffer_size, node); });
}
template <typename T, typename... Others> bool deserialize(const char*& data, size_t& data_len, std::list<T, Others...>& v)
{
    int32_t count = 0;
    if (!deserialize(data, data_len, count))
        return false;

    for (int32_t i = 0; i < count; i++) {
        T tmp;
        if (!deserialize(data, data_len, tmp))
            return false;
        v.push_back(std::move(tmp));
    }
    return true;
}
template <typename T, typename... Others> bool serialize(char*& buffer, size_t& buffer_size, const std::set<T, Others...>& v)
{
    return serialize(buffer, buffer_size, (int32_t)v.size()) && std::all_of(v.begin(), v.end(), [&](const T& node) { return serialize(buffer, buffer_size, node); });
}

template <typename T, typename... Others> bool deserialize(const char*& data, size_t& data_len, std::set<T, Others...>& v)
{
    int32_t count = 0;
    if (!deserialize(data, data_len, count))
        return false;

    for (int32_t i = 0; i < count; i++) {
        T tmp;
        if (!deserialize(data, data_len, tmp))
            return false;
        v.insert(std::move(tmp));
    }
    return true;
}

template <typename T, typename... Others> bool serialize(char*& buffer, size_t& buffer_size, const std::unordered_set<T, Others...>& v)
{
    return serialize(buffer, buffer_size, (int32_t)v.size()) && std::all_of(v.begin(), v.end(), [&](const T& node) { return serialize(buffer, buffer_size, node); });
}

template <typename T, typename... Others> bool deserialize(const char*& data, size_t& data_len, std::unordered_set<T, Others...>& v)
{
    int32_t count = 0;
    if (!deserialize(data, data_len, count))
        return false;

    for (int32_t i = 0; i < count; i++) {
        T tmp;
        if (!deserialize(data, data_len, tmp))
            return false;
        v.insert(std::move(tmp));
    }
    return true;
}

template <typename K, typename V, typename... Others> bool serialize(char*& buffer, size_t& buffer_size, const std::map<K, V, Others...>& v)
{
    return serialize(buffer, buffer_size, (int32_t)v.size()) && std::all_of(v.begin(), v.end(), [&](const typename std::map<K, V, Others...>::value_type& node) { return serialize(buffer, buffer_size, node.first) && serialize(buffer, buffer_size, node.second); });
}

template <typename KType, typename VType, typename... Others> bool deserialize(const char*& data, size_t& data_len, std::map<KType, VType, Others...>& v)
{
    int32_t count = 0;
    if (!deserialize(data, data_len, count))
        return false;

    for (int32_t i = 0; i < count; i++) {
        KType tk;
        VType tv;
        if (!deserialize(data, data_len, tk) || !deserialize(data, data_len, tv))
            return false;
        v[std::move(tk)] = std::move(tv);
    }
    return true;
}

template <typename K, typename V, typename... Others> bool serialize(char*& buffer, size_t& buffer_size, const std::unordered_map<K, V, Others...>& v)
{
    return serialize(buffer, buffer_size, (int32_t)v.size()) && std::all_of(v.begin(), v.end(), [&](const typename std::unordered_map<K, V, Others...>::value_type& node) { return serialize(buffer, buffer_size, node.first) && serialize(buffer, buffer_size, node.second); });
}

template <typename KType, typename VType, typename... Others> bool deserialize(const char*& data, size_t& data_len, std::unordered_map<KType, VType, Others...>& v)
{
    int32_t count = 0;
    if (!deserialize(data, data_len, count))
        return false;

    for (int32_t i = 0; i < count; i++) {
        KType tk;
        VType tv;
        if (!deserialize(data, data_len, tk) || !deserialize(data, data_len, tv))
            return false;
        v[std::move(tk)] = std::move(tv);
    }
    return true;
}

template <typename T, const int C> bool serialize(char*& buffer, size_t& buffer_size, T (&table)[C])
{
    return std::all_of(table, table + C, [&](const T& node) { return serialize(buffer, buffer_size, node); });
}

template <typename first_type, typename... other_types> bool serialize(char*& buffer, size_t& buffer_size, const first_type& first_arg, const other_types&... other_args) { return serialize(buffer, buffer_size, first_arg) && serialize(buffer, buffer_size, other_args...); }

template <typename first_type, typename... other_types> bool deserialize(const char*& data, size_t& data_len, first_type& first_arg, other_types&... other_args) { return deserialize(data, data_len, first_arg) && deserialize(data, data_len, other_args...); }
}