#ifndef CRAWLER_HPP
#define CRAWLER_HPP

#include <iostream>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <queue>
#include <set>
#include <nlohmann/json.hpp>

#include "../../../include/doc_meta.hpp"

struct Crawler {
    Crawler() {};
    Crawler(std::string path) : origin_path{path} {};
    
    void start();
    std::vector<std::string> getDocumentPaths();
    std::string getDocumentContents(const std::string path);
    void registerDocument(const std::filesystem::path path);
    
    void pushDocumentsToStore(const std::set<docmeta::DocumentMeta> documents);
    std::string loadText(const std::string path);

    std::string origin_path;
    std::set<docmeta::DocumentMeta> documents;
};


#endif