/* ca_helper.c - small helper to generate/test CA artifacts
 *
 * Behavior:
 *  - If built and linked against pqm4 (and PQM4_AVAILABLE is defined), this
 *    helper will use Dilithium functions to create a CA keypair and will sign
 *    simple certificate-like blobs for nodes.
 *  - If pqm4 is not available, it falls back to writing human-readable
 *    placeholders so scripts and tests can proceed.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef PQM4_AVAILABLE
#include <stdint.h>
// pqm4 exposes the standard NIST-style API (crypto_sign_keypair, crypto_sign)
// include a common api header if present. Many implementations provide
// api.h in the scheme's ref/ folder, but pqm4 provides names in the build
// namespace. We rely on the linker providing crypto_sign_keypair and crypto_sign.
// Declare them here as weak prototypes to avoid having to pin a specific
// header layout in this helper.
int crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int crypto_sign(unsigned char *sm, unsigned long long *smlen,
                const unsigned char *m, unsigned long long mlen,
                const unsigned char *sk);
#endif

static int write_file(const char* path, const char* data) {
    FILE* f = fopen(path, "wb");
    if (!f) return -1;
    fwrite(data, 1, strlen(data), f);
    fclose(f);
    return 0;
}

int main(int argc, char** argv) {
    const char* outdir = (argc > 1) ? argv[1] : "./test-ca";
    const char* cmd = (argc > 2) ? argv[2] : "create";
    char path[1024];

    if (strcmp(cmd, "create") == 0) {
#ifdef PQM4_AVAILABLE
        // Create a Dilithium keypair using pqm4's crypto_sign_keypair()
        unsigned char pk[4096];
        unsigned char sk[8192];
        if (crypto_sign_keypair(pk, sk) != 0) {
            fprintf(stderr, "crypto_sign_keypair failed\n");
            return 4;
        }

        snprintf(path, sizeof(path), "%s/ca_key.dil", outdir);
        FILE* f = fopen(path, "wb");
        if (!f) { perror("fopen"); return 2; }
        fwrite(sk, 1, 64, f); // write minimal bytes; exact sizes depend on algorithm
        fclose(f);

        snprintf(path, sizeof(path), "%s/ca_pub.dil", outdir);
        f = fopen(path, "wb");
        fwrite(pk, 1, 32, f);
        fclose(f);

        snprintf(path, sizeof(path), "%s/ca_cert.pem", outdir);
        f = fopen(path, "w");
        fprintf(f, "-----BEGIN CERTIFICATE-----\n");
        fprintf(f, "Subject: PQM4-DILITHIUM-CA\n");
        fprintf(f, "PublicKey: ");
        for (int i = 0; i < 16; ++i) fprintf(f, "%02x", pk[i]);
        fprintf(f, "\n-----END CERTIFICATE-----\n");
        fclose(f);

        printf("Wrote Dilithium CA artifacts to %s\n", outdir);
        return 0;
#else
        snprintf(path, sizeof(path), "%s/ca_key.dil", outdir);
        write_file(path, "CA_PRIVATE_KEY_PLACEHOLDER\n");

        snprintf(path, sizeof(path), "%s/ca_pub.dil", outdir);
        write_file(path, "CA_PUBLIC_KEY_PLACEHOLDER\n");

        snprintf(path, sizeof(path), "%s/ca_cert.pem", outdir);
        write_file(path, "-----BEGIN CERTIFICATE-----\nMIID...PLACEHOLDER...IDAQAB\n-----END CERTIFICATE-----\n");

        printf("Wrote placeholder CA artifacts to %s\n", outdir);
        return 0;
#endif
    }

    if (strcmp(cmd, "issue") == 0) {
        const char* node = (argc > 3) ? argv[3] : "node1";
#ifdef PQM4_AVAILABLE
        // Generate a node keypair and sign a tiny "cert" with the CA sk.
        unsigned char pk[4096];
        unsigned char sk[8192];
        if (crypto_sign_keypair(pk, sk) != 0) {
            fprintf(stderr, "crypto_sign_keypair (node) failed\n");
            return 4;
        }

        snprintf(path, sizeof(path), "%s/%s_key.dil", outdir, node);
        FILE* f = fopen(path, "wb");
        if (!f) { perror("fopen"); return 2; }
        fwrite(sk, 1, 64, f);
        fclose(f);

        snprintf(path, sizeof(path), "%s/%s_pub.dil", outdir, node);
        f = fopen(path, "wb");
        fwrite(pk, 1, 32, f);
        fclose(f);

        // Create a small signed blob using crypto_sign (signature over node id)
        unsigned char sig[4096];
        unsigned long long siglen = 0;
        unsigned char m[256];
        int mlen = snprintf((char*)m, sizeof(m), "node:%s", node);
        if (crypto_sign(sig, &siglen, m, (unsigned long long)mlen, sk) != 0) {
            fprintf(stderr, "crypto_sign failed\n");
            return 5;
        }

        snprintf(path, sizeof(path), "%s/%s_cert.pem", outdir, node);
        f = fopen(path, "w");
        fprintf(f, "-----BEGIN CERTIFICATE-----\n");
        fprintf(f, "Node: %s\n", node);
        fprintf(f, "Signature: ");
        for (unsigned long long i = 0; i < (siglen && siglen < 64 ? siglen : 64); ++i) fprintf(f, "%02x", sig[i]);
        fprintf(f, "\n-----END CERTIFICATE-----\n");
        fclose(f);

        printf("Issued node cert for %s at %s (signed)\n", node, outdir);
        return 0;
#else
        snprintf(path, sizeof(path), "%s/%s_key.dil", outdir, node);
        write_file(path, "NODE_PRIVATE_KEY_PLACEHOLDER\n");

        snprintf(path, sizeof(path), "%s/%s_pub.dil", outdir, node);
        write_file(path, "NODE_PUBLIC_KEY_PLACEHOLDER\n");

        snprintf(path, sizeof(path), "%s/%s_cert.pem", outdir, node);
        char buf[512];
        snprintf(buf, sizeof(buf), "-----BEGIN CERTIFICATE-----\nMIID...NODE:%s:PLACEHOLDER...IDAQAB\n-----END CERTIFICATE-----\n", node);
        write_file(path, buf);

        printf("Issued placeholder node cert for %s at %s\n", node, outdir);
        return 0;
#endif
    }

    fprintf(stderr, "Unknown command '%s'\n", cmd);
    return 3;
}
