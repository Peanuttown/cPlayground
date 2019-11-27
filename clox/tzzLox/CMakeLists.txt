PROJECT(lox)
SET(src_list main.c chunk.c debug.c value.c scanner.c vm.c)
add_executable(lox ${src_list})
