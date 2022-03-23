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

    SECTION("remove all HTML tags.") {
        std::string html_doc = "<header><h1>Test Page</h1><p>This is a test page filled with common HTML elements to be used to provide visual feedback whilst building CSS systems and frameworks.</p></header>";

        WebCrawler crawler = WebCrawler();
        std::string retrunedText = crawler.removeTags(html_doc);


        REQUIRE(retrunedText == " Test Page This is a test page filled with common HTML elements to be used to provide visual feedback whilst building CSS systems and frameworks.");
    }

    SECTION("register a page.") {
        std::string html_doc = "<header><h1>Test Page</h1><p>This is a test page filled with common HTML elements to be used to provide visual feedback whilst building CSS systems and frameworks.</p></header>";
        std::set<pagemeta::PageMeta> expectedPages = { 
            pagemeta::PageMeta(1, " Test Page This is a test page filled with common HTML elements to be used to provide visual feedback whilst building CSS systems and frameworks.", "https://zelebrate.xyz") 
        };

        WebCrawler crawler = WebCrawler();
        crawler.registerPage("https://zelebrate.xyz", html_doc);


        REQUIRE(crawler.pages == expectedPages);
    }
}