// Gets XOR of two numbers
// RAM[5] = RAM[3] XOR RAM[4]
// (A OR B) AND (A NAND B) 
    @R3
    D=M 
    @R4
    D=D&M   

    @R10
    M=!D      
// RAM[10] <- (A NAND B)

    @R4
    D=M 
    @R3
    D=D|M  
// D = (A OR B)

    @R10
    D=D&M 
    
    @R5
    M=D
(INFINITE_LOOP)
@INFINITE_LOOP
0;JMP
