#include "../include/trivium.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    int overall_pass = 1;

    // Test 1: All zero key and IV, encrypt 16 zero bytes
    {
        uint8_t key[TRIVIUM_KEY_SIZE] = {0};
        uint8_t iv[TRIVIUM_IV_SIZE] = {0};
        uint8_t plaintext[16] = {0};
        uint8_t ciphertext[16] = {0};
        uint8_t expected[16] = {0xFB, 0xE0, 0xBF, 0x26, 0x58, 0x59, 0x05, 0x1B, 0x51, 0x7A, 0x2E, 0x4E, 0x23, 0x9F, 0xC9, 0x7F};
        TriviumContext ctx;

        trivium_init(&ctx, key, iv);
        trivium_encrypt(&ctx, plaintext, ciphertext, 16);

        if (memcmp(ciphertext, expected, 16) == 0) {
            printf("Test 1: PASS\n");
        } else {
            printf("Test 1: FAIL\n");
            overall_pass = 0;
        }
    }

    // Test 2: Key 80000000000000000000, IV all zero, encrypt 16 zero bytes
    {
        uint8_t key[TRIVIUM_KEY_SIZE] = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        uint8_t iv[TRIVIUM_IV_SIZE] = {0};
        uint8_t plaintext[16] = {0};
        uint8_t ciphertext[16] = {0};
        uint8_t expected[16] = {0x38, 0xEB, 0x86, 0xFF, 0x73, 0x0D, 0x7A, 0x9C, 0xAF, 0x8D, 0xF1, 0x3A, 0x44, 0x20, 0x54, 0x0D};
        TriviumContext ctx;

        trivium_init(&ctx, key, iv);
        trivium_encrypt(&ctx, plaintext, ciphertext, 16);

        if (memcmp(ciphertext, expected, 16) == 0) {
            printf("Test 2: PASS\n");
        } else {
            printf("Test 2: FAIL\n");
            overall_pass = 0;
        }
    }

    // Test 3: Specific key and IV, encrypt "hello"
    {
        uint8_t key[TRIVIUM_KEY_SIZE] = {0x0F, 0x62, 0xB5, 0x08, 0x5B, 0xAE, 0x01, 0x54, 0xA7, 0xFA};
        uint8_t iv[TRIVIUM_IV_SIZE] = {0x28, 0x8F, 0xF6, 0x5D, 0xC4, 0x2B, 0x92, 0xF9, 0x60, 0xC7};
        uint8_t plaintext[5] = {'h', 'e', 'l', 'l', 'o'};
        uint8_t ciphertext[5] = {0};
        uint8_t expected[5] = {0xCC, 0x5D, 0x00, 0x01, 0x19};
        TriviumContext ctx;

        trivium_init(&ctx, key, iv);
        trivium_encrypt(&ctx, plaintext, ciphertext, 5);

        if (memcmp(ciphertext, expected, 5) == 0) {
            printf("Test 3: PASS\n");
        } else {
            printf("Test 3: FAIL\n");
            overall_pass = 0;
        }
    }

    return overall_pass ? 0 : 1;
}
