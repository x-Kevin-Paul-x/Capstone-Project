"""Sign a firmware file using pluggable backend.

Usage:
    python sign.py --key private.pem --in firmware.bin --out firmware.bin.sig

If `--genkey` is specified, a keypair will be generated and saved to `private.pem` and `public.pem`.
"""
import argparse
from pathlib import Path

# Choose backend here. PQC backend is now available.
from backend_pqc import generate_keypair, sign as sign_data


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--genkey", action="store_true", help="Generate a new keypair")
    p.add_argument("--key", type=Path, default=Path("private.pem"), help="Private key path")
    p.add_argument("--pub", type=Path, default=Path("public.pem"), help="Public key path")
    p.add_argument("--in", dest="infile", type=Path, required=False, help="Firmware file to sign")
    p.add_argument("--out", dest="outfile", type=Path, default=Path("firmware.sig"), help="Signature output file")
    args = p.parse_args()

    if args.genkey:
        priv, pub = generate_keypair()
        args.key.write_bytes(priv)
        args.pub.write_bytes(pub)
        print(f"Generated keys: {args.key} and {args.pub}")
        return

    if not args.infile:
        p.error("--in is required unless --genkey is used")

    data = args.infile.read_bytes()
    priv = args.key.read_bytes()
    sig = sign_data(priv, data)
    args.outfile.write_bytes(sig)
    print(f"Wrote signature to {args.outfile}")


if __name__ == "__main__":
    main()
