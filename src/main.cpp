#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>
#include "indexer.hpp"
#include "crawler.hpp"
#include "doc_store.hpp"
#include "ranker.hpp"

int main() {
    Crawler crawler = Crawler(SEARCHENGINE_ROOT_DIR "/dummy-text");
    crawler.start();

    std::string specialCharsPath = INDEXING_ROOT_DIR "/documents/special.txt";
    std::string stopwordsPath = INDEXING_ROOT_DIR "/documents/stopwords.json";

    Indexer indexer = Indexer(specialCharsPath, stopwordsPath, "repository.json", "index.json");
    indexer.generateIndex();
    
    Ranker ranking = Ranker("index.json", "store.json");
    std::vector<docmeta::DocumentMeta> foundDocuments = ranking.searchFor("Lorem");

    for (auto& doc: foundDocuments) {
        std::cout << doc << std::endl;
    }

    return 0;
}

#endif