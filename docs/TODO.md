# Project TODO — PQC Secure Transport (Capstone)

- [x] Analyze requirements & research (literature review, threat model inputs)
- [x] Update and finalize Implementation_plan.md
- [ ] Scaffold repository: `pqc-secure-transport` (CMake, src/, include/, tests/, bench/)
- [ ] Add ADRs folder and templates (`docs/ADRs/`)
- [ ] Implement KEM module (CRYSTALS-Kyber-768) — liboqs prototype
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
