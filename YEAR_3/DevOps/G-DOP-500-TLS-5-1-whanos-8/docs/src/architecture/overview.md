# Architecture Overview

Whanos is built on a modular architecture combining four key DevOps technologies.

## System Architecture

```
┌────────────────────────────────────────────────────────────────┐
│                         HOST MACHINE                            │
│                                                                 │
│  ┌──────────────┐  ┌───────────────┐  ┌────────────────────┐   │
│  │   Jenkins    │  │    Docker     │  │    Kubernetes      │   │
│  │              │  │   Registries  │  │      (k3d)         │   │
│  │  Port: 8080  │  │               │  │                    │   │
│  │              │  │  - whanos     │  │  ┌──────────────┐  │   │
│  │ ┌──────────┐ │  │    :5000      │  │  │ Server Node  │  │   │
│  │ │ Pipeline │ │  │  - k3d        │  │  │              │  │   │
│  │ │          │ │  │    :5001      │  │  └──────────────┘  │   │
│  │ │ 1. Detect│ │  │               │  │  ┌──────────────┐  │   │
│  │ │ 2. Build │ │  │               │  │  │ Agent Node 0 │  │   │
│  │ │ 3. Push  │◄─┼─►│               │◄─┼─►│              │  │   │
│  │ │ 4. Deploy│ │  │               │  │  └──────────────┘  │   │
│  │ └──────────┘ │  │               │  │  ┌──────────────┐  │   │
│  │              │  │               │  │  │ Agent Node 1 │  │   │
│  └──────────────┘  └───────────────┘  │  │              │  │   │
│                                        │  └──────────────┘  │   │
│                                        └────────────────────┘   │
│                                                                 │
│  Docker Network: k3d-whanos-cluster                            │
└────────────────────────────────────────────────────────────────┘
```

## Core Components

### 1. Jenkins CI/CD Server

**Purpose**: Orchestrates the entire build and deployment pipeline

**Technology**: Jenkins with Configuration as Code (JCasC)

**Key Features**:
- Automatic language detection
- Docker-in-Docker (DinD) capability
- Kubernetes integration
- Job DSL for dynamic job creation

**Port**: 8080

### 2. Docker Registries

**Purpose**: Store Docker images for applications

**Registries**:
- **whanos-registry** (port 5000): Public-facing registry
- **k3d-whanos-registry** (port 5001): Internal k3d registry

**Key Features**:
- HTTP API for image management
- Integrated with k3d cluster
- Accessible from Jenkins and Kubernetes

### 3. Kubernetes Cluster (k3d)

**Purpose**: Orchestrate containerized applications

**Technology**: k3d (k3s in Docker) - Lightweight Kubernetes

**Cluster Configuration**:
- **1 Server Node**: Control plane + etcd
- **2 Agent Nodes**: Worker nodes for running pods
- **LoadBalancer**: Exposes services on port 8081

**Key Features**:
- Multi-node cluster for high availability
- Integrated Docker registry
- Namespace isolation (`whanos` namespace)

### 4. Infrastructure as Code (Terraform)

**Purpose**: Deploy and manage infrastructure

**Technology**: Terraform with Docker provider

**Modules**:
- `jenkins`: Deploys Jenkins container
- `registry`: Deploys Docker registries
- `aks` (optional): For cloud deployments

## Data Flow

### Build Pipeline Flow

```
1. Developer pushes to Git repository
         ↓
2. Jenkins webhook triggered (every minute check)
         ↓
3. Language Detection
   - Check for Makefile (C)
   - Check for pom.xml (Java)
   - Check for package.json (JavaScript)
   - Check for requirements.txt (Python)
   - Check for main.bf (Befunge)
         ↓
4. Docker Build
   - If no Dockerfile: Use whanos standalone image
   - If Dockerfile exists: Use custom Dockerfile with whanos base
         ↓
5. Tag and Push to Registry
   - Tag with build number
   - Tag as latest
   - Push to k3d-whanos-registry:5000
         ↓
6. Kubernetes Deployment (if whanos.yml exists)
   - Parse whanos.yml
   - Create/Update Deployment
   - Create Service (if ports defined)
   - Wait for rollout
         ↓
7. Success! Application is running
```

### Image Hierarchy

```
Official Base Images
  ├── gcc:13.2           → whanos-c (base)
  ├── maven:3.9-jdk21    → whanos-java (base)
  ├── node:20.9          → whanos-javascript (base)
  ├── python:3.12        → whanos-python (base)
  └── python:3.12+pyfunge → whanos-befunge (base)
                ↓
        ONBUILD Instructions
                ↓
        Application Image
      (in k3d-whanos-registry)
                ↓
        Kubernetes Deployment
```

## Network Architecture

### Docker Networks

1. **k3d-whanos-cluster**: Main network connecting all components
   - Jenkins container
   - k3d cluster nodes
   - k3d registry

2. **whanos-network**: Network for whanos-registry

### Port Mapping

| Service | Internal Port | External Port | Protocol |
|---------|---------------|---------------|----------|
| Jenkins UI | 8080 | 8080 | HTTP |
| Jenkins Agent | 50000 | 50000 | TCP |
| Registry (whanos) | 5000 | 5000 | HTTP |
| Registry (k3d) | 5000 | 5001 | HTTP |
| K8s API Server | 6443 | 6443 | HTTPS |
| K8s LoadBalancer | 80 | 8081 | HTTP |

## Storage Architecture

### Persistent Volumes

- **Jenkins Home**: `/var/jenkins_home` (Docker volume)
- **Registry Data**: `/var/lib/registry` (Docker volume)
- **k3d Data**: Managed by k3d

### Mounted Directories

- **Whanos Images**: `/var/jenkins_home/whanos-images/`
  - Contains all Dockerfile.base and Dockerfile.standalone
  - Mounted from host `images/` directory

## Security Architecture

### Authentication

- **Jenkins**: Admin user with configurable password
- **Docker Registry**: Insecure HTTP (localhost only)
- **Kubernetes**: Certificate-based authentication

### Network Isolation

- All components run in Docker networks
- Kubernetes namespace isolation (`whanos`)
- No external access to internal registry

### Secrets Management

- Jenkins credentials stored in Jenkins credentials store
- Kubernetes secrets for sensitive data
- Git credentials for private repositories

## Scalability

### Horizontal Scaling

- **Kubernetes**: Add more agent nodes to k3d cluster
- **Jenkins**: Add Jenkins agents for parallel builds

### Vertical Scaling

- Increase Docker resources (CPU/Memory)
- Adjust Kubernetes resource requests/limits

## High Availability

- **Multiple K8s Nodes**: 2 agent nodes for pod distribution
- **LoadBalancer**: Distributes traffic across pods
- **Replicas**: Configure via `whanos.yml`

## Technology Stack Summary

| Layer | Technology | Version |
|-------|-----------|---------|
| Container Runtime | Docker | 20.10+ |
| CI/CD | Jenkins | Latest (LTS) |
| IaC | Terraform | 1.0+ |
| Orchestration | k3d/k3s | v1.31+ |
| Registry | Docker Registry | 2.0 |
| Automation | Bash Scripts | - |

## Next Steps

- Learn about [Components](./components.md) in detail
- Understand [Jenkins](./jenkins.md) configuration
- Explore [Kubernetes](./kubernetes.md) setup
- Review [Docker Registry](./registry.md) details
