#ifndef DOCUMENT_CRAWLER_HPP
#define DOCUMENT_CRAWLER_HPP

#include <iostream>
#include <string_view>
#include <filesystem>
#include <queue>
#include <set>

#include <glog/logging.h>
#include <nlohmann/json.hpp>

#include "../../../../include/doc_meta.hpp"
#include "../../../../include/helpers.hpp"
#include "../../document-store/include/doc_store.hpp"

struct DocumentCrawler {
    DocumentCrawler() {};
    DocumentCrawler(std::string path) : origin_path{path} {};
    DocumentCrawler(std::set<docmeta::DocumentMeta>* crawler_found_documents) : documents{crawler_found_documents} {};
    DocumentCrawler(DocStore store, std::set<docmeta::DocumentMeta>* crawler_found_documents, std::string path) : store{store}, documents{crawler_found_documents}, origin_path{path} {};
    
    void start();
    std::vector<std::string> getDocumentPaths();
    void registerDocument(std::string path);
    
    DocStore store;
    std::string origin_path;
    std::set<docmeta::DocumentMeta>* documents;
};


#endif