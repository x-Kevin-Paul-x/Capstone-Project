"""Verify a firmware signature using pluggable backend.

Usage:
    python verify.py --pub public.pem --in firmware.bin --sig firmware.bin.sig
"""
import argparse
from pathlib import Path

# Choose backend here. Swap to a PQC backend implementation when available.
from backend_rsa import verify as verify_data


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--pub", type=Path, default=Path("public.pem"), help="Public key path")
    p.add_argument("--in", dest="infile", type=Path, required=True, help="Firmware file to verify")
    p.add_argument("--sig", dest="sigfile", type=Path, required=True, help="Signature file")
    args = p.parse_args()

    data = args.infile.read_bytes()
    pub = args.pub.read_bytes()
    sig = args.sigfile.read_bytes()

    ok = verify_data(pub, data, sig)
    print("VALID" if ok else "INVALID")
    return 0 if ok else 2


if __name__ == "__main__":
    exit(main())
