#include "serialize.h"
#include "tuple_util.h"
#include "gtest/gtest.h"

struct encode {
    encode(size_t sz = 1024 * 1024)
    {
        buffer.resize(sz);
        pos = buffer.data();
        left = buffer.size();
    }
    template <typename T> bool operator()(T& i) { 
        return serializer::serialize(pos, left, i) && serializer::serialize(pos, left, verify++); 
    }

    std::vector<char> buffer;
    char* pos;
    size_t left;
    int8_t verify = 8;
};

struct decode {
    decode(const std::vector<char>& in)
        : buffer(in)
    {
        pos = buffer.data();
        left = buffer.size();
    }
    template <typename T> bool operator()(T& i) 
    {
        int8_t check = 0;
        return serializer::deserialize(pos, left, i) && serializer::deserialize(pos, left, check) && check == verify++;
    }

    const std::vector<char>& buffer;
    const char* pos;
    size_t left;
    int8_t verify = 8;
};

TEST(tuple_util, for_each)
{
    auto in = std::make_tuple(1, 2.0, std::string("3"));
    decltype(in) out;

    encode e;
    auto ret = tuple_util<decltype(in)>::for_each(&in, &e);
    ASSERT_TRUE(ret);

    decode d(e.buffer);
    ret = tuple_util<decltype(out)>::for_each(&out, &d);
    ASSERT_TRUE(ret);

    ASSERT_EQ(std::get<0>(in), std::get<0>(out));
    ASSERT_EQ(std::get<1>(in), std::get<1>(out));
    ASSERT_EQ(std::get<2>(in), std::get<2>(out));
}

TEST(tuple_util, index) { 
    auto in = std::make_tuple(1, 2.0, std::string("3")); 
    auto index_d = tuple_util<decltype(in)>::index<double>();
    ASSERT_EQ(index_d, 1);

    auto index_f = tuple_util<decltype(in)>::index<float>(); 
    ASSERT_EQ(index_f, -1);
}
