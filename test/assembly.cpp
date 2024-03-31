export module assembly;

import <filesystem>;
import <fstream>;
import <iostream>;
import <optional>;
import <cassert>;
import <stdexcept>;
import <string>;
import <vector>;
import <limits>;

import file_util;
import test_harness;

using std::string;
namespace fs = std::filesystem;

static std::vector<uint8_t> assemble(const string& src);

export class Assembly {
    string source;
    std::optional<std::vector<uint8_t>> binary;

    public:
    Assembly(string source) : source(source) {
    }

    // lazy
    std::vector<uint8_t> bytes() {
        if (!this->binary.has_value()) {
            this->binary = assemble(this->source);
        }
        return this->binary.value();
    }
};


static void run_process(string&& cmd);
static TemporaryFile write_source(const string& text);
static TemporaryFile run_assembler(const fs::path& src);

static std::vector<uint8_t> assemble(const string& src) {
    const auto srcpath = write_source(src + "\n");
    const auto binpath = run_assembler(srcpath.path());
    const auto contents = FileUtil::get_contents(binpath.path());
    assert(contents.length() != 0);

    std::vector<uint8_t> res{};
    for (const auto byte : contents) {
        res.push_back(byte);
    }

    return res;
}

static TemporaryFile run_assembler(const fs::path& src) {
    const string ASSEMBLER{"riscv64-linux-gnu-as"};
    const string OBJCOPY{"riscv64-linux-gnu-objcopy"};

    TemporaryFile temp;
    run_process(ASSEMBLER + " " + src.native() + " -o " + temp.path().native());

    TemporaryFile rv;
    run_process(OBJCOPY + " -O binary " + temp.path().native() + " " + rv.path().native());

    return rv;
}

static TemporaryFile write_source(const string& text) {
    const TemporaryFile rv;
    std::ofstream file(rv.path());
    file << text;
    file.close();
    return rv;
}

static void run_process(string&& cmd) {
    FILE* process = popen(cmd.c_str(), "w");
    if (process == nullptr) {
        throw std::runtime_error("Failed to open process: " + cmd);
    }

    if (pclose(process)) {
        throw std::runtime_error("Process failed: " + cmd);
    }
}

#include "harness.h"
TEST_CASE(basic, {
    Assembly assembly{"addi x1, x2, 42"};
    REQUIRE_EQUALS(assembly.bytes().size(), 4u);
});
