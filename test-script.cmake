set(CTEST_SOURCE_DIRECTORY "${CTEST_SCRIPT_DIRECTORY}")
set(CTEST_BINARY_DIRECTORY "${CTEST_SCRIPT_DIRECTORY}/bin")
set(CTEST_CMAKE_COMMAND "/usr/local/bin/cmake")
set(CTEST_COMMAND "/usr/local/bin/ctest -D Experimental")

ctest_empty_binary_directory("${CTEST_BINARY_DIRECTORY}")

set(CTEST_SITE "${CMAKE_SYSTEM_NAME}")
set(CTEST_BUILD_NAME "experimental build")

set(CTEST_CMAKE_GENERATOR "Unix Makefiles")
set(CTEST_COVERAGE_COMMAND "gcov")

ctest_start("Experimental")
ctest_configure()
ctest_build()
ctest_test()
ctest_coverage()
ctest_submit()