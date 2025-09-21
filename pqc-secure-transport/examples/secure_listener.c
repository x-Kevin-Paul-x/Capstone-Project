#include <stdio.h>
#include <string.h>
#include "pqc.h"

int main(void) {
    uint8_t in[256];
    size_t inlen = sizeof(in);
    pqc_session_t s;

    /* stub: pretend we received data and decrypted */
    if (pqc_decrypt(&s, (const uint8_t*)"stub", 4, NULL, 0, in, &inlen) == 0) {
        printf("secure_listener: received %zu bytes (stub)\n", inlen);
    } else {
        printf("secure_listener: decrypt failed (stub)\n");
    }
    return 0;
}
