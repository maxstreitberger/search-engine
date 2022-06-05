#ifndef CONCURRENT_DOC_STORE_HPP
#define CONCURRENT_DOC_STORE_HPP

#include <iostream>
#include <set>
#include <queue>
#include <vector>
#include <algorithm>
#include <atomic>

#include "../../../../include/doc_meta.hpp"
#include "../../../../include/concurrent_thread_queue.hpp"

typedef enum { NEW, UPDATED, NONE } DocumentStatus;

struct ConcurrentDocStore {
    ConcurrentDocStore() {};
    ConcurrentDocStore(ConcurrentThreadQueue<docmeta::DocumentMeta>* crawler_store_pipeline, ConcurrentThreadQueue<const docmeta::DocumentMeta*>* repository_pipeline, std::set<docmeta::DocumentMeta>* document_store, std::atomic<bool>* store_flag) : crawler_store_pipeline{crawler_store_pipeline}, repository_pipeline{repository_pipeline}, doc_store{document_store}, store_flag{store_flag} {};

    void receiveDocuments();
    void process(docmeta::DocumentMeta doc);
    void addToStore(std::set<docmeta::DocumentMeta>* currentStore, docmeta::DocumentMeta* doc);
    void updateStore(std::set<docmeta::DocumentMeta>* currentStore, docmeta::DocumentMeta* doc);
    DocumentStatus checkForChanges(std::set<docmeta::DocumentMeta>* currentStore, docmeta::DocumentMeta* doc);
    void pushToRepository(std::set<docmeta::DocumentMeta>* currentStore, ConcurrentThreadQueue<const docmeta::DocumentMeta*>* repository_pipeline, docmeta::DocumentMeta doc);

    ConcurrentThreadQueue<docmeta::DocumentMeta>* crawler_store_pipeline;
    ConcurrentThreadQueue<const docmeta::DocumentMeta*>* repository_pipeline;

    std::set<docmeta::DocumentMeta>* crawler_docs;
    std::set<docmeta::DocumentMeta>* doc_store;
    std::atomic<bool>* store_flag;
};

#endif