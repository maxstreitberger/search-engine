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

#ifndef CONCURRENT_BASIC_DOCUMENT_CPP
#define CONCURRENT_BASIC_DOCUMENT_CPP

#include <iostream>
#include <thread>
#include <set>
#include <atomic>
#include <memory>

#include "doc_meta.hpp"
#include "concurrent_doc_crawler.hpp"
#include "concurrent_doc_store.hpp"
#include "concurrent_indexer.hpp"
#include "ranker.hpp"

#include "concurrent_thread_queue.hpp"

#include "CLI11.hpp"

int main(int argc, const char** argv) {
    CLI::App app{"Concurrent document search engine"};

    std::string searchTerm;
    std::string path;

    app.add_option("-s,--search", searchTerm, "This allows you to specify what the search engine should search for. Without it you don't get any items back.");
    app.add_option("-p,--path", path, "Use this option to specify where the search engine should search. You need to specify the absolute path to the directory. ");

    CLI11_PARSE(app, argc, argv);

    std::string specialCharsPath = INDEXING_ROOT_DIR "/documents/special.txt";
    std::string stopwordsPath = INDEXING_ROOT_DIR "/documents/stopwords.txt";

    std::set<docmeta::DocumentMeta> document_store;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    std::atomic<bool> crawler_flag = true;
    std::atomic<bool> store_flag = true;
    std::atomic<bool> index_flag = true;

    ConcurrentThreadQueue<docmeta::DocumentMeta> crawler_store_pipeline;
    crawler_store_pipeline.flag = &crawler_flag;
    ConcurrentThreadQueue<const docmeta::DocumentMeta*> repository_pipeline;
    repository_pipeline.flag = &store_flag;

    ConcurrentDocumentCrawler crawler = ConcurrentDocumentCrawler(&crawler_store_pipeline, path, &crawler_flag);
    ConcurrentDocStore store = ConcurrentDocStore(&crawler_store_pipeline, &repository_pipeline, &document_store, &store_flag);
    ConcurrentIndexer indexer = ConcurrentIndexer(specialCharsPath, stopwordsPath, &repository_pipeline, &index, &index_flag);
    Ranker ranker = Ranker(&document_store, &index);


    std::thread crawler_thread = std::thread(&ConcurrentDocumentCrawler::start, crawler);
    std::thread store_thread = std::thread(&ConcurrentDocStore::receiveDocuments, store);
    std::thread indexer_thread = std::thread(&ConcurrentIndexer::generateIndex, indexer);
    crawler_thread.join();
    store_thread.join();
    indexer_thread.join();    

    std::vector<docmeta::DocumentMeta> foundDocuments = ranker.searchFor(searchTerm);

    if (foundDocuments.empty()) {
        std::cout << "No document(s) containing '" << searchTerm << "' found." << std::endl;
    } else {
        std::cout << "Found " << foundDocuments.size() << " document(s):" << std::endl;

        for (auto& doc: foundDocuments) {
            std::cout << doc << std::endl;
        }
    }

    return 0;
}

#endif