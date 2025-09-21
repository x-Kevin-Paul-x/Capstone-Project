Title: Choose Kyber-768 (KEM) and Dilithium-III (Signatures)
Status: Accepted
Context: Project requires a lightweight PQC layer suited for embedded HRC scenarios. NIST-selected algorithms preferred.
Decision: Use CRYSTALS-Kyber-768 for KEM (session establishment) and CRYSTALS-Dilithium-III for signatures.
Consequences:
- Aligns design with NIST PQC categories (Cat 3).
- liboqs provides reference implementations for prototyping.
- Ascon-128a retained for AEAD in the data plane.
