#ifndef DOC_STORE_CPP
#define DOC_STORE_CPP

#include "doc_store.hpp"

/*
    DocStore reads crawler-found-docs.txt file, which contains all the files the crawler found. It goes through it line by line (documnet by document) and checks if there are any new or updated documents. It then updates or adds these documents to the repository where the indexer can pick them up. In addition, these documents get pushed into the permanent store.
    
    The crawler can't add new docs to the crawler-found-docs.txt file while the DocStore reads it => Either not crawl at all during this time (possible for document crawler) or find other solution (needed for web crawler)
*/

void DocStore::loadStore() {
    std::ifstream ifs("store.json");
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    nlohmann::json crawler_docs;

    try {
        crawler_docs = nlohmann::json::parse(content);
        docsInStore = crawler_docs.get<std::set<docmeta::CrawlerDocMeta>>();
    }
    catch (nlohmann::json::parse_error& ex) {
        std::cerr << "parse error at byte " << ex.byte << std::endl;
    }
}

void DocStore::checkForNewFiles() {
    std::ifstream ifs("crawler-found-docs.json");
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    nlohmann::json jsonDocs;
    std::set<docmeta::CrawlerDocMeta> crawlerDocuments;

    std::vector<docmeta::CrawlerDocMeta> newOrUpdatedDocs;

    try {
        jsonDocs = nlohmann::json::parse(content);
        crawlerDocuments = jsonDocs.get<std::set<docmeta::CrawlerDocMeta>>();
    }
    catch (nlohmann::json::parse_error& ex) {
        std::cerr << "parse error at byte " << ex.byte << std::endl;
    }

    for (auto& crawler_doc: crawlerDocuments) {
        auto it = docsInStore.find(crawler_doc);
        if (it != docsInStore.end()) {
            if (it->content != crawler_doc.content) {
                std::cout << "Update doc_" << it->id << " content to: " << crawler_doc.content << std::endl;
                docsInStore.erase(it);
            } else {
                std::cout << "Is still the same" << std::endl;
                continue;
            }
        }
        docsInStore.insert(crawler_doc);
        newOrUpdatedDocs.push_back(crawler_doc);
    }
    
    saveForIndexer(newOrUpdatedDocs);
    saveStore();
}

void DocStore::saveStore() {
    std::ofstream file("store.json");
    nlohmann::json doc_json = docsInStore;
    file << std::setw(4) << doc_json;
    file.close();
}

void DocStore::saveForIndexer(const std::vector<docmeta::CrawlerDocMeta> docs) {
    if (!docs.empty()) {
        std::ofstream file("repository.json");
        nlohmann::json doc_json = docs;
        file << std::setw(4) << doc_json;
        file.close();
    }
}

#endif