# Quick Start

Get Whanos up and running in less than 5 minutes!

## TL;DR

```bash
# Clone the repository
git clone https://github.com/your-org/whanos.git
cd whanos

# Deploy the infrastructure
./scripts/deploy.sh

# Access Jenkins
open http://localhost:8080

# Get admin password
docker exec whanos-jenkins cat /var/jenkins_home/secrets/initialAdminPassword
```

## Step-by-Step Guide

### 1. Prerequisites Check

Ensure you have the following installed:

```bash
# Check Docker
docker --version
# Docker version 20.10+ required

# Check Git
git --version
```

The deploy script will automatically install:
- k3d (for Kubernetes cluster)
- kubectl (for Kubernetes CLI)
- Terraform (if not present)

### 2. Deploy Whanos Infrastructure

From the repository root:

```bash
./scripts/deploy.sh
```

This script will:
- ✅ Check prerequisites
- ✅ Install k3d and kubectl if needed
- ✅ Create a 3-node Kubernetes cluster
- ✅ Deploy Jenkins using Terraform
- ✅ Deploy Docker registries
- ✅ Configure networking

Expected output:
```
╔════════════════════════════════════════════════════════════════╗
║                                                                ║
║     ✓ WHANOS DEPLOYED WITH TERRAFORM (Docker Provider)!        ║
║                                                                ║
╚════════════════════════════════════════════════════════════════╝

🌐 Access Points:
   Jenkins:  http://localhost:8080
   Registry: http://localhost:5000
   K8s Apps: http://localhost:8081
```

### 3. Access Jenkins

1. **Open Jenkins** in your browser:
   ```
   http://localhost:8080
   ```

2. **Get the admin password**:
   ```bash
   docker exec whanos-jenkins cat /var/jenkins_home/secrets/initialAdminPassword
   ```

3. **Login** with:
   - Username: `admin`
   - Password: `admin` (default) or the password from step 2

### 4. Build Base Images

Before linking projects, you need to build the Whanos base images:

1. Navigate to **Whanos base images** folder
2. Click on **Build all base images**
3. Click **Build Now**

Wait for all 5 base images to build:
- whanos-c
- whanos-java
- whanos-javascript
- whanos-python
- whanos-befunge

### 5. Link Your First Project

#### Option A: Use Test Applications

The repository includes test applications. Let's deploy the Python app:

1. In Jenkins, click on **link-project**
2. Click **Build with Parameters**
3. Fill in:
   - **REPOSITORY_URL**: `file:///path/to/whanos/test-apps/python-app` (or your Git URL)
   - **PROJECT_NAME**: `python-test-app`
   - **GIT_CREDENTIALS_ID**: (leave empty for public repos)
   - **DEFAULT_BRANCH**: `main`
4. Click **Build**

#### Option B: Link Your Own Repository

1. Create a Whanos-compatible repository (see [Creating a Compatible Repository](./user-guide/compatible-repo.md))
2. Use the **link-project** job with your repository URL

### 6. Monitor the Build

1. Go to **Projects** folder
2. Click on your project name
3. Watch the pipeline execute:
   - 🔍 Detect Language
   - 🔨 Build Application Image
   - 📤 Push to Registry
   - 🚀 Deploy to Kubernetes (if `whanos.yml` exists)

### 7. Verify Deployment

Check if your application is running in Kubernetes:

```bash
# Setup kubeconfig
export KUBECONFIG=$(k3d kubeconfig write whanos-cluster)

# Check deployments
kubectl get deployments -n whanos

# Check pods
kubectl get pods -n whanos

# Check services
kubectl get services -n whanos
```

## What's Next?

Now that Whanos is running, you can:

- 📚 Learn about [Whanos-Compatible Repositories](./user-guide/compatible-repo.md)
- ⚙️ Configure [Kubernetes Deployment](./kubernetes/whanos-yml.md) with `whanos.yml`
- 🔧 Understand the [Architecture](./architecture/overview.md)
- 🐛 Check [Troubleshooting](./troubleshooting/common-issues.md) if you encounter issues

## Cleanup

To remove the Whanos infrastructure:

```bash
./scripts/destroy.sh
```

This will:
- Stop and remove all containers
- Delete the k3d cluster
- Remove Terraform state

---

## Common Quick Start Issues

### Jenkins not starting

```bash
# Check container logs
docker logs whanos-jenkins

# Restart Jenkins
docker restart whanos-jenkins
```

### kubectl connection refused

```bash
# Reconnect to k3d cluster
export KUBECONFIG=$(k3d kubeconfig write whanos-cluster)
```

### Port already in use

```bash
# Check what's using port 8080
lsof -i :8080

# Stop conflicting service or change port in terraform/environments/dev/main.tf
```

For more issues, see the [Troubleshooting](./troubleshooting/common-issues.md) section.
