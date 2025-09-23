# Firmware Signing Prototype

This prototype demonstrates signing a firmware binary and verifying it with a pluggable backend. It now uses CRYSTALS-Dilithium as the backend.

Requirements
- Python 3.9+
- Install dependencies: `pip install -r requirements.txt`

Usage
- Generate a keypair:

```powershell
python sign.py --genkey --key private.pem --pub public.pem
```

- Sign firmware:

```powershell
python sign.py --in ..\..\firmware\minimal_firmware.bin --key private.pem --out firmware.sig
```

- Verify signature:

```powershell
python verify.py --pub public.pem --in ..\..\firmware\minimal_firmware.bin --sig firmware.sig
```

Run tests (pytest):

```powershell
pip install -r requirements.txt
pytest -q
```
