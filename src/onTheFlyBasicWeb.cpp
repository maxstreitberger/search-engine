#ifndef ON_THE_FLY_BASIC_WEB_CPP
#define ON_THE_FLY_BASIC_WEB_CPP

#include <iostream>
#include "crawler.hpp"
#include "indexer.hpp"
#include "ranker.hpp"

int main() {
    std::set<docmeta::DocumentMeta> page_store;
    std::vector<docmeta::DocumentMeta> repository;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    std::string specialCharsPath = INDEXING_ROOT_DIR "/documents/special.txt";
    std::string stopwordsPath = INDEXING_ROOT_DIR "/documents/stopwords.json";

    WebCrawler crawler = WebCrawler(&page_store, &repository, "https://zelebrate.xyz");
    crawler.start();

    Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
    indexer.generateIndex();

    Ranker ranking = Ranker(&page_store, &index);
    std::vector<docmeta::DocumentMeta> foundDocuments = ranking.searchFor("Lorem");

    for (auto& doc: foundDocuments) {
        std::cout << doc << std::endl;
    }

    return 0;
}

#endif