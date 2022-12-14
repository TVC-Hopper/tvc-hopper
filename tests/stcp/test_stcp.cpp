#include <iostream>
#include <string>

#include <catch2/catch_test_macros.hpp>

#include <stcp/stcp.h>

extern uint32_t StcpCrc32(uint8_t *buffer, uint16_t length);

TEST_CASE("crc32", "[stcp]")
{
    std::string buffer = "hello";
    REQUIRE(StcpCrc32((uint8_t *)buffer.c_str(), 5) == 907060870);
    buffer = "testtest";
    REQUIRE(StcpCrc32((uint8_t *)buffer.c_str(), 8) == 3966352177);
    buffer = "\xFA\xFA";
    REQUIRE(StcpCrc32((uint8_t *)buffer.c_str(), 2) == 4074897610);
}

TEST_CASE("escape1", "[stcp]")
{
    std::string buffer = "\x7A";
    std::string correct = "\x7A\x7B\x7A\x7F\x7F";
    uint16_t size = (uint16_t)buffer.length();
    uint8_t *output = StcpEscape((uint8_t *)buffer.c_str(), &size);

    REQUIRE(size == correct.length());

    for (int i = 0; i < correct.length(); ++i)
    {
        REQUIRE(output[i] == correct[i]);
    }
}

TEST_CASE("escape2", "[stcp]")
{
    std::string buffer = "\x7Ahihello\x7A\x7A";
    std::string correct = "\x7A\x7B\x7Ahihello\x7B\x7A\x7B\x7A\x7F\x7F";
    uint16_t size = (uint16_t)buffer.length();
    uint8_t *output = StcpEscape((uint8_t *)buffer.c_str(), &size);

    REQUIRE(size == correct.length());

    for (int i = 0; i < correct.length(); ++i)
    {
        REQUIRE(output[i] == correct[i]);
    }
}

TEST_CASE("escape3", "[stcp]")
{
    std::string buffer = "\x7Ahi\x7Ahello\x7A";
    std::string correct = "\x7a\x7B\x7Ahi\x7B\x7Ahello\x7B\x7A\x7F\x7F";
    uint16_t size = (uint16_t)buffer.length();
    uint8_t *output = StcpEscape((uint8_t *)buffer.c_str(), &size);

    REQUIRE(size == correct.length());

    for (int i = 0; i < correct.length(); ++i)
    {
        REQUIRE(output[i] == correct[i]);
    }
}
