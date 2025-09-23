# ADR 0001 — Kyber & Dilithium for KEM and Signatures
+
+Status: Proposed
+
+Context
+- The project requires PQC primitives for both key exchange (KEM) and digital signatures for firmware and message authentication.
+- NIST has selected candidates (Kyber family for KEMs and CRYSTALS-Dilithium for signatures) offering a balance of performance and security.
+
+Decision
+- Prototype with Kyber (KEM) and Dilithium (signatures) using PQClean/liboqs implementations at NIST level 2 or 3 depending on device capability.
+
+Consequences
+- Pros: Good performance, relatively small public keys and ciphertexts compared to other PQC candidates; robust reference implementations.
+- Cons: Larger sizes than classical ECC; careful profiling required for constrained devices.
+
+-- End ADR 0001
