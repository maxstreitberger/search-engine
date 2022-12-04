<a id="top"></a>
# Testing

All tests are placed inside the `testing` directory where they are split up into the different testing components (e.g. `document-crawler`, `indexer`, `OnTheFlyDocumentSearch`). The name of the testing executables have the format of `[directory_name]_tests`.

## Running
1. Clone the repository: 
   ```
   git clone https://github.com/maxstreitberger/search-engine.git
   ```
2. Generate a buildsystem:
    ```
    cmake -S . -B bin -DSEEKER_DEVELOPMENT_BUILD=YES
    ```
3. Build application:
    ```
    make -C bin [or cmake --build bin]
    ```
4. Go into the compiled component testing directory e.g. `indexer`:
   ```
   cd bin/testing/indexer
   ```
5. Run the testing executable:
    ```
    ./indexer_tests
    ```