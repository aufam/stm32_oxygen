file(READ version.txt OXYGEN_VERSION)
message("OXYGEN_VERSION : ${OXYGEN_VERSION}")

# sources
file(GLOB_RECURSE OXYGEN_SOURCES oxygen/*.*)
add_library(oxygen ${OXYGEN_SOURCES})

# include dirs
target_include_directories(oxygen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# defines
target_compile_definitions(oxygen PUBLIC -DOXYGEN_VERSION="${OXYGEN_VERSION}")

# depends
target_link_libraries(oxygen etl)
target_link_libraries(oxygen periph)