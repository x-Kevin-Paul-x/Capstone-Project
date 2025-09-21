# Project TODO — PQC Secure Transport (Capstone)

- [x] Analyze requirements & research (literature review, threat model inputs)
- [x] Update and finalize Implementation_plan.md
 - [x] Scaffold repository: `pqc-secure-transport` (CMake, src/, include/, tests/, bench/)
- [ ] Add ADRs folder and templates (`docs/ADRs/`)
-- [ ] Implement KEM module (CRYSTALS-Kyber-768) — (pqm4 preferred for embedded builds)
- [ ] Implement Signature module (CRYSTALS-Dilithium-III) — cert tooling
- [ ] Implement AEAD module (Ascon-128a) and envelope format
- [ ] Implement Key Manager (keystore, rotation, CRL handling, optional TPM/TEE hooks)
- [ ] Expose C API (pqc_session_t, pqc_handshake, pqc_encrypt, pqc_decrypt)
- [ ] Implement ROS2 wrappers: `secure_talker`, `secure_listener`
- [ ] Build benchmark harness and automation scripts (`bench/`)
- [ ] Run functional security tests: MITM proxy, replay, downgrade
- [ ] Run performance tests: latency/jitter/CPU/RAM for 256B/1KB/4KB @ 10/100 Hz
- [ ] Optional: embedded microbenchmarks (pqm4 on Cortex-M4) and side-channel checks
- [ ] Documentation: ADRs, threat_model.md, test_plan.md, deployment.md, AI usage log
- [ ] Optimization & hardening: buffer reuse, constant-time flags, zeroization
- [ ] Prepare demo (Gazebo/RVIZ teleop or sensor stream) and final report
- [ ] Prepare slides and submission artifacts
- [ ] Tag release and push artifacts to GitHub (semantic versioning)

Notes
- Follow templates in `docs/` for ADRs, threat model, and test plans.
- Prioritize MVP lane (application-layer ROS2 wrappers) before advanced integration.

# TODO (updated)

## Phase 1 — COMPLETE
Status: Done
- Literature review & implementation plan
- Threat model
- Test plan
- Deployment / CA design & operator guide (design only)
- ADRs (algorithm selection, wrapper, ROS2 integration)
- Project scaffold (CMake, include/, src/ stubs)
  - pqm4 stub added: `third_party/pqm4/` (README + CMakeLists stub)
  - CA script templates added: `scripts/ca/create_ca.*`, `scripts/ca/issue_node_cert.*`
  - CA helper tool added: `tools/ca_helper/ca_helper` (placeholder mode). When `pqm4` is integrated, `ca_helper` will perform real Dilithium keygen and signing.
- Examples & basic tests scaffolds

Acceptance criteria satisfied for Phase 1:
- ADRs committed
- Public API declared in `include/pqc.h`
- Build + example scaffolds run (CMake)
- High-level documentation (deployment, threat model, test plan)

Implementation status (code-level)
- `pqc-secure-transport/include/pqc.h` declares the public API: `pqc_session_t`, `pqc_handshake`, `pqc_rekey`, `pqc_encrypt`, `pqc_decrypt`.
- `pqc-secure-transport/src/pqc.c` contains scaffold/stub implementations:
  - `pqc_handshake` and `pqc_rekey` currently return an error (TODO: implement Dilithium auth + Kyber KEM flow).
  - `pqc_encrypt` and `pqc_decrypt` are simple pass-through/memcpy placeholders (TODO: replace with Ascon-128a AEAD and tag verification).
- Examples (`examples/secure_talker.c`, `examples/secure_listener.c`) exercise the API with the current stubs.
- `tools/ca_helper` has conditional pqm4-aware code but will fall back to placeholder behavior unless linked to pqm4 at build time.

Short summary: the Phase 1 artifacts (documentation, ADRs, repo scaffold, and public API) are present and marked COMPLETE. The cryptographic core (KEM, signatures, AEAD) remains unimplemented and is intentionally represented by stubs — these are Phase 2 deliverables.

## Phase 2 — Implementation & Hardening (next)
Target: deliver working PQC handshake, AEAD, CA tooling, tests, and protocol spec.

Priority tasks (owner / ETA / acceptance criteria)
- Integrate pqm4 for PQC primitives (Kyber KEM, Dilithium signatures)
  - Owner: kevin
  - ETA: 2025-10-15
  - Acceptance criteria:
    - `third_party/pqm4` available and linked by CMake
    - Host-side build exposes Kyber encaps/decaps and Dilithium sign/verify
    - Unit tests for KEM/signature added and passing locally
