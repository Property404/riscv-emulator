#pragma once
#include <stdbool.h>

#define assert(condition) __assert(condition, ""#condition)

void __assert(bool condition, const char* msg);
