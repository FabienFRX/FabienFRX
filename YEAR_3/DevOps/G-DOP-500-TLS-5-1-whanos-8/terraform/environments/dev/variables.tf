variable "docker_host" {
  description = "Docker host socket"
  type        = string
  default     = "unix:///var/run/docker.sock"
}

variable "registry_port" {
  description = "Port for Docker registry"
  type        = number
  default     = 5000
}

variable "jenkins_port" {
  description = "Port for Jenkins"
  type        = number
  default     = 8080
}

variable "location" {
  description = "Azure location for resources"
  type        = string
  default     = "westeurope"
}

variable "jenkins_admin_username" {
  description = "Jenkins admin username"
  type        = string
  default     = "admin"
}

variable "ssh_public_key_path" {
  description = "Path to SSH public key"
  type        = string
  default     = "~/.ssh/id_rsa.pub"
}

variable "created_by" {
  description = "Creator tag for resources"
  type        = string
  default     = "Whanos Team"
}
