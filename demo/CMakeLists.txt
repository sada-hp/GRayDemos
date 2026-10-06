include_directories(${DemoPrj_SOURCE_DIR}/engine/include)
include_directories(${DemoPrj_SOURCE_DIR}/engine/source)

file(GLOB_RECURSE ROOT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

add_executable(demo ${ROOT_SOURCES})
target_link_libraries(demo PUBLIC source)

add_compile_definitions(DEBUG=$<CONFIG:Debug>)
set_target_properties(demo PROPERTIES  RUNTIME_OUTPUT_DIRECTORY ${COMPILE_PATH})
set_target_properties(demo PROPERTIES  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${COMPILE_PATH})
set_target_properties(demo PROPERTIES  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${COMPILE_PATH})