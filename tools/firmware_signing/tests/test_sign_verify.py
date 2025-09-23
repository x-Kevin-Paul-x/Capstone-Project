import subprocess
from pathlib import Path

TOOLS_DIR = Path(__file__).resolve().parents[1]
# Project root is parents[3] from this test file: tools/firmware_signing/tests/
FIRMWARE = Path(__file__).resolve().parents[3] / "firmware" / "minimal_firmware.bin"
PRIV = TOOLS_DIR / "private.pem"
PUB = TOOLS_DIR / "public.pem"
SIG = TOOLS_DIR / "firmware.sig"


def test_generate_and_sign_verify(tmp_path):
    # Generate keys
    subprocess.check_call(["python", str(TOOLS_DIR / "sign.py"), "--genkey", "--key", str(PRIV), "--pub", str(PUB)])
    assert PRIV.exists() and PUB.exists()

    # Sign
    subprocess.check_call(["python", str(TOOLS_DIR / "sign.py"), "--in", str(FIRMWARE), "--key", str(PRIV), "--out", str(SIG)])
    assert SIG.exists()

    # Verify
    ret = subprocess.call(["python", str(TOOLS_DIR / "verify.py"), "--pub", str(PUB), "--in", str(FIRMWARE), "--sig", str(SIG)])
    assert ret == 0

    # Tamper firmware and ensure verification fails
    tampered = tmp_path / "tampered.bin"
    data = FIRMWARE.read_bytes()
    tampered.write_bytes(data + b"MOD")
    ret = subprocess.call(["python", str(TOOLS_DIR / "verify.py"), "--pub", str(PUB), "--in", str(tampered), "--sig", str(SIG)])
    assert ret != 0
