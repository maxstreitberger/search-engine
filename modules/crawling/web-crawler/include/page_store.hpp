#ifndef PAGE_CRAWLER_DOC_STORE_HPP
#define PAGE_CRAWLER_DOC_STORE_HPP

#include "crawler.hpp"

struct PageStore {
    PageStore() {};
    PageStore(std::set<pagemeta::PageMeta> crawler_pages, std::set<pagemeta::PageMeta>* page_store, std::vector<pagemeta::PageMeta>* repository) : crawler_pages{crawler_pages}, page_store{page_store}, repository{repository} {};

    void processPages();
    void addNewPagesToStore(std::set<pagemeta::PageMeta>* currentStore, std::vector<pagemeta::PageMeta> pages);
    void updatePagesInStore(std::set<pagemeta::PageMeta>* currentStore, std::vector<pagemeta::PageMeta> pages);
    std::pair<std::vector<pagemeta::PageMeta>, std::vector<pagemeta::PageMeta>> checkForChanges(std::set<pagemeta::PageMeta>* currentStore, std::set<pagemeta::PageMeta> page);
    
    std::set<pagemeta::PageMeta> crawler_pages;
    std::set<pagemeta::PageMeta>* page_store;
    std::vector<pagemeta::PageMeta>* repository;
};

#endif