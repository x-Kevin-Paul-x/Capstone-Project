# Deployment and Operator Guide

This document provides instructions for deploying, managing, and operating the `pqc-secure-transport` layer within a ROS 2 environment. It is intended for system administrators and operators responsible for the security of the human-robot collaboration workspace.

## 1. System Components

The deployment consists of the following components:

- **`pqc-secure-transport` library**: The core C library providing the PQC security functions.
- **`secure_talker` / `secure_listener`**: Example ROS 2 nodes demonstrating how to use the library.
- **Certificate Authority (CA) Tooling**: A set of scripts for creating a test CA and issuing node certificates.
- **Node Keystores**: Directories containing the cryptographic keys and certificates for each authenticated node.

## 2. Prerequisites

- A working ROS 2 installation.
- The `pqc-secure-transport` project built from source.
- OpenSSL command-line tools installed (for the CA scripts).

## 3. Phase 1: Certificate Authority (CA) Setup

Authentication in this system is based on digital certificates signed by a trusted Certificate Authority. For the MVP, a simple, file-based test CA is used.

**For a production environment, a hardware-backed, offline root CA is strongly recommended.**

### 3.1. Creating the Test CA

A script will be provided to generate the root CA certificate and private key.

```bash
# (This script will be created in a later phase)
# ./scripts/ca/create_ca.sh
```

This command will create a `ca/` directory in the project root, containing:
- `ca_cert.pem`: The public certificate of the CA.
- `ca_key.pem`: The private key of the CA (must be kept secret).

## 4. Phase 2: Node Provisioning

Each trusted ROS 2 node must be provisioned with its own unique identity, which consists of a private key and a certificate signed by the CA.

### 4.1. Issuing a Node Certificate

A script will be provided to handle this process.

```bash
# (This script will be created in a later phase)
# ./scripts/ca/issue_node_cert.sh <node_name>
#
# Example:
# ./scripts/ca/issue_node_cert.sh robot_arm_controller
```

This command will:
1. Generate a new private/public key pair for the node (using CRYSTALS-Dilithium).
2. Create a Certificate Signing Request (CSR).
3. Sign the CSR with the CA's private key to create the final node certificate.
4. Place the `private_key.pem` and `node_cert.pem` into a directory, e.g., `keystores/<node_name>/`.

### 4.2. Securely Deploying Keys to Nodes

The generated keystore directory (e.g., `keystores/robot_arm_controller/`) must be securely copied to the machine where the node will run. **This transfer must be done over a secure channel** (e.g., `scp`, physical transfer) to prevent key theft.

The `secure_talker` and `secure_listener` applications will expect a path to this keystore directory to be provided at runtime.

## 5. Phase 3: Running the Secure System

### 5.1. Starting the Secure Nodes

The example nodes are started from the command line. Each node needs to be pointed to its own keystore directory and the CA's public certificate.

**Terminal 1: Start the Secure Listener**
```bash
# (Example command, will be finalized during implementation)
ros2 run pqc_secure_transport secure_listener \
  --ros-args -p keystore_path:=/path/to/keystores/listener_node \
             -p ca_cert_path:=/path/to/ca/ca_cert.pem
```

**Terminal 2: Start the Secure Talker**
```bash
ros2 run pqc_secure_transport secure_talker \
  --ros-args -p keystore_path:=/path/to/keystores/talker_node \
             -p ca_cert_path:=/path/to/ca/ca_cert.pem
```

The nodes will perform an authenticated handshake (Dilithium + Kyber) before starting to exchange encrypted data (Ascon).

## 6. Phase 4: Maintenance and Key Management

### 6.1. Session Rekeying

The library will automatically handle session rekeying based on the policy defined in the implementation plan (e.g., every 10 minutes or 10,000 messages). This is transparent to the operator.

### 6.2. Certificate Revocation

If a node's private key is compromised, its certificate must be revoked.

A Certificate Revocation List (CRL) will be used for this purpose.

1.  **Revoking a Certificate**:
    ```bash
    # (This script will be created in a later phase)
    # ./scripts/ca/revoke_node_cert.sh <node_name>
    ```
    This updates the CRL file.

2.  **Distributing the CRL**:
    The updated CRL must be distributed to all nodes in the system. The library will be configured to check this CRL during the handshake, rejecting connections from any node whose certificate is on the list. The distribution mechanism will be defined during implementation (e.g., via a secure out-of-band channel or a dedicated ROS 2 topic).

### 6.3. Logging and Monitoring

The secure transport layer will log key events, such as:
- Handshake success/failure.
- Rekeying events.
- AEAD verification failures (potential tampering).
- Certificate validation failures (revoked or expired certs).

**No secret key material will ever be written to the logs.** Operators should monitor these logs for signs of anomalous or malicious activity.
