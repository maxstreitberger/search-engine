#ifndef THREAD_MANAGER_HPP
#define THREAD_MANAGER_HPP

#include <iostream>
#include <queue>
#include <memory>
#include <atomic>
#include <thread>
#include "queue_handler.hpp"
#include "pre_computed_doc_crawler.hpp"
#include "pre_computed_doc_store.hpp"
#include "pre_computed_indexer.hpp"

struct ThreadManager {
    ThreadManager() {
        rflag.store(true);
    }

    void setAndStart(PreComputedDocumentCrawler crawler, PreComputedDocStore store, PreComputedIndexer indexer) {
        crawler_thread = std::thread(&PreComputedDocumentCrawler::start, crawler);
        store_thread = std::thread(&PreComputedDocStore::receiveDocuments, store);
        indexer_thread = std::thread(&PreComputedIndexer::generateIndex, indexer);
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