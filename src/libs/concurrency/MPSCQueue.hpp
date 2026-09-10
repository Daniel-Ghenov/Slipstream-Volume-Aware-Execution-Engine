#ifndef MPSC_QUEUE_HPP
#define MPSC_QUEUE_HPP

#include <array>
#include <atomic>
#include <cstddef>
#include <mutex>
#include <optional>
#include <utility>


template <typename T, std::size_t Capacity>
class MPSCQueue {
    static_assert(Capacity > 0, "Capacity must be greater than zero");

private:
    static constexpr std::size_t SlotCount = Capacity + 1;

    std::array<T, SlotCount> buffer{};
    std::atomic<std::size_t> head{0};
    std::atomic<std::size_t> tail{0};
    std::mutex producerLock;

    static std::size_t next(std::size_t index) {
        return (index + 1) % SlotCount;
    }

public:
    MPSCQueue() = default;

    MPSCQueue(const MPSCQueue& other) = delete;
    MPSCQueue& operator=(const MPSCQueue& other) = delete;
    MPSCQueue(MPSCQueue&& other) = delete;
    MPSCQueue& operator=(MPSCQueue&& other) = delete;


    bool push(const T& value) {
        return emplace(value);
    }

    bool push(T&& value) {
        return emplace(std::move(value));
    }

    std::optional<T> pop() {
        std::size_t currentTail = tail.load(std::memory_order_relaxed);
        if (currentTail == head.load(std::memory_order_acquire))
            return std::nullopt;

        T value = std::move(buffer[currentTail]);
        tail.store(next(currentTail), std::memory_order_release);
        return value;
    }


    bool empty() const {
        return tail.load(std::memory_order_relaxed) == head.load(std::memory_order_acquire);
    }

private:
    template <typename U>
    bool emplace(U&& value) {
        std::lock_guard lock(producerLock);

        std::size_t currentHead = head.load(std::memory_order_relaxed);
        std::size_t nextHead = next(currentHead);
        if (nextHead == tail.load(std::memory_order_acquire))
            return false;

        buffer[currentHead] = std::forward<U>(value);
        head.store(nextHead, std::memory_order_release);
        return true;
    }
};

#endif //MPSC_QUEUE_HPP
