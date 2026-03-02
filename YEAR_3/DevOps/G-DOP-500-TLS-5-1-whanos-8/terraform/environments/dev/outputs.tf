# Whanos Infrastructure Outputs

output "registry_endpoint" {
  description = "Docker registry endpoint"
  value       = "localhost:${local.registry_port}"
}

output "jenkins_url" {
  description = "Jenkins access URL"
  value       = "http://localhost:${local.jenkins_port}"
}

output "jenkins_container_name" {
  description = "Jenkins container name"
  value       = module.jenkins.container_name
}

output "registry_container_name" {
  description = "Registry container name"
  value       = module.registry.container_name
}

output "network_name" {
  description = "Docker network name"
  value       = docker_network.whanos_network.name
}
