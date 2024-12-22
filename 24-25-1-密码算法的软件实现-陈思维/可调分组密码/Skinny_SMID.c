#include <stdio.h>
#include <stdint.h>
#include <arm_neon.h>

int main()
{
    uint8_t a[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    __m128i b = _mm_loadu_si128((__m128i *) a);
}