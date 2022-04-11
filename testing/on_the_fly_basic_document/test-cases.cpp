#include <catch2/catch_test_macros.hpp>
#include "engine.hpp"
#include "helpers.hpp"
#include "doc_crawler.hpp"

std::string specialCharsPath = SEARCHENGINE_TESTING_DIR "/resources/special.txt";
std::string stopwordsPath = SEARCHENGINE_TESTING_DIR "/resources/stopwords.txt";

TEST_CASE("On-The-Fly Basic Document Search: No document returned.", "[on-the-fly][basic][document][e2e]") {
    std::set<docmeta::DocumentMeta> crawler_found_documents;
    std::set<docmeta::DocumentMeta> documents_in_store;
    std::vector<docmeta::DocumentMeta> repository;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    DocStore store = DocStore(&crawler_found_documents, &documents_in_store, &repository);
    DocumentCrawler crawler = DocumentCrawler(store, &crawler_found_documents, SEARCHENGINE_TESTING_DIR "/resources/test-documents");
    Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
    Ranker ranker = Ranker(&documents_in_store, &index);

    std::vector<docmeta::DocumentMeta> foundDocuments = engine::runSearch(crawler, indexer, ranker, "test");
    REQUIRE( foundDocuments.empty() );
}

TEST_CASE("On-The-Fly Basic Document Search: A single document is returned.", "[on-the-fly][basic][document][e2e]") {
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

    std::vector<docmeta::DocumentMeta> foundDocuments = engine::runSearch(crawler, indexer, ranker, "quis");
    REQUIRE( !foundDocuments.empty() );
    REQUIRE( foundDocuments[0].content == expected_text );
}

TEST_CASE("On-The-Fly Basic Document Search: Multiple documents are found and are sorted in decreasing order of search term ('lorem') appearance.", "[on-the-fly][basic][document][e2e]") {
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

    std::vector<docmeta::DocumentMeta> foundDocuments = engine::runSearch(crawler, indexer, ranker, "lorem");
    REQUIRE( !foundDocuments.empty() );
    REQUIRE( foundDocuments[0].content == doc1_text );
    REQUIRE( foundDocuments[1].content == doc3_text );
    REQUIRE( foundDocuments[2].content == doc2_text );
}