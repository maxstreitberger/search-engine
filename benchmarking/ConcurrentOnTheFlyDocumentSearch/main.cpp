#ifndef CONCURRENT_ON_THE_FLY_DOCUMENT_SEARCH_BENCHMARKING_MAIN_CPP
#define CONCURRENT_ON_THE_FLY_DOCUMENT_SEARCH_BENCHMARKING_MAIN_CPP

#include "nonius.hpp"
#include "helpers.hpp"
#include "concurrent_doc_crawler.hpp"
#include "concurrent_doc_store.hpp"
#include "concurrent_indexer.hpp"
#include "ranker.hpp"

std::string specialCharsPath = SEARCHENGINE_BENCHMARKING_DIR "/resources/special.txt";
std::string stopwordsPath = SEARCHENGINE_BENCHMARKING_DIR "/resources/stopwords.txt";

std::string searchPath;

void otf_doc_multiple_docs_returned(nonius::chronometer meter) {
    std::set<docmeta::DocumentMeta> document_store;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    std::atomic<bool> crawler_flag = true;
    std::atomic<bool> store_flag = true;
    std::atomic<bool> index_flag = true;

    ConcurrentThreadQueue<docmeta::DocumentMeta> crawler_store_pipeline;
    crawler_store_pipeline.flag = &crawler_flag;
    ConcurrentThreadQueue<const docmeta::DocumentMeta*> repository_pipeline;
    repository_pipeline.flag = &store_flag;

    ConcurrentDocumentCrawler crawler = ConcurrentDocumentCrawler(&crawler_store_pipeline, searchPath, &crawler_flag);
    ConcurrentDocStore store = ConcurrentDocStore(&crawler_store_pipeline, &repository_pipeline, &document_store, &store_flag);
    ConcurrentIndexer indexer = ConcurrentIndexer(specialCharsPath, stopwordsPath, &repository_pipeline, &index, &index_flag);
    Ranker ranker = Ranker(&document_store, &index);

    std::thread crawler_thread = std::thread(&ConcurrentDocumentCrawler::start, crawler);
    std::thread store_thread = std::thread(&ConcurrentDocStore::receiveDocuments, store);
    std::thread indexer_thread = std::thread(&ConcurrentIndexer::generateIndex, indexer);

    crawler_thread.join();
    store_thread.join();
    indexer_thread.join();    

    meter.measure([&ranker] { return ranker.searchFor("lorem"); });
}

int main(int argc, char* argv[]) {
    searchPath = argv[1];

    nonius::configuration cfg;
    cfg.samples = 500;
    cfg.output_file = "ConcurrentOnTheFlyDocumentSearch_benchmarking.csv";
    nonius::benchmark benchmarks[] = {
        nonius::benchmark("On-The-Fly Document Search", otf_doc_multiple_docs_returned)
    };

    // nonius::go(cfg, std::begin(benchmarks), std::end(benchmarks), nonius::standard_reporter());
    nonius::go(cfg, std::begin(benchmarks), std::end(benchmarks), nonius::csv_reporter());

    return 0;
}

#endif