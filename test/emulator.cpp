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

TEST_CASE(jal, {
    Assembly assembly{R"TEXT(
    nop
    jal next
    nop
next:
    nop
    )TEXT"};

    Emulator emulator(0, assembly.bytes());

    emulator.step();
    emulator.step();

    REQUIRE_EQUALS(emulator.get_register(Register::RA), 0x08u);
    REQUIRE_EQUALS(emulator.get_ip(), 0x0cu);
});

TEST_CASE(lw_negative_offset, {
    Assembly assembly{R"TEXT(
    addi fp, zero, 0x100
    lw	a5,-20(fp)
    )TEXT"};

    Emulator emulator(0, assembly.bytes());
    emulator.get_memory_mut().store32(0x100 - 20, 0x0decaf00);

    emulator.step();
    REQUIRE_EQUALS(emulator.get_register(Register::FP), 0x100u);
    emulator.step();
    REQUIRE_EQUALS(emulator.get_register(Register::ARG5), 0x0decaf00u);
});

TEST_CASE(lwu_negative_offset, {
    Assembly assembly{R"TEXT(
    addi fp, zero, 0x100
    lwu	a5,-20(fp)
    )TEXT"};

    Emulator emulator(0, assembly.bytes());
    emulator.get_memory_mut().store32(0x100 - 20, 0x0decaf00);

    emulator.step();
    REQUIRE_EQUALS(emulator.get_register(Register::FP), 0x100u);
    emulator.step();
    REQUIRE_EQUALS(emulator.get_register(Register::ARG5), 0x0decaf00u);
});

TEST_CASE(ld_negative_offset, {
    Assembly assembly{R"TEXT(
    addi fp, zero, 0x100
    ld	a5,-20(fp)
    )TEXT"};

    Emulator emulator(0, assembly.bytes());
    emulator.get_memory_mut().store64(0x100 - 20, 0x0decaf00);

    emulator.step();
    REQUIRE_EQUALS(emulator.get_register(Register::FP), 0x100u);
    emulator.step();
    REQUIRE_EQUALS(emulator.get_register(Register::ARG5), 0x0decaf00u);
});

TEST_CASE(sw_negative_offset, {
    Assembly assembly{R"TEXT(
    addi fp, zero, 0x100
    sw	x0,-20(fp)
    )TEXT"};

    Emulator emulator(0, assembly.bytes());
    emulator.step();
    REQUIRE_EQUALS(emulator.get_register(Register::FP), 0x100u);
    emulator.step();
    REQUIRE_EQUALS(emulator.get_memory().load32(0x100 - 20), 0u);
});

TEST_CASE(sd_negative_offset, {
    Assembly assembly{R"TEXT(
    addi fp, zero, 0x100
    sd	x0,-20(fp)
    )TEXT"};

    Emulator emulator(0, assembly.bytes());
    emulator.step();
    REQUIRE_EQUALS(emulator.get_register(Register::FP), 0x100u);
    emulator.step();
    REQUIRE_EQUALS(emulator.get_memory().load64(0x100 - 20), 0u);
});
