// ---------------- Program Start --------------------- //
@MAIN
0;JMP
// ---------------- Function Declarations ------------- //

(XOR)
    // ------------------ XOR ------------------------ //
    //  Args: R3, R4
    // ----------------------------------------------- //
    //  Out: R5         Return: R8                     //
    //  Reserved: R16
    @R3
    D=M
    @R3
    D=M 
    @R4
    D=D&M
    @R16
    M=!D
    @R4
    D=M 
    @R3
    D=D|M 
    @R16
    D=D&M 
    @R5
    M=D
    // --------------- Return to Caller --------------- //
    @R8         // Return to address 8
    A=M
    0;JMP

(ROTATE)
    // Args: R3, R4     Out: R5     Return to R7        //
    (LOOP)
        @R4
        D=M
        @rotateReturn
        D;JEQ

        @R4
        M=D-1

        // Store left shift by one to R5
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
        @R5
        D=M
        @R3
        M=D

    @LOOP
    0;JMP

    (rotateReturn)
    @R7     // Return to address in R7
    A=M
    0;JMP

(FUNCTION)
    //  Args: R3, R4
    // ------------------------------------------------ //
    //  Out: R5         return: R7                      //
    // ------------------------------------------------ //
    @R4
    M=!M
    @XOR_CALLER_NESTED
    D=A
    @R8
    M=D         // Setting return for XOR
                // R5 will have the output of this function
    // --------------- Store nested return to R8 ----- //
    @XOR
    0;JMP
    (XOR_CALLER_NESTED)
    (functionReturn)
    // Get The value of the XOR and output this to function out
    @R7
    A=M
    0;JMP


// ===============================================
//  MAIN
// -----------------------------------------------
//  Description:
//      Fiestal Encryption algorithm
//      - Split the plain text into 
//      - Apply the (FUNCTION) which applies the (XOR)
//      - Loop through this process for i = 0, 1, ..., n
// -----------------------------------------------
//  Args: R3, R4    Out: R5     return R7

(MAIN)     
    @255        // Clearing the last 8 bits from R2
    D=A
    @R1
    M=D&M

    // ======= Normalize the Key for MAIN ======= //
    // First 8 bits must be same as last
    @R1  
    D=M
    @R13
    M=D             // Duplicate to R13 temporarily
    @R1
    D=M
    @R3
    M=D             // Set Arg 1 of ROTATE
    @8
    D=A 
    @R4
    M=D             // Setting the parameters for R4
    @callback0      // Setting the return
    D=A
    @R7
    M=D
    @ROTATE
    0;JMP
    (callback0)

    // ----- Combining R1 and R5 ------ //
    @R5
    D=M
    @R1
    M=D|M

    // ----- Split R2 to L, R stored in R10, R11 ----- //
    @R2
    D=M
    @R10
    M=D 
                    // Rotate 8-bits to get R0 as first 8 bits
    @8
    D=A
    @R4
    M=D             // Setting parameters for R4 for rotation
                    // Setting R3
    @R10
    D=M
    @R3
    M=D
    
    @callback1      // Setting the return
    D=A
    @R7     
    M=D
    @ROTATE         // Call the ROTATE function
    0;JMP
    (callback1)
    @R5             // Using stored value
    D=M
    @R11
    M=D             // Setting R

    // -----------------  Set loop var -------------- //
    @4
    D=A
    @R9                                                                                 
    M=D
    (MAIN_LOOP)
        @R9
        D=M
        @JOIN
        D;JEQ

        @R9
        M=M-1 

        // ----------------- F(R10, R1) ----------------- //
        @R10
        D=M
        @R3
        M=D             // Store R11 as arg
        @R1
        D=M
        @R4
        M=D             // Store R1 as arg 

        @callback2
        D=A
        @R7
        M=D
        @FUNCTION
        0;JMP

        (callback2)
        // ---------------- R1 = L0 XOR R5 ---------------- //
        @R5
        D=M
        @R3
        M=D         // Arg 1 stored as F(Ri, Ki)
        @R11
        D=M 
        @R4 
        M=D         // Arg 2 stored as Li

        @callback3
        D=A 
        @R8 
        M=D         // R8 <- Address to callback3
        @XOR
        0;JMP       // Call XOR

        (callback3)
        // ----------- Li+1 = Ri -------------------------- //
        @R10
        D=M
        @R11
        M=D

        // ------------- Ri+1 -> R10 ---------------- //
        @R5
        D=M
        @R10 
        M=D

        // ------------- Do a left shift to the key ------- //
        @R1
        D=M
        @R3
        M=D         // Set args 3 and 4
        @R4
        M=1         // Rotate once
        @callback4  
        D=A
        @R7
        M=D         // Set the return
        @ROTATE
        0;JMP

        (callback4)
        @R5
        D=M
        @R1
        M=D         // Rotate key stored

        // ------------- Loop until 4 times --------------- //
        @MAIN_LOOP
        0;JMP

    (JOIN)
        // --------- Join Ln and Rn to store as R0 -------- //
        @255        // Clearing the first 8 bits from R10
        D=A
        @R10
        M=D&M

        @255        // Clearing the last 8 bits from R11
        D=A
        @R11
        M=D&M

        // ---------- Rotate L3 by 8 ------------- //
        @R11
        D=M
        @R3
        M=D         // Set args 3 and 4

        @R8
        D=A         
        @R4
        M=D

        @callback6  
        D=A
        @R7
        M=D         // Set the return
        @ROTATE     // R5 is L3 rotated
        0;JMP
        (callback6)

    // ---------------- Or and Save to R0 -------------------- //
    @R5
    D=M
    @R10
    M=D|M   // Doing an or
    D=M
    @R0
    M=D    // Storing encrypted message
    
(INFINITE_LOOP)
@INFINITE_LOOP
0;JMP