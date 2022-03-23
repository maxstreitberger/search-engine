#ifndef PAGE_CRAWLER_DOC_STORE_CPP
#define PAGE_CRAWLER_DOC_STORE_CPP

#include "page_store.hpp"

void PageStore::processPages() {
    std::pair<std::vector<pagemeta::PageMeta>, std::vector<pagemeta::PageMeta>> changesAndAddition = checkForChanges(page_store, crawler_pages);
    addNewPagesToStore(page_store, changesAndAddition.first);
    updatePagesInStore(page_store, changesAndAddition.second);

    repository->insert( repository->end(), changesAndAddition.first.begin(), changesAndAddition.first.end() );
    repository->insert( repository->end(), changesAndAddition.second.begin(), changesAndAddition.second.end() );
}

std::pair<std::vector<pagemeta::PageMeta>, std::vector<pagemeta::PageMeta>> PageStore::checkForChanges(std::set<pagemeta::PageMeta>* currentStore, std::set<pagemeta::PageMeta> pages) {
    std::vector<pagemeta::PageMeta> newPages;
    std::vector<pagemeta::PageMeta> updatedPages;

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

void PageStore::addNewPagesToStore(std::set<pagemeta::PageMeta>* currentStore, std::vector<pagemeta::PageMeta> pages) {
    for (auto& newPage: pages) {
        currentStore->insert(newPage);
    }
}

void PageStore::updatePagesInStore(std::set<pagemeta::PageMeta>* currentStore, std::vector<pagemeta::PageMeta> pages) {
    for (auto& updatedPage: pages) {
        auto it = currentStore->find(updatedPage);
        currentStore->erase(it);
        currentStore->insert(updatedPage);
    }
}

#endif