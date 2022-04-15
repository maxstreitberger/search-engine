#ifndef PRE_COMPUTED_DOC_STORE_HPP
#define PRE_COMPUTED_DOC_STORE_HPP

#include <iostream>
#include <set>
#include <glog/logging.h>
#include <nlohmann/json.hpp>
#include <queue>
#include <vector>

#include "../../../../include/doc_meta.hpp"
#include "../../../../include/thread_queue.hpp"

struct PreComputedDocStore {
    PreComputedDocStore() {};
    PreComputedDocStore(ThreadQueue<docmeta::DocumentMeta>* pipeline, std::set<docmeta::DocumentMeta>* document_store, std::vector<docmeta::DocumentMeta>* repository) : crawler_pipeline{pipeline}, doc_store{document_store}, repository{repository} {};

    void processDocuments();
    void addNewDocumentsToStore(std::set<docmeta::DocumentMeta>* currentStore, std::vector<docmeta::DocumentMeta> docs);
    void updateDocumentsInStore(std::set<docmeta::DocumentMeta>* currentStore, std::vector<docmeta::DocumentMeta> docs);
    std::pair<std::vector<docmeta::DocumentMeta>, std::vector<docmeta::DocumentMeta>> checkForChanges(std::set<docmeta::DocumentMeta>* currentStore, std::set<docmeta::DocumentMeta>* docs);
    
    ThreadQueue<docmeta::DocumentMeta>* crawler_pipeline;

    std::set<docmeta::DocumentMeta>* crawler_docs;
    std::set<docmeta::DocumentMeta>* doc_store;
    std::vector<docmeta::DocumentMeta>* repository;
};

#endif