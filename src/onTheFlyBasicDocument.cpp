#ifndef ON_THE_FLY_BASIC_DOCUMENT_CPP
#define ON_THE_FLY_BASIC_DOCUMENT_CPP

#include <iostream>
#include "indexer.hpp"
#include "crawler.hpp"
#include "doc_store.hpp"
#include "ranker.hpp"
#include <nlohmann/json.hpp>

int main() {
    std::set<docmeta::DocumentMeta> document_store;
    std::vector<docmeta::DocumentMeta> repository;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;
    
    std::string specialCharsPath = INDEXING_ROOT_DIR "/documents/special.txt";
    std::string stopwordsPath = INDEXING_ROOT_DIR "/documents/stopwords.json";

    Crawler crawler = Crawler(&document_store, &repository, SEARCHENGINE_ROOT_DIR "/dummy-text");
    crawler.start();

    Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
    indexer.generateIndex();
    
    Ranker ranking = Ranker(&document_store, &index);
    std::vector<docmeta::DocumentMeta> foundDocuments = ranking.searchFor("Lorem");

    for (auto& doc: foundDocuments) {
        std::cout << doc << std::endl;
    }

    return 0;
}

#endif