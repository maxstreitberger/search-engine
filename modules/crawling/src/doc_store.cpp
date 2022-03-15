#ifndef DOC_STORE_CPP
#define DOC_STORE_CPP

#include "doc_store.hpp"

/*
    DocStore reads crawler-found-docs.txt file, which contains all the files the crawler found. It goes through it line by line (documnet by document) and checks if there are any new or updated documents. It then updates or adds these documents to the repository where the indexer can pick them up. In addition, these documents get pushed into the permanent store.
    
    The crawler can't add new docs to the crawler-found-docs.txt file while the DocStore reads it => Either not crawl at all during this time (possible for document crawler) or find other solution (needed for web crawler)
*/

void DocStore::processDocuments() {
    std::set<docmeta::DocumentMeta> currentStore = loadDocuments(store_path);
    std::set<docmeta::DocumentMeta> documentsFoundByCrawler = loadDocuments(crawled_docs_path);
    std::pair<std::vector<docmeta::DocumentMeta>, std::vector<docmeta::DocumentMeta>> changesAndAddition = checkForChanges(&currentStore, documentsFoundByCrawler);
    addNewDocumentsToStore(&currentStore, changesAndAddition.first);
    updateDocumentsInStore(&currentStore, changesAndAddition.second);
    writeStoreToFile(currentStore);

    std::vector<docmeta::DocumentMeta> allMotifications;
    allMotifications.reserve( changesAndAddition.first.size() + changesAndAddition.second.size() );
    allMotifications.insert( allMotifications.end(), changesAndAddition.first.begin(), changesAndAddition.first.end() );
    allMotifications.insert( allMotifications.end(), changesAndAddition.second.begin(), changesAndAddition.second.end() );

    writeChangesToRepository(allMotifications, repo_path);
    
}

std::set<docmeta::DocumentMeta> DocStore::loadDocuments(std::string path) {
    std::ifstream ifs(path);
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    nlohmann::json crawler_docs;
    std::set<docmeta::DocumentMeta> documentsInStore;

    try {
        crawler_docs = nlohmann::json::parse(content);
        documentsInStore = crawler_docs.get<std::set<docmeta::DocumentMeta>>();
    }
    catch (nlohmann::json::parse_error& ex) {
        std::cerr << "parse error at byte " << ex.byte << std::endl;
    }

    return documentsInStore;
}

std::pair<std::vector<docmeta::DocumentMeta>, std::vector<docmeta::DocumentMeta>> DocStore::checkForChanges(std::set<docmeta::DocumentMeta>* currentStore, std::set<docmeta::DocumentMeta> docs) {
    std::vector<docmeta::DocumentMeta> newDocs;
    std::vector<docmeta::DocumentMeta> updatedDocs;

    for (auto& crawler_doc: docs) {
        auto it = currentStore->find(crawler_doc);
        if (it != currentStore->end()) {
            if (it->content != crawler_doc.content) {
                updatedDocs.push_back(crawler_doc);
            } else {
                continue;
            }
        } else {
            newDocs.push_back(crawler_doc);
        }
    }

    return std::make_pair(newDocs, updatedDocs);
}

void DocStore::addNewDocumentsToStore(std::set<docmeta::DocumentMeta>* currentStore, std::vector<docmeta::DocumentMeta> docs) {
    for (auto& newDoc: docs) {
        currentStore->insert(newDoc);
    }
}

void DocStore::updateDocumentsInStore(std::set<docmeta::DocumentMeta>* currentStore, std::vector<docmeta::DocumentMeta> docs) {
    for (auto& updatedDoc: docs) {
        auto it = currentStore->find(updatedDoc);
        currentStore->erase(it);
        currentStore->insert(updatedDoc);
    }
}

void DocStore::writeStoreToFile(std::set<docmeta::DocumentMeta> docs) {
    std::ofstream file(store_path);
    nlohmann::json doc_json = docs;
    file << std::setw(4) << doc_json;
    file.close();
}

void DocStore::writeChangesToRepository(const std::vector<docmeta::DocumentMeta> docs, std::string repoPath) {
    if (!docs.empty()) {
        std::ofstream file(repoPath);
        nlohmann::json doc_json = docs;
        file << std::setw(4) << doc_json;
        file.close();
    }
}

#endif