#ifndef DOC_STORE_HPP
#define DOC_STORE_HPP

#include "crawler.hpp"

struct DocStore {
    DocStore() {};
    DocStore(std::string storePath, std::string crawledDocsPath, std::string repoPath) : store_path{storePath}, crawled_docs_path{crawledDocsPath}, repo_path{repoPath} {};

    void processDocuments();
    std::set<docmeta::CrawlerDocMeta> loadDocuments(std::string path);
    void addNewDocumentsToStore(std::set<docmeta::CrawlerDocMeta>* currentStore, std::vector<docmeta::CrawlerDocMeta> docs);
    void updateDocumentsInStore(std::set<docmeta::CrawlerDocMeta>* currentStore, std::vector<docmeta::CrawlerDocMeta> docs);
    std::pair<std::vector<docmeta::CrawlerDocMeta>, std::vector<docmeta::CrawlerDocMeta>> checkForChanges(std::set<docmeta::CrawlerDocMeta>* currentStore, std::set<docmeta::CrawlerDocMeta> docs);


    void writeStoreToFile(std::set<docmeta::CrawlerDocMeta> docs);
    void writeChangesToRepository(const std::vector<docmeta::CrawlerDocMeta> docs, std::string repoPath);

    std::string store_path;
    std::string crawled_docs_path;
    std::string repo_path;
};

#endif