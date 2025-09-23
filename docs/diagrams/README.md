# Architecture Diagrams

This folder contains the Mermaid diagram source files (`.mmd`) and their corresponding PNG exports for the Capstone PQC Layer project.

## Diagram Files

### 1. System Architecture Overview
- **Source**: `system-architecture.mmd`
- **PNG**: `system-architecture.png` (1920x1080, dark theme)
- **Description**: Complete system architecture showing all layers from hardware to cloud services, including PQC components, stakeholders, data flows, and security boundaries.

### 2. Secure Boot Process
- **Source**: `secure-boot-sequence.mmd`
- **PNG**: `secure-boot-sequence.png` (1600x1200, dark theme)
- **Description**: Detailed sequence diagram showing the firmware build, signing, delivery, and verification process during device boot.

### 3. Secure Communication Flow
- **Source**: `secure-communication-sequence.mmd`
- **PNG**: `secure-communication-sequence.png` (1600x1200, dark theme)
- **Description**: Step-by-step process of establishing hybrid TLS sessions and message-level authentication between robot and cloud.

### 4. Key Rotation Process
- **Source**: `key-rotation-sequence.mmd`
- **PNG**: `key-rotation-sequence.png` (1600x1200, dark theme)
- **Description**: Automated key lifecycle management showing provisioning, rotation, and revocation processes.

## Usage

### Viewing PNG Files
- Open the `.png` files directly in any image viewer
- Use in presentations, documentation, or reports
- High resolution suitable for printing or projection

### Editing Diagrams
1. Modify the `.mmd` source files
2. Regenerate PNG using Mermaid CLI:
   ```powershell
   mmdc -i [diagram-name].mmd -o [diagram-name].png -w 1920 -H 1080 --theme dark
   ```

### Mermaid CLI Commands Used
```powershell
# Install Mermaid CLI
npm install -g @mermaid-js/mermaid-cli

# Convert to PNG (example)
mmdc -i system-architecture.mmd -o system-architecture.png -w 1920 -H 1080 --theme dark
```

## Alternative Export Methods

### Method 1: Online Mermaid Editor
1. Go to https://mermaid.live/
2. Paste the `.mmd` content
3. Click "Actions" → "Download PNG"

### Method 2: VS Code Extension
1. Install "Mermaid Preview" extension
2. Open `.mmd` file
3. Use preview pane to export

### Method 3: GitHub Rendering
- GitHub automatically renders Mermaid diagrams in Markdown files
- Use browser screenshot tools for quick exports

## File Specifications
- **Architecture Diagram**: 1920x1080px (presentation format)
- **Sequence Diagrams**: 1600x1200px (document format)
- **Theme**: Dark theme for better visibility
- **Format**: PNG with transparent background where applicable