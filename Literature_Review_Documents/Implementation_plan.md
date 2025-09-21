# Implementation Plan: Unified Lightweight Post-Quantum Cryptographic Layer for Human-Robot Collaborative Workspaces in Industry 5.0

## Project Overview

**Problem Statement:** Human–robot collaborative workspaces in Industry 5.0 face rising risk from future quantum-capable adversaries. Existing security stacks lack a unified, lightweight post-quantum cryptographic (PQC) layer that preserves real-time guarantees and human safety.

**Objective:** Deliver a unified, lightweight PQC layer for secure, low-latency communication in human-robot collaboration (HRC), emphasizing practical integration with ROS2, measurable KPIs, and repeatable documentation for each engineering step.

---

## Executive Summary of Proposed Changes (corrections & focus)

- Replace incorrect algorithm choice "Ascon-Sign" (not a NIST PQC signature) with CRYSTALS-Dilithium for signatures. Retain Ascon-128a as a lightweight AEAD for data-plane encryption.
- Primary primitives: CRYSTALS-Kyber-768 (KEM, NIST Cat 3), CRYSTALS-Dilithium-III (signatures, NIST Cat 3), Ascon-128a (AEAD).
- Focus MVP: application-layer PQC wrapper for ROS2 topics/services (`pqc-secure-transport`) to demonstrate feasibility, benchmarking, and security testing. Leave CAN/EtherNet-IP/5G/BCI as documented future work.
- Add in-depth Phase 1 (research foundation), and a mandatory Documentation & Engineering Process covering ADRs, test plans, and acceptance criteria.

---

## Corrected Cryptography Stack (short)

   - KEM: CRYSTALS-Kyber-768 (pqm4 preferred for embedded builds) — session key establishment (NIST Cat 3).
- Signatures: CRYSTALS-Dilithium-III — node identity and certs (NIST Cat 3).
- AEAD: Ascon-128a for per-message confidentiality/integrity; AES-256-GCM as alternate on hardware-accelerated platforms.
- Optional/Hybrid: ECDH(P-256) + Kyber-768 for transitional interop (documented, opt-in).
- Avoid claiming "AES-256 equivalent"; use NIST security categories.

---

## Layered Architecture (high level)

```

## Current status & Next steps (updated 2025-09-22)

Current status:

- Phase 1 (Research Foundation & Architecture Design) - COMPLETE. The repository contains ADRs, threat model, test plan, and a scaffolded `pqc-secure-transport` with API stubs.
- `third_party/pqm4/` — the `mupq/pqm4` submodule is present and nested submodules initialized (host-side PQC implementations available).
- `tools/ca_helper/` — CA helper added and updated to be pqm4-aware (conditional compilation via `PQM4_AVAILABLE`); falls back to placeholders when pqm4 is not linked.
- `docs/protocol.md` and `docs/RTM.md` — protocol spec and requirements traceability added.

Next steps (short, actionable):

1. Ensure `tools/ca_helper` is built against pqm4 so it writes full key bytes and uses the CA SK to sign node certs (update include paths and link flags in CMake).
2. Implement `pqc-secure-transport/src/pqc.c` to use pqm4 Kyber (KEM) and Dilithium (sign/verify) and integrate Ascon-128a AEAD.
3. Add unit tests for handshake, KEM encaps/decaps, AEAD tag verification, and replay protection. Add CI job to run them.
4. Replace placeholder CA script fallbacks with calls to the compiled `ca_helper` or direct pqm4-based tools.

If you want, I can proceed to (A) wire the CA helper to use the exact Dilithium variant headers and sizes and produce proper binary/PEM outputs, or (B) implement the pqc internals in `src/pqc.c`. Tell me which to do next.
Application Layer (HRC control, telemetry, safety)
  └─ Unified PQC Layer (pqc-secure-transport)
       ├─ Identity (Dilithium certs & key mgmt)
       ├─ Handshake (Kyber KEM ± hybrid)
       ├─ Data plane AEAD (Ascon-128a / AES-GCM)
       ├─ Rekeying & key lifecycle
       └─ Logging & monitoring (no secrets logged)
Transport Layer (ROS2 topics/services over DDS)
Hardware Layer (Robot controllers, edge compute, sensors)
```

