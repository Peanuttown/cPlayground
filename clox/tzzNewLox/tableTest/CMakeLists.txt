PROJECT(TABLE_TEST)
set(src_list table_test.c ../table.c ../array.c ../memory.c ../value.c ../stringObject.c)
add_executable(test ${src_list})
