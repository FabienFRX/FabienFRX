# Summary

[Introduction](./introduction.md)

# Getting Started

- [Quick Start](./quick-start.md)
- [Prerequisites](./prerequisites.md)
- [Installation](./installation.md)

# Architecture

- [Overview](./architecture/overview.md)
- [Components](./architecture/components.md)
  - [Jenkins](./architecture/jenkins.md)
  - [Docker Registry](./architecture/registry.md)
  - [Kubernetes Cluster](./architecture/kubernetes.md)

# Docker Images

- [Base Images](./docker/base-images.md)
  - [C](./docker/languages/c.md)
  - [Java](./docker/languages/java.md)
  - [JavaScript](./docker/languages/javascript.md)
  - [Python](./docker/languages/python.md)
  - [Befunge](./docker/languages/befunge.md)
- [Standalone Images](./docker/standalone-images.md)
- [Custom Dockerfiles](./docker/custom-dockerfiles.md)

# Jenkins Configuration

- [Configuration as Code (JCasC)](./jenkins/jcasc.md)
- [Jobs Overview](./jenkins/jobs.md)
  - [Base Images Jobs](./jenkins/base-images-jobs.md)
  - [Link Project Job](./jenkins/link-project.md)
- [Pipeline](./jenkins/pipeline.md)
  - [Language Detection](./jenkins/language-detection.md)
  - [Build Process](./jenkins/build-process.md)
  - [Registry Push](./jenkins/registry-push.md)
  - [Kubernetes Deployment](./jenkins/k8s-deployment.md)

# Kubernetes Deployment

- [Cluster Setup](./kubernetes/cluster-setup.md)
- [whanos.yml Configuration](./kubernetes/whanos-yml.md)
- [Deployments](./kubernetes/deployments.md)
- [Services and Networking](./kubernetes/services.md)

# User Guide

- [Creating a Whanos-Compatible Repository](./user-guide/compatible-repo.md)
- [Linking a Project](./user-guide/linking-project.md)
- [Configuring Deployment](./user-guide/configuring-deployment.md)
- [Private Repositories](./user-guide/private-repos.md)
- [Monitoring Builds](./user-guide/monitoring.md)

# Administration

- [Deployment with Terraform](./admin/terraform.md)
- [Infrastructure Management](./admin/infrastructure.md)
- [Backup and Recovery](./admin/backup.md)
- [Scaling](./admin/scaling.md)

# Troubleshooting

- [Common Issues](./troubleshooting/common-issues.md)
- [Jenkins Problems](./troubleshooting/jenkins.md)
- [Docker Issues](./troubleshooting/docker.md)
- [Kubernetes Issues](./troubleshooting/kubernetes.md)
- [Network Problems](./troubleshooting/network.md)

# Reference

- [Environment Variables](./reference/env-vars.md)
- [Configuration Files](./reference/config-files.md)
- [CLI Commands](./reference/cli-commands.md)
- [API Reference](./reference/api.md)

# Appendix

- [FAQ](./appendix/faq.md)
- [Glossary](./appendix/glossary.md)
- [Contributing](./appendix/contributing.md)
- [License](./appendix/license.md)
