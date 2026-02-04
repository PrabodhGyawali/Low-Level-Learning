set pagination off
set logging file gdb_debug.log
set logging on

# Break at main to set up watchpoints
break main
run < test_input/test_1401_inserts.txt

# Set watchpoint on table->num_rows when it gets close to max
watch table->num_rows if table->num_rows >= 1399

# Continue to let it run until watchpoint hits
continue

# When watchpoint hits, show the current state
print table->num_rows

# Show the stack
bt

# Continue to see what happens next
continue