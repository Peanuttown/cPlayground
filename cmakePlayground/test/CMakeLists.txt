add_library(testTzz SHARED test.c)
add_subdirectory(../mod2 libmod2)
target_link_libraries(testTzz mod2)
