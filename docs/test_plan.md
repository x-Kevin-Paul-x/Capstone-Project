# Test Plan for PQC Secure Transport Layer

This document outlines the testing and validation strategy for the `pqc-secure-transport` library. The goal is to ensure correctness, measure performance against defined KPIs, and validate security claims through adversarial testing.

## 1. Test Categories

The testing process is divided into four main categories:
1.  **Unit Tests**: Verify the correctness of individual functions and modules in isolation.
2.  **Microbenchmarks**: Measure the raw performance of the core cryptographic primitives on target hardware.
3.  **System & Performance Tests**: Measure the end-to-end performance and overhead of the integrated ROS2 secure wrapper.
4.  **Security Validation Tests**: Actively attempt to break the security guarantees of the system.

---

## 2. Unit Tests

- **Objective**: Ensure that each component of the C API and internal modules behaves as expected.
- **Location**: `pqc-secure-transport/tests/`
- **Framework**: A lightweight C unit testing framework (e.g., Unity, CTest).
- **Test Cases**:
  - **Handshake**: Test key generation, encapsulation, decapsulation, and session key derivation.
  - **AEAD**: Test encryption and decryption with valid/invalid keys, nonces, and tags. Check that tampering with ciphertext, AAD, or the tag results in a verification failure.
  - **Envelope**: Test the binary message envelope parser with well-formed and malformed inputs.
  - **Key Manager**: Test key storage, retrieval, and rotation logic.
  - **API**: Test the main `pqc_*` functions for correct return values and error handling.

---

## 3. Microbenchmarks

- **Objective**: Quantify the computational cost of the selected PQC algorithms to inform optimization and identify bottlenecks.
- **Location**: `pqc-secure-transport/bench/`
- **Methodology**:
  - Measure CPU cycles and/or wall-clock time for the following operations:
    - `CRYSTALS-Kyber-768`: `crypto_kem_keypair`, `crypto_kem_enc`, `crypto_kem_dec`.
    - `CRYSTALS-Dilithium-III`: `crypto_sign_keypair`, `crypto_sign`, `crypto_sign_open`.
    - `Ascon-128a`: `crypto_aead_encrypt`, `crypto_aead_decrypt` for various payload sizes.
- **Target Platforms**:
  - **Desktop/Server**: Standard development machine (for baseline).
  - **Embedded**: Raspberry Pi 4 and/or a Cortex-M4 microcontroller (via pqm4 or similar).
- **Reporting**: Results will be reported in cycles/op and/or microseconds/op.

---

## 4. System & Performance Tests

- **Objective**: Measure the real-world impact of the secure layer on a ROS2 system and validate against the non-functional requirements (KPIs).
- **Location**: `pqc-secure-transport/examples/` (using `secure_talker`/`secure_listener`) and `scripts/`.
- **KPIs**:
  - **Latency**: Added median latency must be **< 10 ms** for a 1 KB message at 100 Hz.
  - **CPU Overhead**: Additional steady-state CPU usage must be **< 20%** on the target endpoint.
  - **Memory Footprint**: Dynamic RAM usage for crypto operations should be **< 64 KB**.
  - **Bandwidth Overhead**: Per-message overhead (header, tag, etc.) must be **< 64 bytes**.
- **Methodology**:
  - A "ping-pong" test will be set up where a message is sent from a talker to a listener, which then sends it back.
  - Measure Round-Trip Time (RTT) for various message sizes (256 B, 1 KB, 4 KB) and frequencies (10 Hz, 100 Hz).
  - Compare the results against a baseline measurement using standard, insecure ROS2 topics.
  - Use system monitoring tools (`top`, `ps`) to measure CPU and memory usage.

---

## 5. Security Validation Tests

- **Objective**: Proactively identify and fix vulnerabilities by simulating attacks.
- **Methodology**:
  - **Man-in-the-Middle (MITM) Attack**:
    - Use a proxy (like `mitmproxy` or a custom script) to intercept and arbitrarily modify the binary envelope on the "secure" topic.
    - **Expected Result**: The `secure_listener` must reject every modified message (AEAD verification failure).
  - **Replay Attack**:
    - Capture a valid message from the secure topic and reinject it onto the network.
    - **Expected Result**: The listener must reject the message based on the sequence number/nonce policy.
  - **Downgrade Attack**:
    - Manually craft a handshake message that suggests a weaker or null ciphersuite.
    - **Expected Result**: The handshake must fail. The protocol should be pinned to the agreed-upon ciphersuite.
  - **Fuzzing**:
    - Use a fuzzing framework (e.g., libFuzzer) to feed the `pqc_decrypt` and envelope parsing functions with random, malformed data.
    - **Expected Result**: The functions must not crash, leak memory, or exhibit undefined behavior. They should fail gracefully.

---

## 6. Reporting and Traceability

- All test plans, scripts, and results will be version-controlled in the repository.
- A Requirements Traceability Matrix will link each requirement from the `Implementation_plan.md` to a specific test case in this document.
- All performance and security test reports will include detailed setup information (hardware, software versions, build flags) to ensure reproducibility.
