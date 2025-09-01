#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define TRIVIUM_KEY_SIZE 	10
#define TRIVIUM_IV_SIZE  	10
#define TRIVIUM_STATE_SIZE	288
#define RA_SIZE			93
#define RB_SIZE			84
#define RC_SIZE			111
#define TRIVIUM_INIT_ROUNDS 	(4 * TRIVIUM_STATE_SIZE)

typedef struct {
	uint8_t ra[RA_SIZE];
	uint8_t rb[RB_SIZE];
	uint8_t rc[RC_SIZE];
} TriviumContext;


void trivium_init(TriviumContext* ctx, const uint8_t* key, const uint8_t* iv) {
	memset(ctx->ra, 0, sizeof(ctx->ra));
	memset(ctx->rb, 0, sizeof(ctx->rb));
	memset(ctx->rc, 0, sizeof(ctx->rc));

	// Load 80-bit key into left-most RA
	for (int i = 0; i < 80; i++) {
	int byte_idx = 9 - (i / 8);
	ctx->ra[i] = (key[byte_idx] >> (7 - (i % 8))) & 1;
	}
	// Load 80-bit iv into state RB
	for (int i = 0; i < 80; i++) {
		int byte_idx = 9 - (i / 8);
		ctx->rb[i] = (iv[byte_idx] >> (7 - (i % 8))) & 1;
	}

	// Set last 3 bits of RC to 1
	ctx->rc[108] = 1;
	ctx->rc[109] = 1;
	ctx->rc[110] = 1;

	uint8_t out_ra;
	uint8_t out_rb;
	uint8_t out_rc;
	// Perform 1152 update rounds without keystream output
	for (int i = 0; i < TRIVIUM_INIT_ROUNDS; i++) {
		out_ra = ctx->ra[65] ^ ctx->ra[92] ^ 
			(ctx->ra[90] & ctx->ra[91]);
		out_rb = ctx->rb[68] ^ ctx->rb[83] ^
			(ctx->rb[81] & ctx->rb[82]);
		out_rc = ctx->rc[65] ^ ctx->rc[110] ^
			(ctx->rc[108] & ctx->rc[109]);
		
		// Set start registers
		uint8_t in_ra = out_rc ^ ctx->ra[68];
		uint8_t in_rb = out_ra ^ ctx->rb[77];
		uint8_t in_rc = out_rb ^ ctx->rc[86];

		// Shift each register
		for (int j = RA_SIZE - 1; j > 0; j--)
			ctx->ra[j] = ctx->ra[j - 1];
		for (int j = RB_SIZE - 1; j > 0; j--)
			ctx->rb[j] = ctx->rb[j - 1];
		for (int j = RC_SIZE - 1; j > 0; j--)
			ctx->rc[j] = ctx->rc[j - 1];

		ctx->ra[0] = in_ra;
		ctx->rb[0] = in_rb;
		ctx->rc[0] = in_rc;
	}
}

int trivium_generate_bit(TriviumContext* ctx) {
	uint8_t linear_ra = ctx->ra[65] ^ ctx->ra[92];
	uint8_t linear_rb = ctx->rb[68] ^ ctx->rb[83];
	uint8_t linear_rc = ctx->rc[65] ^ ctx->rc[110];

    uint8_t out_ra = linear_ra ^ (ctx->ra[90] & ctx->ra[91]);
	uint8_t out_rb = linear_rb ^ (ctx->rb[81] & ctx->rb[82]);
	uint8_t out_rc = linear_rc ^ (ctx->rc[108] & ctx->rc[109]);

	// Set start registers
	uint8_t in_ra = out_rc ^ ctx->ra[68];
	uint8_t in_rb = out_ra ^ ctx->rb[77];
	uint8_t in_rc = out_rb ^ ctx->rc[86];
	
	// Shift each register 
	for (int j = RA_SIZE - 1; j > 0; j--)
		ctx->ra[j] = ctx->ra[j - 1];
	for (int j = RB_SIZE - 1; j > 0; j--)
		ctx->rb[j] = ctx->rb[j - 1];
	for (int j = RC_SIZE - 1; j > 0; j--)
		ctx->rc[j] = ctx->rc[j - 1];
	
	// Set start registers
	ctx->ra[0] = in_ra;
	ctx->rb[0] = in_rb;
	ctx->rc[0] = in_rc;

	return linear_ra ^ linear_rb ^ linear_rc;
}
 
int trivium_generate_keystream(TriviumContext* ctx, uint8_t* keystream, size_t length) {
	for (size_t i = 0; i < length; i++) {
		uint8_t ks = 0;
		for (int bit = 0; bit < 8; bit++) {
			uint8_t z = trivium_generate_bit(ctx);
			ks |= (z << bit);
		}
		keystream[i] = ks;
	}
} 

void trivium_encrypt(TriviumContext* ctx, const uint8_t* plaintext, uint8_t* cipher_text, size_t length) {
	uint8_t* keystream = malloc(length);
	if (!keystream) return;
	trivium_generate_keystream(ctx, keystream, length);
	for (size_t i = 0; i < length; i++) {
		cipher_text[i] = plaintext[i] ^ keystream[i];
	}
	free(keystream);
}

