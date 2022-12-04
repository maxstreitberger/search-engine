#ifndef THREAD_HANDLER_HPP
#define THREAD_HANDLER_HPP

#include <iostream>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "thread_queue.hpp"
#include "doc_meta.hpp"

struct QueueHandler {
    QueueHandler() {};
    QueueHandler(QueueHandler const& other) = delete;
    QueueHandler& operator=(const QueueHandler&) = delete;
    
    void exit_queues() {
        crawler_store_pipeline.stop_queue();
        repository_pipeline.stop_queue();
    }
    
    ThreadQueue<docmeta::DocumentMeta> crawler_store_pipeline;
    ThreadQueue<const docmeta::DocumentMeta*> repository_pipeline;
};

#endif