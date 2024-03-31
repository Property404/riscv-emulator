import <vector>;
import <cstdint>;
import <typeinfo>;
import <iostream>;
import emulator;
import memory;
import file_util;

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        std::cerr << "Expected one argument" << std::endl;
        return 1;
    }
    const auto binary = FileUtil::get_contents(argv[1]);
    Emulator emulator(0, std::vector<uint8_t>(binary.cbegin(), binary.cend()));

    // Write
    emulator.register_ecall(64,[](const Emulator& emulator) {
            const auto fd = emulator.get_register(Register::ARG0);
            const auto buf = emulator.get_register(Register::ARG1);
            const auto count = emulator.get_register(Register::ARG2);
            std::cout<<"Write syscall: "<< fd <<" " << buf << " " << count << std::endl;
    });

    // Exit
    emulator.register_ecall(93,[](const Emulator& emulator) {
            std::cout<<" Exiting emulator" << std::endl;
            std::exit(emulator.get_register(Register::ARG0));
    });
}
