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
 - Project scaffold (CMake, include/, src/ stubs)
 - pqm4 stub added: `third_party/pqm4/` (README + CMakeLists stub)
 - CA script templates added: `scripts/ca/create_ca.*`, `scripts/ca/issue_node_cert.*`
 - CA helper tool added: `tools/ca_helper/ca_helper` (placeholder mode). When `pqm4` is integrated, `ca_helper` will perform real Dilithium keygen and signing.
- Examples & basic tests scaffolds

Acceptance criteria satisfied for Phase 1:
- ADRs committed
- Public API stubs in include/
- Build + example scaffolds run (CMake)
- High-level documentation (deployment, threat model, test plan)

## Phase 2 — Implementation & Hardening (next)
Target: deliver working PQC handshake, AEAD, CA tooling, tests, and protocol spec.

Tasks (owner / ETA)
- Integrate pqm4 for PQC primitives (Kyber KEM, Dilithium signatures)  
  Owner: kevin  
  ETA: 2025-10-15
- Implement Ascon-128a AEAD and replace pqc_encrypt/pqc_decrypt stubs  
  Owner: kevin  
  ETA: 2025-10-15
- Implement pqc_handshake and pqc_rekey (Dilithium verify + Kyber encaps/decaps)  
  Owner: kevin  
  ETA: 2025-10-22
- Add protocol specification (docs/protocol.md) with envelope layout, nonces, KDF inputs, seq/replay policy  
  Owner: kevin  
  ETA: 2025-10-05
- CA tooling and cert issuance scripts (scripts/ca/create_ca.sh, issue_node_cert.sh) with example keys (test CA)  
  Owner: kevin  
  ETA: 2025-10-01
- Unit tests: handshake success/failure, KEM encaps/decaps, AEAD tag verification and replay protection  
  Owner: kevin  
  ETA: 2025-10-15
- Integration tests & CI: build matrix, run unit tests, run examples in CI runner  
  Owner: kevin / CI admin  
  ETA: 2025-10-30
- CRL / revocation handling and keystore format specification + tests  
  Owner: kevin  
  ETA: 2025-11-01
- Benchmarks & performance measurements (bench/)  
  Owner: kevin  
  ETA: 2025-11-15

Done criteria for each task:
- Code merged to main with tests passing
- Documentation updated (docs/protocol.md, docs/deployment.md)
- Example usage documented in README and operator guide

Notes:
- Use pqm4 (not liboqs) as the primary PQC implementation target (see implementation plan).
- Keep the current public API in include/pqc.h stable; implement internals in src/pqc.c.
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
