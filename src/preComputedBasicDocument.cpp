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

int main(int argc, const char** argv) {
    FLAGS_log_dir = SEARCHENGINE_ROOT_DIR "/bin/logs";
    google::InitGoogleLogging(argv[0]);

    ThreadQueue<docmeta::DocumentMeta> crawler_pipeline;

    std::set<docmeta::DocumentMeta> crawler_found_documents;
    std::set<docmeta::DocumentMeta> document_store;
    std::vector<docmeta::DocumentMeta> repository;

    PreComputedDocStore store = PreComputedDocStore(&crawler_pipeline, &document_store, &repository);
    PreComputedDocumentCrawler crawler = PreComputedDocumentCrawler(&crawler_pipeline, SEARCHENGINE_ROOT_DIR "/dummy-text");

    std::thread crawler_thread (&PreComputedDocumentCrawler::start, crawler);
    std::thread store_thread (&PreComputedDocStore::receiveDocuments, store);

    crawler_thread.join();              
    store_thread.join(); 

    std::cout << "threads completed.\n";

    return 0;
}

#endif