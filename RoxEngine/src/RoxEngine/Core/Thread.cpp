#include "Thread.h"

static thread_local std::string sThreadName = "";

std::string ThreadName() {
    return sThreadName;
}
void SetThreadName(const std::string& name) {
    sThreadName = name;
}