// This file is adapted from www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/mult/Mult.tst

load FeistelEncryption.asm,
output-file FeistelEncrytion1.out,
compare-to FeistelEncryption1.cmp,
output-list RAM[0]%D2.6.2;

set RAM[0] 0,   // Set test arguments
set RAM[1] 0,
set RAM[2] 0;
repeat 2000 {
  ticktock;
}
output;
