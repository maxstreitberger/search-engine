#ifndef CONCURRENT_THREAD_QUEUE_HPP
#define CONCURRENT_THREAD_QUEUE_HPP

#include <iostream>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <optional>

template<typename T>
struct ConcurrentThreadQueue {
    ConcurrentThreadQueue() {};    
    ConcurrentThreadQueue(ConcurrentThreadQueue const& other) {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue=other.data_queue;
        got_data=other.got_data;
        flag = other.flag;
    }
    ConcurrentThreadQueue& operator=(const ConcurrentThreadQueue&) = delete;
    
    void push(T new_value) {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        //data_cond.notify_one();
    }
    
    // void wait_and_pop(T& value) {
    //     std::unique_lock<std::mutex> lk(mut);
    //     // if (flag->load()) {
    //     //     data_cond.wait(lk,[this] { return !data_queue.empty(); });
    //     //     value=data_queue.front();
    //     //     data_queue.pop();
    //     // } else {
    //     //     value=data_queue.front();
    //     //     data_queue.pop();
    //     // }
    //     data_cond.wait(lk,[this] {
    //         //std::cout << waiter << " WAITING" << std::endl;
    //         if (!data_queue.empty()) {
    //             got_data = true;
    //             return true;
    //         } else {
    //             return !flag->load();
    //         }
    //     });
    //     if (got_data) {
    //         value=data_queue.front();
    //         data_queue.pop();
    //         got_data = false;
    //     }
    // }
    std::pair<int, std::optional<T>> pop() {
        std::unique_lock<std::mutex> lk(mut);
        if (!data_queue.empty()) {
            T data = data_queue.front();
            data_queue.pop();
            lk.unlock();
            return std::pair(1, data);                  // return new item
        } else {
            if (flag->load()) {
                lk.unlock();
                return std::pair(0, std::nullopt);      // keep waiting for new item
            } else {
                lk.unlock();
                return std::pair(-1, std::nullopt);     // exit queue
            }
        } 
    }

    void stop_queue() {
        flag->store(false);
        data_cond.notify_all();
    }

    std::atomic<bool>* flag;

    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
    bool got_data = false;
};

#endif