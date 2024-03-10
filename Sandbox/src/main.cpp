#include <iostream>
#include <RoxEngine/Core/Logger.h>
#include <RoxEngine/Core/Thread.h>
#include <thread>

void threada() {
    SetThreadName("Thread A");
    CORE_TRACE("");
    CORE_INFO("");
    CORE_WARN("");
    CORE_ERROR("");
    CORE_CRITICAL("");
}
void threadb() {
    SetThreadName("Thread B");
    TRACE("");
    INFO("");
    WARN("");
    ERROR("");
    CRITICAL("");
}

int main(){
    SetThreadName("GameThread");
    CORE_TRACE("Hello from core logger");
    TRACE("Hello from client logger");

    std::thread a(threada);
    std::thread b(threadb);

    while(true) {
        if(a.joinable() && b.joinable()) {
            break;
        }
    }
    a.join();
    b.join();
}