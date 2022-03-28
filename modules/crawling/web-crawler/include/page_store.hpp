#ifndef PAGE_CRAWLER_DOC_STORE_HPP
#define PAGE_CRAWLER_DOC_STORE_HPP

#include "crawler.hpp"

struct PageStore {
    PageStore() {};
    PageStore(std::set<docmeta::DocumentMeta> crawler_pages, std::set<docmeta::DocumentMeta>* page_store, std::vector<docmeta::DocumentMeta>* repository) : crawler_pages{crawler_pages}, page_store{page_store}, repository{repository} {};

    void processPages();
    void addNewPagesToStore(std::set<docmeta::DocumentMeta>* currentStore, std::vector<docmeta::DocumentMeta> pages);
    void updatePagesInStore(std::set<docmeta::DocumentMeta>* currentStore, std::vector<docmeta::DocumentMeta> pages);
    std::pair<std::vector<docmeta::DocumentMeta>, std::vector<docmeta::DocumentMeta>> checkForChanges(std::set<docmeta::DocumentMeta>* currentStore, std::set<docmeta::DocumentMeta> page);
    
    std::set<docmeta::DocumentMeta> crawler_pages;
    std::set<docmeta::DocumentMeta>* page_store;
    std::vector<docmeta::DocumentMeta>* repository;
};

#endif