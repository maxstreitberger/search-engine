#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>
#include "parser.hpp"
#include "crawler.hpp"
#include "doc_store.hpp"

int main() {
    Crawler crawler = Crawler(SEARCHENGINE_ROOT_DIR "/dummy-text");
    crawler.start();

    // NEXT: Connect Crawler with Indexer

    // Parser parser = Parser(SEARCHENGINE_ROOT_DIR "/modules/indexing/documents/special.txt");
    // parser.registerDocument("dummy-text/demo.txt");
    // parser.registerDocument("dummy-text/demo2.txt");

    // std::set<DocumentMeta>::iterator it;

    // for(it = parser.documents.begin(); it != parser.documents.end(); it++) {
    //     parser.parse(it);
    // }

    return 0;
}

#endif