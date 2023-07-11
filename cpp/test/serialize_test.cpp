#include "serialize.h"
#include "gtest/gtest.h"

#define BASE_NUMBER_TYPES char, unsigned char, short, unsigned short, int, unsigned, long, unsigned long, long long, unsigned long long, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double, bool

template <typename T> class serialize_test_number : public ::testing::Test {
};

TYPED_TEST_SUITE_P(serialize_test_number);

static int64_t rand64()
{
    int32_t low = rand();
    int32_t high = rand();
    int sign = rand() % 2 == 0 ? 1 : -1;
    return (((int64_t)high << 32) | low) * sign;
}

TYPED_TEST_P(serialize_test_number, num)
{
    std::vector<char> buffer(1024);

    for (size_t i = 0; i < 4096; i++) {
        TypeParam in = (TypeParam)rand64();
        TypeParam out = 0;
        auto count = sizeof(in);

        auto indata = buffer.data();
        auto insize = buffer.size();

        auto ret = serializer::serialize(indata, insize, in);
        ASSERT_TRUE(ret);
        ASSERT_EQ(buffer.size() - insize, count);

        const char* outdata = buffer.data();
        auto outsize = buffer.size();
        ret = serializer::deserialize(outdata, outsize, out);
        ASSERT_TRUE(ret);
        ASSERT_EQ(buffer.size() - outsize, count);
        ASSERT_EQ(in, out);
    }
}

TYPED_TEST_P(serialize_test_number, array)
{
    if (std::is_same<TypeParam, char>())
        return;

    std::vector<char> buffer(1024 * 1024);
    TypeParam in[789];
    TypeParam out[789] = { 0 };

    for (size_t i = 0; i < sizeof(in) / sizeof(in[0]); i++) {
        in[i] = (TypeParam)rand64();
    }

    auto indata = buffer.data();
    auto insize = buffer.size();

    auto ret = serializer::serialize(indata, insize, in);
    ASSERT_TRUE(ret);

    const char* outdata = buffer.data();
    auto outsize = buffer.size();
    ret = serializer::deserialize(outdata, outsize, out);
    ASSERT_TRUE(ret);

    for (size_t i = 0; i < sizeof(in) / sizeof(in[0]); i++) {
        ASSERT_EQ(in[i], out[i]);
    }
}

TYPED_TEST_P(serialize_test_number, vector)
{
    std::vector<char> buffer(1024 * 1024);
    std::vector<TypeParam> in;
    std::vector<TypeParam> out;

    for (size_t i = 0; i < 4096; i++) {
        in.push_back((TypeParam)rand64());
    }

    auto indata = buffer.data();
    auto insize = buffer.size();
    auto count = in.size() * sizeof(TypeParam) + sizeof(int32_t);

    auto ret = serializer::serialize(indata, insize, in);
    ASSERT_TRUE(ret);
    ASSERT_EQ(buffer.size() - insize, count);

    const char* outdata = buffer.data();
    auto outsize = buffer.size();
    ret = serializer::deserialize(outdata, outsize, out);
    ASSERT_TRUE(ret);
    ASSERT_EQ(buffer.size() - outsize, count);
    ASSERT_EQ(in.size(), out.size());
    for (size_t i = 0; i < in.size(); i++) {
        ASSERT_EQ(in[i], out[i]);
    }
}

TYPED_TEST_P(serialize_test_number, map)
{
    std::vector<char> buffer(1024 * 1024);
    std::map<TypeParam, TypeParam> in;
    std::map<TypeParam, TypeParam> out;

    for (size_t i = 0; i < 4096; i++) {
        in.insert(std::make_pair((TypeParam)rand64(), (TypeParam)rand64()));
    }

    auto indata = buffer.data();
    auto outsize = buffer.size();
    auto count = in.size() * sizeof(TypeParam) * 2 + sizeof(int32_t);

    auto ret = serializer::serialize(indata, outsize, in);
    ASSERT_TRUE(ret);
    ASSERT_EQ(buffer.size() - outsize, count);

    const char* outdata = buffer.data();
    outsize = buffer.size();
    ret = serializer::deserialize(outdata, outsize, out);
    ASSERT_TRUE(ret);
    ASSERT_EQ(buffer.size() - outsize, count);
    ASSERT_EQ(in.size(), out.size());

    for (auto& i : in) {
        ASSERT_EQ(in[i.first], out[i.first]);
    }
}

TYPED_TEST_P(serialize_test_number, set)
{
    std::vector<char> buffer(1024 * 1024);
    std::set<TypeParam> in;
    std::set<TypeParam> out;

    for (size_t i = 0; i < 4096; i++) {
        in.insert((TypeParam)rand64());
    }

    auto indata = buffer.data();
    auto insize = buffer.size();
    auto count = in.size() * sizeof(TypeParam) + sizeof(int32_t);

    auto ret = serializer::serialize(indata, insize, in);
    ASSERT_TRUE(ret);
    ASSERT_EQ(buffer.size() - insize, count);

    const char* outdata = buffer.data();
    auto outsize = buffer.size();
    ret = serializer::deserialize(outdata, outsize, out);
    ASSERT_TRUE(ret);
    ASSERT_EQ(buffer.size() - outsize, count);
    ASSERT_EQ(in.size(), out.size());

    for (auto& i : in) {
        ASSERT_EQ(out.count(i), 1);
    }
}

REGISTER_TYPED_TEST_SUITE_P(serialize_test_number, num, array, vector, map, set);
typedef testing::Types<BASE_NUMBER_TYPES> num_types;
INSTANTIATE_TYPED_TEST_SUITE_P(serialize, serialize_test_number, num_types);


TEST(serialize_test_string, const_char_ptr)
{
    std::vector<char> buffer(1024 * 1024);
    const char* in = "1234561232456";
    char out[1024];

    auto indata = buffer.data();
    auto insize = buffer.size();
    auto count = strlen(in) + 1;

    auto ret = serializer::serialize(indata, insize, in);
    ASSERT_TRUE(ret);
    ASSERT_EQ(buffer.size() - insize, count);

    const char* outdata = buffer.data();
    auto outsize = buffer.size();
    ret = serializer::deserialize(outdata, outsize, out);
    ASSERT_TRUE(ret);
    ASSERT_EQ(buffer.size() - outsize, count);

    for (size_t i = 0; i < count; i++) {
        ASSERT_EQ(in[i], out[i]);
    }
}

TEST(serialize_test_string, string)
{
    std::vector<char> buffer(1024 * 1024);
    
    std::string in = "1234561232456";
    std::string out;

    auto indata = buffer.data();
    auto insize = buffer.size();
    auto count = in.size() + 1;

    auto ret = serializer::serialize(indata, insize, in);
    ASSERT_TRUE(ret);
    ASSERT_EQ(buffer.size() - insize, count);

    const char* outdata = buffer.data();
    auto outsize = buffer.size();
    ret = serializer::deserialize(outdata, outsize, out);
    ASSERT_TRUE(ret);
    ASSERT_EQ(buffer.size() - outsize, count);

    for (size_t i = 0; i < count; i++) {
        ASSERT_EQ(in[i], out[i]);
    }
}