terraform {
  required_version = ">= 1.5.0"
  
  required_providers {
    docker = {
      source  = "kreuzwerker/docker"
      version = "~> 3.0"
    }
  }
}

provider "docker" {
  host = "unix:///var/run/docker.sock"
}

locals {
  environment     = "local"
  registry_port   = 5000
  jenkins_port    = 8080
  k3d_port        = 8081
  
  labels = {
    environment = local.environment
    project     = "whanos"
    managed_by  = "terraform"
  }
}

# Docker Network
resource "docker_network" "whanos_network" {
  name = "whanos-network"
  
  labels {
    label = "project"
    value = "whanos"
  }
  
  labels {
    label = "managed_by"
    value = "terraform"
  }
}

# Docker Registry (replaces Azure ACR)
module "registry" {
  source = "../../modules/registry"
  
  registry_name = "whanos-registry"
  registry_port = local.registry_port
  network_id    = docker_network.whanos_network.id
  labels        = local.labels
}

# Jenkins CI/CD Server
module "jenkins" {
  source = "../../modules/jenkins"
  
  jenkins_name       = "whanos-jenkins"
  jenkins_port       = local.jenkins_port
  registry_url       = "localhost:${local.registry_port}"
  network_id         = docker_network.whanos_network.id
  labels             = local.labels
  
  depends_on = [module.registry]
}
