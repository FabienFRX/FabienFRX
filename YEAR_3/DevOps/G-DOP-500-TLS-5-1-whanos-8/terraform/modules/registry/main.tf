# Docker Registry Module
# Replaces Azure Container Registry (ACR) with local Docker registry

terraform {
  required_providers {
    docker = {
      source  = "kreuzwerker/docker"
      version = "~> 3.0"
    }
  }
}

variable "registry_name" {
  description = "Name of the registry container"
  type        = string
}

variable "registry_port" {
  description = "External port for the registry"
  type        = number
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

# Docker volume for registry data
resource "docker_volume" "registry_data" {
  name = "${var.registry_name}-data"
  
  labels {
    label = "project"
    value = "whanos"
  }
}

# Registry image
resource "docker_image" "registry" {
  name         = "registry:2"
  keep_locally = false
}

# Registry container
resource "docker_container" "registry" {
  name  = var.registry_name
  image = docker_image.registry.image_id
  
  restart = "unless-stopped"
  
  ports {
    internal = 5000
    external = var.registry_port
  }
  
  volumes {
    volume_name    = docker_volume.registry_data.name
    container_path = "/var/lib/registry"
  }
  
  networks_advanced {
    name = var.network_id
  }
  
  env = [
    "REGISTRY_STORAGE_DELETE_ENABLED=true"
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
  value = docker_container.registry.id
}

output "container_name" {
  value = docker_container.registry.name
}

output "registry_url" {
  value = "localhost:${var.registry_port}"
}
