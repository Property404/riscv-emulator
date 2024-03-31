export module test_harness;

import <iostream>;
import <vector>;
import <typeinfo>;
import <functional>;

using namespace std;


export class TestCase final {
    using Action = std::function<void(TestCase*)>;

    // The test procedure itself.
    Action action;

    const char* file_name;
    const char* name;

    int assertions_passed;

    template<typename T,typename U>
    void assert_equals_inner(const char* type1, const char* type2, T value1, U value2){
        if (value1 != value2) {
            std::cerr
                << "Assert failed: "
                << value1 << "(" << type1 << ")" << " != "
                << value2 << "(" << type2 << ")" << std::endl;
            std::exit(1);
        } else {
            this->assertions_passed++;
        }
    }

 public:
    TestCase(const char* file_name, const char* name, Action action);

    /// Run the test case.
    void run() {
        std::cout<< " Running test <" << this->file_name << "::" << this->name << ">\n";
        (this->action)(this);
    }

    /// Run all test cases
    static void run_all();

    template<typename T,typename U>
    void assert_equals(const char* type1, const char* type2, T value1, U value2){
        this->assert_equals_inner<
                std::remove_reference_t<T>,
                std::remove_reference_t<U>
            >
            (type1, type2, value1, value2);
    }


    void assert(const char* msg, bool condition){
        if (condition) {
            this->assertions_passed++;
        } else {
            std::cerr << "Assert failed: " << msg  << std::endl;
            std::exit(1);
        }
    }

    void print_stats() {
        std::cout<< this->assertions_passed << " assertions passed" << std::endl;
    }
};

static TestCase* cases[20];
static size_t cases_len = 0;

/// Define the test case.
TestCase::TestCase(const char* file_name, const char* name, Action action) {
        this->file_name = file_name;
        this->name = name;

        this->action = action;

        cases[cases_len] = this;
        cases_len++;
}

void TestCase::run_all() {
    for(size_t i=0;i<cases_len;i++) {
        cases[i]->run();
        cases[i]->print_stats();
    }
}

// https://stackoverflow.com/questions/31050113/
// how-to-extract-the-source-filename-without-path-and-suffix-at-compile-time
export constexpr const char* __extract_file_name(const char* path) {
    const char* file = path;
    while (*path) {
        if (*path++ == '/') {
            file = path;
        }
    }
    return file;
}
