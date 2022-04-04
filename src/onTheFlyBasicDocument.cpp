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

#ifndef ON_THE_FLY_BASIC_DOCUMENT_CPP
#define ON_THE_FLY_BASIC_DOCUMENT_CPP

#include <iostream>
#include "indexer.hpp"
#include "doc_crawler.hpp"
#include "ranker.hpp"
#include "engine.hpp"

#include <nlohmann/json.hpp>
#include <lyra/lyra.hpp>

int main(int argc, const char** argv) {
    FLAGS_log_dir = "/tmp";
    google::InitGoogleLogging(argv[0]);
    LOG(INFO) << "Start onTheFlyBasicDocument search engine";

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

    std::set<docmeta::DocumentMeta> document_store;
    std::vector<docmeta::DocumentMeta> repository;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;
    
    std::string specialCharsPath = INDEXING_ROOT_DIR "/documents/special.txt";
    std::string stopwordsPath = INDEXING_ROOT_DIR "/documents/stopwords.json";

    DocumentCrawler crawler = DocumentCrawler(&document_store, &repository, SEARCHENGINE_ROOT_DIR "/dummy-text");
    Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
    Ranker ranker = Ranker(&document_store, &index);

    std::vector<docmeta::DocumentMeta> foundDocuments = engine::runSearch(crawler, indexer, ranker, searchTerm);

    if (foundDocuments.empty()) {
        LOG(INFO) << "No document(s) containing '" << searchTerm << "' found.";
        std::cout << "No document(s) containing '" << searchTerm << "' found." << std::endl;
    } else {
        LOG(INFO) << "Found " << foundDocuments.size() << " document(s)";
        std::cout << "Found " << foundDocuments.size() << " document(s):" << std::endl;

        for (auto& doc: foundDocuments) {
            std::cout << doc << std::endl;
        }
    }

    return 0;
}

#endif