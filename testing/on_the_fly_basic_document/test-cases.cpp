#include <catch2/catch_test_macros.hpp>
#include "engine.hpp"
#include "helpers.hpp"
#include "crawler.hpp"

TEST_CASE("On-The-Fly Basic Document Search", "[on-the-fly, basic, document, e2e]") {
    SECTION("No document returned.") {
        std::set<docmeta::DocumentMeta> document_store;
        std::vector<docmeta::DocumentMeta> repository;
        std::map<std::string, std::set<tokenmeta::TokenMeta>> index;
        
        std::string specialCharsPath = INDEXING_ROOT_DIR "/documents/special.txt";
        std::string stopwordsPath = INDEXING_ROOT_DIR "/documents/stopwords.json";

        Crawler crawler = Crawler(&document_store, &repository, SEARCHENGINE_TESTING_DIR "/on_the_fly_basic_document/resources/test-documents");
        Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
        Ranker ranker = Ranker(&document_store, &index);

        std::vector<docmeta::DocumentMeta> foundDocuments = engine::runSearch(crawler, indexer, ranker, "test");
        REQUIRE( foundDocuments.empty() );
    }

    SECTION("A single document is returned.") {
        std::string doc1_path = SEARCHENGINE_TESTING_DIR "/on_the_fly_basic_document/resources/test-documents/doc1.txt";
        std::string expected_text = helpers::loadFile(doc1_path);

        std::set<docmeta::DocumentMeta> document_store;
        std::vector<docmeta::DocumentMeta> repository;
        std::map<std::string, std::set<tokenmeta::TokenMeta>> index;
        
        std::string specialCharsPath = INDEXING_ROOT_DIR "/documents/special.txt";
        std::string stopwordsPath = INDEXING_ROOT_DIR "/documents/stopwords.json";

        Crawler crawler = Crawler(&document_store, &repository, SEARCHENGINE_TESTING_DIR "/on_the_fly_basic_document/resources/test-documents");
        Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
        Ranker ranker = Ranker(&document_store, &index);

        std::vector<docmeta::DocumentMeta> foundDocuments = engine::runSearch(crawler, indexer, ranker, "quis");
        REQUIRE( !foundDocuments.empty() );
        REQUIRE( foundDocuments[0].content == expected_text );
    }

    SECTION("Multiple documents are found and are sorted in decreasing order of search term appearance.") {
        std::string doc1_path = SEARCHENGINE_TESTING_DIR "/on_the_fly_basic_document/resources/test-documents/doc1.txt";
        std::string doc2_path = SEARCHENGINE_TESTING_DIR "/on_the_fly_basic_document/resources/test-documents/doc2.txt";
        std::string doc3_path = SEARCHENGINE_TESTING_DIR "/on_the_fly_basic_document/resources/test-documents/folder1/doc3.txt";

        std::string doc1_text = helpers::loadFile(doc1_path);
        std::string doc2_text = helpers::loadFile(doc2_path);
        std::string doc3_text = helpers::loadFile(doc3_path);

        std::set<docmeta::DocumentMeta> document_store;
        std::vector<docmeta::DocumentMeta> repository;
        std::map<std::string, std::set<tokenmeta::TokenMeta>> index;
        
        std::string specialCharsPath = INDEXING_ROOT_DIR "/documents/special.txt";
        std::string stopwordsPath = INDEXING_ROOT_DIR "/documents/stopwords.json";

        Crawler crawler = Crawler(&document_store, &repository, SEARCHENGINE_TESTING_DIR "/on_the_fly_basic_document/resources/test-documents");
        Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
        Ranker ranker = Ranker(&document_store, &index);

        std::vector<docmeta::DocumentMeta> foundDocuments = engine::runSearch(crawler, indexer, ranker, "lorem");
        REQUIRE( !foundDocuments.empty() );
        REQUIRE( foundDocuments[0].content == doc1_text );
        REQUIRE( foundDocuments[1].content == doc3_text );
        REQUIRE( foundDocuments[2].content == doc2_text );
    }
}