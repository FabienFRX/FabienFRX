# Jenkins Module
# Creates Jenkins container with Docker and kubectl integration

terraform {
  required_providers {
    docker = {
      source  = "kreuzwerker/docker"
      version = "~> 3.0"
    }
  }
}

variable "jenkins_name" {
  description = "Name of the Jenkins container"
  type        = string
}

variable "jenkins_port" {
  description = "External port for Jenkins"
  type        = number
}

variable "registry_url" {
  description = "Docker registry URL"
  type        = string
}

variable "network_id" {
  description = "Docker network ID"
  type        = string
}

variable "labels" {
  description = "Labels to apply to resources"
  type        = map(string)
  default     = {}
}

# Docker volume for Jenkins data
resource "docker_volume" "jenkins_data" {
  name = "${var.jenkins_name}-data"
  
  labels {
    label = "project"
    value = "whanos"
  }
}

# Build custom Jenkins image with plugins and configuration
resource "docker_image" "jenkins" {
  name = "whanos-jenkins:latest"

  build {
    context    = abspath("${path.root}/../../../jenkins")
    dockerfile = "Dockerfile"
    tag        = ["whanos-jenkins:latest"]
    label = {
      project    = "whanos"
      managed_by = "terraform"
    }
  }

  keep_locally = true
}

# Jenkins container
resource "docker_container" "jenkins" {
  name  = var.jenkins_name
  image = docker_image.jenkins.image_id
  user  = "0:0"

  restart = "unless-stopped"
  
  ports {
    internal = 8080
    external = var.jenkins_port
  }
  
  ports {
    internal = 50000
    external = 50000
  }
  
  volumes {
    volume_name    = docker_volume.jenkins_data.name
    container_path = "/var/jenkins_home"
  }
  
  # Mount Docker socket for Docker-in-Docker
  volumes {
    host_path      = "/var/run/docker.sock"
    container_path = "/var/run/docker.sock"
  }
  
  # Mount Whanos images (Dockerfiles)
  volumes {
    host_path      = abspath("${path.root}/../../../images")
    container_path = "/var/jenkins_home/whanos-images"
    read_only      = true
  }
  
  # Mount test applications
  volumes {
    host_path      = abspath("${path.root}/../../../test-apps")
    container_path = "/var/jenkins_home/test-apps"
    read_only      = true
  }

  # Mount Jenkins JCasC configuration
  volumes {
    host_path      = abspath("${path.root}/../../../jenkins/config/jenkins.yaml")
    container_path = "/var/jenkins_home/casc_configs/jenkins.yaml"
    read_only      = true
  }

  # Mount kubeconfig if exists
  volumes {
    host_path      = pathexpand("~/.kube")
    container_path = "/root/.kube"
    read_only      = true
  }
  
  networks_advanced {
    name = var.network_id
  }
  
  env = [
    "DOCKER_HOST=unix:///var/run/docker.sock",
    "REGISTRY_URL=${var.registry_url}",
    "K3D_CLUSTER_NAME=whanos-cluster",
    "CASC_JENKINS_CONFIG=/var/jenkins_home/casc_configs/jenkins.yaml",
    "JAVA_OPTS=-Djenkins.install.runSetupWizard=false"
  ]
  
  dynamic "labels" {
    for_each = var.labels
    content {
      label = labels.key
      value = labels.value
    }
  }
}

output "container_id" {
  value = docker_container.jenkins.id
}

output "container_name" {
  value = docker_container.jenkins.name
}

output "jenkins_url" {
  value = "http://localhost:${var.jenkins_port}"
}
