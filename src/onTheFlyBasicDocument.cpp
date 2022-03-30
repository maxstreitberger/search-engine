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
#include "crawler.hpp"
#include "ranker.hpp"
#include <nlohmann/json.hpp>

int main(int argc, char *argv[]) {
    FLAGS_log_dir = "/tmp";
    google::InitGoogleLogging(argv[0]);
    LOG(INFO) << "Start onTheFlyBasicDocument search engine";

    if (argc == 1) {
        LOG(ERROR) << "No flags were added";
        return 1;
    }

    std::string searchTerm;

    for (int i = 1; i <= argc - 1; i++) {
        std::string value = argv[i];

        auto it = value.find('=');
        if (it != std::string::npos) {
            if ((value.find("search") != std::string::npos) || (value.find("-s=") != std::string::npos)) {
                searchTerm = value.substr(it + 1, std::string::npos);
            }
        } else {
            if ((value.find("search") != std::string::npos) || (value == "-s")) {
                searchTerm = argv[i+1];
            }
        }
    }

    std::set<docmeta::DocumentMeta> document_store;
    std::vector<docmeta::DocumentMeta> repository;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;
    
    std::string specialCharsPath = INDEXING_ROOT_DIR "/documents/special.txt";
    std::string stopwordsPath = INDEXING_ROOT_DIR "/documents/stopwords.json";

    Crawler crawler = Crawler(&document_store, &repository, SEARCHENGINE_ROOT_DIR "/dummy-text");
    crawler.start();

    Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
    indexer.generateIndex();
    
    Ranker ranking = Ranker(&document_store, &index);
    std::vector<docmeta::DocumentMeta> foundDocuments = ranking.searchFor(searchTerm);

    LOG(INFO) << "Found " << foundDocuments.size() << " document(s)";

    for (auto& doc: foundDocuments) {
        std::cout << doc << std::endl;
    }

    return 0;
}

#endif