#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>

#include "indexer.hpp"
#include "ranker.hpp"

namespace engine {
    template<typename Crawler>
    std::vector<docmeta::DocumentMeta> runSearch(Crawler crawler, Indexer indexer, Ranker ranker, std::string searchTerm) {
        crawler.start();
        indexer.generateIndex();
        return ranker.searchFor(searchTerm);
    }
}

#endif