import assembly;
import memory;
import emulator;
import test_harness;

#include "harness.h"
TEST_CASE(adding, {
    Assembly assembly{R"TEXT(
    addi x1, x2, 42 
    add x2, x1, x1 
    )TEXT"};
    Emulator emulator(0, assembly.bytes());

    REQUIRE_EQUALS(emulator.get_ip(), 0u);

    emulator.step();
    REQUIRE_EQUALS(emulator.get_register(1), 42u);
    REQUIRE_EQUALS(emulator.get_register(2), 0u);

    emulator.step();
    REQUIRE_EQUALS(emulator.get_ip(), 8u);
    REQUIRE_EQUALS(emulator.get_register(1), 42u);
    REQUIRE_EQUALS(emulator.get_register(2), 84u);
});

#include "harness.h"
TEST_CASE(sub_by_add, {
    Assembly assembly{R"TEXT(
    li x1, 25
    add x1, x1, -5 
    )TEXT"};
    Emulator emulator(0, assembly.bytes());

    REQUIRE_EQUALS(emulator.get_ip(), 0u);

    emulator.step();
    emulator.step();
    REQUIRE_EQUALS(emulator.get_register(1), 20u);
});


TEST_CASE(load_store, {
    Assembly assembly{R"TEXT(
    addi x1, x0, 42 
    sw x1, 0x80(x0)
    lw x3, 0x80(x0)
    )TEXT"};
    Emulator emulator(0, assembly.bytes());
    emulator.step();
    emulator.step();

    REQUIRE_EQUALS(emulator.get_ip(), 8u);
    REQUIRE_EQUALS(emulator.get_register(1), 42u);
    REQUIRE_EQUALS(emulator.get_memory().load32(0x80), 42u);

    emulator.step();
    REQUIRE_EQUALS(emulator.get_register(3), 42u);
});

TEST_CASE(auipc, {
    Assembly assembly{R"TEXT(
    nop
    nop
    auipc x1, 0x800
    )TEXT"};

    Emulator emulator(0, assembly.bytes());

    emulator.step();
    emulator.step();
    emulator.step();

    REQUIRE_EQUALS(emulator.get_register(1), 0x800008u);
});
