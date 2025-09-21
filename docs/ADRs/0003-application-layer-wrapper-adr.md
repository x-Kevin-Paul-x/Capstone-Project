# 0003: Architecture for ROS2 Integration - Application-Layer Wrapper

**Status**: Accepted

**Context**:
To integrate PQC into a ROS2 system, there are two primary architectural paths:

1.  **Path A: Application-Layer Wrapper**: Create a wrapper that intercepts ROS2 messages from standard topics, secures them using our PQC layer, and republishes them on new, "secure" topics.
2.  **Path B: Native DDS Integration**: Replace or deeply modify the underlying DDS/DDS-Security middleware to natively support the selected PQC ciphersuites.

The project's primary goal is to deliver a feasible Minimum Viable Product (MVP) that demonstrates the viability of PQC for HRC, with a focus on rapid prototyping and clear performance benchmarks.

**Decision**:
We will implement an **application-layer wrapper** (`pqc-secure-transport`) for the MVP.

This wrapper will consist of a C API and a pair of sample ROS2 nodes (`secure_talker` and `secure_listener`) that perform the following functions:
- A `secure_talker` node will subscribe to a standard (insecure) ROS2 topic.
- It will serialize the message, apply authenticated encryption (Ascon-128a) using a pre-established session key, and publish the resulting binary data to a new, "secure" topic.
- A `secure_listener` node will subscribe to the "secure" topic, decrypt and verify the binary data, and then deserialize and publish the original message to a local, insecure topic for consumption by the target application.

The handshake for establishing the session key (using Kyber and Dilithium) will be handled by the C API, with the sample nodes demonstrating its usage.

**Consequences**:

- **Positive**:
  - **Lower Integration Friction**: This approach requires no modifications to the core ROS2 or DDS middleware, making it significantly easier to develop, deploy, and test.
  - **Faster Prototyping**: It allows the team to focus entirely on the cryptographic implementation and its performance, accelerating the delivery of the MVP.
  - **Clear Benchmarking**: The overhead of the PQC layer can be isolated and measured precisely by comparing the performance of the wrapped topics to the insecure baseline.

- **Negative**:
  - **Performance Overhead**: This approach introduces a "double-wrapping" of messages (serialize -> encrypt -> publish -> subscribe -> decrypt -> deserialize), which will likely add more latency and CPU overhead than a native DDS implementation.
  - **Limited Scope**: This method only secures topic data. It does not secure other parts of the ROS2 graph, such as node discovery, parameter events, or service calls, which would require deeper integration.

- **Neutral**:
  - This decision explicitly prioritizes speed and feasibility for the MVP, while documenting the native DDS integration path as a clear item for future work. This aligns with the phased approach of the project.
