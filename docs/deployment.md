## Deployment Guide — Post-Quantum Cryptographic Layer for Robotics

This document describes deployment considerations for the Capstone project: a post-quantum cryptographic (PQC) layer for secure human-robot collaboration.

Environments
- Development: developer workstations, CI runners, hardware-in-the-loop (HIL) test benches.
- Staging: lab robots and a staging cloud environment replicating production topology.
- Production: factory floor robots, cloud services, and management consoles.

Deployment artifacts
- Signed firmware images (artifact: `firmware/<name>-signed.bin`)
- Public key bundles used by bootloaders (artifact: `keys/public_bundle.json`)
- Middleware packages with hybrid-TLS/DDS patches
- KMS configuration and seed provisioning packages

Prerequisites
- CI runner capable of signing artifacts and storing signer keys in a secure store (hardware-backed if possible).
- Secure provisioning channel for initial device onboarding (USB or authenticated network bootstrap).
- Observability stack (Prometheus/Grafana or equivalent) for collecting handshake, verification, and latency metrics.

Deployment steps (high-level)
1. Provision test devices with base firmware and record device identifiers.
2. Generate PQC key material in KMS and provision public keys to staging bootloaders.
3. Build and sign firmware artifact in CI; publish to signed artifact repository.
4. Stage firmware to a small set of devices; run HIL verification and smoke tests.
5. Gradually roll out to production devices using canary groups while monitoring metrics and rollback triggers.

Rollback & Recovery
- Maintain a minimal recovery image signed with a separate recovery key stored offline.
- Devices must support a fallback path to recovery mode when signature checks fail.

Monitoring & Alerts
- Handshake failures at scale
- Unexpected increase in verification time or boot time
- Key rotation events and provisioning failures

Security notes
- Use hardware secure elements (TPM/SE) for private key storage where possible.
- Audit all key material access and keep a tamper-evident audit trail.

---

## Notes
Adjust steps to match your environment (ROS/ROS2, DDS implementation, hardware platform). This is a living document and should be updated alongside ADRs and the implementation plan.
