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

struct DocumentCrawler {
    DocumentCrawler() {};
    DocumentCrawler(std::string path) : origin_path{path} {};
    DocumentCrawler(std::set<docmeta::DocumentMeta>* document_store, std::vector<docmeta::DocumentMeta>* repository) : doc_store{document_store}, repository{repository} {};
    DocumentCrawler(std::set<docmeta::DocumentMeta>* document_store, std::vector<docmeta::DocumentMeta>* repository, std::string path) : doc_store{document_store}, repository{repository}, origin_path{path} {}
    
    void start();
    std::vector<std::string> getDocumentPaths();
    void registerDocument(std::string path);
    
    std::string origin_path;
    std::set<docmeta::DocumentMeta> documents;

    std::set<docmeta::DocumentMeta>* doc_store;
    std::vector<docmeta::DocumentMeta>* repository;
};


#endif