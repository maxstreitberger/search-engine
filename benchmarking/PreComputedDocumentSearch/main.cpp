#ifndef PRE_COMPUTED_DOCUMENT_SEARCH_BENCHMARKING_MAIN_CPP
#define PRE_COMPUTED_DOCUMENT_SEARCH_BENCHMARKING_MAIN_CPP

#include "nonius.hpp"
#include "helpers.hpp"
#include "pre_computed_doc_crawler.hpp"
#include "pre_computed_doc_store.hpp"
#include "pre_computed_indexer.hpp"
#include "ranker.hpp"
#include "thread_manager.hpp"

std::string specialCharsPath = SEARCHENGINE_BENCHMARKING_DIR "/resources/special.txt";
std::string stopwordsPath = SEARCHENGINE_BENCHMARKING_DIR "/resources/stopwords.txt";

std::string searchPath;

void otf_doc_multiple_docs_returned(nonius::chronometer meter) {
    ThreadManager tmanager = ThreadManager();
    std::set<docmeta::DocumentMeta> document_store;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    PreComputedIndexer indexer = PreComputedIndexer(specialCharsPath, stopwordsPath, &tmanager.qhandler.repository_pipeline, &index);
    PreComputedDocStore store = PreComputedDocStore(&tmanager.qhandler.crawler_store_pipeline, &tmanager.qhandler.repository_pipeline, &document_store);
    PreComputedDocumentCrawler crawler = PreComputedDocumentCrawler(&tmanager.qhandler.crawler_store_pipeline, &tmanager.rflag, searchPath);
    Ranker ranker = Ranker(&document_store, &index);

    tmanager.setAndStart(crawler, store, indexer);

    meter.measure([&ranker] { return ranker.searchFor("testtest"); });

    tmanager.stopThreads();
}

int main(int argc, char* argv[]) {
    searchPath = argv[1];

    nonius::configuration cfg;
    cfg.samples = 500;
    cfg.output_file = "PreComputedDocumentSearch_benchmarking.csv";
    nonius::benchmark benchmarks[] = {
        nonius::benchmark("Pre-Computed Document Search", otf_doc_multiple_docs_returned)
    };

    // nonius::go(cfg, std::begin(benchmarks), std::end(benchmarks), nonius::standard_reporter());
    nonius::go(cfg, std::begin(benchmarks), std::end(benchmarks), nonius::csv_reporter());

    return 0;
}

#endif