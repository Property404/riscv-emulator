#include "framework.h"
import <vector>;
import memory;
TEST_CASE(basic, {
    Memory memory({0xba,0xdd,0xec,0xaf});
    REQUIRE( memory.load8(0) == 0xba );

    memory.store32(0, 0xDEADBEEF);
    REQUIRE( memory.load32(0) == 0xDEADBEEF );
});
