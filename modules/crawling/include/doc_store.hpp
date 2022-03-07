#ifndef DOC_STORE_HPP
#define DOC_STORE_HPP

#include "crawler.hpp"

struct DocStore {
    DocStore(std::string path) : store_path{path} {};

    void loadStore();
    void checkForNewFiles();
    void updateStore(const docmeta::CrawlerDocMeta doc);
    void saveStore();
    void saveForIndexer(const std::vector<docmeta::CrawlerDocMeta> docs);

    std::string store_path;
    std::set<docmeta::CrawlerDocMeta> docsInStore;
};

#endif