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

#ifndef PRE_COMPUTED_BASIC_DOCUMENT_CPP
#define PRE_COMPUTED_BASIC_DOCUMENT_CPP

#include <iostream>
#include <thread>
#include <set>

#include "thread_queue.hpp"
#include "queue_handler.hpp"
#include "doc_meta.hpp"
#include "pre_computed_doc_crawler.hpp"
#include "pre_computed_doc_store.hpp"
#include "pre_computed_indexer.hpp"
#include "ranker.hpp"
#include "thread_manager.hpp"

#include "CLI11.hpp"

int main(int argc, const char** argv) {
    CLI::App app{"Pre-Computed document search engine"};

    std::string searchTerm;
    std::string path;

    app.add_option("-p,--path", path, "Use this option to specify where the search engine should search. You need to specify the absolute path to the directory. ");

    CLI11_PARSE(app, argc, argv);

    std::string specialCharsPath = INDEXING_ROOT_DIR "/documents/special.txt";
    std::string stopwordsPath = INDEXING_ROOT_DIR "/documents/stopwords.txt";

    ThreadManager tmanager;
    std::set<docmeta::DocumentMeta> document_store;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    PreComputedIndexer indexer = PreComputedIndexer(specialCharsPath, stopwordsPath, &tmanager.qhandler.repository_pipeline, &index);
    PreComputedDocStore store = PreComputedDocStore(&tmanager.qhandler.crawler_store_pipeline, &tmanager.qhandler.repository_pipeline, &document_store);
    PreComputedDocumentCrawler crawler = PreComputedDocumentCrawler(&tmanager.qhandler.crawler_store_pipeline, &tmanager.rflag, path);
    Ranker ranker = Ranker(&document_store, &index);

    tmanager.setAndStart(crawler, store, indexer);

    while (1) {
        std::cout << "Search for: ";
        std::cin >> searchTerm;

        if (searchTerm != "exit()") {
            std::vector<docmeta::DocumentMeta> foundDocuments = ranker.searchFor(searchTerm);

            if (foundDocuments.empty()) {
                std::cout << "No document(s) containing '" << searchTerm << "' found." << std::endl;
            } else {
                std::cout << "Found " << foundDocuments.size() << " document(s):" << std::endl;

                for (auto& doc: foundDocuments) {
                    std::cout << doc << std::endl;
                }
            }   
        } else {
            tmanager.stopThreads();
            break;
        }
    }

    return 0;
}

#endif