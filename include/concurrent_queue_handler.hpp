// #ifndef CONCURRENT_THREAD_HANDLER_HPP
// #define CONCURRENT_THREAD_HANDLER_HPP

// #include <iostream>
// #include <queue>
// #include <memory>
// #include <mutex>
// #include <condition_variable>
// #include "concurrent_thread_queue.hpp"
// #include "doc_meta.hpp"
// #include <atomic>

// struct ConcurrentQueueHandler {
//     ConcurrentQueueHandler(std::atomic<bool>* crawler_flag, std::atomic<bool>* store_flag, std::atomic<bool>* indexer_flag) {
//         crawler_store_pipeline = ConcurrentThreadQueue(crawler_flag);
//         repository_pipeline = ConcurrentThreadQueue(store_flag);
//     }

//     ConcurrentQueueHandler(ConcurrentQueueHandler const& other) = delete;
//     ConcurrentQueueHandler& operator=(const ConcurrentQueueHandler&) = delete;
    
//     void exit_queues() {
//         crawler_store_pipeline.stop_queue();
//         repository_pipeline.stop_queue();
//     }
    
//     ConcurrentThreadQueue<docmeta::DocumentMeta> crawler_store_pipeline;
//     ConcurrentThreadQueue<const docmeta::DocumentMeta*> repository_pipeline;
// };

// #endif