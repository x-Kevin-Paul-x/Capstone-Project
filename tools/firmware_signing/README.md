# Firmware Signing Prototype

This prototype demonstrates signing a firmware binary and verifying it with a pluggable backend. It uses RSA as a placeholder backend; replace `backend_rsa.py` with PQC bindings when available (see `backend_pqc_stub.py`).

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

Extending to PQC
- Implement a real PQC backend with the same API as `backend_rsa.py`. See `backend_pqc_stub.py` for the API surface.
- Consider `liboqs` (Open Quantum Safe) or `PQClean` wrappers. When available, update `sign.py`/`verify.py` imports to point to the PQC backend.
