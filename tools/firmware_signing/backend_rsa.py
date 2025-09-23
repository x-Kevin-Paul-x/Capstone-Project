"""RSA backend for firmware signing prototype.

This backend provides a simple sign/verify API that mirrors what a PQC backend would provide.
"""
from typing import Tuple
from cryptography.hazmat.primitives.asymmetric import rsa, padding
from cryptography.hazmat.primitives import hashes, serialization


def generate_keypair(key_size: int = 2048) -> Tuple[bytes, bytes]:
    private_key = rsa.generate_private_key(public_exponent=65537, key_size=key_size)
    priv_pem = private_key.private_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PrivateFormat.PKCS8,
        encryption_algorithm=serialization.NoEncryption(),
    )
    pub_pem = private_key.public_key().public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo,
    )
    return priv_pem, pub_pem


def sign(private_pem: bytes, data: bytes) -> bytes:
    private_key = serialization.load_pem_private_key(private_pem, password=None)
    signature = private_key.sign(
        data,
        padding.PSS(mgf=padding.MGF1(hashes.SHA256()), salt_length=padding.PSS.MAX_LENGTH),
        hashes.SHA256(),
    )
    return signature


def verify(public_pem: bytes, data: bytes, signature: bytes) -> bool:
    from cryptography.exceptions import InvalidSignature
    public_key = serialization.load_pem_public_key(public_pem)
    try:
        public_key.verify(
            signature,
            data,
            padding.PSS(mgf=padding.MGF1(hashes.SHA256()), salt_length=padding.PSS.MAX_LENGTH),
            hashes.SHA256(),
        )
        return True
    except InvalidSignature:
        return False
