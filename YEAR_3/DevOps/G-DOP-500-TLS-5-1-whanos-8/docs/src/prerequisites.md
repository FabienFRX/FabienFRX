# Prerequisites

Before installing Whanos, ensure your system meets the following requirements.

## Required Software

### Docker (Required)

**Minimum Version**: 20.10+
**Recommended**: Latest stable version

```bash
# Check Docker installation
docker --version

# Verify Docker is running
docker ps
```

#### Install Docker

**Ubuntu/Debian**:
```bash
curl -fsSL https://get.docker.com -o get-docker.sh
sudo sh get-docker.sh
sudo usermod -aG docker $USER
```

**macOS**:
```bash
# Install Docker Desktop
brew install --cask docker
```

**Windows**:
Download Docker Desktop from [docker.com](https://www.docker.com/products/docker-desktop)

### Git (Required)

**Minimum Version**: 2.0+

```bash
# Check Git installation
git --version
```

#### Install Git

**Ubuntu/Debian**:
```bash
sudo apt update
sudo apt install git
```

**macOS**:
```bash
brew install git
```

## Auto-Installed Components

The `deploy.sh` script automatically installs the following if not present:

### k3d

Lightweight Kubernetes distribution running in Docker.

```bash
# Manually install (optional)
curl -s https://raw.githubusercontent.com/k3d-io/k3d/main/install.sh | bash
```

### kubectl

Kubernetes command-line tool.

```bash
# Manually install (optional)
curl -LO "https://dl.k8s.io/release/$(curl -L -s https://dl.k8s.io/release/stable.txt)/bin/linux/amd64/kubectl"
chmod +x kubectl
sudo mv kubectl /usr/local/bin/
```

### Terraform (Optional)

Infrastructure as Code tool for deployment.

**Minimum Version**: 1.0+

```bash
# Check Terraform installation
terraform --version
```

If not installed, download from [terraform.io](https://www.terraform.io/downloads)

## System Requirements

### Hardware

| Component | Minimum | Recommended |
|-----------|---------|-------------|
| CPU | 2 cores | 4+ cores |
| RAM | 4 GB | 8+ GB |
| Disk Space | 20 GB | 50+ GB |

### Operating System

Whanos has been tested on:

- ✅ Ubuntu 20.04+
- ✅ Debian 11+
- ✅ macOS 11+ (Big Sur and later)
- ✅ WSL2 on Windows 10/11

### Network

- Internet connection for downloading images
- Ports available:
  - `8080` - Jenkins UI
  - `5000` - Docker registry (whanos-registry)
  - `5001` - Docker registry (k3d-whanos-registry)
  - `6443` - Kubernetes API server
  - `8081` - Kubernetes LoadBalancer
  - `50000` - Jenkins agent communication

## Optional Tools

### mdbook (for building documentation)

```bash
cargo install mdbook
```

### jq (for JSON processing)

```bash
# Ubuntu/Debian
sudo apt install jq

# macOS
brew install jq
```

### curl (for API testing)

Usually pre-installed on most systems.

## Verification

Run this script to verify all prerequisites:

```bash
#!/bin/bash

echo "Checking prerequisites..."

# Check Docker
if command -v docker &> /dev/null; then
    echo "✅ Docker: $(docker --version)"
else
    echo "❌ Docker: Not installed"
fi

# Check Git
if command -v git &> /dev/null; then
    echo "✅ Git: $(git --version)"
else
    echo "❌ Git: Not installed"
fi

# Check k3d (optional)
if command -v k3d &> /dev/null; then
    echo "✅ k3d: $(k3d version | head -1)"
else
    echo "⚠️  k3d: Not installed (will be auto-installed)"
fi

# Check kubectl (optional)
if command -v kubectl &> /dev/null; then
    echo "✅ kubectl: $(kubectl version --client --short 2>/dev/null)"
else
    echo "⚠️  kubectl: Not installed (will be auto-installed)"
fi

# Check Terraform (optional)
if command -v terraform &> /dev/null; then
    echo "✅ Terraform: $(terraform version | head -1)"
else
    echo "⚠️  Terraform: Not installed (optional)"
fi

# Check ports
echo ""
echo "Checking port availability..."
for port in 8080 5000 5001 6443 8081 50000; do
    if lsof -Pi :$port -sTCP:LISTEN -t >/dev/null 2>&1; then
        echo "❌ Port $port: Already in use"
    else
        echo "✅ Port $port: Available"
    fi
done
```

## Next Steps

Once prerequisites are met:

1. Proceed to [Installation](./installation.md)
2. Or skip directly to [Quick Start](./quick-start.md) if you want to get started immediately
