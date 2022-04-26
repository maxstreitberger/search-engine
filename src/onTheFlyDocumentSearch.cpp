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

#ifndef ON_THE_FLY_DOCUMENT_SEARCH_CPP
#define ON_THE_FLY_DOCUMENT_SEARCH_CPP

#include <iostream>
#include "indexer.hpp"
#include "doc_crawler.hpp"
#include "doc_store.hpp"
#include "ranker.hpp"
#include "engine.hpp"

#include <glog/logging.h>
#include "CLI11.hpp"

int main(int argc, const char** argv) {
    FLAGS_log_dir = "/tmp";
    google::InitGoogleLogging(argv[0]);
    DLOG(INFO) << "Start on-the-fly document search engine";
    
    CLI::App app{"On-The-Fly document search engine"};

    std::string searchTerm;
    std::string path;
    int maxReturnDocumentCount;

    app.add_option("-s,--search", searchTerm, "This allows you to specify what the search engine should search for. Without it you don't get any items back.");
    app.add_option("-p,--path", path, "Use this option to specify where the search engine should search. You need to specify the absolute path to the directory. ");
    app.add_option("-m,--max", maxReturnDocumentCount, "If this option is set to zero, you will get all items that the search engine has found. Otherwise, the search engine will only return x amount of items.");

    CLI11_PARSE(app, argc, argv);

    std::string specialCharsPath = INDEXING_ROOT_DIR "/documents/special.txt";
    std::string stopwordsPath = INDEXING_ROOT_DIR "/documents/stopwords.txt";

    std::set<docmeta::DocumentMeta> crawler_found_documents;
    std::set<docmeta::DocumentMeta> documents_in_store;
    std::vector<docmeta::DocumentMeta> repository;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    DocStore store = DocStore(&crawler_found_documents, &documents_in_store, &repository);
    DocumentCrawler crawler = DocumentCrawler(store, &crawler_found_documents, path);
    Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
    Ranker ranker = Ranker(&documents_in_store, &index, maxReturnDocumentCount);

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