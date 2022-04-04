#include <catch2/catch_test_macros.hpp>
#include "engine.hpp"
#include "helpers.hpp"
#include "web_crawler.hpp"

TEST_CASE("On-The-Fly Basic Web Search", "[on-the-fly, basic, web, e2e]") {
    SECTION("No document returned.") {
        std::set<docmeta::DocumentMeta> document_store;
        std::vector<docmeta::DocumentMeta> repository;
        std::map<std::string, std::set<tokenmeta::TokenMeta>> index;
        
        std::string specialCharsPath = INDEXING_ROOT_DIR "/documents/special.txt";
        std::string stopwordsPath = INDEXING_ROOT_DIR "/documents/stopwords.json";

        WebCrawler crawler = WebCrawler(&document_store, &repository, "https://www.zelebrate.xyz");
        Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
        Ranker ranker = Ranker(&document_store, &index);

        std::vector<docmeta::DocumentMeta> foundDocuments = engine::runSearch(crawler, indexer, ranker, "hello");
        REQUIRE( foundDocuments.empty() );
    }

    SECTION("A single document is returned.") {
        std::string search_engine_path = SEARCHENGINE_TESTING_DIR "/on_the_fly_basic_web/resources/test-documents/search-engine.txt";
        std::string expected_text = helpers::loadFile(search_engine_path);

        std::set<docmeta::DocumentMeta> document_store;
        std::vector<docmeta::DocumentMeta> repository;
        std::map<std::string, std::set<tokenmeta::TokenMeta>> index;
        
        std::string specialCharsPath = INDEXING_ROOT_DIR "/documents/special.txt";
        std::string stopwordsPath = INDEXING_ROOT_DIR "/documents/stopwords.json";

        WebCrawler crawler = WebCrawler(&document_store, &repository, "https://www.zelebrate.xyz");
        Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
        Ranker ranker = Ranker(&document_store, &index);

        std::vector<docmeta::DocumentMeta> foundDocuments = engine::runSearch(crawler, indexer, ranker, "fully");
        REQUIRE( !foundDocuments.empty() );
        REQUIRE( foundDocuments[0].content == expected_text );
    }

    SECTION("Multiple documents are found and are sorted in decreasing order of search term appearance.") {
        std::string index_path = SEARCHENGINE_TESTING_DIR "/on_the_fly_basic_web/resources/test-documents/index.txt";
        std::string index_text = helpers::loadFile(index_path);
        
        std::string search_engine_path = SEARCHENGINE_TESTING_DIR "/on_the_fly_basic_web/resources/test-documents/search-engine.txt";
        std::string search_engine_text = helpers::loadFile(search_engine_path);

        std::set<docmeta::DocumentMeta> document_store;
        std::vector<docmeta::DocumentMeta> repository;
        std::map<std::string, std::set<tokenmeta::TokenMeta>> index;
        
        std::string specialCharsPath = INDEXING_ROOT_DIR "/documents/special.txt";
        std::string stopwordsPath = INDEXING_ROOT_DIR "/documents/stopwords.json";

        WebCrawler crawler = WebCrawler(&document_store, &repository, "https://www.zelebrate.xyz");
        Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
        Ranker ranker = Ranker(&document_store, &index);

        std::vector<docmeta::DocumentMeta> foundDocuments = engine::runSearch(crawler, indexer, ranker, "engine");
        REQUIRE( !foundDocuments.empty() );
        REQUIRE( foundDocuments[0].content == search_engine_text );
        REQUIRE( foundDocuments[1].content == index_text );
    }
}