<a id="top"></a>
# Benchmarking

Currently, [nonius](https://nonius.io) is used to benchmark the search engine. All benchmarks are placed inside the `benchmarking` directory where they are split up into the different testing components. At the moment, only the two executables are benchmarked (`on_the_fly_document` and `on_the_fly_web`). The name of the testing executables have the format of `[directory_name]_benchmarking`.

## Running
1. Download repository
2. Generate a buildsystem `cmake -S . -B bin/release`
3. Build application `make` or `cmake --build .`
4. Go into the compiled component benchmarking directory e.g. `cd bin/benchmarking/on_the_fly_document`
4. Run the benchmarking executable e.g. `./on_the_fly_document_benchmarking`