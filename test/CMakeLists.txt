add_executable(test
    main.cc
    forward_list.cc
    list.cc
)

target_link_libraries(test
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    spdlog::spdlog
    ${Boost_LIBRARIES}
)
