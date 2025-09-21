/* ca_helper.c - small helper to generate/test CA artifacts
 * This program acts as a bridge: when pqm4 is available and linked, it can
 * generate Dilithium keypairs and produce cert-like blobs. When pqm4 is not
 * available, it writes placeholder files (so scripts can proceed during Phase 1).
 *
 * This file intentionally contains minimal logic and no PQC calls; wire pqm4
 * calls where indicated after adding the real library.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
    const char* outdir = (argc > 1) ? argv[1] : "./test-ca";
    const char* cmd = (argc > 2) ? argv[2] : "create";
    char path[1024];

    if (strcmp(cmd, "create") == 0) {
        snprintf(path, sizeof(path), "%s/ca_key.dil", outdir);
        FILE* f = fopen(path, "w");
        if (!f) { perror("fopen"); return 2; }
        fprintf(f, "CA_PRIVATE_KEY_PLACEHOLDER\n");
        fclose(f);

        snprintf(path, sizeof(path), "%s/ca_pub.dil", outdir);
        f = fopen(path, "w");
        fprintf(f, "CA_PUBLIC_KEY_PLACEHOLDER\n");
        fclose(f);

        snprintf(path, sizeof(path), "%s/ca_cert.pem", outdir);
        f = fopen(path, "w");
        fprintf(f, "-----BEGIN CERTIFICATE-----\nMIID...PLACEHOLDER...IDAQAB\n-----END CERTIFICATE-----\n");
        fclose(f);

        printf("Wrote placeholder CA artifacts to %s\n", outdir);
        return 0;
    }

    if (strcmp(cmd, "issue") == 0) {
        const char* node = (argc > 3) ? argv[3] : "node1";
        snprintf(path, sizeof(path), "%s/%s_key.dil", outdir, node);
        FILE* f = fopen(path, "w");
        if (!f) { perror("fopen"); return 2; }
        fprintf(f, "NODE_PRIVATE_KEY_PLACEHOLDER for %s\n", node);
        fclose(f);

        snprintf(path, sizeof(path), "%s/%s_pub.dil", outdir, node);
        f = fopen(path, "w");
        fprintf(f, "NODE_PUBLIC_KEY_PLACEHOLDER for %s\n", node);
        fclose(f);

        snprintf(path, sizeof(path), "%s/%s_cert.pem", outdir, node);
        f = fopen(path, "w");
        fprintf(f, "-----BEGIN CERTIFICATE-----\nMIID...NODE:%s:PLACEHOLDER...IDAQAB\n-----END CERTIFICATE-----\n", node);
        fclose(f);

        printf("Issued placeholder node cert for %s at %s\n", node, outdir);
        return 0;
    }

    fprintf(stderr, "Unknown command '%s'\n", cmd);
    return 3;
}
