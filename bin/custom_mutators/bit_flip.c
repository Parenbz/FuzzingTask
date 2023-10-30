#include "/home/parenbz/AFLplusplus/include/afl-fuzz.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DATA_SIZE (1000)

static const u8 flip1bit_mask[] = {
    0x01,
    0x02,
    0x04,
    0x08,
    0x10,
    0x20,
    0x40,
    0x80
};

static const u8 flip2bit_mask[] = {
    0b00000011,
    0b00001100,
    0b00110000,
    0b11000000
};

static const u8 flip4bit_mask[] = {
    0b00001111,
    0b11110000
};

static s8 interest8[] = {
    -128,
    -1,
    0,
    1,
    16,
    32,
    64,
    100,
    127
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
        data->mutated_out[rand() % (buf_size - 2) + 2] ^= flip1bit_mask[rand() % 8];
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

size_t afl_custom_havoc_mutation(bit_flip_mutator_t *data, u8 *buf, size_t buf_size, u8 **out_buf, size_t max_size) {

    *out_buf  = buf;

    if (buf_size > 2) {
        (*out_buf)[rand() % (buf_size - 2) + 2] ^= flip1bit_mask[rand() % 8];
        (*out_buf)[rand() % (buf_size - 2) + 2] ^= flip2bit_mask[rand() % 4];
        (*out_buf)[rand() % (buf_size - 2) + 2] ^= flip4bit_mask[rand() % 2];
        (*out_buf)[rand() % (buf_size - 2) + 2] += rand() % 36 + 1;
        (*out_buf)[rand() % (buf_size - 2) + 2] -= rand() % 36 + 1;
        (*out_buf)[rand() % (buf_size - 2) + 2] = interest8[rand() % 9];
    }

    return buf_size;
}

void afl_custom_deinit(bit_flip_mutator_t *data) {
    free(data->post_process_buf);
    free(data->mutated_out);
    free(data);
}