- Implement Ascon-128a AEAD and replace `pqc_encrypt`/`pqc_decrypt` stubs
  - Owner: kevin
  - ETA: 2025-10-15
  - Acceptance criteria:
    - Ascon-128a AEAD implemented and covered by unit tests
    - AEAD tag verification and associated-data handling implemented
    - Tests include tag-failure and replay/sequence checks
- Implement `pqc_handshake` and `pqc_rekey` (Dilithium verify + Kyber encaps/decaps)
  - Owner: kevin
  - ETA: 2025-10-22
  - Acceptance criteria:
    - `pqc_handshake` derives session key via authenticated KEM and validates Dilithium certs
    - `pqc_rekey` performs periodic secure rekeying using KEM
    - Integration test: `secure_talker` <-> `secure_listener` establish session and exchange authenticated AEAD payloads
- Add protocol specification (`docs/protocol.md`) with envelope layout, nonces, KDF inputs, seq/replay policy
  - Owner: kevin
  - ETA: 2025-10-05
  - Acceptance criteria:
    - `docs/protocol.md` contains message-flow diagrams, envelope bytes, KDF derivation, and replay protection design
    - Examples reference the protocol and show sequence handling
- CA tooling and cert issuance scripts (scripts/ca/create_ca.sh, issue_node_cert.sh)
  - Owner: kevin
  - ETA: 2025-10-01
  - Acceptance criteria:
    - `tools/ca_helper` builds against pqm4 and performs Dilithium keygen and signing
    - Scripts call compiled helper when available; fallback behavior removed or documented
    - Test CA keys and example node certs are present under `test-ca/`
- Unit tests: handshake success/failure, KEM encaps/decaps, AEAD tag verification and replay protection
  - Owner: kevin
  - ETA: 2025-10-15
  - Acceptance criteria:
    - Unit tests in `pqc-secure-transport/tests` cover happy/failure paths
    - Tests runnable via CTest or provided runner and fail fast on errors
- Integration tests & CI: build matrix, run unit tests, run examples in CI runner
  - Owner: kevin / CI admin
  - ETA: 2025-10-30
  - Acceptance criteria:
    - CI builds on Linux/Windows and runs unit tests
    - Integration job runs example handshake and basic message exchange
- CRL / revocation handling and keystore format specification + tests
  - Owner: kevin
  - ETA: 2025-11-01
+- Benchmarks & performance measurements (bench/)
  - Owner: kevin
  - ETA: 2025-11-15
  - Acceptance criteria:
    - `bench/` contains a reproducible harness and run script for common scenarios

Done criteria for each task:
- Code merged to `main` with tests passing
- Documentation updated (`docs/protocol.md`, `docs/deployment.md`)
- Example usage documented in README and operator guide

Notes:
- Use pqm4 (not liboqs) as the primary PQC implementation target (see implementation plan).
- Keep the current public API in `include/pqc.h` stable; implement internals in `src/pqc.c`.
- Mark each completed task with a PR and link ADRs/tests for traceability.

## Updates (2025-09-22)

- `tools/ca_helper` has been updated to include pqm4-aware code behind `PQM4_AVAILABLE` and its `CMakeLists.txt` now sets the compile definition when the pqm4 target is present. The helper still falls back to placeholder mode when pqm4 is not linked.
- `third_party/pqm4/` contains the real `mupq/pqm4` submodule (nested submodules initialized). This enables direct linking to pqm4 primitives for host-side builds.

## Immediate Phase 2 Actions (short)

1. Link `tools/ca_helper` to pqm4 and update include paths so the helper writes full key material and uses the CA SK to sign node certs. (Owner: kevin, ETA: 2025-10-01)
2. Implement KEM/signature/AEAD internals in `pqc-secure-transport/src/pqc.c` and add unit tests for handshake and AEAD flows. (Owner: kevin, ETA: 2025-10-15)
3. Replace placeholder script fallbacks with calls to the compiled `ca_helper` or direct pqm4-based tools. (Owner: kevin, ETA: 2025-10-01)

Status flags:

- [x] `tools/ca_helper` pqm4-aware helper added (build-time conditional)
- [ ] Link/compile `tools/ca_helper` against pqm4 and finalize key formats
