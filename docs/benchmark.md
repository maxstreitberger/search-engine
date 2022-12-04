<a id="top"></a>
# Benchmarking

Currently, [nonius](https://nonius.io) is used to benchmark the search engine. All benchmarks are placed inside the `benchmarking` directory where they are split up into the different testing components. At the moment, only the two executables are benchmarked (`OnTheFlyDocumentSearch` and `OnTheFlyWebSearch`). The name of the testing executables have the format of `[directory_name]_benchmarking`.

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
4. Go into the compiled component benchmarking directory e.g. `OnTheFlyDocumentSearch`:
   ```
   cd bin/benchmarking/OnTheFlyDocumentSearch
   ```
5. Run the benchmarking executable:
    ```
    ./OnTheFlyDocumentSearch_benchmarking
    ```