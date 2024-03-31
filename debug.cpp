export module debug;

import <iostream>;
import <string>;
import <string_view>;
import <vector>;
import <ranges>;
import <optional>;

import emulator;

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::hex;

static std::optional<size_t> register_from_string(const string& reg);
static std::vector<string> split_string(const string& str);

export void interactive_debug(Emulator& emulator) {
    string input;

    while (true) {
        cout<<hex << "0x" << emulator.get_ip() << ": 0x" << emulator.current_instruction() << endl;
        while(true) {
            cout << ">>>";
            std::getline(cin, input);
            const auto args = split_string(input);
            if (args.size() == 0 || args.at(0) == "s") {
                emulator.step();
                break;
            } else if (args.at(0) == "reg") {
                if (args.size() < 2) {
                    cout << "Expected at least one argument for reg" << endl;
                    continue;
                }
                for (unsigned i=1; i < args.size();i++) {
                    const auto reg = register_from_string(args.at(i));
                    if (reg.has_value()) {
                        cout << args.at(i) << ": " << emulator.get_register(reg.value()) << endl;
                    } else {
                        cout << "Unrecognized register name: " << args.at(i) << endl;
                    }
                }
            } else {
                cout << "Invalid debug command" << endl;
            }
        }
    }
}

static std::vector<string> split_string(const string& str) {
    std::vector<string> res{};

    if (str == "") {
        return res;
    }

    size_t last = 0;
    size_t next = 0;

    while ((next = str.find(' ', last)) != string::npos) {
        res.push_back(str.substr(last, next-last));
        last = next + 1;
    }

    res.push_back(str.substr(last));

    return res;
}

static std::optional<size_t> register_from_string(const string& reg) {
    const char* table[][2] = {
        {"x0", "zero"},
        {"x1", "ra"},
        {"x2", "sp"},
        {"x3", "gp"},
        {"x4", "tp"},
        {"x5", "t0"},
        {"x6", "t1"},
        {"x7", "t2"},
        {"x8", "s0"},
        {"x9", "s1"},
        {"x10", "a0"},
        {"x11", "a1"},
        {"x12", "a2"},
        {"x12", "a2"},
    };

    for (unsigned i=0;i<sizeof(table)/sizeof(table[0]);i++) {
        for(const auto alias : table[i]) {
            if (reg == alias) {
                return i;
            }
        }
    }

    return {};
}