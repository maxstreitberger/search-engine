#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "crawler.hpp"

TEST_CASE("Crawler can", "[crawler][store]") {
    std::ofstream file(CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt");
    file << "Hello, World!";
    file.close();

    SECTION("Crawler can retrieve all documents paths in a given folder.") {
        std::vector<std::string> expectedDocuments = { 
            CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt", 
            CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo2.txt", 
            CRAWLER_TESTING_ROOT_DIR "/testing-documents/test-folder/demo3.txt" 
        };

        Crawler crawler = Crawler(CRAWLER_TESTING_ROOT_DIR "/testing-documents");

        std::vector<std::string> visitedDocuments = crawler.getDocumentPaths();

        REQUIRE( expectedDocuments == visitedDocuments );
    }

    SECTION("Crawler can retrieve the contents of a given document") {
        std::string expectedContents = "Hello, World!";

        Crawler crawler = Crawler();

        std::string visitedContents = crawler.getDocumentContents(CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt");

        REQUIRE( expectedContents == visitedContents );
    }

    SECTION("Crawler can register a new document") {
        std::set<docmeta::CrawlerDocMeta> expectedMeta = { docmeta::CrawlerDocMeta(1, "Hello, World!", CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt") };

        Crawler crawler = Crawler();
        crawler.registerDocument(CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt");

        REQUIRE( expectedMeta == crawler.documents );
    }

    SECTION("Crawler can identify and update an already registered document") {
        std::set<docmeta::CrawlerDocMeta> expectedMeta = { docmeta::CrawlerDocMeta(1, "Hello, World!", CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt") };

        Crawler crawler = Crawler();
        crawler.registerDocument(CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt");
        CHECK(expectedMeta == crawler.documents);

        std::ofstream file(CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt");
        file << "What's up?";
        file.close();

        crawler.registerDocument(CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt");

        CHECK( expectedMeta != crawler.documents );
        REQUIRE (crawler.documents.begin()->content == "What's up?" );
    }

    SECTION("Crawler can push found documents to the store") {
        std::set<docmeta::CrawlerDocMeta> documentsToPush = { docmeta::CrawlerDocMeta(1, "Hello, World!", CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt") };

        nlohmann::json expectedOutput = documentsToPush;

        Crawler crawler = Crawler();
        crawler.pushDocumentsToStore(documentsToPush);

        std::ifstream file("crawler-found-docs.json");
        std::string resultOutput((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
        file.close();


        REQUIRE( expectedOutput.dump() == resultOutput );
    }
}