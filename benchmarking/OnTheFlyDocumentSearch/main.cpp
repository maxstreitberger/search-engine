#ifndef ON_THE_FLY_DOCUMENT_SEARCH_BENCHMARKING_MAIN_CPP
#define ON_THE_FLY_DOCUMENT_SEARCH_BENCHMARKING_MAIN_CPP

#include "nonius.hpp"
#include "engine.hpp"
#include "helpers.hpp"
#include "doc_crawler.hpp"
#include <glog/logging.h>

std::string specialCharsPath = SEARCHENGINE_TESTING_DIR "/resources/special.txt";
std::string stopwordsPath = SEARCHENGINE_TESTING_DIR "/resources/stopwords.txt";

void otf_doc_nothing_returned(nonius::chronometer meter) {
    std::set<docmeta::DocumentMeta> crawler_found_documents;
    std::set<docmeta::DocumentMeta> documents_in_store;
    std::vector<docmeta::DocumentMeta> repository;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    DocStore store = DocStore(&crawler_found_documents, &documents_in_store, &repository);
    DocumentCrawler crawler = DocumentCrawler(store, &crawler_found_documents, SEARCHENGINE_TESTING_DIR "/resources/test-documents");
    Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
    Ranker ranker = Ranker(&documents_in_store, &index);

    meter.measure([&crawler, &indexer, &ranker] { return engine::runSearch(crawler, indexer, ranker, "test"); });
}

void otf_doc_single_doc_returned(nonius::chronometer meter) {
    std::string doc1_path = SEARCHENGINE_TESTING_DIR "/resources/test-documents/doc1.txt";
    std::string expected_text = helpers::loadFile(doc1_path);

    std::set<docmeta::DocumentMeta> crawler_found_documents;
    std::set<docmeta::DocumentMeta> documents_in_store;
    std::vector<docmeta::DocumentMeta> repository;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    DocStore store = DocStore(&crawler_found_documents, &documents_in_store, &repository);
    DocumentCrawler crawler = DocumentCrawler(store, &crawler_found_documents, SEARCHENGINE_TESTING_DIR "/resources/test-documents");
    Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
    Ranker ranker = Ranker(&documents_in_store, &index);

    meter.measure([&crawler, &indexer, &ranker] { return engine::runSearch(crawler, indexer, ranker, "quis"); });
}

void otf_doc_multiple_docs_returned(nonius::chronometer meter) {
    std::string doc1_path = SEARCHENGINE_TESTING_DIR "/resources/test-documents/doc1.txt";
    std::string doc2_path = SEARCHENGINE_TESTING_DIR "/resources/test-documents/doc2.txt";
    std::string doc3_path = SEARCHENGINE_TESTING_DIR "/resources/test-documents/folder1/doc3.txt";

    std::string doc1_text = helpers::loadFile(doc1_path);
    std::string doc2_text = helpers::loadFile(doc2_path);
    std::string doc3_text = helpers::loadFile(doc3_path);

    std::set<docmeta::DocumentMeta> crawler_found_documents;
    std::set<docmeta::DocumentMeta> documents_in_store;
    std::vector<docmeta::DocumentMeta> repository;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    DocStore store = DocStore(&crawler_found_documents, &documents_in_store, &repository);
    DocumentCrawler crawler = DocumentCrawler(store, &crawler_found_documents, SEARCHENGINE_TESTING_DIR "/resources/test-documents");
    Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
    Ranker ranker = Ranker(&documents_in_store, &index);

    meter.measure([&crawler, &indexer, &ranker] { return engine::runSearch(crawler, indexer, ranker, "lorem"); });
}

int main(int argc, char* argv[]) {
    FLAGS_log_dir = "/tmp";
    google::InitGoogleLogging(argv[0]);

    nonius::configuration cfg;
    // cfg.output_file = "OnTheFlyDocumentSearch_benchmarking.csv";
    nonius::benchmark benchmarks[] = {
        nonius::benchmark("On-The-Fly Document Search: No document returned.", otf_doc_nothing_returned),
        nonius::benchmark("On-The-Fly Document Search: A single document is returned.", otf_doc_single_doc_returned),
        nonius::benchmark("On-The-Fly Document Search: Multiple documents are found and are sorted in decreasing order of search term ('lorem') appearance.", otf_doc_multiple_docs_returned)
    };

    nonius::go(cfg, std::begin(benchmarks), std::end(benchmarks), nonius::standard_reporter());
    // nonius::go(cfg, std::begin(benchmarks), std::end(benchmarks), nonius::csv_reporter());

    return 0;
}

#endif