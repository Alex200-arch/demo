add_executable(test-bm
                main.cpp
                decimal64.cpp)

target_link_libraries(test-bm benchmark)