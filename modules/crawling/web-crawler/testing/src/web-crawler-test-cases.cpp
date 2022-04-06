#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "web_crawler.hpp"
#include "../../../../../include/helpers.hpp"

TEST_CASE("Web Crawler can", "[web-crawler]") {
    SECTION("retrieve HTML page from given URL.") {
        WebCrawler crawler = WebCrawler();
        std::string htmlDoc = crawler.getHTML("https://www.zelebrate.xyz");
        REQUIRE_FALSE(htmlDoc.empty());
    }

    SECTION("extract URLs from HTML and add them to a queue.") {
        std::string path = WEB_CRAWLER_TESTING_ROOT_DIR "/test-html-pages/index.html";
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
        std::set<docmeta::DocumentMeta> expectedPages = { 
            docmeta::DocumentMeta(1, " Test Page This is a test page filled with common HTML elements to be used to provide visual feedback whilst building CSS systems and frameworks.", "https://zelebrate.xyz") 
        };

        std::set<docmeta::DocumentMeta> foundPages;
        WebCrawler crawler = WebCrawler(&foundPages);
        crawler.registerPage("https://zelebrate.xyz", html_doc);


        REQUIRE(foundPages == expectedPages);
    }

    SECTION("identify and update an already registered page.") {
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

    SECTION("recognize 'https://zelebrate.xyz' as valid URL") {
        WebCrawler crawler = WebCrawler();
        std::string url = "https://zelebrate.xyz";
        bool isValidURL = crawler.checkIfURL(&url);
        REQUIRE(isValidURL);
    }

    SECTION("recognize 'abc' as invalid URL") {
        WebCrawler crawler = WebCrawler();
        std::string url = "abc";
        bool isValidURL = crawler.checkIfURL(&url);
        REQUIRE_FALSE(isValidURL);
    }

    SECTION("recognize ' ' as invalid URL") {
        WebCrawler crawler = WebCrawler();
        std::string url = " ";
        bool isValidURL = crawler.checkIfURL(&url);
        REQUIRE_FALSE(isValidURL);
    }

    SECTION("recognize 'abc://helloworld.abc' as invalid URL") {
        WebCrawler crawler = WebCrawler();
        std::string url = "abc://helloworld.abc";
        bool isValidURL = crawler.checkIfURL(&url);
        REQUIRE_FALSE(isValidURL);
    }

    SECTION("recognize 'https://zelebrate.xyz/helloworld' as valid URL") {
        WebCrawler crawler = WebCrawler();
        std::string url = "https://zelebrate.xyz/helloworld";
        bool isValidURL = crawler.checkIfURL(&url);
        REQUIRE(isValidURL);
    }

    SECTION("recognize 'https://www.zelebrate.xyz' as valid URL") {
        WebCrawler crawler = WebCrawler();
        std::string url = "https://www.zelebrate.xyz";
        bool isValidURL = crawler.checkIfURL(&url);
        REQUIRE(isValidURL);
    }

    SECTION("crawl to a specified depth (in this test: only crawl the first page).") {
        std::string indexPath = WEB_CRAWLER_TESTING_ROOT_DIR "/expected-files/expected-index.txt";
        std::string indexText = helpers::loadFile(indexPath);

        std::set<docmeta::DocumentMeta> expected_store = {
            docmeta::DocumentMeta(1, indexText, "https://zelebrate.xyz"),
        };

        std::set<docmeta::DocumentMeta> crawler_found_pages;
        std::set<docmeta::DocumentMeta> pages_in_store;
        std::vector<docmeta::DocumentMeta> repository;

        DocStore store = DocStore(&crawler_found_pages, &pages_in_store, &repository);                                   // <---- Injecting Doc_Store
        WebCrawler crawler = WebCrawler(store, &crawler_found_pages, "https://zelebrate.xyz", 1);
        crawler.start();

        REQUIRE( pages_in_store == expected_store );
    }
}