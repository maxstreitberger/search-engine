#include <catch2/catch_test_macros.hpp>
#include "web_crawler.hpp"
#include "helpers.hpp"

TEST_CASE("Web crawler end-to-end test.", "[crawler, e2e]") {
    std::string indexPath = SEARCHENGINE_TESTING_DIR "/resources/test-pages/index.txt";
    std::string indexText = helpers::loadFile(indexPath);

    std::string searchEnginePath = SEARCHENGINE_TESTING_DIR "/resources/test-pages/search-engine.txt";
    std::string searchEngineText = helpers::loadFile(searchEnginePath);

    std::set<docmeta::DocumentMeta> expected_store = {
        docmeta::DocumentMeta(1, indexText, "https://zelebrate.xyz"),
        docmeta::DocumentMeta(2, searchEngineText, "https://zelebrate.xyz/searchengine.html"),
    };

    std::vector<docmeta::DocumentMeta> expected_repository = {
        docmeta::DocumentMeta(1, indexText, "https://zelebrate.xyz"),
        docmeta::DocumentMeta(2, searchEngineText, "https://zelebrate.xyz/searchengine.html"),
    };

    std::set<docmeta::DocumentMeta> crawler_found_pages;
    std::set<docmeta::DocumentMeta> pages_in_store;
    std::vector<docmeta::DocumentMeta> repository;

    DocStore store = DocStore(&crawler_found_pages, &pages_in_store, &repository);                                   // <---- Injecting Doc_Store
    WebCrawler crawler = WebCrawler(store, &crawler_found_pages, "https://zelebrate.xyz");
    crawler.start();

    REQUIRE( pages_in_store == expected_store );
    REQUIRE( repository == expected_repository );
}
