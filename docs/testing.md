<a id="top"></a>
# Testing

All tests are placed inside the `testing` directory where they are split up into the different testing components (e.g. `document-crawler`, `indexer`, `on_the_fly_document`). The name of the testing executables have the format of `[directory_name]_tests`.

## Running
1. Download repository
2. Generate a buildsystem `cmake -S . -B bin/release`
3. Build application `make` or `cmake --build .`
4. Go into the compiled component testing directory e.g. `cd bin/Testing/indexer`
5. Run the testing executable e.g. `./indexer_tests`
