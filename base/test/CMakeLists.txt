add_executable(thread_pool_test thread_pool_test.cc)
target_link_libraries(thread_pool_test
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    spdlog::spdlog
)
