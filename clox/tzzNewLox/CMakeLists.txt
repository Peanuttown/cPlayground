set(src_list 
	main.c 
	ioutil.c
	memory.c
	chunk.c
	vm.c
	scanner.c 
	compile.c
	object.c
	array.c
	value.c
	debug.c
	)
add_compile_options(-g)
add_executable(lox ${src_list})
