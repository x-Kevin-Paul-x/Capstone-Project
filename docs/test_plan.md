# Test Plan — Capstone PQC Layer

This test plan covers unit, integration, hardware-in-the-loop (HIL), performance, and security tests required to validate the PQC layer.

1. Unit tests
- Crypto primitive wrappers: sign/verify, kem/encap/decap; use reference test vectors.
- Key management API: provisioning, rotation, revocation.

2. Integration tests
- Bootloader verifies signed firmware and rejects modified images.
- Hybrid TLS handshake success between robot client and cloud server.
- Middleware message signing/verification across nodes.

3. HIL tests
- Deploy signed firmware to real devices; validate boot, sensor initialization, and basic motion commands.

4. Performance benchmarks
- Handshake latency (classical vs hybrid vs PQC-only).
- Sign/verify throughput for representative message rates.
- FHE primitive runtime for target analytics workload.

5. Security tests
- Tamper tests: modify firmware image and confirm bootloader rejects it.
- Replay attacks and message injection: middleware fuzzing.
- Key compromise simulation: verify revocation and reprovision flow.

6. Acceptance criteria
- All critical tests pass in CI for the prototype branch.
- End-to-end tests demonstrate acceptable latency for real-time topics (define numeric thresholds per robot).

