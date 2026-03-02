# Installation

Complete guide to installing and deploying the Whanos infrastructure.

## Installation Methods

Whanos can be deployed using two methods:

1. **Automated Deployment** (Recommended) - Using the `deploy.sh` script
2. **Manual Deployment** - Using Terraform directly

## Method 1: Automated Deployment

### Step 1: Clone the Repository

```bash
git clone https://github.com/your-org/whanos.git
cd whanos
```

### Step 2: Run the Deployment Script

```bash
./scripts/deploy.sh
```

The script will:

1. ✅ Check for required prerequisites (Docker, Git)
2. ✅ Install k3d if not present
3. ✅ Install kubectl if not present
4. ✅ Create a k3d Kubernetes cluster named `whanos-cluster` with:
   - 1 server node (control plane)
   - 2 agent nodes (workers)
   - Integrated Docker registry
5. ✅ Deploy Jenkins using Terraform
6. ✅ Deploy Docker registries
7. ✅ Configure networking between components
8. ✅ Create Kubernetes namespace `whanos`

### Step 3: Verify Installation

```bash
# Check Docker containers
docker ps --filter "name=whanos"

# Check k3d cluster
k3d cluster list

# Check Kubernetes nodes
export KUBECONFIG=$(k3d kubeconfig write whanos-cluster)
kubectl get nodes

# Access Jenkins
open http://localhost:8080
```

Expected containers:
- `whanos-jenkins` - Jenkins CI/CD server
- `whanos-registry` - Docker registry (port 5000)
- `k3d-whanos-registry` - k3d internal registry (port 5001)
- `k3d-whanos-cluster-server-0` - Kubernetes control plane
- `k3d-whanos-cluster-agent-0` - Kubernetes worker 1
- `k3d-whanos-cluster-agent-1` - Kubernetes worker 2
- `k3d-whanos-cluster-serverlb` - Load balancer

## Method 2: Manual Deployment with Terraform

### Step 1: Create k3d Cluster

```bash
# Create k3d registry
k3d registry create whanos-registry --port 5001

# Create k3d cluster
k3d cluster create whanos-cluster \
  --agents 2 \
  --registry-use k3d-whanos-registry:5001 \
  --port "8081:80@loadbalancer" \
  --api-port 6443 \
  --wait
```

### Step 2: Deploy with Terraform

```bash
cd terraform/environments/dev

# Initialize Terraform
terraform init

# Review the plan
terraform plan

# Apply the configuration
terraform apply
```

### Step 3: Create Kubernetes Namespace

```bash
export KUBECONFIG=$(k3d kubeconfig write whanos-cluster)
kubectl create namespace whanos
```

### Step 4: Verify Deployment

```bash
# Check Terraform state
terraform show

# Verify containers
docker ps

# Check Kubernetes
kubectl get all -n whanos
```

## Post-Installation Steps

### 1. Configure Jenkins

Get the initial admin password:

```bash
docker exec whanos-jenkins cat /var/jenkins_home/secrets/initialAdminPassword
```

Or use the default credentials:
- Username: `admin`
- Password: `admin`

### 2. Build Base Images

Before using Whanos, you must build the base Docker images:

1. Access Jenkins at `http://localhost:8080`
2. Navigate to **Whanos base images**
3. Run **Build all base images**

This will build:
- `whanos-c`
- `whanos-java`
- `whanos-javascript`
- `whanos-python`
- `whanos-befunge`

### 3. Verify Network Connectivity

```bash
# Test Jenkins to k3d cluster
docker exec whanos-jenkins kubectl get nodes

# Test Docker registry
curl http://localhost:5000/v2/_catalog
curl http://localhost:5001/v2/_catalog
```

## Configuration Files

### Terraform Configuration

Located in `terraform/environments/dev/`:

```
terraform/environments/dev/
├── main.tf          # Main configuration
├── variables.tf     # Input variables
└── outputs.tf       # Output values
```

### Jenkins Configuration

Located in `jenkins/config/`:

```
jenkins/config/
├── jenkins.yaml     # Jenkins Configuration as Code (JCasC)
└── job-dsl/         # Job DSL scripts
```

### Dockerfile Directory

Located in `images/`:

```
images/
├── c/
│   ├── Dockerfile.base
│   └── Dockerfile.standalone
├── java/
│   ├── Dockerfile.base
│   └── Dockerfile.standalone
├── javascript/
│   ├── Dockerfile.base
│   └── Dockerfile.standalone
├── python/
│   ├── Dockerfile.base
│   └── Dockerfile.standalone
└── befunge/
    ├── Dockerfile.base
    └── Dockerfile.standalone
```

## Environment Variables

The deployment uses the following environment variables (optional):

| Variable | Default | Description |
|----------|---------|-------------|
| `CLUSTER_NAME` | `whanos-cluster` | Name of the k3d cluster |
| `REGISTRY_PORT` | `5000` | External registry port |
| `JENKINS_PORT` | `8080` | Jenkins UI port |

Example:

```bash
export JENKINS_PORT=9090
./scripts/deploy.sh
```

## Troubleshooting Installation

### Port Already in Use

```bash
# Find what's using the port
lsof -i :8080

# Kill the process or change the port
export JENKINS_PORT=9090
```

### k3d Cluster Creation Failed

```bash
# Remove existing cluster
k3d cluster delete whanos-cluster

# Try again
k3d cluster create whanos-cluster --agents 2
```

### Terraform Apply Failed

```bash
# Check Terraform logs
terraform apply

# If state is corrupted
terraform destroy
terraform apply
```

### Docker Permission Denied

```bash
# Add user to docker group
sudo usermod -aG docker $USER

# Log out and back in, or:
newgrp docker
```

## Uninstallation

To completely remove the Whanos infrastructure:

```bash
./scripts/destroy.sh
```

This will:
1. Stop all containers
2. Remove Docker containers and networks
3. Delete the k3d cluster
4. Clean up Terraform state

Manual cleanup:

```bash
# Stop containers
docker stop $(docker ps -a --filter "name=whanos" -q)
docker rm $(docker ps -a --filter "name=whanos" -q)

# Delete k3d cluster
k3d cluster delete whanos-cluster
k3d registry delete whanos-registry

# Clean Terraform
cd terraform/environments/dev
terraform destroy
```

## Next Steps

After successful installation:

1. 📚 Read the [Architecture Overview](./architecture/overview.md)
2. 🚀 Try the [Quick Start](./quick-start.md) guide
3. 👤 Learn how to [Link a Project](./user-guide/linking-project.md)
4. ⚙️ Configure [Kubernetes Deployment](./kubernetes/whanos-yml.md)
