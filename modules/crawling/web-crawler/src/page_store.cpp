#ifndef PAGE_CRAWLER_DOC_STORE_CPP
#define PAGE_CRAWLER_DOC_STORE_CPP

#include "page_store.hpp"

void PageStore::processPages() {
    std::pair<std::vector<docmeta::DocumentMeta>, std::vector<docmeta::DocumentMeta>> changesAndAddition = checkForChanges(page_store, crawler_pages);
    addNewPagesToStore(page_store, changesAndAddition.first);
    updatePagesInStore(page_store, changesAndAddition.second);

    repository->insert( repository->end(), changesAndAddition.first.begin(), changesAndAddition.first.end() );
    repository->insert( repository->end(), changesAndAddition.second.begin(), changesAndAddition.second.end() );
}

std::pair<std::vector<docmeta::DocumentMeta>, std::vector<docmeta::DocumentMeta>> PageStore::checkForChanges(std::set<docmeta::DocumentMeta>* currentStore, std::set<docmeta::DocumentMeta> pages) {
    std::vector<docmeta::DocumentMeta> newPages;
    std::vector<docmeta::DocumentMeta> updatedPages;

    for (auto& crawler_pages: pages) {
        auto it = currentStore->find(crawler_pages);
        if (it != currentStore->end()) {
            if (it->content != crawler_pages.content) {
                updatedPages.push_back(crawler_pages);
            } else {
                continue;
            }
        } else {
            newPages.push_back(crawler_pages);
        }
    }

    return std::make_pair(newPages, updatedPages);
}

void PageStore::addNewPagesToStore(std::set<docmeta::DocumentMeta>* currentStore, std::vector<docmeta::DocumentMeta> pages) {
    for (auto& newPage: pages) {
        currentStore->insert(newPage);
    }
}

void PageStore::updatePagesInStore(std::set<docmeta::DocumentMeta>* currentStore, std::vector<docmeta::DocumentMeta> pages) {
    for (auto& updatedPage: pages) {
        auto it = currentStore->find(updatedPage);
        currentStore->erase(it);
        currentStore->insert(updatedPage);
    }
}

#endif