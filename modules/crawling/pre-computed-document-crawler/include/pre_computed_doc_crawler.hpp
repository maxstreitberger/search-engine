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
    PreComputedDocumentCrawler(ThreadQueue<docmeta::DocumentMeta>* pipeline) : crawler_store_pipeline{pipeline} {};
    PreComputedDocumentCrawler(ThreadQueue<docmeta::DocumentMeta>* pipeline, std::atomic<bool>* keepThreadRunning, std::string path) : crawler_store_pipeline{pipeline}, keepRunning{keepThreadRunning}, origin_path{path} {};
    
    void start();
    std::vector<std::string> getDocumentPaths();
    void registerDocument(std::string path);

    std::string origin_path;
    ThreadQueue<docmeta::DocumentMeta>* crawler_store_pipeline;
    std::atomic<bool>* keepRunning;
};


#endif