#ifndef DOC_STORE_HPP
#define DOC_STORE_HPP

#include "crawler.hpp"

struct DocStore {
    DocStore() {};
    DocStore(std::string storePath, std::string crawledDocsPath, std::string repoPath) : store_path{storePath}, crawled_docs_path{crawledDocsPath}, repo_path{repoPath} {};

    void processDocuments();
    std::set<docmeta::DocumentMeta> loadDocuments(std::string path);
    void addNewDocumentsToStore(std::set<docmeta::DocumentMeta>* currentStore, std::vector<docmeta::DocumentMeta> docs);
    void updateDocumentsInStore(std::set<docmeta::DocumentMeta>* currentStore, std::vector<docmeta::DocumentMeta> docs);
    std::pair<std::vector<docmeta::DocumentMeta>, std::vector<docmeta::DocumentMeta>> checkForChanges(std::set<docmeta::DocumentMeta>* currentStore, std::set<docmeta::DocumentMeta> docs);


    void writeStoreToFile(std::set<docmeta::DocumentMeta> docs);
    void writeChangesToRepository(const std::vector<docmeta::DocumentMeta> docs, std::string repoPath);

    std::string store_path;
    std::string crawled_docs_path;
    std::string repo_path;
};

#endif