# Protocol Specification for pqc-secure-transport

This document defines the handshake, session key derivation, message envelope layout, sequence/nonce policy, error codes, and ciphersuite identifiers for the `pqc-secure-transport` project.

Status: Draft — completes Phase 1 documentation requirements. Implementation depends on integrating `pqm4` for PQC primitives.

## Glossary
- KEM: Key Encapsulation Mechanism (Kyber)
- SIG: Signature algorithm (Dilithium)
- AEAD: Authenticated Encryption with Associated Data (Ascon-128a)
- KDF: HKDF-SHA256 (recommended)

## Ciphersuite identifier
- `0x01` = Kyber-768 | Dilithium-III | Ascon-128a | HKDF-SHA256

## Handshake sequence
1. A -> B: HELLO, Version(1), Ciphersuite(0x01), CertA (Dilithium cert), NonceA (32 bytes)
2. B -> A: HELLO_ACK, Version(1), Ciphersuite(0x01), CertB (Dilithium cert), NonceB (32 bytes), KyberCiphertext (variable)
3. A -> B: CONFIRM, KDF_Tag (optional key-confirmation HMAC-SHA256)

Notes:
- Certificates are signed Dilithium public keys in PEM-like container. Validation: verify certificate signature chain using test CA provided by the operator.
- Nonces: 32 bytes randomly generated per handshake endpoint.

## Session Key Derivation
- Kyber shared secret derived by decapsulation on recipient endpoint.
- KDF input: HKDF-SHA256(salt = NonceA || NonceB, IKM = kyber_shared_secret || CertA_hash || CertB_hash)
- Derive: 32-byte AES-like symmetric key K_enc and 32-byte K_auth (or a single 32/64 byte symmetric key for AEAD as required by Ascon API).

## Message envelope layout (binary)
- Header (fixed-size, big-endian fields)
  - Version (1 byte)
  - Ciphersuite (1 byte)
  - Flags (1 byte) — bitfield (e.g., 0x01 = compressed, 0x02 = reserved)
  - Seq (8 bytes) — monotonically increasing 64-bit sequence number
  - Nonce (12 bytes) — per-message nonce (constructed from session counter + seq or random per-message nonce)
  - AAD length (2 bytes)
- AAD (variable): application-level AAD (e.g., topic name, qos metadata)
- Ciphertext (variable)
- Tag (16 bytes) — Ascon-128a tag size

Total header overhead: 1+1+1+8+12+2 = 25 bytes + AAD

Nonce construction:
- Use session-specific random 12-byte base nonce (derived from KDF) XORed with 64-bit seq to produce unique per-message nonce.
- Reject messages where seq <= last_seen_seq (replay) unless within an explicit allowed window for reordering (window size configurable, default 64).

## Rekeying
- Rekey triggers: elapsed time > T (default 10 minutes) OR messages_sent > M (default 10k) OR manual operator trigger.
- Rekey uses Kyber encaps/decaps exchange similar to handshake but with short control header and KDF derivation including previous session `K_prev` for forward secrecy.
- During rekey, accept messages encrypted under either previous or new key for an overlap window of 10 seconds (configurable).

## Error handling
- On AEAD tag failure: discard packet and increment `tag_fail` counter. If `tag_fail` exceeds threshold (default 5 within 1 minute), drop session and trigger a rekey/handshake.
- On signature/cert verification failure: abort handshake and log the error. Optionally report to operator.
- On sequence window violation: drop packet and log; if repeated, alert operator and optionally blacklist peer.

## Replay protection
- Maintain `last_seen_seq` per peer session; accept packets with `seq` strictly greater than `last_seen_seq` or within the reordering window. For unreliable transport, reordering window default = 64.

## AAD semantics
- AAD MUST include the topic/service name, QoS metadata, and any message-specific routing information to prevent cross-topic replay or substitution.

## Implementation notes
- Use constant-time primitives where available. Avoid variable-time string parsing in hot paths.
- Store session secrets in secure memory regions when available and zeroize on session teardown.
- Use HKDF-SHA256 as a portable KDF; document environment-specific alternatives.

## Versioning
- Version field increments on breaking changes to the envelope layout or KDF inputs.

## Example flow (short)
- 1) Node A and B exchange HELLOs with nonces and certificates.
- 2) B sends Kyber ciphertext; A decapsulates and both derive K.
- 3) A sends CONFIRM with KDF tag; both start sending data-plane messages using Ascon-128a with seq starting at 1.


Appendix: sizes and constants
- Nonce length (handshake): 32 bytes
- Per-message nonce length: 12 bytes
- Ascon-128a tag: 16 bytes
- Seq: 8 bytes (uint64)


Status: this protocol doc completes the Phase 1 specification requirement. Implementation requires integrating pqm4 for Kyber/Dilithium operations.
