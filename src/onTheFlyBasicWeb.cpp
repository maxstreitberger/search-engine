#ifndef ON_THE_FLY_BASIC_WEB_CPP
#define ON_THE_FLY_BASIC_WEB_CPP

#include <iostream>
#include "crawler.hpp"

int main() {
    WebCrawler crawler = WebCrawler("https://zelebrate.xyz");
    crawler.start();

    return 0;
}

#endif