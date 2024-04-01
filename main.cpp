import <vector>;
import <cstdint>;
import <string>;
import <typeinfo>;
import <stdexcept>;
import <iostream>;
import emulator;
import memory;
import debug;
import file_util;

constexpr size_t DEFAULT_LOAD_ADDRESS = 0x1000;

int main(int argc, const char* argv[]) {
    if (argc != 2 && argc != 3 ) {
        std::cerr << "Expected one argument" << std::endl;
        return 1;
    }
    bool debug = false;
    if (argv[1]  == std::string{"--debug"}) {
        debug = true;
        argc--;
        argv++;
    }

    const auto binary = FileUtil::get_contents(argv[1]);
    Memory memory(DEFAULT_LOAD_ADDRESS, std::vector<uint8_t>(binary.cbegin(), binary.cend()));
    Emulator emulator(0x1000, memory);

    // Write
    emulator.register_ecall(64,[](const Emulator& emulator) {
            const auto fd = emulator.get_register(Register::ARG0);
            const auto buf = emulator.get_register(Register::ARG1);
            const auto count = emulator.get_register(Register::ARG2);

            if (fd != 1) {
                throw std::runtime_error("write() only supports stdout");
            }

            for (unsigned i=0; i < count; i++) {
                std::cout << emulator.get_memory().load8(buf + i);
            }
    });

    // Exit
    emulator.register_ecall(93,[](Emulator& emulator) {
            std::cout<<" Exiting emulator" << std::endl;
            emulator.exit();
    });

    if (debug) {
        interactive_debug(emulator);
    } else {
        emulator.run();
    }
}
