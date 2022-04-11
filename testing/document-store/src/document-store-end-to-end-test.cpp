#include <catch2/catch_test_macros.hpp>
#include "doc_store.hpp"

TEST_CASE("Document Store end-to-end test: Unique documents.", "[store][e2e]") {
    std::set<docmeta::DocumentMeta> crawler_docs = {
        docmeta::DocumentMeta(1, "Hello, World!", "Some Example Path #1"),
        docmeta::DocumentMeta(2, "Test!", "Some Example Path #2")
    };

    std::set<docmeta::DocumentMeta> expected_store = {
        docmeta::DocumentMeta(1, "Hello, World!","Some Example Path #1"),
        docmeta::DocumentMeta(2, "Test!", "Some Example Path #2")
    };

    std::vector<docmeta::DocumentMeta> expected_repository = {
        docmeta::DocumentMeta(1, "Hello, World!","Some Example Path #1"),
        docmeta::DocumentMeta(2, "Test!", "Some Example Path #2")
    };

    std::set<docmeta::DocumentMeta> document_store;
    std::vector<docmeta::DocumentMeta> repository;

    DocStore store = DocStore(&crawler_docs, &document_store, &repository);
    store.processDocuments();

    REQUIRE( document_store == expected_store);
    REQUIRE( repository == expected_repository);
}

TEST_CASE("Document Store end-to-end test: Updated document.", "[store][e2e]") {
    std::set<docmeta::DocumentMeta> crawler_docs = {
        docmeta::DocumentMeta(2, "Test!", "Some Example Path #1")
    };

    std::set<docmeta::DocumentMeta> expected_store = {
        docmeta::DocumentMeta(1, "Test!", "Some Example Path #1")
    };

    std::vector<docmeta::DocumentMeta> expected_repository = {
        docmeta::DocumentMeta(1, "Test!", "Some Example Path #1")
    };

    std::set<docmeta::DocumentMeta> document_store = {
        docmeta::DocumentMeta(1, "Hello, World!", "Some Example Path #1"),
    };
    std::vector<docmeta::DocumentMeta> repository;

    DocStore store = DocStore(&crawler_docs, &document_store, &repository);
    store.processDocuments();

    REQUIRE( document_store == expected_store);
    REQUIRE( repository == expected_repository);
}

TEST_CASE("Document Store end-to-end test: Nothing to change.", "[store][e2e]") {
    std::set<docmeta::DocumentMeta> crawler_docs = {
        docmeta::DocumentMeta(2, "Test!", "Some Example Path #1")
    };

    std::set<docmeta::DocumentMeta> expected_store = {
        docmeta::DocumentMeta(2, "Test!", "Some Example Path #1")
    };

    std::set<docmeta::DocumentMeta> document_store = {
        docmeta::DocumentMeta(2, "Test!", "Some Example Path #1")
    };
    std::vector<docmeta::DocumentMeta> repository;

    DocStore store = DocStore(&crawler_docs, &document_store, &repository);
    store.processDocuments();

    REQUIRE( document_store == expected_store);
    REQUIRE( repository.empty() );
}

