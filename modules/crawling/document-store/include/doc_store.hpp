#ifndef DOC_STORE_HPP
#define DOC_STORE_HPP

#include <iostream>
#include <set>
#include <glog/logging.h>

#include "doc_meta.hpp"

struct DocStore {
    DocStore() {};
    DocStore(std::set<docmeta::DocumentMeta>* crawler_docs, std::set<docmeta::DocumentMeta>* document_store, std::vector<docmeta::DocumentMeta>* repository) : crawler_docs{crawler_docs}, doc_store{document_store}, repository{repository} {};

    void processDocuments();
    void addNewDocumentsToStore(std::set<docmeta::DocumentMeta>* currentStore, std::vector<docmeta::DocumentMeta> docs);
    void updateDocumentsInStore(std::set<docmeta::DocumentMeta>* currentStore, std::vector<docmeta::DocumentMeta> docs);
    std::pair<std::vector<docmeta::DocumentMeta>, std::vector<docmeta::DocumentMeta>> checkForChanges(std::set<docmeta::DocumentMeta>* currentStore, std::set<docmeta::DocumentMeta>* docs);
    
    std::set<docmeta::DocumentMeta>* crawler_docs;
    std::set<docmeta::DocumentMeta>* doc_store;
    std::vector<docmeta::DocumentMeta>* repository;
};

#endif