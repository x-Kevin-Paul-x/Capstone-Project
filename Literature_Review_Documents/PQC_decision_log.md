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

Template created automatically. Use this file to track measurements and finalize algorithm selections for the main implementation plan.
