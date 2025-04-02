#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <inttypes.h>

#define UPPER_MASK		    0x80000000
#define LOWER_MASK		    0x7fffffff
#define TEMPERING_MASK_B	0x9d2c5680
#define TEMPERING_MASK_C	0xefc60000
#define STATE_VECTOR_LENGTH 624
#define STATE_VECTOR_M      397

typedef struct rand_gen
{
    uint64_t mt[STATE_VECTOR_LENGTH];
    uint64_t index;
} rand_gen_t;

void rand_gen_seed(rand_gen_t* rand, uint64_t seed)
{
    rand->mt[0] = seed & 0xffffffff;
    for (rand->index = 1; rand->index < STATE_VECTOR_LENGTH; rand->index++)
        rand->mt[rand->index] = (6069 * rand->mt[rand->index - 1]) & 0xffffffff;
}

uint64_t rand_gen_next_int(rand_gen_t* rand)
{
    uint64_t y;
    uint64_t mag[2] = {
        0x0,
        0x9908b0df
    };

    if (rand->index >= STATE_VECTOR_LENGTH|| rand->index < 0)
    {
        if (rand->index >= STATE_VECTOR_LENGTH + 1 || rand->index < 0)
            rand_gen_seed(rand, 4357);

        for (int64_t i = 0; i < STATE_VECTOR_LENGTH - STATE_VECTOR_M; i++)
        {
            y = (rand->mt[i] & UPPER_MASK) | (rand->mt[i + 1] & LOWER_MASK);
            rand->mt[i] = rand->mt[i + STATE_VECTOR_M] ^ (y >> 1) ^ mag[y & 0x1];
        }

        for (int64_t i = 0; i < STATE_VECTOR_LENGTH - 1; i++)
        {
            y = (rand->mt[i] & UPPER_MASK) | (rand->mt[i + 1] & LOWER_MASK);
            rand->mt[i] = rand->mt[i + (STATE_VECTOR_M - STATE_VECTOR_LENGTH)] ^ (y >> 1) ^ mag[y & 0x1];
        }

        y = (rand->mt[STATE_VECTOR_LENGTH - 1] & UPPER_MASK) | (rand->mt[0] & LOWER_MASK);
        rand->mt[STATE_VECTOR_LENGTH - 1] = rand->mt[STATE_VECTOR_M - 1] ^ (y >> 1) ^ mag[y & 0x1];
        rand->index = 0;
    }
    y = rand->mt[rand->index++];
    y ^= (y >> 11);
    y ^= (y << 7) & TEMPERING_MASK_B;
    y ^= (y << 15) & TEMPERING_MASK_C;
    y ^= (y >> 18);

    return y;
}

double rand_gen_next(rand_gen_t* rand)
{
    return (double)rand_gen_next_int(rand) / 0xffffffff;
}

int main()
{
    rand_gen_t rng = {0};
    rand_gen_seed(&rng, time(NULL));

    for (size_t i = 0; i < 10; i++)
        printf("%f\n", rand_gen_next(&rng));

    for (size_t i = 0; i < 10; i++)
        printf("%"PRIu64"\n", rand_gen_next_int(&rng));

    return 0;
}