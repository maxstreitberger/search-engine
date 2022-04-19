#include <glog/logging.h>
#include <catch2/catch_session.hpp>

int main( int argc, char* argv[] ) {
    FLAGS_log_dir = "/tmp";
    google::InitGoogleLogging(argv[0]);
    return Catch::Session().run( argc, argv );
}