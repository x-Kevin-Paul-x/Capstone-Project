"""PQC backend using CRYSTALS-Dilithium from dilithium-py."""
from typing import Tuple
from dilithium_py.dilithium import Dilithium2

def generate_keypair() -> Tuple[bytes, bytes]:
    """Generates a new Dilithium keypair."""
    pk, sk = Dilithium2.keygen()
    return sk, pk


def sign(private_key: bytes, data: bytes) -> bytes:
    """Signs data with a Dilithium private key."""
    return Dilithium2.sign(sk_bytes=private_key, m=data)


def verify(public_key: bytes, data: bytes, signature: bytes) -> bool:
    """Verifies a signature with a Dilithium public key."""
    return Dilithium2.verify(pk_bytes=public_key, m=data, sig_bytes=signature)
