#ifndef CONCURRENT_ENGINE_HPP
#define CONCURRENT_ENGINE_HPP

#include <vector>
#include "ranker.hpp"

namespace engine {
    template<typename Crawler, typename Store, typename Indexer>
    std::vector<docmeta::DocumentMeta> runSearch(Crawler crawler, Store store, Indexer indexer, Ranker ranker, std::string searchTerm) {
        std::thread indexer_thread = std::thread(&Indexer::generateIndex, indexer);
        std::thread store_thread = std::thread(&Store::receiveDocuments, store);
        std::thread crawler_thread = std::thread(&Crawler::start, crawler);

        indexer_thread.join();
        store_thread.join();
        crawler_thread.join();

        std::vector<docmeta::DocumentMeta> foundDocuments = ranker.searchFor(searchTerm);
        return ranker.searchFor(searchTerm);
    }
}

#endif