#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "crawler.hpp"

TEST_CASE("Web Crawler can", "[web-crawler]") {
    SECTION("retrieve HTML page from given URL.") {
        WebCrawler crawler = WebCrawler();
        std::string htmlDoc = crawler.getHTML("https://www.zelebrate.xyz");
        REQUIRE_FALSE(htmlDoc.empty());
    }

    SECTION("extract URLs from HTML and add them to a queue.") {
        std::ifstream exampleFile(WEB_CRAWLER_TESTING_ROOT_DIR "/test-html-pages/index.html");
        std::string htmlDoc((std::istreambuf_iterator<char>(exampleFile)), (std::istreambuf_iterator<char>()));
        std::queue<std::string> expectedURLs;
        expectedURLs.push("https://github.com/maxstreitberger/search-engine");
        expectedURLs.push("http://www.example.com/searchengine.html");

        WebCrawler crawler = WebCrawler("http://www.example.com");
        std::queue<std::string> retrievedURLs;
        crawler.getURLs(htmlDoc, &retrievedURLs);

        while (!retrievedURLs.empty()) {
            std::string retrieved = retrievedURLs.front();
            retrievedURLs.pop();

            std::string expected = expectedURLs.front();
            expectedURLs.pop();

            REQUIRE(retrieved == expected);            
        }
    }

    SECTION("extract the base URL from the given path.") {
        WebCrawler crawler = WebCrawler("http://www.example.com/something/somethingelse");
        REQUIRE(crawler.base_url == "http://www.example.com");
    }
}