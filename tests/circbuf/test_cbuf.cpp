#include <catch2/catch_test_macros.hpp>

#include <stdint.h>
#include <stddef.h>

#include <circbuf/cbuf.h>

TEST_CASE( "circular buffer initialization" ) {
    const uint8_t size = 3;
    uint8_t int_buffer[size];

    CircBuf_t buf;
    CircBufInitParams_t params = {
        .buffer = int_buffer,
        .max_size = 3
    };

    CircBufInit(&params, &buf);
    REQUIRE_FALSE(buf.is_full);
    REQUIRE(buf.buffer == int_buffer);
    REQUIRE(buf.size == 0);

    SECTION( "adding elements" ) {
        REQUIRE(CircBufPush(&buf, 8));
        REQUIRE(CircBufPush(&buf, 2));
        REQUIRE(CircBufPush(&buf, 5));

        REQUIRE(CircBufIsFull(&buf));

        REQUIRE(CircBufPop(&buf) == 8);
        REQUIRE_FALSE(CircBufIsFull(&buf));

        REQUIRE(CircBufPop(&buf) == 2);
        REQUIRE(CircBufPop(&buf) == 5);

        SECTION( "more elements are added" ) {
            REQUIRE(CircBufPush(&buf, 10));
            REQUIRE(CircBufPush(&buf, 11));
            REQUIRE(CircBufPush(&buf, 12));

            REQUIRE(CircBufIsFull(&buf));

            REQUIRE(CircBufPop(&buf) == 10);
            REQUIRE_FALSE(CircBufIsFull(&buf));

            REQUIRE(CircBufPop(&buf) == 11);
            REQUIRE(CircBufPop(&buf) == 12);
        }
    }
}


