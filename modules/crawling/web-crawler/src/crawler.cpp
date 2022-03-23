#ifndef WEB_CRAWLER_CPP
#define WEB_CRAWLER_CPP

#include "crawler.hpp"

WebCrawler::WebCrawler(std::string path) {
    origin_path = path;
    extractBaseURL(&origin_path);
}

std::vector<std::string> WebCrawler::start() {
    std::queue<std::string> urls;
    urls.push(origin_path);
    std::vector<std::string> foundDocs;

    while (!urls.empty()) {
        std::string htmlDoc = getHTML(urls.front());
        foundDocs.push_back(htmlDoc);
        getURLs(htmlDoc, &urls);
        urls.pop();
    }
    return foundDocs;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string WebCrawler::getHTML(std::string url) {
    CURL * curl;
    CURLcode res;
    std::string buffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.data());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return buffer;
}

void WebCrawler::search_for_links(GumboNode* node, std::queue<std::string>* urls) {
    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }

    GumboAttribute* href;
    if (node->v.element.tag == GUMBO_TAG_A && (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
        if (href->value[0] == '/' && href->value[1] == '/') {
            std::string url = href->value;
            urls->push(protocol + url.erase(0, 2));
        } else if (href->value[0] == '/') {
            urls->push(base_url + href->value);
        } else if (href->value[0] != '#') {
            urls->push(href->value);
        }
    }

    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        search_for_links(static_cast<GumboNode*>(children->data[i]), urls);
    }
}

void WebCrawler::extractBaseURL(std::string* url) {
    std::size_t found = url->find('/');
    if (found != std::string::npos) {
        protocol = url->substr(0, found + 2);
        base_url = url->substr(found + 2);
    }

    found = base_url.find('/');
    if (found != std::string::npos) {
        base_url = base_url.substr(0, found);
    }
    base_url = protocol + base_url;
}

void WebCrawler::getURLs(std::string htmlDoc, std::queue<std::string>* urls) {
    GumboOutput* output = gumbo_parse(htmlDoc.c_str());
    
    search_for_links(output->root, urls);
    
    gumbo_destroy_output(&kGumboDefaultOptions, output);
}

#endif