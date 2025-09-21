# 0002: PQC Algorithm Selection for Secure Transport Layer

**Status**: Accepted

**Context**:
The project requires a set of post-quantum cryptographic primitives to secure communications in human-robot collaborative workspaces. The chosen algorithms must provide confidentiality, integrity, and authenticity, while meeting the performance constraints of embedded and real-time systems. The selection process is guided by the NIST PQC Standardization process and the specific requirements of the project, such as low latency and small memory footprint.

**Decision**:
We have selected the following PQC algorithms for the core cryptographic operations:
- **Key Encapsulation Mechanism (KEM)**: CRYSTALS-Kyber-768 for session key establishment.
- **Digital Signature Algorithm**: CRYSTALS-Dilithium-III for node identity and authentication.
- **Authenticated Encryption with Associated Data (AEAD)**: Ascon-128a for per-message confidentiality and integrity.

**Consequences**:
- **Positive**:
  - Aligns with the NIST PQC standardization, leveraging extensively vetted algorithms.
  - Kyber and Dilithium provide NIST Category 3 security, which is robust for the defined threat model.
  - Ascon-128a is a lightweight AEAD optimized for constrained environments, fitting the real-time and embedded needs of the project.
  - Well-supported in libraries like liboqs and pqm4, which will speed up development and testing.
- **Negative**:
  - As with any new cryptographic standard, long-term implementation vulnerabilities are less understood than for older algorithms.
  - Performance on specific embedded targets must be carefully benchmarked to ensure it meets non-functional requirements.
- **Neutral**:
  - The project will depend on third-party libraries for these cryptographic primitives. This adds a dependency but reduces implementation complexity and risk.
  - The plan includes options for hybrid modes (e.g., ECDH + Kyber) to support transitional interoperability, which will be documented as an optional feature.
