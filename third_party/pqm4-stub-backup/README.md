pqm4 placeholder

This directory is a placeholder stub for the pqm4 post-quantum cryptography library.

How to add the real pqm4 sources:

Option A: Add as a git submodule
  git submodule add https://github.com/microsoft/pqm4 third_party/pqm4
  git submodule update --init --recursive

Option B: Vendor minimal sources
  - Copy required KEM and Signature implementations (Kyber, Dilithium) into this directory.

Building with CMake
- The top-level CMakeLists.txt in the project will look for pqm4 under `third_party/pqm4`.
- This stub contains a minimal `CMakeLists.txt` that provides guidance and an interface target to avoid build failures when pqm4 is not present.

Notes
- We prefer to vendor only the required algorithms to keep binary size small.
- See docs/ADRs/0002-pqc-algorithm-selection-adr.md for algorithm choices and rationale.