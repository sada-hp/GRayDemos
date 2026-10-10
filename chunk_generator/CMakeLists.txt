include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)

file(GLOB_RECURSE CODE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_library(chunk_generator STATIC ${CODE_SOURCES})
add_compile_definitions(DEBUG=$<CONFIG:Debug>)

target_compile_definitions(chunk_generator PUBLIC EXPORT_API)
target_precompile_headers(chunk_generator PUBLIC source/dummy.hpp)

set_target_properties(chunk_generator PROPERTIES  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${COMPILE_PATH})
set_target_properties(chunk_generator PROPERTIES  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${COMPILE_PATH})
