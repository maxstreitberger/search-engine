#ifndef PRE_COMPUTED_DOC_STORE_HPP
#define PRE_COMPUTED_DOC_STORE_HPP

#include <iostream>
#include <set>
#include <glog/logging.h>
#include <nlohmann/json.hpp>
#include <queue>
#include <vector>
#include <atomic>

#include "../../../../include/doc_meta.hpp"
#include "../../../../include/thread_queue.hpp"

typedef enum { NEW, UPDATED, NONE } DocumentStatus;

struct PreComputedDocStore {
    PreComputedDocStore() {};
    PreComputedDocStore(ThreadQueue<docmeta::DocumentMeta>* crawler_store_pipeline, ThreadQueue<const docmeta::DocumentMeta*>* repository_pipeline, std::atomic<bool>* keepThreadRunning, std::set<docmeta::DocumentMeta>* document_store) : crawler_store_pipeline{crawler_store_pipeline}, repository_pipeline{repository_pipeline}, keepRunning{keepThreadRunning}, doc_store{document_store} {};

    void receiveDocuments();
    void process(docmeta::DocumentMeta doc);
    void addToStore(std::set<docmeta::DocumentMeta>* currentStore, docmeta::DocumentMeta* doc);
    void updateStore(std::set<docmeta::DocumentMeta>* currentStore, docmeta::DocumentMeta* doc);
    DocumentStatus checkForChanges(std::set<docmeta::DocumentMeta>* currentStore, docmeta::DocumentMeta* doc);
    void pushToRepository(std::set<docmeta::DocumentMeta>* currentStore, ThreadQueue<const docmeta::DocumentMeta*>* repository_pipeline, docmeta::DocumentMeta doc);

    std::atomic<bool>* keepRunning;

    ThreadQueue<docmeta::DocumentMeta>* crawler_store_pipeline;
    ThreadQueue<const docmeta::DocumentMeta*>* repository_pipeline;

    std::set<docmeta::DocumentMeta>* crawler_docs;
    std::set<docmeta::DocumentMeta>* doc_store;
};

#endif