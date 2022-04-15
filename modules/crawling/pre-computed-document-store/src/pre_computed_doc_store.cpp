#ifndef PRE_COMPUTED_DOC_STORE_CPP
#define PRE_COMPUTED_DOC_STORE_CPP

#include "pre_computed_doc_store.hpp"

void PreComputedDocStore::receiveDocuments() {
    LOG(INFO) << "Receive documents from crawler";
    while(1) {
        docmeta::DocumentMeta doc;
        crawler_store_pipeline->wait_and_pop(doc);
        process(doc);
    }
}

void PreComputedDocStore::process(docmeta::DocumentMeta doc) {
    LOG(INFO) << "Process document";
    
    DocumentStatus isNewDocument = checkForChanges(doc_store, &doc);
    switch (isNewDocument) {
    case NEW:
        add(doc_store, repository_pipeline, &doc);
        break;
    case UPDATED:
        update(doc_store, repository_pipeline, &doc);
        break;
    default:
        break;
    }
}

DocumentStatus PreComputedDocStore::checkForChanges(std::set<docmeta::DocumentMeta>* currentStore, docmeta::DocumentMeta* doc) {
    LOG(INFO) << "Check if the document is new or updated";

    std::set<docmeta::DocumentMeta>::iterator it = std::find_if(currentStore->begin(), currentStore->end(), [&doc](const docmeta::DocumentMeta doc_in_store) {
        return doc_in_store.path == doc->path;
    });

    if (it != currentStore->end()) {
        if (it->content != doc->content) {
            LOG(INFO) << "Update document with id=" << it->id;
            return UPDATED;
        } else {
            LOG(INFO) << "Document is not new or updated (id=" << it->id << ")";
            return NONE;
        }
    } else {
        LOG(INFO) << "Document is new (path=" << doc->path << ")";
        return NEW;
    }
}

void PreComputedDocStore::add(std::set<docmeta::DocumentMeta>* currentStore, ThreadQueue<docmeta::DocumentMeta>* repository_pipeline, docmeta::DocumentMeta* doc) {
    doc->id = currentStore->size() + 1;
    currentStore->insert(*doc);
    repository_pipeline->push(*doc);
}

void PreComputedDocStore::update(std::set<docmeta::DocumentMeta>* currentStore, ThreadQueue<docmeta::DocumentMeta>* repository_pipeline, docmeta::DocumentMeta* doc) {
    LOG(INFO) << "Update doc (id= " << doc->id << ") to " << doc->content;
    
    std::set<docmeta::DocumentMeta>::iterator it = std::find_if(currentStore->begin(), currentStore->end(), [&doc](const docmeta::DocumentMeta doc_in_store) {
        LOG(INFO) << "Document with path = " << doc->path << " was already crawled"; 
        return doc_in_store.path == doc->path;
    });

    int old_id = it->id;
    doc->id = old_id;
    currentStore->erase(it);
    currentStore->insert(*doc);
    repository_pipeline->push(*doc);
}

#endif
