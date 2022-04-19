#ifndef ON_THE_FLY_BASIC_WEB_BENCHMARKING_MAIN_CPP
#define ON_THE_FLY_BASIC_WEB_BENCHMARKING_MAIN_CPP

#include "nonius.hpp"
#include "engine.hpp"
#include "helpers.hpp"
#include "web_crawler.hpp"
#include <glog/logging.h>

std::string specialCharsPath = SEARCHENGINE_TESTING_DIR "/resources/special.txt";
std::string stopwordsPath = SEARCHENGINE_TESTING_DIR "/resources/stopwords.txt";

void otf_basic_web_nothing_returned(nonius::chronometer meter) {
    std::set<docmeta::DocumentMeta> crawler_found_pages;
    std::set<docmeta::DocumentMeta> pages_in_store;
    std::vector<docmeta::DocumentMeta> repository;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    DocStore store = DocStore(&crawler_found_pages, &pages_in_store, &repository);
    WebCrawler crawler = WebCrawler(store, &crawler_found_pages, "https://zelebrate.xyz");
    Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
    Ranker ranker = Ranker(&pages_in_store, &index);

    meter.measure([&crawler, &indexer, &ranker] { return engine::runSearch(crawler, indexer, ranker, "hello"); });
}

void otf_basic_web_single_doc_returned(nonius::chronometer meter) {
    std::string search_engine_path = SEARCHENGINE_TESTING_DIR "/resources/test-pages/search-engine.txt";
    std::string expected_text = helpers::loadFile(search_engine_path);

    std::set<docmeta::DocumentMeta> crawler_found_pages;
    std::set<docmeta::DocumentMeta> pages_in_store;
    std::vector<docmeta::DocumentMeta> repository;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    DocStore store = DocStore(&crawler_found_pages, &pages_in_store, &repository);
    WebCrawler crawler = WebCrawler(store, &crawler_found_pages, "https://zelebrate.xyz");
    Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
    Ranker ranker = Ranker(&pages_in_store, &index);

    meter.measure([&crawler, &indexer, &ranker] { return engine::runSearch(crawler, indexer, ranker, "fully"); });
}

void otf_basic_web_multiple_docs_returned(nonius::chronometer meter) {
    std::string index_path = SEARCHENGINE_TESTING_DIR "/resources/test-pages/index.txt";
    std::string index_text = helpers::loadFile(index_path);
    
    std::string search_engine_path = SEARCHENGINE_TESTING_DIR "/resources/test-pages/search-engine.txt";
    std::string search_engine_text = helpers::loadFile(search_engine_path);

    std::set<docmeta::DocumentMeta> crawler_found_pages;
    std::set<docmeta::DocumentMeta> pages_in_store;
    std::vector<docmeta::DocumentMeta> repository;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    DocStore store = DocStore(&crawler_found_pages, &pages_in_store, &repository);
    WebCrawler crawler = WebCrawler(store, &crawler_found_pages, "https://zelebrate.xyz");
    Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
    Ranker ranker = Ranker(&pages_in_store, &index);

    meter.measure([&crawler, &indexer, &ranker] { return engine::runSearch(crawler, indexer, ranker, "engine"); });
}

int main(int argc, char* argv[]) {
    FLAGS_log_dir = "/tmp";
    google::InitGoogleLogging(argv[0]);

    nonius::configuration cfg;
    // cfg.output_file = "on_the_fly_basic_document_benchmarks.csv";
    nonius::benchmark benchmarks[] = {
        nonius::benchmark("On-The-Fly Basic Web Search: No document returned.", otf_basic_web_nothing_returned),
        nonius::benchmark("On-The-Fly Basic Web Search: A single document is returned.", otf_basic_web_single_doc_returned),
        nonius::benchmark("On-The-Fly Basic Web Search: Multiple documents are found and are sorted in decreasing order of search term appearance.", otf_basic_web_multiple_docs_returned)
    };

    nonius::go(cfg, std::begin(benchmarks), std::end(benchmarks), nonius::standard_reporter());
    // nonius::go(cfg, std::begin(benchmarks), std::end(benchmarks), nonius::csv_reporter());

    return 0;
}

#endif