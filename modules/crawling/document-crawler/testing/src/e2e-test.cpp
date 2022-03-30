#include <catch2/catch_test_macros.hpp>
#include "crawler.hpp"

TEST_CASE("Document crawler end-to-end test.", "[crawler, e2e]") {
    std::set<docmeta::DocumentMeta> expected_store = {
        docmeta::DocumentMeta(1, "What's up?", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt"),
        docmeta::DocumentMeta(2, "Quisque gravida erat at turpis sodales euismod ut vitae justo. Donec sem arcu, sagittis sit amet mollis id, rutrum at velit. Aliquam erat volutpat. Aenean vestibulum dolor eget sem tristique, a rutrum justo pretium. Fusce accumsan dictum venenatis. In pretium nisl dictum leo semper auctor nec nec elit. Proin eget dapibus tellus. Fusce vel metus eget eros tempor semper. Donec vehicula imperdiet condimentum. Vestibulum dolor tortor, commodo sed diam sit amet, euismod blandit dui. Proin dui urna, tincidunt vel ullamcorper sit amet, porttitor id nunc. Nunc ornare bibendum dapibus. Phasellus nec nunc facilisis, imperdiet ipsum ac, tempus ligula. Vestibulum sit amet mi mollis, sagittis quam ut, auctor sem. Proin vel enim in elit porttitor euismod. Proin efficitur sem sem, eget vehicula libero sollicitudin et.", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo2.txt"),
        docmeta::DocumentMeta(3, "Fusce dictum dolor sapien. Donec ullamcorper iaculis ultrices. Donec condimentum dui ipsum, vel mollis eros congue at. Aenean sed accumsan odio, eget maximus neque. Vivamus faucibus lectus odio, a hendrerit augue posuere a. Nam commodo sagittis lacinia. Cras vitae orci at enim aliquet congue. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Duis egestas felis dapibus sapien pulvinar, in imperdiet dui pellentesque. Aliquam sed turpis in libero condimentum suscipit. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi et sapien sit amet ante consequat tristique. Fusce varius enim quis est bibendum fermentum.", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/test-folder/demo3.txt")
    };

    std::vector<docmeta::DocumentMeta> expected_repository = {
        docmeta::DocumentMeta(1, "What's up?", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt"),
        docmeta::DocumentMeta(2, "Quisque gravida erat at turpis sodales euismod ut vitae justo. Donec sem arcu, sagittis sit amet mollis id, rutrum at velit. Aliquam erat volutpat. Aenean vestibulum dolor eget sem tristique, a rutrum justo pretium. Fusce accumsan dictum venenatis. In pretium nisl dictum leo semper auctor nec nec elit. Proin eget dapibus tellus. Fusce vel metus eget eros tempor semper. Donec vehicula imperdiet condimentum. Vestibulum dolor tortor, commodo sed diam sit amet, euismod blandit dui. Proin dui urna, tincidunt vel ullamcorper sit amet, porttitor id nunc. Nunc ornare bibendum dapibus. Phasellus nec nunc facilisis, imperdiet ipsum ac, tempus ligula. Vestibulum sit amet mi mollis, sagittis quam ut, auctor sem. Proin vel enim in elit porttitor euismod. Proin efficitur sem sem, eget vehicula libero sollicitudin et.", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo2.txt"),
        docmeta::DocumentMeta(3, "Fusce dictum dolor sapien. Donec ullamcorper iaculis ultrices. Donec condimentum dui ipsum, vel mollis eros congue at. Aenean sed accumsan odio, eget maximus neque. Vivamus faucibus lectus odio, a hendrerit augue posuere a. Nam commodo sagittis lacinia. Cras vitae orci at enim aliquet congue. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Duis egestas felis dapibus sapien pulvinar, in imperdiet dui pellentesque. Aliquam sed turpis in libero condimentum suscipit. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi et sapien sit amet ante consequat tristique. Fusce varius enim quis est bibendum fermentum.", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/test-folder/demo3.txt")
    };

    std::set<docmeta::DocumentMeta> document_store;
    std::vector<docmeta::DocumentMeta> repository;

    Crawler crawler = Crawler(&document_store, &repository, DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents");
    crawler.start();

    REQUIRE( document_store == expected_store );
    REQUIRE( repository == expected_repository );
}
