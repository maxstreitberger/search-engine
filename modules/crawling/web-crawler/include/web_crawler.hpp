#ifndef WEB_CRAWLER_HPP
#define WEB_CRAWLER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include <regex>
#include <thread>
#include <chrono>

#include <glog/logging.h>
#include <curl/curl.h>
#include <gumbo.h>

#include "doc_meta.hpp"
#include "doc_store.hpp"

struct WebCrawler {
    WebCrawler() {};
    WebCrawler(std::string path) : origin_path{path} { extractBaseURL(&origin_path); };
    WebCrawler(std::set<docmeta::DocumentMeta>* crawler_found_pages) : pages{crawler_found_pages} {};
    WebCrawler(DocStore store, std::set<docmeta::DocumentMeta>* crawler_found_pages, std::string path) : store{store}, pages{crawler_found_pages}, origin_path{path} { extractBaseURL(&origin_path); };
    
    void start();
    std::string getHTML(std::string url);
    void extractBaseURL(std::string* url);
    void getURLs(std::string htmlDoc, std::queue<std::string>* urls);
    void search_for_links(GumboNode* node, std::queue<std::string>* urls);
    std::string removeTags(std::string htmlDoc);
    std::string cleanText(GumboNode* node);
    void registerPage(std::string url, std::string htmlDoc);
    bool checkIfURL(const std::string* url);
    bool checkIfURLWasAlreadyVisited(std::string* url);

    std::string origin_path;
    std::string base_url;
    std::string protocol;

    DocStore store;
    std::set<docmeta::DocumentMeta>* pages;
    std::vector<std::string> already_visited_pages;
};

#endif