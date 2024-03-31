import <vector>;
import <cstdint>;
import <typeinfo>;
import <iostream>;
import emulator;
import memory;

int main() {
    Emulator emulator(0, std::vector<uint8_t>{3,1,4,1});

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
