file(GLOB base_srcs "*.cc" "*.cpp" "*.cxx")
file(GLOB base_headers "*.h" "*.hpp")

add_library(base ${base_srcs} ${base_headers})

target_link_libraries(base
    PUBLIC
    ${Boost_LIBRARIES}
)

install(TARGETS base DESTINATION lib)
install(FILES ${base_headers} DESTINATION include/base)

add_subdirectory(test)
