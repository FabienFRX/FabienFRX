# Introduction

Welcome to **Whanos** - Automatically Deploy (Nearly) Anything with a Snap!

## What is Whanos?

Whanos is a powerful DevOps infrastructure that combines Docker, Jenkins, Kubernetes, and Infrastructure as Code (Terraform) to provide automatic containerization and deployment of applications written in multiple programming languages.

Named after the purple whale, Whanos brings together the "Infinity Stones" of DevOps:

- 🐳 **Containerization** with Docker
- 🔄 **Task Automation** with Jenkins
- 📦 **Configuration Management** with Terraform
- ☸️ **Orchestration** with Kubernetes

## Key Features

### Multi-Language Support

Whanos supports automatic detection and containerization of applications written in:

- **C** (GCC 13.2)
- **Java** (Java SE 21 with Maven)
- **JavaScript** (Node.js 20.9)
- **Python** (3.12)
- **Befunge** (Befunge-93)

### Automated Pipeline

When you push to a Whanos-compatible repository, the system automatically:

1. ✅ Fetches the Git repository
2. ✅ Analyzes content to determine the programming language
3. ✅ Containerizes the application using pre-built base images
4. ✅ Pushes the image to a Docker registry
5. ✅ Deploys to Kubernetes (if configured with `whanos.yml`)

### Infrastructure as Code

The entire Whanos infrastructure is deployed using Terraform, ensuring:

- **Reproducibility**: Deploy identical environments with a single command
- **Version Control**: Infrastructure changes are tracked in Git
- **Idempotency**: Re-running deployment is safe and predictable
- **Modularity**: Reusable Terraform modules for each component

## Architecture Overview

```
┌─────────────────────────────────────────────────────────┐
│                    Developer                            │
│                        ↓                                │
│                   git push                              │
└─────────────────────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────┐
│                     Jenkins                             │
│  ┌──────────────────────────────────────────────────┐   │
│  │  1. Detect Language (C/Java/JS/Python/Befunge)   │   │
│  │  2. Build Docker Image                           │   │
│  │  3. Push to Registry                             │   │
│  │  4. Deploy to Kubernetes (if whanos.yml exists)  │   │
│  └──────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────┘
                         ↓
         ┌───────────────┴───────────────┐
         ↓                               ↓
┌─────────────────┐           ┌─────────────────────┐
│ Docker Registry │           │  Kubernetes Cluster │
│                 │           │   (3 nodes: k3d)    │
│  - Base Images  │           │                     │
│  - App Images   │           │  - Deployments      │
│                 │           │  - Services         │
│  Port: 5000     │           │  - Pods             │
└─────────────────┘           └─────────────────────┘
```

## Use Cases

Whanos is ideal for:

- **🚀 Rapid Prototyping**: Quickly containerize and deploy applications
- **🎓 Educational Purposes**: Learn DevOps practices in a complete environment
- **🧪 Testing**: Create isolated test environments for applications
- **📦 Microservices**: Deploy multiple services automatically
- **🔄 CI/CD Pipelines**: Automate the entire build-test-deploy cycle

## Philosophy

Whanos follows the principle of **Convention over Configuration**:

- Place your application in an `app/` directory
- Include the appropriate language marker file (e.g., `requirements.txt` for Python)
- Optionally add a `whanos.yml` for Kubernetes configuration
- Push to your repository
- **Whanos handles the rest!**

## What's Next?

- Ready to dive in? Check out the [Quick Start](./quick-start.md) guide
- Want to understand the architecture? Read the [Architecture Overview](./architecture/overview.md)
- Need to troubleshoot? Visit the [Troubleshooting](./troubleshooting/common-issues.md) section

---

Let's embark on your DevOps journey with Whanos! 🐋
