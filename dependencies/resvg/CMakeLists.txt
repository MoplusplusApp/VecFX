message(STATUS "Fetching resvg sources...")
message(STATUS "This can take several minutes, give it time.")

include(CMakePrintHelpers)
FetchContent_MakeAvailable(resvg)

set(RESVG_DIR ${resvg_SOURCE_DIR})
cmake_print_variables(RESVG_DIR)
message(STATUS "resvg successfully downloaded, building resvg...")
execute_process(COMMAND cargo build --release --manifest-path ${RESVG_DIR}/Cargo.toml)
