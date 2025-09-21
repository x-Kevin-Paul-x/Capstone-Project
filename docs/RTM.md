# Requirements Traceability Matrix (RTM)

This RTM maps high-level requirements to design artifacts and test cases for the pqc-secure-transport project.

| Req ID | Requirement summary | Design artifact (ADR / doc) | Test(s) | Status |
|--------|---------------------|----------------------------|---------|--------|
| R01 | Mutual authentication of nodes | ADR: Dilithium selection; docs/protocol.md (handshake) | Handshake sign/verify unit test | Draft |
| R02 | Confidentiality and integrity of messages | ADR: Ascon selection; docs/protocol.md (envelope) | AEAD encrypt/decrypt/test vectors | Draft |
| R03 | Forward secrecy (periodic rekey) | Implementation plan; docs/protocol.md (rekey) | Rekey integration test | Draft |
| R04 | Replay resistance | docs/protocol.md (seq/nonce policy) | Replay injection test | Draft |
| R05 | Performance constraints (latency) | Implementation plan (KPIs) | Latency benchmarks 256B/1KB | Draft |
| R06 | CA tooling for test deployment | docs/deployment.md; scripts/ca/* | Generate test certs; integration demo | Partial (templates added) |

Notes:
- Status `Draft` indicates design exists but tests/automation need to be implemented.
- Requirement R06 remains `Partial` until scripts/ca generate real Dilithium keys (pqm4 integration).

This RTM is intended to be updated as unit/integration tests are added and pass.
