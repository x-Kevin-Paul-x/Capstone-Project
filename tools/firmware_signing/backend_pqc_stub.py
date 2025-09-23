"""PQC backend stub.

This file defines the same API as `backend_rsa.py` but raises NotImplementedError for generate_keypair.
Replace with a real PQC backend (e.g., liboqs bindings or PQClean) when available.
"""
from typing import Tuple


def generate_keypair() -> Tuple[bytes, bytes]:
    raise NotImplementedError("PQC backend not yet implemented. Replace with liboqs/PQClean wrapper.")


def sign(private_pem: bytes, data: bytes) -> bytes:
    raise NotImplementedError("PQC signing not implemented in stub.")


def verify(public_pem: bytes, data: bytes, signature: bytes) -> bool:
    raise NotImplementedError("PQC verification not implemented in stub.")
