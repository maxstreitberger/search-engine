#include <catch2/catch_test_macros.hpp>
#include "crawler.hpp"

TEST_CASE("Web crawler end-to-end test.", "[crawler, e2e]") {
    std::ifstream indexFile(WEB_CRAWLER_TESTING_ROOT_DIR "/expected-files/expected-index.txt");
    std::string indexText((std::istreambuf_iterator<char>(indexFile)), (std::istreambuf_iterator<char>()));

    std::ifstream searchEngineFile(WEB_CRAWLER_TESTING_ROOT_DIR "/expected-files/expected-search-engine.txt");
    std::string searchEngineText((std::istreambuf_iterator<char>(searchEngineFile)), (std::istreambuf_iterator<char>()));

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
