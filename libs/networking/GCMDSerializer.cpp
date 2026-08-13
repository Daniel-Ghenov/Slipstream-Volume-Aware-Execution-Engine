#include "GCMDSerializer.h"
#include "GCMDTypes.h"
#include <chrono>

void GCMDSerializer::sendHeartBeat() {
    HeartbeatBody body{getNow()};
}

uint64_t GCMDSerializer::getNow() {
    return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count());
}
