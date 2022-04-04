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

#ifndef ON_THE_FLY_BASIC_WEB_CPP
#define ON_THE_FLY_BASIC_WEB_CPP

#include <iostream>
#include "crawler.hpp"
#include "indexer.hpp"
#include "ranker.hpp"

#include <glog/logging.h>
#include <lyra/lyra.hpp>

int main(int argc, char *argv[]) {
    FLAGS_log_dir = "/tmp";
    google::InitGoogleLogging(argv[0]);
    LOG(INFO) << "Start onTheFlyBasicWeb search engine";

    std::string searchTerm;
    bool show_help = false;

    auto cli = lyra::help(show_help) | lyra::opt(searchTerm, "searchTerm")
                                    ["-s"]["--search"]("Get documents that include the search term.");
    
    auto result = cli.parse({ argc, argv });
    if (!result) {
        LOG(ERROR) << "Error in command line: " << result.errorMessage();
        std::cerr << cli << "\n";
        return 1;
    }

    if (show_help) {
        std::cout << cli << "\n";
        return 0;
    }

    std::set<docmeta::DocumentMeta> page_store;
    std::vector<docmeta::DocumentMeta> repository;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    std::string specialCharsPath = INDEXING_ROOT_DIR "/documents/special.txt";
    std::string stopwordsPath = INDEXING_ROOT_DIR "/documents/stopwords.json";

    WebCrawler crawler = WebCrawler(&page_store, &repository, "https://zelebrate.xyz");
    crawler.start();

    Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
    indexer.generateIndex();

    Ranker ranking = Ranker(&page_store, &index);
    std::vector<docmeta::DocumentMeta> foundDocuments = ranking.searchFor(searchTerm);

    if (foundDocuments.empty()) {
        LOG(INFO) << "No page(s) containing '" << searchTerm << "' found.";
        std::cout << "No page(s) containing '" << searchTerm << "' found." << std::endl;
    } else {
        LOG(INFO) << "Found " << foundDocuments.size() << " pages(s)";
        std::cout << "Found " << foundDocuments.size() << " pages(s):" << std::endl;

        for (auto& doc: foundDocuments) {
            std::cout << doc << std::endl;
        }
    }

    return 0;
}

#endif