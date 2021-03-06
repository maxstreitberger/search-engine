#include <catch2/catch_test_macros.hpp>
#include "web_crawler.hpp"
#include "helpers.hpp"

TEST_CASE("Web Crawler can retrieve HTML page from given URL.", "[web-crawler]") {
    WebCrawler crawler = WebCrawler();
    std::string htmlDoc = crawler.getHTML("https://www.zelebrate.xyz");
    REQUIRE_FALSE(htmlDoc.empty());
}

TEST_CASE("Web Crawler can extract URLs from HTML and add them to a queue.", "[web-crawler]") {
    std::string path = SEARCHENGINE_TESTING_DIR "/resources/test-pages/index.txt";
    std::string htmlDoc = helpers::loadFile(path);
    std::queue<std::string> expectedURLs;
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

TEST_CASE("Web Crawler can extract the base URL from the given path.", "[web-crawler]") {
    WebCrawler crawler = WebCrawler("http://www.example.com/something/somethingelse");
    REQUIRE(crawler.base_url == "http://www.example.com");
}

TEST_CASE("Web Crawler can remove all HTML tags.", "[web-crawler]") {
    std::string html_doc = "<header><h1>Test Page</h1><p>This is a test page filled with common HTML elements to be used to provide visual feedback whilst building CSS systems and frameworks.</p></header>";

    WebCrawler crawler = WebCrawler();
    std::string retrunedText = crawler.removeTags(html_doc);

    REQUIRE(retrunedText == " Test Page This is a test page filled with common HTML elements to be used to provide visual feedback whilst building CSS systems and frameworks.");
}

TEST_CASE("Web Crawler can register a page.", "[web-crawler]") {
    std::string html_doc = "<header><h1>Test Page</h1><p>This is a test page filled with common HTML elements to be used to provide visual feedback whilst building CSS systems and frameworks.</p></header>";
    std::set<docmeta::DocumentMeta> expectedPages = { 
        docmeta::DocumentMeta(1, " Test Page This is a test page filled with common HTML elements to be used to provide visual feedback whilst building CSS systems and frameworks.", "https://zelebrate.xyz") 
    };

    std::set<docmeta::DocumentMeta> foundPages;
    WebCrawler crawler = WebCrawler(&foundPages);
    crawler.registerPage("https://zelebrate.xyz", html_doc);

    REQUIRE(foundPages == expectedPages);
}

TEST_CASE("Web Crawler can identify and update an already registered page.", "[web-crawler]") {
    std::string html_doc = "<header><h1>Test Page</h1><p>This is a test page filled with common HTML elements to be used to provide visual feedback whilst building CSS systems and frameworks.</p></header>";
    std::set<docmeta::DocumentMeta> expectedPages_BeforeUpdating = { 
        docmeta::DocumentMeta(1, " Test Page This is a test page filled with common HTML elements to be used to provide visual feedback whilst building CSS systems and frameworks.", "https://zelebrate.xyz") 
    };

    std::string html_doc_updated = "<header><h1>Hello World Page</h1><p>This is a test page filled with common HTML elements to be used to provide visual feedback whilst building CSS systems and frameworks.</p></header>";
    std::set<docmeta::DocumentMeta> expectedPages_AfterUpdating = { 
        docmeta::DocumentMeta(1, " Hello World Page This is a test page filled with common HTML elements to be used to provide visual feedback whilst building CSS systems and frameworks.", "https://zelebrate.xyz") 
    };

    std::set<docmeta::DocumentMeta> foundPages;
    WebCrawler crawler = WebCrawler(&foundPages);
    crawler.registerPage("https://zelebrate.xyz", html_doc);
    CHECK(foundPages == expectedPages_BeforeUpdating);

    crawler.registerPage("https://zelebrate.xyz", html_doc_updated);
    CHECK(foundPages == expectedPages_AfterUpdating);
}

TEST_CASE("Web Crawler can recognize 'https://zelebrate.xyz' as a valid URL", "[web-crawler]") {
    WebCrawler crawler = WebCrawler();
    std::string url = "https://zelebrate.xyz";
    bool isValidURL = crawler.checkIfURL(&url);
    REQUIRE(isValidURL);
}

TEST_CASE("Web Crawler can recognize 'abc' as an invalid URL", "[web-crawler]") {
    WebCrawler crawler = WebCrawler();
    std::string url = "abc";
    bool isValidURL = crawler.checkIfURL(&url);
    REQUIRE_FALSE(isValidURL);
}

TEST_CASE("Web Crawler can recognize ' ' as an invalid URL", "[web-crawler]") {
    WebCrawler crawler = WebCrawler();
    std::string url = " ";
    bool isValidURL = crawler.checkIfURL(&url);
    REQUIRE_FALSE(isValidURL);
}

TEST_CASE("Web Crawler can recognize 'abc://helloworld.abc' as an invalid URL", "[web-crawler]") {
    WebCrawler crawler = WebCrawler();
    std::string url = "abc://helloworld.abc";
    bool isValidURL = crawler.checkIfURL(&url);
    REQUIRE_FALSE(isValidURL);
}

TEST_CASE("Web Crawler can recognize 'https://zelebrate.xyz/helloworld' as an valid URL", "[web-crawler]") {
    WebCrawler crawler = WebCrawler();
    std::string url = "https://zelebrate.xyz/helloworld";
    bool isValidURL = crawler.checkIfURL(&url);
    REQUIRE(isValidURL);
}

TEST_CASE("Web Crawler can recognize 'https://www.zelebrate.xyz' as an valid URL", "[web-crawler]") {
    WebCrawler crawler = WebCrawler();
    std::string url = "https://www.zelebrate.xyz";
    bool isValidURL = crawler.checkIfURL(&url);
    REQUIRE(isValidURL);
}
