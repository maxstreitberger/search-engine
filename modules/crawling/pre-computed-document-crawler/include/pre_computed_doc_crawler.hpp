#ifndef DOCUMENT_CRAWLER_HPP
#define DOCUMENT_CRAWLER_HPP

#include <iostream>
#include <string_view>
#include <filesystem>
#include <queue>
#include <set>
#include <thread>

#include <glog/logging.h>
#include <nlohmann/json.hpp>

#include "../../../../include/doc_meta.hpp"
#include "../../../../include/helpers.hpp"
#include "../../../../include/thread_queue.hpp"

struct PreComputedDocumentCrawler {
    PreComputedDocumentCrawler() {};
    PreComputedDocumentCrawler(std::string path) : origin_path{path} {};
    PreComputedDocumentCrawler(std::set<docmeta::DocumentMeta>* crawler_found_documents) : documents{crawler_found_documents} {};
    PreComputedDocumentCrawler(ThreadQueue<docmeta::DocumentMeta>* pipeline, std::string path) : crawler_pipeline{pipeline}, origin_path{path} {};
    
    void start();
    std::vector<std::string> getDocumentPaths();
    void registerDocument(std::string path);

    ThreadQueue<docmeta::DocumentMeta>* crawler_pipeline;

    std::string origin_path;
    std::set<docmeta::DocumentMeta>* documents;
};


#endif