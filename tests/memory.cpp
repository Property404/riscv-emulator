#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstdint>

import memory;

uint32_t factorial( uint32_t number ) {
    return number <= 1 ? number : factorial(number-1) * number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    Memory memory({0xba,0xdd,0xec,0xaf});
    REQUIRE( memory.load8(0) == 0xba );

    memory.store32(0, 0xDEADBEEF);
    REQUIRE( memory.load32(0) == 0xDEADBEEF );
}
