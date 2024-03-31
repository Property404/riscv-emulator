export module assembly;

import <filesystem>;
import <fstream>;
import <iostream>;
import <optional>;
import <cassert>;
import <random>;
import <stdexcept>;
import <string>;
import <string_view>;
import <vector>;
import <limits>;

import test_harness;

using std::string;
using std::string_view;
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

struct TemporaryFile {
    fs::path inner_path;

    public :
    TemporaryFile() {
        this->inner_path = fs::temp_directory_path();

        static std::mt19937_64 generator;
        std::uniform_int_distribution<uint64_t> distribution(0, std::numeric_limits<uint64_t>::max());
        const uint64_t random_number = distribution(generator);

        this->inner_path = this->inner_path / (string{"dev.dagans.ricky."} + std::to_string(random_number));
    }

    const fs::path& path() const {
        return this->inner_path;
    }

    ~TemporaryFile() {
        fs::remove(this->inner_path);
    }
};


static void run_process(string&& cmd);
static TemporaryFile write_source(const string& text);
static TemporaryFile run_assembler(const fs::path& src);
std::string getFileContents(const fs::path& path);

static std::vector<uint8_t> assemble(const string& src) {
    const auto srcpath = write_source(src + "\n");
    const auto binpath = run_assembler(srcpath.path());
    const auto contents = getFileContents(binpath.path());
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

std::string getFileContents(const fs::path& path) {
	std::ifstream fp(path.native());
	std::string contents;
	std::stringstream buffer;
	if (fp.fail()) {
		throw std::runtime_error(string{"Could not open file: "} + path.native());
	}
	buffer << fp.rdbuf();
	contents = buffer.str();
	fp.close();
	return contents;
}

#include "harness.h"
TEST_CASE(basic, {
    Assembly assembly{"addi x1, x2, 42"};
    REQUIRE_EQUALS(assembly.bytes().size(), 4u);
});
