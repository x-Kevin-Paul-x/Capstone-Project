# Protocols and Integration

This document specifies the secure communication protocols and message formats used by the Capstone project. It focuses on hybrid key exchange, message-level authentication, and middleware integration (ROS/ROS2).

1. Hybrid TLS Key Exchange
- Use TLS 1.3 with a hybrid key exchange: combine classical ECDH (P-256) with a PQC KEM (Kyber family) to derive the session key. This approach provides immediate post-quantum protection while retaining interoperability.
- Implementation options:
	- OpenSSL + liboqs (preferred for prototyping).
	- Vendor PQC-enabled TLS stacks for production if available.

Handshake derivation:
- Client and server perform ECDH -> derive pre-master secret.
- Client encapsulates PQC KEM ciphertext to server public KEM key; server decapsulates.
- Combine ECDH shared secret + KEM shared secret using HKDF to derive TLS keying material.

2. Message formats & signing
- Transport-level: TLS (hybrid) provides confidentiality and integrity.
- Message-level: critical control messages are additionally signed using a PQC signature (Dilithium) to provide non-repudiation and cross-checks in case transport is compromised.
- Message envelope (JSON for readability in examples):
	{
		"header": {
			"msg_id": "uuid",
			"timestamp": "ISO8601",
			"sender": "node-id",
			"signature": "base64(sig)"
		},
		"payload": { ... }
	}

3. Middleware (ROS/ROS2) integration
- ROS2 DDS transport should be configured to use the hybrid TLS plugin where possible.
- For message-level signing, provide SDK bindings or middleware plugins so application code can sign and verify messages transparently.

4. Key provisioning and rotation
- Devices bootstrapped via KMS: each device obtains an identity certificate and PQC key pairs during provisioning.
- Key rotation: support overlapping validity for old and new keys and distribute revocation lists via middleware update channels.

5. Performance & QoS
- For real-time topics, avoid per-message signatures where possible; prefer channel/session authentication and selective signing of critical commands.
- Monitor latency and jitter introduced by the hybrid handshake and message verification; use session keep-alives and resumption to minimize handshake frequency.

6. Interoperability
- Maintain a clear version negotiation protocol for PQC usage so older clients can interoperate using classical TLS until they are upgraded.

---

This document should be kept in sync with the `Implementation_plan.md` and the `PQC_decision_log.md` for algorithm choices and parameter sets.
