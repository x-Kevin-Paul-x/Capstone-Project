#!/usr/bin/env bash
set -euo pipefail

# issue_node_cert.sh - Issue a node certificate signed by the test CA (template)
# Usage: issue_node_cert.sh <ca_dir> <node_name>

CA_DIR="${1:-./test-ca}"
NODE_NAME="${2:-node1}"
OUTDIR="${CA_DIR}/issued"
mkdir -p "$OUTDIR"

CA_KEY="$CA_DIR/ca_key.dil"
CA_CERT="$CA_DIR/ca_cert.pem"

if [ ! -f "$CA_KEY" ]; then
  echo "CA key not found: $CA_KEY" >&2
  exit 2
fi

NODE_KEY="$OUTDIR/${NODE_NAME}_key.dil"
NODE_PUB="$OUTDIR/${NODE_NAME}_pub.dil"
NODE_CERT="$OUTDIR/${NODE_NAME}_cert.pem"
CA_HELPER="$(dirname "$0")/../../tools/ca_helper/ca_helper"
if [ -x "$CA_HELPER" ]; then
  echo "Found ca_helper -> $CA_HELPER : delegating node cert issuance"
  "$CA_HELPER" "$OUTDIR" "issue" "$NODE_NAME"
  echo "Issued node cert via ca_helper: $(realpath "$NODE_CERT")"
  exit 0
fi

# Fallback placeholder node key/cert generation
printf "NODE_PRIVATE_KEY_PLACEHOLDER for $NODE_NAME\n" > "$NODE_KEY"
printf "NODE_PUBLIC_KEY_PLACEHOLDER for $NODE_NAME\n" > "$NODE_PUB"

# Create a placeholder cert signed by CA (template)
cat > "$NODE_CERT" <<EOF
-----BEGIN CERTIFICATE-----
MIID...NODE:${NODE_NAME}:PLACEHOLDER...IDAQAB
-----END CERTIFICATE-----
EOF

echo "Issued placeholder node cert: $NODE_CERT"
