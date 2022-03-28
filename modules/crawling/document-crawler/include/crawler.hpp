#ifndef DOCUMENT_CRAWLER_HPP
#define DOCUMENT_CRAWLER_HPP

#include <iostream>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <queue>
#include <set>

#include <glog/logging.h>
#include <nlohmann/json.hpp>

#include "../../../../include/doc_meta.hpp"

struct Crawler {
    Crawler() {};
    Crawler(std::string path) : origin_path{path} {};
    Crawler(std::set<docmeta::DocumentMeta>* document_store, std::vector<docmeta::DocumentMeta>* repository) : doc_store{document_store}, repository{repository} {};
    Crawler(std::set<docmeta::DocumentMeta>* document_store, std::vector<docmeta::DocumentMeta>* repository, std::string path) : doc_store{document_store}, repository{repository}, origin_path{path} {}
    
    void start();
    std::vector<std::string> getDocumentPaths();
    std::string getDocumentContents(const std::string path);
    void registerDocument(const std::filesystem::path path);
    
    std::string loadText(const std::string path);

    std::string origin_path;
    std::set<docmeta::DocumentMeta> documents;

    std::set<docmeta::DocumentMeta>* doc_store;
    std::vector<docmeta::DocumentMeta>* repository;
};


#endif