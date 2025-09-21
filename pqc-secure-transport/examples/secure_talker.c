#include <stdio.h>
#include <string.h>
#include "pqc.h"

int main(void) {
    uint8_t plaintext[] = "hello secure_talker";
    uint8_t out[256];
    size_t outlen = sizeof(out);

    pqc_session_t s;
    /* stub: no real handshake */
    if (pqc_encrypt(&s, plaintext, sizeof(plaintext), NULL, 0, out, &outlen) == 0) {
        printf("secure_talker: sent %zu bytes (stub)\n", outlen);
    } else {
        printf("secure_talker: encrypt failed (stub)\n");
    }
    return 0;
}
