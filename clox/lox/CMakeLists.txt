cmake_minimum_required(VERSION 3.10)
project(lox)
set(src_list main.c debug.c chunk.c memory.c value.c vm.c compiler.c scanner.c)
add_executable(lox ${src_list})
