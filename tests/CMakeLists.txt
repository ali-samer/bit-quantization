enable_testing()

if (SWAML_BITQUANT_ENABLE_TESTS)
    find_package(GTest REQUIRED)
    include(GoogleTest)

    add_executable(swaml_bitquant_test swaml_bitquant.cpp)
    target_link_libraries(swaml_bitquant_test
            PRIVATE GTest::GTest GTest::Main
            PRIVATE swaml_bitquant_lib)

    gtest_discover_tests(swaml_bitquant_test)
endif ()