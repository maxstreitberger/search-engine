#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>
#include "parser.hpp"
#include "crawler.hpp"
#include "doc_store.hpp"
#include "ranking.hpp"

int main() {
    Crawler crawler = Crawler(SEARCHENGINE_ROOT_DIR "/dummy-text");
    crawler.start();

    Parser parser = Parser(SEARCHENGINE_ROOT_DIR "/modules/indexing/documents/special.txt");
    parser.loadCrawlerDocuments();
    std::set<docmeta::DocumentMeta>::iterator it;

    for(it = parser.documents.begin(); it != parser.documents.end(); it++) {
        parser.parse(it);
    }

    Ranking ranking = Ranking();
    std::vector<docmeta::RankingDocumentMeta> foundDocuments = ranking.searchFor("Lorem");

    for (auto& doc: foundDocuments) {
        std::cout << doc << std::endl;
    }

    return 0;
}

#endif