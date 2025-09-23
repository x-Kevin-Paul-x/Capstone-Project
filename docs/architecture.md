# Multilayered Architecture — Capstone PQC Layer

This diagram shows the end-to-end layers for secure boot, secure communications, middleware integrity, and supporting services (CI/KMS/observability) used by the project.

```mermaid
flowchart TB
  %% Top-level stakeholders
  subgraph Stakeholders[Stakeholders]
    U[Human Operator / DevOps]
  end

  %% Cloud / Backend services
  subgraph Cloud[Cloud / Backend]
    SRV[Control / Cloud App]
    CI[CI/CD Pipeline\nBuild + PQC Signing]
    AR[(Signed Artifact Repository)]
    KMS[Key Management Service\n(Provisioning, Rotation, Revocation)]
    OBS[(Observability: Logs, Metrics, Traces)]
  end

  %% Network security layer (logical)
  subgraph Network[Secure Network Channel]
    HYB[Hybrid TLS Session\nECDH + Kyber KEM]
  end

  %% Robot/Device side
  subgraph Device[Robot Device]
    direction TB
    subgraph HW[Hardware Layer]
      TPM[TPM / Secure Element]
      SOC[MCU/SoC + Peripherals]
      IO[Actuators & Sensors]
    end

    subgraph Boot[Boot & Firmware Layer]
      BR[Boot ROM]
      BL[Bootloader\nPQC Signature Verify + Recovery]
      FW[Firmware Application]
    end

    subgraph Runtime[OS / Middleware / Networking]
      OS[RTOS/OS + Drivers]
      MW[Middleware SDK / Wrapper\nMessage Signing (Dilithium)]
      NET[Networking Stack\n(TLS/DTLS + OQS when available)]
    end
  end

  %% Test infrastructure
  HIL[HIL Test Bench / Flasher / Serial Monitor]

  %% Stakeholder interactions
  U -->|Operate / Observe| SRV
  U -->|Trigger CI / Releases| CI
  U -->|Dashboards| OBS

  %% CI to artifacts and keys
  CI -->|Publish signed firmware\nfirmware/<name>-signed.bin + metadata| AR
  KMS -->|Provision device public keys\nRecovery keys / revocation| TPM

  %% Delivery path to device
  AR -->|OTA/USB Delivery| BL
  TPM -.->|Public key bundle| BL
  BR --> BL
  BL -->|Verify Dilithium signature| BL
  BL -->|Boot on success| FW
  BL -->|Rollback / Recovery on fail| FW

  %% Runtime bindings
  FW --> OS
  OS --> IO
  FW --> MW
  MW --> NET
  NET <--> HYB
  HYB <--> SRV

  %% Observability
  FW -- Telemetry/Logs --> OBS
  CI -- Build/Test/Sign logs --> OBS

  %% Test bench
  HIL -.->|Flash, reboot, validate boot state| BL
  HIL -.->|Sanity checks, motion cmd| FW
```

Key responsibilities by layer
- Hardware Layer: Secure storage (TPM/SE), root-of-trust, device I/O.
- Boot & Firmware: Verify PQC signature (Dilithium) over firmware at boot; rollback to recovery image on failure; application firmware runtime.
- OS / Middleware / Networking: Drivers and RTOS; middleware SDK for message signing; secure channels using Hybrid TLS (ECDH + Kyber KEM) for key establishment.
- Cloud / Backend: CI builds and signs firmware; artifact repository distributes signed images; KMS provisions public keys to devices and manages rotation/revocation; observability collects metrics and logs.
- HIL: Automates flashing, reboot, and pass/fail validation (boot success, reject tampered images).

Optional supporting views
- Secure boot flow: Dev/CI builds → signs → publishes → device bootloader verifies → boots or recovers.
- Secure comms flow: Device establishes hybrid TLS (ECDH + Kyber) with server; middleware signs critical messages (Dilithium) for integrity/non-repudiation.

How to view
- GitHub renders Mermaid diagrams in Markdown by default.
- In VS Code, install a Mermaid preview extension or use the built-in Markdown preview to view the diagram.
