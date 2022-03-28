#ifndef WEB_CRAWLER_HPP
#define WEB_CRAWLER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>

#include <curl/curl.h>

#include "gumbo/gumbo.h"
#include "../../../../include/doc_meta.hpp"

struct WebCrawler {
    WebCrawler() {};
    WebCrawler(std::string path);
    WebCrawler(std::set<docmeta::DocumentMeta>* store, std::vector<docmeta::DocumentMeta>* repo);
    WebCrawler(std::set<docmeta::DocumentMeta>* store, std::vector<docmeta::DocumentMeta>* repo, std::string path);
    
    void start();
    std::string getHTML(std::string url);
    void extractBaseURL(std::string* url);
    void getURLs(std::string htmlDoc, std::queue<std::string>* urls);
    void search_for_links(GumboNode* node, std::queue<std::string>* urls);
    std::string removeTags(std::string htmlDoc);
    std::string cleanText(GumboNode* node);

    void registerPage(std::string url, std::string htmlDoc);

    std::string origin_path;
    std::string base_url;
    std::string protocol;

    std::set<docmeta::DocumentMeta> pages;
    std::set<docmeta::DocumentMeta>* page_store;
    std::vector<docmeta::DocumentMeta>* repository;
};

#endif