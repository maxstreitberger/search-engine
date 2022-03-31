#ifndef DOC_STORE_CPP
#define DOC_STORE_CPP

#include "doc_store.hpp"

void DocStore::processDocuments() {
    LOG(INFO) << "Process crawled documents";
    std::pair<std::vector<docmeta::DocumentMeta>, std::vector<docmeta::DocumentMeta>> changesAndAddition = checkForChanges(doc_store, crawler_docs);
    addNewDocumentsToStore(doc_store, changesAndAddition.first);
    updateDocumentsInStore(doc_store, changesAndAddition.second);

    repository->insert( repository->end(), changesAndAddition.first.begin(), changesAndAddition.first.end() );
    repository->insert( repository->end(), changesAndAddition.second.begin(), changesAndAddition.second.end() );
}

std::pair<std::vector<docmeta::DocumentMeta>, std::vector<docmeta::DocumentMeta>> DocStore::checkForChanges(std::set<docmeta::DocumentMeta>* currentStore, std::set<docmeta::DocumentMeta> docs) {
    LOG(INFO) << "Check for changes in documents";
    std::vector<docmeta::DocumentMeta> newDocs;
    std::vector<docmeta::DocumentMeta> updatedDocs;

    for (auto& crawler_doc: docs) {
        std::set<docmeta::DocumentMeta>::iterator it = std::find_if(currentStore->begin(), currentStore->end(), [&crawler_doc](const docmeta::DocumentMeta doc) {
            LOG(INFO) << "Document with path = " << doc.path << " was already crawled"; 
            return doc.path == crawler_doc.path;
        });
        if (it != currentStore->end()) {
            if (it->content != crawler_doc.content) {
                LOG(INFO) << "Found document in store with same content (id=" << it->id << ")";
                docmeta::DocumentMeta updated_doc = docmeta::DocumentMeta(it->id, crawler_doc.content, crawler_doc.path);
                updatedDocs.push_back(updated_doc);
            } else {
                LOG(INFO) << "Document is not new or updated (id=" << it->id << ")";
                continue;
            }
        } else {
            LOG(INFO) << "Document is new (id=" << crawler_doc.id << ")";
            newDocs.push_back(crawler_doc);
        }
    }

    return std::make_pair(newDocs, updatedDocs);
}

void DocStore::addNewDocumentsToStore(std::set<docmeta::DocumentMeta>* currentStore, std::vector<docmeta::DocumentMeta> docs) {
    for (auto& newDoc: docs) {
        LOG(INFO) << "Add new document (id= " << newDoc.id << ", content: "<< newDoc.content << ") to the store";
        currentStore->insert(newDoc);
    }
}

void DocStore::updateDocumentsInStore(std::set<docmeta::DocumentMeta>* currentStore, std::vector<docmeta::DocumentMeta> docs) {
    for (auto& updatedDoc: docs) {
        LOG(INFO) << "Update doc (id= " << updatedDoc.id << ") to " << updatedDoc.content;
        std::set<docmeta::DocumentMeta>::iterator it = std::find_if(currentStore->begin(), currentStore->end(), [&updatedDoc](const docmeta::DocumentMeta doc) {
            LOG(INFO) << "Document with path = " << doc.path << " was already crawled"; 
            return doc.path == updatedDoc.path;
        });
        int old_id = it->id;
        updatedDoc.id = old_id;
        currentStore->erase(it);
        currentStore->insert(updatedDoc);
    }
}

#endif