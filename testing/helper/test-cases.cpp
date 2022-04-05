#include <catch2/catch_test_macros.hpp>
#include "helpers.hpp"

TEST_CASE("Can remove white space infront of a text", "[on-the-fly, basic, web, e2e]") {
    std::string text = "  hello";
    std::string expectedText = "hello";
    helpers::ltrim(text);
    REQUIRE( text == expectedText );
}

TEST_CASE("Can remove white space behind a text", "[on-the-fly, basic, web, e2e]") {
    std::string text = "hello  ";
    std::string expectedText = "hello";
    helpers::rtrim(text);
    REQUIRE( text == expectedText );
}

TEST_CASE("Can remove white space around a text", "[on-the-fly, basic, web, e2e]") {
    std::string text = "  hello  ";
    std::string expectedText = "hello";
    helpers::trim(text);
    REQUIRE( text == expectedText );
}

TEST_CASE("Can make a text to lowercase", "[on-the-fly, basic, web, e2e]") {
    std::string text = "HELLO";
    std::string expectedText = "hello";
    helpers::toLower(text);
    REQUIRE( text == expectedText );
}

TEST_CASE("Can load text", "[on-the-fly, basic, web, e2e]") {
    std::string filePath = SEARCHENGINE_TESTING_DIR "/helper/test-file.txt";
    std::string returnedText = helpers::loadFile(filePath);
    REQUIRE( returnedText == "Hello, World!" );
}