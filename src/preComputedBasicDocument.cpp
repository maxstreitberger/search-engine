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
#include <queue>

#include "thread_queue.hpp"
#include "doc_meta.hpp"
#include "pre_computed_doc_crawler.hpp"
#include "pre_computed_doc_store.hpp"
#include "pre_computed_indexer.hpp"

int main(int argc, const char** argv) {
    FLAGS_log_dir = SEARCHENGINE_ROOT_DIR "/bin/logs";
    google::InitGoogleLogging(argv[0]);

    std::string specialCharsPath = INDEXING_ROOT_DIR "/documents/special.txt";
    std::string stopwordsPath = INDEXING_ROOT_DIR "/documents/stopwords.txt";

    ThreadQueue<docmeta::DocumentMeta> crawler_store_pipeline;
    ThreadQueue<docmeta::DocumentMeta> repository_pipeline;

    std::set<docmeta::DocumentMeta> document_store;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    PreComputedDocStore store = PreComputedDocStore(&crawler_store_pipeline, &repository_pipeline, &document_store);
    PreComputedDocumentCrawler crawler = PreComputedDocumentCrawler(&crawler_store_pipeline, SEARCHENGINE_ROOT_DIR "/dummy-text");
    PreComputedIndexer indexer = PreComputedIndexer(specialCharsPath, stopwordsPath, &repository_pipeline, &index);

    std::thread crawler_thread (&PreComputedDocumentCrawler::start, crawler);
    std::thread store_thread (&PreComputedDocStore::receiveDocuments, store);
    std::thread indexer_thread (&PreComputedIndexer::generateIndex, indexer);

    crawler_thread.join();              
    store_thread.join(); 
    indexer_thread.join();

    std::cout << "threads completed.\n";

    return 0;
}

#endif