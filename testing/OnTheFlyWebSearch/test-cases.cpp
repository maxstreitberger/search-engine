#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "engine.hpp"
#include "helpers.hpp"
#include "web_crawler.hpp"

std::string specialCharsPath = SEARCHENGINE_TESTING_DIR "/resources/special.txt";
std::string stopwordsPath = SEARCHENGINE_TESTING_DIR "/resources/stopwords.txt";

TEST_CASE("On-The-Fly Web Search: No document returned.", "[on-the-fly][web][e2e]") {
    std::set<docmeta::DocumentMeta> crawler_found_pages;
    std::set<docmeta::DocumentMeta> pages_in_store;
    std::vector<docmeta::DocumentMeta> repository;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    DocStore store = DocStore(&crawler_found_pages, &pages_in_store, &repository);
    WebCrawler crawler = WebCrawler(store, &crawler_found_pages, "https://zelebrate.xyz");
    Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
    Ranker ranker = Ranker(&pages_in_store, &index);

    std::vector<docmeta::DocumentMeta> foundPages = engine::runSearch(crawler, indexer, ranker, "hello");
    REQUIRE( foundPages.empty() );

    crawler_found_pages = {};
    pages_in_store = {};
    repository = {};
    index = {};
    BENCHMARK("On-The-Fly Web Search: No document returned.") {
        return engine::runSearch(crawler, indexer, ranker, "hello");
    };
}

TEST_CASE("On-The-Fly Web Search: A single document is returned.", "[on-the-fly][web][e2e]") {
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

    std::vector<docmeta::DocumentMeta> foundPages = engine::runSearch(crawler, indexer, ranker, "fully");
    REQUIRE( !foundPages.empty() );
    REQUIRE( foundPages[0].content == expected_text );

    crawler_found_pages = {};
    pages_in_store = {};
    repository = {};
    index = {};
    BENCHMARK("On-The-Fly Web Search: A single document is returned.") {
        return engine::runSearch(crawler, indexer, ranker, "fully");
    };
}

TEST_CASE("On-The-Fly Web Search: Multiple documents are found and are sorted in decreasing order of search term appearance.", "[on-the-fly][web][e2e]") {
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

    std::vector<docmeta::DocumentMeta> foundPages = engine::runSearch(crawler, indexer, ranker, "engine");
    REQUIRE( !foundPages.empty() );
    REQUIRE( foundPages[0].content == search_engine_text );
    REQUIRE( foundPages[1].content == index_text );

    crawler_found_pages = {};
    pages_in_store = {};
    repository = {};
    index = {};
    BENCHMARK("On-The-Fly Web Search: Multiple documents are found and are sorted in decreasing order of search term appearance.") {
        return engine::runSearch(crawler, indexer, ranker, "engine");
    };
}
