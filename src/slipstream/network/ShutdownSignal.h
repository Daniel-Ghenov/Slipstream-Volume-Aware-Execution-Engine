#ifndef SHUTDOWN_SIGNAL_H
#define SHUTDOWN_SIGNAL_H

#include <atomic>
#include <latch>

class ShutdownSignal {
public:
    void trigger() {
        if (!triggered.exchange(true, std::memory_order_acq_rel)) {
            latch.count_down();
        }
    }

    void wait() {
        latch.wait();
    }

private:
    std::latch latch{1};
    std::atomic<bool> triggered{false};
};

#endif // SHUTDOWN_SIGNAL_H
