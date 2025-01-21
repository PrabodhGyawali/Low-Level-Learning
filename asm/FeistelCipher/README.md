Time to complete: Less than 15 hours from 06/05/24

# Feistal Cipher
- plaintext, $P$, is split to to two equal size parts $L_0$ and $R_0$ where: $$P = L_0R_0$$
- A function F is applied to one half of the plaintext, combined with a key, and the result is XOR'd with the other half of the plaintext
- This is often applied multiple times
    $$L_{i+1} = R_{i}$$
    $$R_{i+1} = L_i\oplus F(R_i, K_i)$$
This is applied for i = 0, 1, ..., n

We will create a 16-bit Feistel cipher which uses 4 rounds the function below:
$$F(A,B)=A \oplus \neg B$$

The keys are derived from a single 8-bit key $K_0$ such that, 
$$K_0 = b_7b_6b_5b_4b_3b_2b_1b_0$$
$$K_1 = b_6b_5b_4b_3b_2b_1b_0b_7$$
$$K_2 = b_5b_4b_3b_2b_1b_0b_7b_6$$
$$K_3 = b_4b_3b_2b_1b_0b_7b_6b_5$$

# Task-Breakdown:
## Memory Management
- RAM[0] --> Encryption Result
- RAM[1] --> $K_0$
- RAM[2] --> Plain-text to be encrypted

## Function Breakdown
- Split RAM[2] into 8-bits 
    - RAM[11] -> $L_0$ & RAM[10] -> $R_0$
- $F(R10, R1)$
- 
## Rotate Function
### Args
- valueToRotate: RAM[3], shift: RAM[4]
### Output
- RAM[5]
### Return
- go to address set in RAM[7]

# Decryption
For decryption:
    $$R_{i} = L_{i+1}$$
    $$L_{i} = R_{i+1}\oplus F(L_{i+1}, K_i)$$
    This is applied for i = n, n-1, ..., 0