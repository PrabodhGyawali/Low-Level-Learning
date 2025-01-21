// This file is adapted from www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/mult/Mult.tst

load XOR.asm,
output-file XOR1.out,
compare-to XOR1.cmp,
output-list RAM[5]%D2.6.2;

set RAM[3] 0,   // Set test arguments
set RAM[4] 1,
set RAM[5] 0,
repeat 100 {
  ticktock;
}
output;
