#ifndef PQC_H
#define PQC_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t key[32];
    uint64_t seq;
} pqc_session_t;

/* Handshake: ID auth (Dilithium) + Kyber KEM -> derive session key */
int pqc_handshake(int fd, pqc_session_t* s);
int pqc_rekey(int fd, pqc_session_t* s);

/* Data plane AEAD */
int pqc_encrypt(pqc_session_t* s, const uint8_t* in, size_t inlen,
                const uint8_t* aad, size_t aadlen, uint8_t* out, size_t* outlen);
int pqc_decrypt(pqc_session_t* s, const uint8_t* in, size_t inlen,
                const uint8_t* aad, size_t aadlen, uint8_t* out, size_t* outlen);

#endif /* PQC_H */
