#ifndef WEB_CRAWLER_HPP
#define WEB_CRAWLER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <curl/curl.h>
#include "gumbo/gumbo.h"

struct WebCrawler {
    WebCrawler() {};
    WebCrawler(std::string path);
    
    std::vector<std::string> start();
    std::string getHTML(std::string url);
    void extractBaseURL(std::string* url);
    void getURLs(std::string htmlDoc, std::queue<std::string>* urls);
    void search_for_links(GumboNode* node, std::queue<std::string>* urls);
    std::string removeTags(std::string htmlDoc);
    std::string cleanText(GumboNode* node);

    std::string origin_path;
    std::string base_url;
    std::string protocol;
};

#endif