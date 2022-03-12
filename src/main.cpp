#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>
#include "indexer.hpp"
#include "crawler.hpp"
#include "doc_store.hpp"
#include "ranking.hpp"

int main() {
    Crawler crawler = Crawler(SEARCHENGINE_ROOT_DIR "/dummy-text");
    crawler.start();

    Indexer indexer = Indexer(SEARCHENGINE_ROOT_DIR "/modules/indexing/documents/special.txt");
    indexer.loadCrawlerDocuments();
    std::set<docmeta::DocumentMeta>::iterator it;

    for(it = indexer.documents.begin(); it != indexer.documents.end(); it++) {
        indexer.parse(it);
    }

    Ranking ranking = Ranking();
    std::vector<docmeta::RankingDocumentMeta> foundDocuments = ranking.searchFor("Lorem");

    for (auto& doc: foundDocuments) {
        std::cout << doc << std::endl;
    }

    return 0;
}

#endif