#ifndef DOCUMENT_CRAWLER_HPP
#define DOCUMENT_CRAWLER_HPP

#include <iostream>
#include <string_view>
#include <filesystem>
#include <queue>
#include <set>
#include <thread>
#include <memory>
#include <atomic>

#include "../../../../include/doc_meta.hpp"
#include "../../../../include/helpers.hpp"
#include "../../../../include/concurrent_thread_queue.hpp"

struct ConcurrentDocumentCrawler {
    ConcurrentDocumentCrawler() {};
    ConcurrentDocumentCrawler(std::string path) : origin_path{path} {};
    ConcurrentDocumentCrawler(ConcurrentThreadQueue<docmeta::DocumentMeta>* pipeline) : crawler_store_pipeline{pipeline} {};
    ConcurrentDocumentCrawler(ConcurrentThreadQueue<docmeta::DocumentMeta>* pipeline, std::string path, std::atomic<bool>* crawler_flag) : crawler_store_pipeline{pipeline}, origin_path{path}, crawler_flag{crawler_flag} {};
    
    void start();
    void registerDocument(std::string path);

    std::string origin_path;
    ConcurrentThreadQueue<docmeta::DocumentMeta>* crawler_store_pipeline;
    std::atomic<bool>* crawler_flag;
};


#endif