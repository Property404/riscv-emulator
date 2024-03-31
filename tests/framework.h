import <typeinfo>;
import <new>;
import <cstdlib>;
import <iostream>;
import test_framework;

#define TEST_CASE(name, ...) static TestCase _puppet_##name\
    (__extract_file_name(__FILE__), #name, [](TestCase* __self) __VA_ARGS__);

#define REQUIRE_EQUALS(value1, value2) do {\
    __self->assert_equals<std::remove_reference_t<decltype(value1)>, \
    std::remove_reference_t<decltype(value2)>>(""#value1, ""#value2, value1, value2);}while(0)
#define REQUIRE(condition) {__self->assert("Assert failed: '" #condition, condition);}
#define REQUIRE_NOT(condition) REQUIRE(!(condition))
