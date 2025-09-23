# Threat Model — Post-Quantum Layer for Robotics

### Assets
- Firmware and bootloader
- Private signing keys (device and CI signer)
- Communication sessions and session keys
- Telemetry and analytics data

### Adversaries
- Remote network attacker: passive eavesdropper or active MITM
- Local attacker with physical access: attempts to extract keys or tamper firmware
- Supply-chain attacker: injects malicious firmware into pipeline
- Insider attacker: compromises CI or KMS credentials

### Threats & Mitigations
- Threat: Eavesdropping of communications enabling future decryption
	- Mitigation: hybrid TLS (PQC KEM + ECDH) for forward security and post-quantum resistance
- Threat: Forged firmware image
	- Mitigation: PQC-signed firmware, secure bootloader verification, recovery mode
- Threat: Key extraction from device
	- Mitigation: use secure element / TPM for private key storage; if software-only, minimize key exposure and support rapid revocation
- Threat: Compromised CI signer
	- Mitigation: hardware-backed CI signing, multi-person/air-gapped key custody, and offline recovery keys
- Threat: Replay or injection of control messages
	- Mitigation: message nonces/sequence numbers, PQC message signatures, transport-layer protection

### Residual risks
- FHE-based analytics may leak side-channel information if improperly implemented; mitigate by side-channel-aware libraries and conservative deployment.

### Assumptions
- Devices are able to perform at least basic PQC ops or can delegate to gateways.
- The organization will maintain a KMS for provisioning and rotation.

