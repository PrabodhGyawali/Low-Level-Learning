#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define TRIVIUM_KEY_SIZE 10
#define TRIVIUM_IV_SIZE  10
#define TRIVIUM_STATE_SIZE 288
#define RA_SIZE 93
#define RB_SIZE 84
#define RC_SIZE 111
#define TRIVIUM_INIT_ROUNDS (4 * TRIVIUM_STATE_SIZE)

typedef struct {
uint8_t ra[RA_SIZE];
uint8_t rb[RB_SIZE];
uint8_t rc[RC_SIZE];
} TriviumContext;

void trivium_init(TriviumContext* ctx, const uint8_t* key, const uint8_t* iv);

int trivium_generate_bit(TriviumContext* ctx);

int trivium_generate_keystream(TriviumContext* ctx, uint8_t* keystream, size_t length);

void trivium_encrypt(TriviumContext* ctx, const uint8_t* plaintext, uint8_t* ciphertext, size_t length);
