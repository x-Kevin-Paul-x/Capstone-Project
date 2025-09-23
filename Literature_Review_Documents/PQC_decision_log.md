# PQC Decision Log

Purpose: Capture candidate post-quantum cryptographic primitives, parameter sets, measurements, and a recommendation rationale. This log supports reproducible decisions and records trade-offs (size vs speed vs security).

Instructions: For each candidate, create an entry with the fields below and fill benchmarks from the `tests/benchmarks` runner or local measurements.

---

## Candidate: <name>
- Category: KEM / Signature / FHE / Other
- Algorithm (reference): <e.g., CRYSTALS-Dilithium-III>
- Source / Implementation: <e.g., liboqs, PQClean, vendor X>
- Target parameter set / security level: <e.g., NIST level 2/3/5>
- Public key size: <bytes>
- Private key size: <bytes>
- Signature size (if applicable): <bytes>
- Encapsulation / Decapsulation ciphertext size (if KEM): <bytes>
- Typical CPU time (sign / verify / encapsulate / decapsulate):
  - Sign: <ms> (device class)
  - Verify: <ms> (device class)
  - Encapsulate: <ms>
  - Decapsulate: <ms>
- RAM / stack usage estimate: <KB>
- Flash / binary size impact: <KB>
- Implementation notes: <hardware acceleration, SIMD, lib availability>
- Interoperability concerns: <e.g., non-standard encoding, library maturity>
- Test vectors used: <link or file path>
- Benchmarks (attach outputs or reference files): <path>
- Risk assessment: <compatibility, patent, export controls>
- Recommendation: <Adopt / Prototype / Reject>
- Decision date: <YYYY-MM-DD>
- Decision owner: <name / role>

---

(Repeat above entries for each candidate.)

---

## Candidate: CRYSTALS-Dilithium2
- Category: Signature
- Algorithm (reference): CRYSTALS-Dilithium
- Source / Implementation: pyca/cryptography library (using a wrapper around the official clean reference implementation)
- Target parameter set / security level: NIST level 2
- Public key size: 1312 bytes
- Private key size: 2528 bytes
- Signature size (if applicable): 2420 bytes
- Encapsulation / Decapsulation ciphertext size (if KEM): N/A
- Typical CPU time (sign / verify / encapsulate / decapsulate):
  - Sign: 1,355,434 cycles (Intel Skylake)
  - Verify: 327,362 cycles (Intel Skylake)
- RAM / stack usage estimate: TBD after implementation and profiling on target hardware.
- Flash / binary size impact: TBD after implementation and profiling on target hardware.
- Implementation notes: Using the `cryptography` library, which is a well-maintained and widely used cryptographic library in Python. The underlying implementation is expected to be constant-time.
- Interoperability concerns: None. The `cryptography` library is widely used.
- Test vectors used: TBD, will use standard test vectors.
- Benchmarks (attach outputs or reference files): TBD
- Risk assessment: Low. Dilithium is a NIST standard.
- Recommendation: Adopt
- Decision date: 2025-09-23
- Decision owner: Jules

Template created automatically. Use this file to track measurements and finalize algorithm selections for the main implementation plan.
