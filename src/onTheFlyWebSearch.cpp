/* Copyright (C) 2022 Maximilian Streitberger
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef ON_THE_FLY_WEB_SEARCH_CPP
#define ON_THE_FLY_WEB_SEARCH_CPP

#include <iostream>
#include "web_crawler.hpp"
#include "indexer.hpp"
#include "ranker.hpp"
#include "engine.hpp"

#include <glog/logging.h>
#include "CLI11.hpp"

int main(int argc, char *argv[]) {
    FLAGS_log_dir = "/tmp";
    google::InitGoogleLogging(argv[0]);
    LOG(INFO) << "Start OnTheFlyWebSearch search engine";

    CLI::App app{"On-The-Fly web search engine"};

    std::string searchTerm;
    std::string url;
    int searchDepth;
    int maxReturnDocumentCount;

    app.add_option("-s,--search", searchTerm, "This allows you to specify what the search engine should search for. Without it you don't get any items back.");
    app.add_option("-u,--url", url, "Use this option to specify where the search engine should search.");
    app.add_option("-d,--depth", searchDepth, "This option allows you to limit how deep the search engine should search.");
    app.add_option("-m,--max", maxReturnDocumentCount, "If this option is set to zero, you will get all items that the search engine has found. Otherwise, the search engine will only return x amount of items.");

    CLI11_PARSE(app, argc, argv);

    std::set<docmeta::DocumentMeta> crawler_found_pages;
    std::set<docmeta::DocumentMeta> pages_in_store;
    std::vector<docmeta::DocumentMeta> repository;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    std::string specialCharsPath = INDEXING_ROOT_DIR "/documents/special.txt";
    std::string stopwordsPath = INDEXING_ROOT_DIR "/documents/stopwords.txt";

    DocStore store = DocStore(&crawler_found_pages, &pages_in_store, &repository);
    WebCrawler crawler = WebCrawler(store, &crawler_found_pages, url, searchDepth);
    Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
    Ranker ranker = Ranker(&pages_in_store, &index, maxReturnDocumentCount);

    std::vector<docmeta::DocumentMeta> foundPages = engine::runSearch(crawler, indexer, ranker, searchTerm);

    if (foundPages.empty()) {
        LOG(INFO) << "No page(s) containing '" << searchTerm << "' found.";
        std::cout << "No page(s) containing '" << searchTerm << "' found." << std::endl;
    } else {
        LOG(INFO) << "Found " << foundPages.size() << " pages(s)";
        std::cout << "Found " << foundPages.size() << " pages(s):" << std::endl;

        for (auto& page: foundPages) {
            std::cout << page << std::endl;
        }
    }

    return 0;
}

#endif