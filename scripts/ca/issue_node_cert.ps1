<#
issue_node_cert.ps1 - Issue a node certificate signed by the test CA (template)
Usage: .\issue_node_cert.ps1 -CaDir ./test-ca -NodeName node1
#>
param(
    [string]$CaDir = "./test-ca",
    [string]$NodeName = "node1"
)

$OutDir = Join-Path $CaDir "issued"
New-Item -ItemType Directory -Path $OutDir -Force | Out-Null

$caKey = Join-Path $CaDir "ca_key.dil"
$caCert = Join-Path $CaDir "ca_cert.pem"

if (-not (Test-Path $caKey)) {
    Write-Error "CA key not found: $caKey"
    exit 2
}

$nodeKey = Join-Path $OutDir ("{0}_key.dil" -f $NodeName)
$nodePub = Join-Path $OutDir ("{0}_pub.dil" -f $NodeName)
$nodeCert = Join-Path $OutDir ("{0}_cert.pem" -f $NodeName)

"NODE_PRIVATE_KEY_PLACEHOLDER for $NodeName" | Out-File -FilePath $nodeKey -Encoding ascii
"NODE_PUBLIC_KEY_PLACEHOLDER for $NodeName" | Out-File -FilePath $nodePub -Encoding ascii
@"
-----BEGIN CERTIFICATE-----
MIID...NODE:{0}:PLACEHOLDER...IDAQAB
-----END CERTIFICATE-----
"@ -f $NodeName | Out-File -FilePath $nodeCert -Encoding ascii

Write-Host "Issued template node cert: $nodeCert"