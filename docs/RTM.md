# Requirements Traceability Matrix (RTM)

This RTM maps high-level project requirements to implementation artifacts, tests, and verification steps.

| Requirement ID | Description | Implemented in | Test / Verification | Status |
|---|---|---:|---|---|
| RQ-SB-01 | Firmware must be authenticated at boot using PQC signatures | tools/firmware_signing, firmware, bootloader | HIL boot test; CI signing job | Planned/Prototype |
| RQ-COMM-01 | Communication must support post-quantum key exchange | docs/protocol.md, middleware | Integration tests, benchmarks | Planned/Prototype |
| RQ-MW-01 | Middleware messages must be integrity protected | middleware signing plugin | Fuzz tests, verification tests | Planned |
| RQ-ANALYTICS-01 | Sensitive analytics must protect data privacy | analytics pipelines, FHE prototype | Correctness benchmarks, latency tests | Planned |

Keep RTM up-to-date as tickets are completed and test results are produced.

