# Threat Model for PQC Secure Transport Layer

This document outlines the threat model for the secure communication layer in a human-robot collaborative workspace. It follows the structured approach defined in the project's implementation plan.

## 1. Assets
The core assets that require protection are:

- **Command & Control Data**: Real-time messages controlling robot actions (e.g., joint positions, velocity commands, setpoints). Unauthorized modification could lead to unsafe behavior or operational failure.
- **Safety & E-Stop Messages**: Critical signals that ensure human safety (e.g., emergency stops, speed limitations). Tampering with these could have catastrophic consequences.
- **Telemetry & Sensor Data**: Information transmitted from the robot about its status and environment. Eavesdropping could reveal sensitive operational data.
- **Firmware & Configuration Images**: Software running on robot controllers and edge nodes. Unauthorized modification could permanently compromise the system.
- **Cryptographic Identity Keys**: The private keys (Dilithium) used by nodes to authenticate themselves. Compromise of these keys allows an adversary to impersonate a legitimate node.
- **Session Keys**: The ephemeral keys (Kyber-derived) used to encrypt data-plane messages. Compromise of these keys would break confidentiality and integrity for a session.

## 2. Adversaries
We consider the following adversaries:

- **On-Path Attacker (Man-in-the-Middle, MITM)**: An adversary positioned on the local network (wired or wireless) who can intercept, read, modify, and inject network packets between nodes. This is the primary adversary for network-level attacks.
- **Compromised Node**: A legitimate node within the ROS2 graph that has been compromised by an attacker, giving them access to its resources, including its identity keys.
- **Malicious Gateway**: A compromised network device (e.g., router, switch) that can manipulate traffic flowing through it.
- **Replay Attacker**: A less sophisticated adversary who captures legitimate network traffic and replays it at a later time to disrupt operations (e.g., replaying a "move" command).
- **Side-Channel Observer (Advanced)**: An adversary with physical proximity who attempts to extract secret keys by observing non-functional characteristics like power consumption, timing, or electromagnetic emissions. This is a concern for embedded controllers.

## 3. Trust Boundaries
The system has several trust boundaries where security controls are critical:

- **Robot Controller ↔ Edge Node**: The link between the robot's hardware controller and the higher-level compute node running ROS2.
- **ROS2 Graph**: The logical boundary of the ROS2 system. Nodes within this boundary are expected to be authenticated.
- **Network Edges**: Points where the local HRC network connects to wider enterprise networks or the internet (e.g., via WiFi, 5G, or wired gateways).

## 4. Misuse Cases & Attack Scenarios
Here are plausible attack scenarios derived from the assets and adversaries:

- **Unsafe Command Injection**: A MITM attacker intercepts a legitimate "move" command and replaces it with one that directs the robot to an unsafe position, potentially causing a collision with a human.
- **E-Stop Replay/Suppression**: An attacker records a message clearing an E-stop condition. When a human legitimately triggers an E-stop, the attacker replays the "clear" message to override the safety command.
- **Node Impersonation**: An attacker who has stolen a node's identity key can use it to impersonate that node, publishing hazardous commands or false telemetry data.
- **Data Exfiltration**: An on-path attacker eavesdrops on unencrypted telemetry data to steal proprietary information about the manufacturing process.
- **Firmware Hijacking**: An attacker without valid credentials attempts to flash a malicious firmware image to a robot controller.

## 5. Security Goals
To counter these threats, the system must achieve the following security goals:

- **Mutual Authentication**: All communicating nodes must verify each other's identities before establishing a session. (Mapped to: **CRYSTALS-Dilithium**)
- **Confidentiality**: The content of all sensitive messages must be unreadable to unauthorized parties. (Mapped to: **Ascon-128a AEAD**)
- **Integrity**: Any tampering or modification of messages must be detectable by the receiver. (Mapped to: **Ascon-128a AEAD**)
- **Replay Resistance**: The system must detect and reject old messages that are resent by an attacker. (Mapped to: **Monotonic sequence numbers within the AEAD**)
- **Forward Secrecy**: The compromise of a node's long-term identity key must not compromise the confidentiality of past communication sessions. (Mapped to: **Periodic rekeying with CRYSTALS-Kyber**)
- **Downgrade Attack Protection**: The protocol must prevent an attacker from forcing nodes to use weaker cryptographic algorithms or no security at all. (Mapped to: **Authenticated handshake and pinned ciphersuites**)
- **Side-Channel Resistance (Best-Effort)**: The cryptographic implementations should, where feasible, use constant-time operations to mitigate side-channel attacks. (Mapped to: **Selection of vetted libraries like pqm4/liboqs**)
