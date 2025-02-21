add_executable(test-gt
                main.cpp
                decimal64.cpp
                decimal_base.cpp)

target_link_libraries(test-gt gtest)