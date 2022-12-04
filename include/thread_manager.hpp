#ifndef THREAD_MANAGER_HPP
#define THREAD_MANAGER_HPP

#include <iostream>
#include <queue>
#include <memory>
#include <atomic>
#include <thread>
#include "queue_handler.hpp"

struct ThreadManager {
    ThreadManager() {
        rflag.store(true);
    }

    template<typename Crawler, typename Store, typename Indexer>
    void setAndStart(Crawler crawler, Store store, Indexer indexer) {
        crawler_thread = std::thread(&Crawler::start, crawler);
        store_thread = std::thread(&Store::receiveDocuments, store);
        indexer_thread = std::thread(&Indexer::generateIndex, indexer);
    }

    void stopThreads() {
        rflag.store(false);
        qhandler.exit_queues();
        crawler_thread.join();
        store_thread.join();
        indexer_thread.join();
    }

    std::thread crawler_thread;
    std::thread store_thread;
    std::thread indexer_thread;
    QueueHandler qhandler;
    std::atomic<bool> rflag;
};



#endif