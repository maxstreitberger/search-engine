#include <catch2/catch_test_macros.hpp>
#include "doc_store.hpp"

TEST_CASE("Document Store end-to-end test.", "[store, e2e]") {
    SECTION("Unique documents") {
        std::set<docmeta::DocumentMeta> crawler_docs = {
            docmeta::DocumentMeta(1, "Hello, World!", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt"),
            docmeta::DocumentMeta(2, "Test!", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo2.txt")
        };

        std::set<docmeta::DocumentMeta> expected_store = {
            docmeta::DocumentMeta(1, "Hello, World!", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt"),
            docmeta::DocumentMeta(2, "Test!", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo2.txt")
        };

        std::vector<docmeta::DocumentMeta> expected_repository = {
            docmeta::DocumentMeta(1, "Hello, World!", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt"),
            docmeta::DocumentMeta(2, "Test!", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo2.txt")
        };

        std::set<docmeta::DocumentMeta> document_store;
        std::vector<docmeta::DocumentMeta> repository;

        DocStore store = DocStore(crawler_docs, &document_store, &repository);
        store.processDocuments();

        REQUIRE( document_store == expected_store);
        REQUIRE( repository == expected_repository);
    }

    SECTION("Updated document") {
        std::set<docmeta::DocumentMeta> crawler_docs = {
            docmeta::DocumentMeta(2, "Test!", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt")
        };

        std::set<docmeta::DocumentMeta> expected_store = {
            docmeta::DocumentMeta(1, "Test!", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt")
        };

        std::vector<docmeta::DocumentMeta> expected_repository = {
            docmeta::DocumentMeta(1, "Test!", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt")
        };

        std::set<docmeta::DocumentMeta> document_store = {
            docmeta::DocumentMeta(1, "Hello, World!", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt"),
        };
        std::vector<docmeta::DocumentMeta> repository;

        DocStore store = DocStore(crawler_docs, &document_store, &repository);
        store.processDocuments();

        REQUIRE( document_store == expected_store);
        REQUIRE( repository == expected_repository);
    }
    
    SECTION("Nothing to change") {
        std::set<docmeta::DocumentMeta> crawler_docs = {
            docmeta::DocumentMeta(2, "Test!", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt")
        };

        std::set<docmeta::DocumentMeta> expected_store = {
            docmeta::DocumentMeta(2, "Test!", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt")
        };

        std::set<docmeta::DocumentMeta> document_store = {
            docmeta::DocumentMeta(2, "Test!", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt")
        };
        std::vector<docmeta::DocumentMeta> repository;

        DocStore store = DocStore(crawler_docs, &document_store, &repository);
        store.processDocuments();

        REQUIRE( document_store == expected_store);
        REQUIRE( repository.empty() );
    }
}
