#ifndef ON_THE_FLY_BASIC_WEB_CPP
#define ON_THE_FLY_BASIC_WEB_CPP

#include <iostream>
#include "crawler.hpp"

int main() {
    std::set<pagemeta::PageMeta> page_store;
    std::vector<pagemeta::PageMeta> repository;

    WebCrawler crawler = WebCrawler(&page_store, &repository, "https://zelebrate.xyz");
    crawler.start();

    for (auto& page: repository) {
        std::cout << page << std::endl;
    }

    for (auto& page: page_store) {
        std::cout << page << std::endl;
    }

    return 0;
}

#endif