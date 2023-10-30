#include "/home/parenbz/AFLplusplus/include/afl-fuzz.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DATA_SIZE (1000)

typedef struct arithmetics_mutator {

    afl_state_t *afl;

    u8 *mutated_out, *post_process_buf;
} arithmetics_mutator_t;

arithmetics_mutator_t *afl_custom_init(afl_state_t *afl, unsigned int seed) {

    srand(seed);

    arithmetics_mutator_t *data = calloc(1, sizeof(arithmetics_mutator_t));
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

size_t afl_custom_fuzz(arithmetics_mutator_t *data, uint8_t *buf, size_t buf_size, u8 **out_buf, 
                        uint8_t *add_buf, size_t add_buf_size, size_t max_size) {
    size_t mutated_size = DATA_SIZE <= max_size ? DATA_SIZE : max_size;

    memcpy(data->mutated_out, buf, buf_size);

    if (buf_size > 2) {
        if (rand() % 2) {
            data->mutated_out[rand() % (buf_size - 2) + 2] += rand() % 36 + 1;
        } else {
            data->mutated_out[rand() % (buf_size - 2) + 2] -= rand() % 36 + 1;
        }
    }

    *out_buf = data->mutated_out;
    return mutated_size;
}

size_t afl_custom_post_process(arithmetics_mutator_t *data, uint8_t *buf, size_t buf_size, uint8_t **out_buf) {
    memcpy(data->post_process_buf, buf, buf_size);

    data->post_process_buf[76] = 0x20;
    data->post_process_buf[77] = 0x00;
    data->post_process_buf[78] = 0x00;
    data->post_process_buf[79] = 0x00;

    *out_buf = data->post_process_buf;

    return buf_size;
}



void afl_custom_deinit(arithmetics_mutator_t *data) {
    free(data->post_process_buf);
    free(data->mutated_out);
    free(data);
}