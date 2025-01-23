// Rotate to make the LSB of RAM[3] into the MSB, stored at RAM[10]

// Get value of RAM[4]
(LOOP)
@R4
D=M
@infiniteLoop
D;JEQ

@R4
M=D-1

// Store left shift by one to RAM[5]
@R3 
D=M
D=D+M
@R5
M=D

// Check if D is larger than 32767
@R3
D=M
@32767
D=A-D
@save_bit
D;JLT

@end
0;JMP

(save_bit)
@R5
M=M+1
(end)
// Change value of RAM[3] for next shift
@R5
D=M
@R3
M=D

@LOOP
0;JMP

(infiniteLoop)
@infiniteLoop
0;JMP