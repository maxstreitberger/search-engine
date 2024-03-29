#ifndef THREAD_QUEUE_HPP
#define THREAD_QUEUE_HPP

#include <iostream>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

template<typename T>
struct ThreadQueue {
    ThreadQueue() {};
    ThreadQueue(ThreadQueue const& other) {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue=other.data_queue;
    }
    ThreadQueue& operator=(const ThreadQueue&) = delete;
    
    void push(T new_value) {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }
    
    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this] {
            // if (flag) {
                return !data_queue.empty(); 
            // } else {
                // return true;
            // }
        });
        // if (flag) {
            value=data_queue.front();
            data_queue.pop();
        // }
    }

    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return false;
        value=data_queue.front();
        data_queue.pop();
        return true;
    }
    
    std::shared_ptr<T> try_pop() {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    
    bool empty() const {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }

    void stop_queue() {
        std::lock_guard<std::mutex> lk(mut);
        flag = false;
        data_cond.notify_all();
    }

    private:
        mutable std::mutex mut;
        std::queue<T> data_queue;
        bool flag = true;
        std::condition_variable data_cond;
};

#endif