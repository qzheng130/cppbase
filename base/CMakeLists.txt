file(GLOB base_SRCS "*.cc" "*.cpp" "*.cxx")
file(GLOB base_HEADERS "*.h" "*.hpp")

add_library(base ${base_SRCS} ${base_HEADERS})
# target_link_libraries(base)
install(TARGETS base DESTINATION lib)
install(FILES ${base_HEADERS} DESTINATION include/base)

add_subdirectory(test)
