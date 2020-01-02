cmake_minimum_required(VERSION 3.10)
project(main)
message(${CMAKE_SOURCE_DIR})
set(src_list 
	main.c
	tree-sitter-json/src/parser.c
	~/projects/cPlayground/clox/tzzNewLox/ioutil.c
	tree-sitter-go/src/parser.c
	)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(~/projects/cPlayground/clox/tzzNewLox)
link_directories(${CMAKE_SOURCE_DIR})
add_executable(main ${src_list})
target_link_libraries(main tree-sitter)