---

## Phase 1: Research Foundation & Architecture Design (Weeks 1–4) — Expanded

1. PQC Algorithm Selection (detailed)
   - Kyber-768 (KEM)
     - Rationale: NIST-selected, efficient KEM with good embedded evidence; available in liboqs.
     - Performance plan: measure encaps/decaps cycles on Pi4 and M4 microbenchmarks via pqm4.
   - Dilithium-III (Signatures)
     - Rationale: NIST PQC signature with deterministic integer arithmetic (avoids floating-point pitfalls); suited for identity/certificates.
     - Use-case: node cert issuance, signed firmware metadata, authenticated handshake.
   - Ascon-128a (AEAD)
     - Rationale: NIST lightweight competition lineage; low-latency per-message protection for constrained payloads.
     - Use-case: data-plane encryption on per-message basis with sequence/nonces.
   - Falcon (optional)
     - Rationale: consider only when verify-heavy workloads benefit; document trade-offs.
   - Hybrid migration
     - Document migration patterns: ECDH+Kyber in handshake when legacy interop required; prefer pure PQC where available.

2. Threat Model (structured)
   - Assets
     - Command topics (joint positions, setpoints), safety messages (E-stop), telemetry, firmware images, identity keys.
   - Adversaries
     - On-LAN MITM, compromised node, malicious gateway, side-channel observer, replay attacker.
   - Boundaries
     - Robot controller ↔ edge node; ROS graph discovery; network edges (WiFi, 5G, wired).
   - Misuse cases
     - MITM injects unsafe commands; replay clears E-stop; impersonation publishes hazardous setpoints.
   - Security goals derived
     - Mutual authentication (Dilithium).
     - Confidentiality + integrity (Ascon AEAD).
     - Forward secrecy via periodic Kyber rekey.
     - Replay resistance: monotonic sequence numbers + AEAD.
     - Pinning cipher-suite and downgrade protection.

3. Requirements (functional & non-functional, measurable)
   - Functional
     - Provide authenticated handshake, session key derivation, AEAD data plane, rekeying, node revocation.
     - Integrate with ROS2 topics/services without modifying DDS core for MVP.
   - Non-functional (KPIs)
     - Latency: added median latency $<10\,\mathrm{ms}$ for 1 KB messages @ 100 Hz (LAN).
     - CPU overhead: $<20\%$ additional steady-state CPU on endpoint.
     - Memory: dynamic RAM footprint $<64\,\text{KB}$ on embedded target for crypto ops (MVP target).
     - Bandwidth: per-message overhead (header+tag+nonce) $<64$ bytes.
   - Acceptance
     - All KPIs must be tested and documented; deviations require mitigation plan.

4. Architecture Design Decisions (to record as ADRs)
   - Application-layer wrapper (Path A) chosen for MVP — lower integration friction and faster demos.
   - DDS/DDS-Security replacement (Path B) documented as stretch goal.
   - Key management with optional TPM/TEE; fallback to encrypted keystore.
   - Rekey policy: default T = 10 minutes or M = 10k messages (parameters subject to benchmarks).

5. Measurement Strategy (defined up front)
   - Microbenchmarks:
     - Kyber encaps/decaps, Dilithium sign/verify, Ascon encrypt/decrypt cycles.
   - System tests:
     - Ping-pong RTT across secure_talker/secure_listener for message sizes 256 B / 1 KB / 4 KB at rates 10/100 Hz.
   - Security tests:
     - MITM via proxy, replay injection, nonce reuse tests, downgrade attempts, fuzzing envelope parser.
   - Reporting:
     - Raw data, plots, configuration artifacts, versions, and environment details are archived.

---

## Phase 2: Core Development (Weeks 5–12)

1. Core modules
   - KEM module: kyber_keygen(), kyber_encaps(), kyber_decaps(). Use liboqs for prototyping; consider optimized/pqm4 for M4 tests.
   - Signature module: dilithium_sign(), dilithium_verify(); certificate issuance tooling.
   - AEAD module: ascon_encrypt(), ascon_decrypt(); nonce and seq policy.
   - Key manager: secure storage, rotation, CRL handling, audit log pointers.

