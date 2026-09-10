#ifndef MPSC_QUEUE_HPP
#define MPSC_QUEUE_HPP

#include <atomic>
#include <mutex>
#include <stdexcept>


template <typename T>
class MPSCQueue {
private:
    struct Node {
        Node* next = nullptr;
        T data;
    };
    std::mutex headM;
    Node* head;
    Node* tail;

public:
    MPSCQueue() {
        Node* stub = new Node();
        head = tail = stub;
    }

    MPSCQueue(const MPSCQueue& other) = delete;
    MPSCQueue& operator=(const MPSCQueue& other) = delete;
    MPSCQueue(MPSCQueue&& other) = delete;
    MPSCQueue& operator=(MPSCQueue&& other) = delete;

    ~MPSCQueue() {
        Node* node = tail;
        while (node != nullptr) {
            Node* next = node->next.load(std::memory_order_relaxed);
            delete node;
            node = next;
        }
    }

    T pop() {
        if (empty())
            throw std::out_of_range("No elements inside of the queue");
        
        Node* next = head->next;
        T temp = next->data;
        delete head;
        head = next;

        return temp;
    }

    void push(const T& data) {
        headM.lock();
        Node* newNode = new Node(data, nullptr);
        tail->next = newNode;
        tail = newNode;
        headM.unlock();
    }

    bool empty() const {
        return head->next == nullptr;
    }
    
};

#endif //MPSC_QUEUE_HPP
