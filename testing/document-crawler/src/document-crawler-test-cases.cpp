#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "doc_crawler.hpp"

TEST_CASE("DocumentCrawler can", "[crawler][store]") {
    std::ofstream file(SEARCHENGINE_TESTING_DIR "/resources/editable-document.txt");
    file << "Hello, World!";
    file.close();

    SECTION("retrieve all documents paths in a given folder.") {
        std::vector<std::string> expectedDocuments = { 
            SEARCHENGINE_TESTING_DIR "/resources/test-documents/doc1.txt", 
            SEARCHENGINE_TESTING_DIR "/resources/test-documents/doc2.txt", 
            SEARCHENGINE_TESTING_DIR "/resources/test-documents/folder1/doc3.txt" 
        };

        DocumentCrawler crawler = DocumentCrawler(SEARCHENGINE_TESTING_DIR "/resources/test-documents");

        std::vector<std::string> visitedDocuments = crawler.getDocumentPaths();

        REQUIRE( expectedDocuments == visitedDocuments );

        BENCHMARK("retrieve all documents paths in a given folder.") {
            return crawler.getDocumentPaths();
        };
    }

    SECTION("register a new document") {
        std::set<docmeta::DocumentMeta> expectedMeta = { docmeta::DocumentMeta(1, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis interdum malesuada nisl, in posuere ligula aliquam et. Vivamus dapibus sapien a maximus suscipit. Maecenas id viverra mauris. Phasellus et lorem quis massa pretium elementum. Donec feugiat diam ut lorem pellentesque, eget eleifend elit iaculis. Mauris vitae libero pulvinar, egestas erat.", SEARCHENGINE_TESTING_DIR "/resources/test-documents/doc1.txt") };
        std::set<docmeta::DocumentMeta> returnedDocs;
        DocumentCrawler crawler = DocumentCrawler(&returnedDocs);
        crawler.registerDocument(SEARCHENGINE_TESTING_DIR "/resources/test-documents/doc1.txt");

        REQUIRE( returnedDocs == expectedMeta );

        returnedDocs = {};
        BENCHMARK("register a new document.") {
            return crawler.registerDocument(SEARCHENGINE_TESTING_DIR "/resources/test-documents/doc1.txt");
        };
    }

    SECTION("identify and update an already registered document") {
        std::set<docmeta::DocumentMeta> expectedMeta = { docmeta::DocumentMeta(1, "Hello, World!", SEARCHENGINE_TESTING_DIR "/resources/editable-document.txt") };
        std::set<docmeta::DocumentMeta> returnedDocs;
        DocumentCrawler crawler = DocumentCrawler(&returnedDocs);
        crawler.registerDocument(SEARCHENGINE_TESTING_DIR "/resources/editable-document.txt");
        REQUIRE( returnedDocs == expectedMeta );

        std::ofstream file(SEARCHENGINE_TESTING_DIR "/resources/editable-document.txt");
        file << "What's up?";
        file.close();

        crawler.registerDocument(SEARCHENGINE_TESTING_DIR "/resources/editable-document.txt");

        REQUIRE( returnedDocs != expectedMeta );
        REQUIRE (returnedDocs.begin()->content == "What's up?" );
    }
}