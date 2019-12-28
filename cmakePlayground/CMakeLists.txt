add_executable(tzz main.c)
add_subdirectory(mod1 mod1Dir)
target_link_libraries(tzz tzzMod1)
