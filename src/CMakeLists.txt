set(SOURCE_FILES
    swaml_bitquant.cpp)
add_library(swaml_bitquant_lib STATIC ${SOURCE_FILES})
target_include_directories(swaml_bitquant_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)