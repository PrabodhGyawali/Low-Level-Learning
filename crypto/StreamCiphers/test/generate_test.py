import os
from pathlib import Path

fd = os.open("./trivium-80.80.test-vectors.txt", os.O_RDONLY)

data = os.read(fd, os.path.getsize("./trivium-80.80.test-vectors.txt"))

os.close(fd)

data = data.decode('utf-8')

vectors = []

lines = data.splitlines()


i = 0

while i < len(lines):
    line = lines[i].strip()
    if line.startswith('Set'):
        vector_num = int(line.split('#')[1].split(':')[0].strip())
        vector = {'num': vector_num, 'streams': {}, 'key': None, 'iv': None, 'xor_digest': None}
        i += 1
        while i < len(lines) and not lines[i].startswith('Set'):
            line = lines[i].lstrip()
            if line.startswith('key ='):
                vector['key'] = line.split('=')[1].strip()
            elif line.startswith('IV ='):
                vector['iv'] = line.split('=')[1].strip()
            elif line.startswith('stream['):
                range_str = line.split('stream[')[1].split(']')[0]
                hex_part = line.split('=')[1].strip()
                i += 1
                while i < len(lines) and lines[i].startswith(' ' * 20):  # Adjust based on indentation, approx 31 spaces
                    hex_part += lines[i].strip()
                    i += 1
                i -= 1
                vector['streams'][range_str] = hex_part
            elif line.startswith('xor-digest ='):
                hex_part = line.split('=')[1].strip()
                i += 1
                while i < len(lines) and lines[i].startswith(' ' * 20):
                    hex_part += lines[i].strip()
                    i += 1
                i -= 1
                vector['xor_digest'] = hex_part
            i += 1
        vectors.append(vector)
    else:
        i += 1

# Output C code

def generate_tests(vectors):
    print('#include "../include/trivium.h"')
    print('#include <stdio.h>')
    print('#include <string.h>')
    print('/**')
    print(' * Test cases obtained using')
    print(' * https://github.com/cantora/avr-crypto-lib/blob/master/testvectors/trivium-80.80.test-vectors')
    print(' */')
    print('int main(void) {')
    print('    int overall_pass = 1;')

    test_num = 1
    for vec in vectors:
        print('    {')
        key_bytes = int(vec["key"], 16).to_bytes(10, "big")
        print('        uint8_t key[TRIVIUM_KEY_SIZE] = {')
        print('            ' + ', '.join([f'0x{byte:02X}' for byte in key_bytes]) + '')
        print('        };')
        iv_bytes = int(vec["iv"], 16).to_bytes(10, "big")
        print('        uint8_t iv[TRIVIUM_IV_SIZE] = {')
        print('            ' + ', '.join([f'0x{byte:02X}' for byte in iv_bytes]) + '')
        print('        };')
        print('        uint8_t plaintext[512] = {0};')
        print('        uint8_t ciphertext[512] = {0};')
        print('        TriviumContext ctx;')
        print('        trivium_init(&ctx, key, iv);')
        print('        trivium_encrypt(&ctx, plaintext, ciphertext, 512);')
        print('        int pass = 1;')
        for range_str, hex_str in vec['streams'].items():
            start, end = range_str.split('..')
            start = int(start); end = int(end)
            size = 64
            stream_bytes = bytes.fromhex(hex_str)
            var_name = f'expected_{start}_{end}'
            print(f'        uint8_t {var_name}[{size}] = {{')
            for j in range(0, len(stream_bytes), 16):
                chunk = stream_bytes[j:j+16]
                print('            ' + ', '.join([f'0x{byte:02X}' for byte in chunk]) + ',')
            print('        };')
            print(f'        if (memcmp(ciphertext + {start}, {var_name}, {64}) != 0) {{ pass = 0; }}')
        print('        uint8_t computed_digest[64] = {0};')
        print('        for (int b = 0; b < 8; b++) {')
        print('            for (int j = 0; j < 64; j++) {')
        print('                computed_digest[j] ^= ciphertext[b * 64 + j];')
        print('            }')
        print('        }')
        digest_bytes = bytes.fromhex(vec['xor_digest'])
        print('        uint8_t expected_digest[64] = {')
        for j in range(0, len(digest_bytes), 16):
            chunk = digest_bytes[j:j+16]
            print('            ' + ', '.join([f'0x{byte:02X}' for byte in chunk]) + ',')
        print('        };')
        print('        if (memcmp(computed_digest, expected_digest, 64) != 0) { pass = 0; }')
        print('        if (pass) {')
        print(f'            printf("Test {test_num}: PASS\\n");')
        print('        } else {')
        print(f'            printf("Test {test_num}: FAIL\\n");')
        print('            overall_pass = 0;')
        print('        }')
        print('    }')
        test_num += 1

    print('    return overall_pass ? 0 : 1;')
    print('}')

generate_tests(vectors)
