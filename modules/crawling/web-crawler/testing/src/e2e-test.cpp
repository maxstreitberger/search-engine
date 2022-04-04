#include <catch2/catch_test_macros.hpp>
#include "crawler.hpp"
#include "../../../../../include/helpers.hpp"

TEST_CASE("Web crawler end-to-end test.", "[crawler, e2e]") {
    std::string indexPath = WEB_CRAWLER_TESTING_ROOT_DIR "/expected-files/expected-index.txt";
    std::string indexText = helpers::loadFile(indexPath);

    std::string searchEnginePath = WEB_CRAWLER_TESTING_ROOT_DIR "/expected-files/expected-search-engine.txt";
    std::string searchEngineText = helpers::loadFile(searchEnginePath);

    std::set<docmeta::DocumentMeta> expected_store = {
        docmeta::DocumentMeta(1, indexText, "https://zelebrate.xyz"),
        docmeta::DocumentMeta(2, searchEngineText, "https://zelebrate.xyz/searchengine.html"),
    };

    std::vector<docmeta::DocumentMeta> expected_repository = {
        docmeta::DocumentMeta(1, indexText, "https://zelebrate.xyz"),
        docmeta::DocumentMeta(2, searchEngineText, "https://zelebrate.xyz/searchengine.html"),
    };

    std::set<docmeta::DocumentMeta> document_store;
    std::vector<docmeta::DocumentMeta> repository;

    WebCrawler crawler = WebCrawler(&document_store, &repository, "https://zelebrate.xyz");
    crawler.start();

    REQUIRE( document_store == expected_store );
    REQUIRE( repository == expected_repository );
}
