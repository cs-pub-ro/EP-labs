#include <stdint.h>
#include <stddef.h>

/* csum_16b1c - 16-bit 1's complement sum
 *  @sum    : initial partial sum (e.g.: tcp/udp pseudo headers)
 *  @buffer : buffer over which sum is computed
 *  @nwords : number of bytes in buffer (can be odd - rpad with zeros)
 *
 *  @return : checksum
 */
uint16_t csum_16b1c(uint64_t sum, uint16_t *buffer, size_t nbytes)
{
    /* calculate sum of all 16b words */
    for (; nbytes > 1; nbytes -= 2)
        sum += *buffer++;

    /* account for number of odd bytes */
    if (nbytes)
        sum += *((uint8_t *) buffer);

    /* fold partial checksum (account for carry) */
    while (sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);

    /* return 1's complement of partial sum */
    return (uint16_t)(~sum);
}

