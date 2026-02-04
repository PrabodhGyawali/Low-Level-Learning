set pagination off
set logging file gdb_free_table_full_debug.log
set loggin on 

break main 
run < test_input/test_1401_inserts.txt 

break free_table:2

watch i if i >= 100

continue