#ifndef PRE_COMPUTED_DOC_STORE_CPP
#define PRE_COMPUTED_DOC_STORE_CPP

#include "pre_computed_doc_store.hpp"

std::ostream & operator <<(std::ostream &os, const std::set<docmeta::DocumentMeta> &m) {
    for (const auto &p : m) {
        os << p << std::endl;
    }
    return os;
}

void PreComputedDocStore::receiveDocuments() {
    while (true) {
        docmeta::DocumentMeta doc;
        crawler_store_pipeline->wait_and_pop(doc);
        if (!doc.content.empty()) {
            process(doc);
        } else {
            break;
        }
    }
}

void PreComputedDocStore::process(docmeta::DocumentMeta doc) {
    
    DocumentStatus isNewDocument = checkForChanges(doc_store, &doc);
    switch (isNewDocument) {
    case NEW:
        addToStore(doc_store, &doc);
        pushToRepository(doc_store, repository_pipeline, doc);
        break;
    case UPDATED:
        updateStore(doc_store, &doc);
        pushToRepository(doc_store, repository_pipeline, doc);
        break;
    default:
        break;
    } 
}

DocumentStatus PreComputedDocStore::checkForChanges(std::set<docmeta::DocumentMeta>* currentStore, docmeta::DocumentMeta* doc) {
    std::set<docmeta::DocumentMeta>::iterator it = std::find_if(currentStore->begin(), currentStore->end(), [&doc](const docmeta::DocumentMeta doc_in_store) {
        return doc_in_store.path == doc->path;
    });

    if (it != currentStore->end()) {
        if (it->content != doc->content) {
            return UPDATED;
        } else {
            return NONE;
        }
    } else {
        return NEW;
    }
}

void PreComputedDocStore::addToStore(std::set<docmeta::DocumentMeta>* currentStore, docmeta::DocumentMeta* doc) {
    doc->id = currentStore->size() + 1;
    currentStore->insert(*doc);
}

void PreComputedDocStore::updateStore(std::set<docmeta::DocumentMeta>* currentStore, docmeta::DocumentMeta* doc) {    
    std::set<docmeta::DocumentMeta>::iterator it = std::find_if(currentStore->begin(), currentStore->end(), [&doc](const docmeta::DocumentMeta doc_in_store) {
        return doc_in_store.path == doc->path;
    });

    int old_id = it->id;
    doc->id = old_id;
    currentStore->erase(it);
    currentStore->insert(*doc);
}

void PreComputedDocStore::pushToRepository(std::set<docmeta::DocumentMeta>* currentStore, ThreadQueue<const docmeta::DocumentMeta*>* repository_pipeline, docmeta::DocumentMeta doc) {
    std::set<docmeta::DocumentMeta>::iterator it = currentStore->find(doc);
    const docmeta::DocumentMeta* document = &(*it);
    repository_pipeline->push(document);
}
#endif