2. API (C interface)
```c
// C
typedef struct { uint8_t key[32]; uint64_t seq; } pqc_session_t;
int pqc_handshake(int fd, pqc_session_t* s);           // Dilithium auth + Kyber KEM → K
int pqc_rekey(int fd, pqc_session_t* s);               // periodic KEM rekey
int pqc_encrypt(pqc_session_t* s, const uint8_t* in, size_t inlen,
                const uint8_t* aad, size_t aadlen, uint8_t* out, size_t* outlen);
int pqc_decrypt(pqc_session_t* s, const uint8_t* in, size_t inlen,
                const uint8_t* aad, size_t aadlen, uint8_t* out, size_t* outlen);
```

3. Implementation notes
   - Use pqm4 for embedded and constrained-target builds (preferred). For desktop prototypes, lightweight wrappers or OQS-OpenSSL can be used as needed.
   - For embedded microbenchmarks, use pqm4 or optimized C implementations and document build flags and trade-offs.

---

## Phase 3: Integration & Optimization (Weeks 13–16)

1. ROS2 integration
   - Path A (MVP): `secure_talker` and `secure_listener` wrappers that:
     - Serialize ROS message → apply AEAD with per-session key → publish binary envelope on a secure topic.
     - Listener: subscribe → AEAD decrypt/verify → deserialize and forward to application callback or virtual topic.
   - QoS: preserve original QoS semantics; ensure ordered delivery where required; support reliable/unreliable modes with distinct replay handling.

2. Optimization strategies
   - Memory pooling and buffer reuse; zero-copy patterns where feasible.
   - Hardware acceleration (AES-NI or ARM Crypto) detection path for AES-GCM where faster.
   - Profile-guided hot-path optimization (AEAD enc/dec).

3. Side-channel hardening
   - Prefer constant-time libraries and avoid variable-time primitives.
   - Use masked NTT variants if available for Kyber on target platforms.
   - Compiler and linker options, stack wiping, and zeroization routines documented.

---

## Phase 4: Testing & Validation (Weeks 17–20)

1. Security validation
   - Formal proofs replaced with standardized library usage + adversarial tests.
   - Side-channel testing limited to microbenchmarks on M4 (power/timing observations) and documented countermeasures.
   - Pen-testing: MITM proxy tests; replay/downgrade tests.

2. Performance benchmarking
   - Latency, jitter, CPU, RAM, and throughput measurements; compare baseline (no PQC) vs. secure layer.
   - Plotting, CI scripts, and reproducible test harnesses.

3. Real-world validation
   - Demo scenario: teleoperation or sensor stream in Gazebo/RVIZ with secure topics.
   - Optional embedded demo: M4 microbenchmark report or Pi4 end-to-end.

---

## Documentation and Engineering Process (applies to every phase)

Purpose: Ensure reproducibility, traceability, and clear grading evidence.

1. Artifacts to produce (per phase)
   - Literature matrix and decision log (Phase 1).
   - Threat model document and attack trees (Phase 1).
   - Architecture Decision Records (ADRs) for each major choice.
   - API spec with Doxygen and examples (Phase 2).
   - Test plans and automation scripts; bench harness (Phase 2/3).
   - Security test report and raw logs (Phase 4).
   - Deployment/operator guide: CA issuance, provisioning, rotation, revocation.

2. Process controls
   - ADR template: context → decision → alternatives considered → rationale → consequences.
   - Requirements traceability matrix: requirement → design element → test case → result.
   - Change log and semantic versioning for the library and sample nodes.
   - AI usage log: record prompts used to generate code/design text; include human review notes especially for crypto-critical code.

3. Acceptance criteria (selected)
   - Handshake: mutual auth + key confirmation; rekey handshake completes without message loss.
   - Data plane: AEAD verification fails on tamper; stale seq rejected.
   - Performance: median added latency $<10\,\mathrm{ms}$ (1 KB @ 100 Hz) or documented explanation and mitigation.
   - Security tests: MITM/spoof/replay/downgrade attacks fail in controlled tests.

4. Templates and artifacts (to include in repo)
   - `docs/ADRs/` with at least: Kyber vs alternatives ADR, Dilithium ADR, Ascon ADR, ROS2 Path ADR.
   - `docs/threat_model.md`
   - `docs/test_plan.md` and `bench/` scripts for reproducibility.
   - `docs/deployment.md` for operator steps.

