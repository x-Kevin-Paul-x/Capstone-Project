# TODO — Actionable Roadmap and Prioritized Tasks

Purpose: turn the Implementation Plan and prototype work into concrete, trackable tickets and sprints. Each task below includes a short description, suggested owner(s), acceptance criteria, dependencies, and an estimated effort. Use these to create issues or sprint cards.

NOTE: Primary references you should keep open while working: `Literature_Review_Documents/Implementation_plan.md`, `Literature_Review_Documents/PQC_decision_log.md`, `tools/firmware_signing/`.

---

## Priority: Sprint 1 — Secure Boot (High)
Goal: produce signed firmware artifacts and a bootloader that verifies PQC signatures in HIL.

- Task: CI Firmware Signing Job
	- Owner: Firmware lead / DevOps
	- Description: Add a CI job that builds firmware artifacts, signs them using a securely stored signer key, and publishes signed artifacts to the artifact repo.
	- Acceptance criteria: CI produces `firmware/<name>-signed.bin` and stores signing metadata; signing key is stored in a secure secret store accessible to CI.
	- Dependencies: `tools/firmware_signing` prototype; CI runner with secret store.
	- Estimate: 3–5 days

- Task: Bootloader Verification & Recovery Path
	- Owner: Firmware lead
	- Description: Modify bootloader to verify signatures on firmware images and implement recovery/rollback when verification fails.
	- Acceptance criteria: Bootloader rejects tampered images; recovery image boots when primary fails; HIL test demonstrates fail/recover.
	- Dependencies: signed firmware artifact from CI job.
	- Estimate: 1–2 weeks

- Task: HIL Smoke Tests for Signed Firmware
	- Owner: Test lead
	- Description: Hardware-in-the-loop test that deploys signed firmware, validates boot, and checks basic robot operation.
	- Acceptance criteria: Pass/Fail report in CI for sample devices; tests fail if signature invalid.
	- Estimate: 2–4 days

---

## Priority: Sprint 2 — Secure Communication (Hybrid TLS) (High)
Goal: Prototype a hybrid TLS handshake (ECDH + PQC KEM) and measure handshake & steady-state performance.

- Task: Hybrid TLS Prototype (liboqs + OpenSSL or wrapped KEM)
	- Owner: Networking lead
	- Description: Produce a working hybrid TLS client and server that derive keys from ECDH + Kyber KEM and exercise a real message flow.
	- Acceptance criteria: Successful hybrid handshake under test; handshake time measured and recorded in `PQC_decision_log.md`.
	- Dependencies: PQC implementation (liboqs/PQClean) available on dev machine or container.
	- Estimate: 1–3 weeks

- Task: Benchmarks and QoS Evaluation
	- Owner: Performance lead
	- Description: Measure handshake latency, throughput, CPU, and memory under representative loads and network conditions (onboard, Wi‑Fi, LTE).
	- Acceptance criteria: Benchmark report with clear pass/fail vs real-time constraints; suggestions for mitigations (session resumption, PSKs).
	- Estimate: 1 week

---

## Priority: Sprint 3 — Middleware Signing & KMS (Medium-High)
Goal: Provide message-level signing or channel-level authenticated sessions and a prototype KMS for provisioning and rotation.

- Task: KMS Prototype (Provisioning & Rotation)
	- Owner: Security / Backend lead
	- Description: Simple KMS service (can be a prototype server) that provides device bootstrap, key issuance, and revocation endpoints.
	- Acceptance criteria: Devices can request keys from KMS over authenticated channel; API documented; revocation list applied to device logic.
	- Estimate: 2–4 weeks

- Task: Middleware Signing SDK / Wrapper
	- Owner: Middleware lead
	- Description: Implement an application-layer wrapper (SDK) to sign & verify messages using PQC signatures; later optional transport-layer integration.
	- Acceptance criteria: Example ROS/ROS2 publisher/subscriber using the SDK performs sign/verify and rejects tampered messages in tests.
	- Dependencies: `PQC_decision_log.md` chosen signatures; edge profiling results.
	- Estimate: 2–4 weeks

---

## Priority: Sprint 4 — Privacy Analytics (Medium)
Goal: Prototype FHE or hybrid analytics approach for one privacy-sensitive pipeline and measure feasibility.

- Task: Analytics Workload Selection
	- Owner: Data lead
	- Description: Pick one representative analytics task (e.g., aggregated telemetry or simple ML inference) to prototype with FHE.
	- Acceptance criteria: Clear workload definition, dataset, and baseline plaintext results for comparison.
	- Estimate: 2–3 days

- Task: FHE Prototype & Benchmark
	- Owner: Data lead
	- Description: Implement chosen workload using a server-side FHE library (e.g., Microsoft SEAL) and measure latency and cost.
	- Acceptance criteria: Correctness vs plaintext baseline; latency report and recommendation (FHE/hybrid/DP).
	- Estimate: 1–3 weeks

---

## Cross-cutting Tasks
- Observability & Metrics
	- Add telemetry for handshake success/failure, firmware verification times, signing/verification latencies to Prometheus/Grafana.
	- Owner: Observability/DevOps
	- Estimate: 1 week

- Documentation & ADRs
	- Keep `Implementation_plan.md`, ADRs, and `PQC_decision_log.md` up-to-date with measured numbers and final decisions.
	- Owner: Project lead
	- Estimate: ongoing

- Security Review & Compliance
	- Conduct an architecture security review before production rollouts; validate procurement and supply-chain controls for secure elements.
	- Owner: Security team / Advisor

---

## Hardware & Procurement
- Procure test devices with TPM or Secure Element support, and at least one gateway device capable of offloading PQC work for constrained devices.
- Owner: Hardware lead
- Estimate: procurement lead time (variable)

---

## Backlog / Nice-to-have (Lower priority)
- Transport-layer PQC integration for DDS/TLS plugin (invasive; for high-assurance deployments).
- Hardware-accelerated PQC (SIMD, ARMv8 crypto extensions) benchmarking and integration.

---

## How to claim a task / convert to tickets
1. Create an issue in the tracker named `Sprint X — <task name>` and paste the corresponding block from this TODO as the issue description.
2. Assign an owner and set an estimate using the provided estimate field.
3. Add related docs links: Implementation plan, ADRs, Decision Log, and tests.

## Quick references
- Implementation Plan: `Literature_Review_Documents/Implementation_plan.md`
- PQC Decision Log: `Literature_Review_Documents/PQC_decision_log.md`
- Firmware signing prototype: `tools/firmware_signing/`
- Tests for signing: `tools/firmware_signing/tests/`

---

If you'd like, I can now:
- create issues from the top N tasks for you, or
- create a `feat/docs-todo` branch and push these changes, or
- add a simple GitHub Actions CI template for the signing CI job.

