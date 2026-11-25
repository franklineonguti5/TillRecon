cmake_minimum_required(VERSION 3.25)

project(
    TillRecon
    VERSION 0.1.0
    DESCRIPTION "Retail Financial Reconciliation Engine"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
include(FetchContent)

add_library(tillrecon_domain STATIC

    src/domain/currency.cpp
    src/domain/money.cpp
    src/domain/transaction.cpp
)

target_include_directories(tillrecon_domain
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(MSVC)

    target_compile_options(tillrecon_domain
        PRIVATE
            /W4
            /permissive-
    )

else()

    target_compile_options(tillrecon_domain
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wsign-conversion
    )

endif()

add_library(tillrecon_ingestion STATIC
    src/ingestion/source_record_identity.cpp
    src/ingestion/raw_source_record.cpp
)

target_include_directories(tillrecon_ingestion
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(tillrecon_ingestion
    PUBLIC
        tillrecon_domain
)

if(MSVC)

    target_compile_options(tillrecon_ingestion
        PRIVATE
            /W4
            /permissive-
    )

else()

    target_compile_options(tillrecon_ingestion
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wsign-conversion
    )

endif()

if(BUILD_TESTING)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    add_executable(
        tillrecon_domain_tests
        tests/unit/domain_tests.cpp
    )

    target_link_libraries(
        tillrecon_domain_tests
        PRIVATE
            tillrecon_domain
            GTest::gtest_main
    )

    include(GoogleTest)

    gtest_discover_tests(tillrecon_domain_tests)

    add_executable(
        tillrecon_ingestion_tests
        tests/unit/ingestion_tests.cpp
    )

    target_link_libraries(
        tillrecon_ingestion_tests
        PRIVATE
            tillrecon_ingestion
            GTest::gtest_main
    )

    gtest_discover_tests(tillrecon_ingestion_tests)

    add_executable(
        tillrecon_raw_source_record_tests
        tests/unit/raw_source_record_tests.cpp
    )

    target_link_libraries(
        tillrecon_raw_source_record_tests
        PRIVATE
            tillrecon_ingestion
            GTest::gtest_main
    )

    gtest_discover_tests(tillrecon_raw_source_record_tests)

endif()