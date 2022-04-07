#include <catch2/catch_test_macros.hpp>
#include "doc_crawler.hpp"

TEST_CASE("DocumentCrawler can", "[crawler][store]") {
    std::ofstream file(DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt");
    file << "Hello, World!";
    file.close();

    SECTION("retrieve all documents paths in a given folder.") {
        std::vector<std::string> expectedDocuments = { 
            DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt", 
            DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo2.txt", 
            DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/test-folder/demo3.txt" 
        };

        DocumentCrawler crawler = DocumentCrawler(DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents");

        std::vector<std::string> visitedDocuments = crawler.getDocumentPaths();

        REQUIRE( expectedDocuments == visitedDocuments );
    }

    SECTION("register a new document") {
        std::set<docmeta::DocumentMeta> expectedMeta = { docmeta::DocumentMeta(1, "Hello, World!", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt") };
        std::set<docmeta::DocumentMeta> returnedDocs;
        DocumentCrawler crawler = DocumentCrawler(&returnedDocs);
        crawler.registerDocument(DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt");

        REQUIRE( returnedDocs == expectedMeta );
    }

    SECTION("identify and update an already registered document") {
        std::set<docmeta::DocumentMeta> expectedMeta = { docmeta::DocumentMeta(1, "Hello, World!", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt") };
        std::set<docmeta::DocumentMeta> returnedDocs;
        DocumentCrawler crawler = DocumentCrawler(&returnedDocs);
        crawler.registerDocument(DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt");
        REQUIRE( returnedDocs == expectedMeta );

        std::ofstream file(DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt");
        file << "What's up?";
        file.close();

        crawler.registerDocument(DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt");

        REQUIRE( returnedDocs != expectedMeta );
        REQUIRE (returnedDocs.begin()->content == "What's up?" );
    }
}