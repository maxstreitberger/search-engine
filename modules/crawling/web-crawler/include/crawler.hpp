#ifndef WEB_CRAWLER_HPP
#define WEB_CRAWLER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>

#include <curl/curl.h>

#include "gumbo/gumbo.h"
#include "../../../../include/page_meta.hpp"

struct WebCrawler {
    WebCrawler() {};
    WebCrawler(std::string path);
    WebCrawler(std::set<pagemeta::PageMeta>* page_store, std::vector<pagemeta::PageMeta>* repository) : page_store{page_store}, repository{repository} {};
    WebCrawler(std::set<pagemeta::PageMeta>* page_store, std::vector<pagemeta::PageMeta>* repository, std::string path) : page_store{page_store}, repository{repository}, origin_path{path} {}
    
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

    std::set<pagemeta::PageMeta> pages;
    std::set<pagemeta::PageMeta>* page_store;
    std::vector<pagemeta::PageMeta>* repository;
};

#endif