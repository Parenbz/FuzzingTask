#include "/home/parenbz/AFLplusplus/include/afl-fuzz.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DATA_SIZE (1000)

static const u8 flipbit1_mask[] = {
    0b00000001,
    0b00000010,
    0b00000100,
    0b00001000,
    0b00010000,
    0b00100000,
    0b01000000,
    0b10000000
};

static const u8 flipbit2_mask[] = {
    0b00000011,
    0b00001100,
    0b00110000,
    0b11000000
};

static const u8 flipbit4_mask[] = {
    0b00001111,
    0b11110000
};

typedef struct bit_flip_mutator {

    afl_state_t *afl;

    u8 *mutated_out, *post_process_buf;
} bit_flip_mutator_t;

bit_flip_mutator_t *afl_custom_init(afl_state_t *afl, unsigned int seed) {

    srand(seed);

    bit_flip_mutator_t *data = calloc(1, sizeof(bit_flip_mutator_t));
    if (!data) {
        perror("afl_custom_init alloc");
        return NULL;
    }

    if ((data->mutated_out = (u8 *)malloc(MAX_FILE)) == NULL) {
        perror("afl_custom_init malloc");
        return NULL;
    }

    if ((data->post_process_buf = (u8 *)malloc(MAX_FILE)) == NULL) {
        perror("afl_custom_init malloc");
        return NULL;
    }

    data->afl = afl;
    return data;
}

size_t afl_custom_fuzz(bit_flip_mutator_t *data, uint8_t *buf, size_t buf_size, u8 **out_buf, 
                        uint8_t *add_buf, size_t add_buf_size, size_t max_size) {
    size_t mutated_size = DATA_SIZE <= max_size ? DATA_SIZE : max_size;

    memcpy(data->mutated_out, buf, buf_size);

    if (buf_size > 2) {
        data->mutated_out[rand() % (buf_size - 2) + 2] ^= 0xff;
        data->mutated_out[rand() % (buf_size - 2) + 2] ^= flipbit1_mask[rand() % 8];
        data->mutated_out[rand() % (buf_size - 2) + 2] += rand() % 36;
        data->mutated_out[rand() % (buf_size - 2) + 2] -= rand() % 36;
    }

    *out_buf = data->mutated_out;
    return mutated_size;
}

size_t afl_custom_post_process(bit_flip_mutator_t *data, uint8_t *buf, size_t buf_size, uint8_t **out_buf) {
    memcpy(data->post_process_buf, buf, buf_size);

    data->post_process_buf[76] = 0x20;
    data->post_process_buf[77] = 0x00;
    data->post_process_buf[78] = 0x00;
    data->post_process_buf[79] = 0x00;

    *out_buf = data->post_process_buf;

    return buf_size;
}



void afl_custom_deinit(bit_flip_mutator_t *data) {
    free(data->post_process_buf);
    free(data->mutated_out);
    free(data);
}