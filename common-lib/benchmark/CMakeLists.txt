add_executable(test-bm
                main.cpp
                Decimal64.cpp)

target_link_libraries(test-bm benchmark)