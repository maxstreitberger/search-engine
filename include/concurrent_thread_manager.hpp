// #ifndef CONCURRENT_THREAD_MANAGER_HPP
// #define CONCURRENT_THREAD_MANAGER_HPP

// #include <iostream>
// #include <queue>
// #include <memory>
// #include <atomic>
// #include <thread>
// #include "concurrent_queue_handler.hpp"

// struct ConcurrentThreadManager {
//     ConcurrentThreadManager(std::atomic<bool>* crawler_flag, std::atomic<bool>* store_flag, std::atomic<bool>* indexer_flag) {
//         qhandler = ConcurrentQueueHandler(crawler_flag, store_flag, indexer_flag);
//     }

//     template<typename Crawler, typename Store, typename Indexer>
//     void setAndStart(Crawler crawler, Store store, Indexer indexer) {
//         crawler_thread = std::thread(&Crawler::start, crawler);
//         store_thread = std::thread(&Store::receiveDocuments, store);
//         indexer_thread = std::thread(&Indexer::generateIndex, indexer);
//     }

//     void stopThreads() {
//         qhandler.exit_queues();
//         crawler_thread.join();
//         store_thread.join();
//         indexer_thread.join();
//     }

//     std::thread crawler_thread;
//     std::thread store_thread;
//     std::thread indexer_thread;
//     ConcurrentQueueHandler qhandler;
// };



// #endif