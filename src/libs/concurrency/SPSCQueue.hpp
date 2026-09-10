#ifndef SPSC_QUEUE_HPP
#define SPSC_QUEUE_HPP

#include <utility>
#include <stdexcept>

template <typename T>
class SPSCQueue {
private:
    struct Node {
        T data;
        Node* next = nullptr;
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    
public:

    SPSCQueue() {
        head = tail = new Node();
        head->next = nullptr;
    }

    SPSCQueue(const SPSCQueue& other) = delete;
    SPSCQueue& operator=(const SPSCQueue& other) = delete;

    SPSCQueue(SPSCQueue&& other) {
        moveFrom(std::move(other));
    }

    SPSCQueue& operator=(SPSCQueue&& other) {
        if (this != &other) {
            free();
            moveFrom(other);
        }
        return this;
    }

    ~SPSCQueue() {
        free();
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
        Node* newNode = new Node(data, nullptr);
        tail->next = newNode;
        tail = newNode;
    }

    bool empty() const {
        return head->next == nullptr;
    }

private:
    void free() {
        while (head != nullptr) {
            Node* temp = head->next;
            delete head;
            head = temp;
        }
    }

    void moveFrom(SPSCQueue&& other) {
        head = other.head;
        tail = other.tail;
        other.head = other.tail = nullptr;
    }

};

#endif //SPSC_QUEUE_HPP
