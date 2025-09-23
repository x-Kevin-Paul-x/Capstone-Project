# ADR 0003 — Application-Layer Wrapper for PQC
+
+Status: Proposed
+
+Context
+- Direct modifications to middleware transport layers (DDS/TLS) can be invasive. A thin application-layer wrapper for signing and verification reduces risk and eases adoption.
+
+Decision
+- Implement an application-layer wrapper that provides transparent sign/verify primitives and a lightweight SDK. Maintain transport-layer integration as an advanced option for high-assurance deployments.
+
+Consequences
+- Pros: Faster adoption, easier rollback, and clearer migration path. Non-invasive to middleware stack.
+- Cons: Slight duplication of functionality and potential overhead for message processing.
+
+-- End ADR 0003
