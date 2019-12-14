PROJECT(LOX)
set(src_list vmTest.c vm.c memory.c parser.c scanner.c stringObject.c array.c stack.c value.c debug.c table.c)
add_executable(loxVM ${src_list})
