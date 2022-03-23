#ifndef ON_THE_FLY_BASIC_WEB_CPP
#define ON_THE_FLY_BASIC_WEB_CPP

#include <iostream>
#include "crawler.hpp"

int main() {
    WebCrawler crawler = WebCrawler("https://zelebrate.xyz");
    std::vector<std::string> docs = crawler.start();
    
    
    for(auto& doc: docs) {
        std::cout << doc << std::endl;
    }

    return 0;
}

#endif