---

## Deliverables

- `pqc-secure-transport` C/C++ library (semantic versioned).
- ROS2 sample nodes: `secure_talker`, `secure_listener`.
- CA tooling: cert issue/revoke scripts and sample certs (test CA).
- Benchmark reports, security test reports, and documentation (ADRs, threat model, deployment guide).
- Optional: embedded microbenchmark report (M4) or Pi4 end-to-end tests.

---

## Timeline (revised concise)

- W1–2: Detailed threat model, ADRs, crypto selection, protocol spec, CA profile.
- W3–6: Core KEM/signature/AEAD modules, PQC handshake design, unit tests.
- W7–10: ROS2 wrappers, rekeying, sample nodes, system tests.
- W11–14: Benchmarks, performance tuning, side-channel mitigations for microbenchmarks.
- W15–16: Security testing, docs, final demo, and report.

---

## Risk Mitigation & Future Work

- Over-scope risk: maintain MVP lane (Path A). Defer DDS core changes, CAN/Ethernet-IP hardening, full BCI integration to future work.
- Side-channel complexity: use vetted libraries and document remaining risks.
- Standards & compliance: map to NIST PQC categories and industrial safety standards (ISO 10218, ISO/TS 15066, IEC 62443).

---

## Appendix: Simple Protocol Sequence (handshake + data plane)

 1. Bootstrapping: Node obtains Dilithium cert from test CA (out-of-band).
2. Handshake:
   - A -> B: CertA (Dilithium-signed), nonceA
   - B -> A: CertB, nonceB, Kyber-encaps(ciphertextB)
   - Both derive session key K = KDF(nonceA || nonceB || kyber_shared)
   - Key confirmation: HMAC/KDF tag exchange (optional)
3. Data plane:
   - For each message: seq++, nonce = session_counter || seq
   - Ciphertext = Ascon-128a-Enc(K, nonce, plaintext, AAD)
   - Envelope: header (version, ciphersuite, seq, nonce, aad_hash), ciphertext, tag
4. Rekey:
   - Trigger per policy: new Kyber encaps/decaps → derive K' and switch over with overlap window.

---

This file is the updated, corrected, and documented Implementation Plan. Follow-up actions (in repo) recommended:
- Commit this file with a clear ADR referencing the corrections.
- Create `docs/` folder with ADR templates and initial threat model.
- Scaffold `pqc-secure-transport` repo structure and starter CMake files.


Progress update (2025-09-22):
- Phase 1 (Research Foundation & Architecture Design) - COMPLETE. The repository now contains the Phase 1 artifacts: ADRs, threat model, test plan, and a scaffolded `pqc-secure-transport` with API stubs.
- pqm4: A pqm4 stub directory was added at `third_party/pqm4/` with guidance to add the real pqm4 sources as a submodule or vendor the minimal required algorithms.
- Protocol spec: `docs/protocol.md` added describing handshake, KDF inputs, envelope layout, and rekey policy.
- Requirements Traceability Matrix: `docs/RTM.md` added to tie requirements to artifacts and tests.
- CA tooling: Script templates and a placeholder helper were added under `scripts/ca/` and `tools/ca_helper/`. The helper currently writes placeholder keys/certs and will perform real pqm4-backed keygen/sign when `pqm4` is integrated.

Next recommended actions (short):
- Add pqm4 as a git submodule or vendor selected sources into `third_party/pqm4` and update `tools/ca_helper` to call pqm4 APIs to generate Dilithium keypairs and sign certs.
- Replace the placeholder operations in `scripts/ca/*` with pqm4-based keygen/signing commands or the compiled `ca_helper` once pqm4 is available.
- Implement `src/pqc.c` internals to call pqm4 Kyber/Dilithium implementations and Ascon-128a AEAD.

Next recommended actions (short):
- Add pqm4 as a git submodule or vendor selected sources into `third_party/pqm4`.
- Replace the placeholder operations in `scripts/ca/*` with pqm4-based keygen/signing tools or a small helper binary that links pqm4.
- Implement `src/pqc.c` internals to call pqm4 Kyber/Dilithium implementations and Ascon-128a AEAD.
