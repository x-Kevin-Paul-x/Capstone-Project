# ADR 0002 — PQC Algorithm Selection Policy
+
+Status: Accepted (Proposed default)
+
+Context
+- Multiple PQC candidates exist with trade-offs in size, speed, and implementation maturity.
+
+Decision
+- Follow a prototype-then-measure approach: implement candidate algorithms in `PQC_decision_log.md`, benchmark them on representative hardware, and finalize selection based on empirical data.
+
+Consequences
+- Pros: Empirical selection reduces surprises when deploying to constrained devices.
+- Cons: Requires investment in benchmarking infrastructure and representative hardware.
+
+-- End ADR 0002
