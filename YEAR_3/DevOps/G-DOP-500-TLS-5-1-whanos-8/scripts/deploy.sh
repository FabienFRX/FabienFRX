#!/bin/bash
#
# Whanos Terraform Deployment Script
# Uses Terraform with Docker provider for localhost deployment
#
# This uses Terraform as Infrastructure as Code (appropriate technology per subject)
# Deploys same infrastructure as deploy-local.sh but using Terraform
#

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

CLUSTER_NAME="whanos-cluster"
TERRAFORM_DIR="terraform/environments/dev"

log_info() {
    echo -e "${BLUE}ℹ${NC}  $1"
}

log_success() {
    echo -e "${GREEN}✓${NC}  $1"
}

log_warning() {
    echo -e "${YELLOW}⚠${NC}  $1"
}

log_error() {
    echo -e "${RED}✗${NC}  $1"
}

# Check prerequisites
check_prerequisites() {
    log_info "Checking prerequisites..."
    
    local missing=0
    
    if ! command -v docker >/dev/null 2>&1; then
        log_error "Docker is not installed"
        missing=1
    fi
    
    if ! command -v terraform >/dev/null 2>&1; then
        log_error "Terraform is not installed"
        log_error "Install from: https://www.terraform.io/downloads"
        missing=1
    fi
    
    if [ $missing -eq 1 ]; then
        exit 1
    fi
    
    log_success "Prerequisites checked"
}

# Install k3d if needed
install_k3d() {
    if ! command -v k3d >/dev/null 2>&1; then
        log_info "Installing k3d..."
        curl -s https://raw.githubusercontent.com/k3d-io/k3d/main/install.sh | bash
        log_success "k3d installed"
    else
        log_success "k3d already installed"
    fi
}

# Install kubectl if needed
install_kubectl() {
    if ! command -v kubectl >/dev/null 2>&1; then
        log_info "Installing kubectl..."
        curl -LO "https://dl.k8s.io/release/$(curl -L -s https://dl.k8s.io/release/stable.txt)/bin/linux/amd64/kubectl"
        chmod +x kubectl
        sudo mv kubectl /usr/local/bin/
        log_success "kubectl installed"
    else
        log_success "kubectl already installed"
    fi
}

# Create k3d cluster
create_k3d_cluster() {
    log_info "Creating k3d Kubernetes cluster (3 nodes: 1 server + 2 agents)..."
    
    if k3d cluster list | grep -q "$CLUSTER_NAME"; then
        log_warning "Cluster already exists"
        return 0
    fi
    
    # Create k3d registry
    if ! docker ps -a --format '{{.Names}}' | grep -q "^k3d-whanos-registry$"; then
        k3d registry create whanos-registry --port 5001
        log_success "k3d registry created"
    fi
    
    # Create cluster with 3 nodes (subject requirement: at least 2 nodes)
    k3d cluster create $CLUSTER_NAME \
        --agents 2 \
        --registry-use k3d-whanos-registry:5001 \
        --port "8081:80@loadbalancer" \
        --api-port 6443 \
        --wait
    
    log_success "k3d cluster created (3 nodes total)"
}

# Deploy with Terraform
deploy_terraform() {
    log_info "Deploying with Terraform (Docker provider)..."
    
    cd "$TERRAFORM_DIR" || exit 1
    
    log_info "Terraform init..."
    terraform init -upgrade
    
    log_info "Terraform validate..."
    terraform validate
    
    log_info "Terraform plan..."
    terraform plan -out=tfplan
    
    log_info "Terraform apply..."
    terraform apply tfplan
    
    rm -f tfplan
    
    cd - >/dev/null || exit 1
    
    log_success "Terraform deployment completed"
}

# Wait for Jenkins
wait_for_jenkins() {
    log_info "Waiting for Jenkins to start (up to 2 minutes)..."
    
    local max_attempts=60
    local attempt=0
    
    while [ $attempt -lt $max_attempts ]; do
        if curl -s http://localhost:8080 >/dev/null 2>&1; then
            log_success "Jenkins is ready"
            return 0
        fi
        attempt=$((attempt + 1))
        sleep 2
    done
    
    log_error "Jenkins did not start in time"
    return 1
}

# Create Kubernetes namespace
create_k8s_namespace() {
    log_info "Creating Kubernetes namespace..."
    kubectl create namespace whanos --dry-run=client -o yaml | kubectl apply -f -
    log_success "Kubernetes namespace 'whanos' ready"
}

# Show outputs
show_outputs() {
    echo ""
    echo -e "${GREEN}╔════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║                                                                ║${NC}"
    echo -e "${GREEN}║     ✓ WHANOS DEPLOYED WITH TERRAFORM (Docker Provider)!        ║${NC}"
    echo -e "${GREEN}║                                                                ║${NC}"
    echo -e "${GREEN}╚════════════════════════════════════════════════════════════════╝${NC}"
    echo ""
    
    log_info "Terraform Outputs:"
    echo ""
    cd "$TERRAFORM_DIR" || exit 1
    terraform output
    cd - >/dev/null || exit 1
    
    echo ""
    echo -e "${BLUE}🔑 Jenkins Admin Password:${NC}"
    echo -e "   ${YELLOW}docker exec whanos-jenkins cat /var/jenkins_home/secrets/initialAdminPassword${NC}"
    echo ""
    echo -e "${BLUE}🌐 Access Points:${NC}"
    echo -e "   Jenkins:  ${YELLOW}http://localhost:8080${NC}"
    echo -e "   Registry: ${YELLOW}http://localhost:5000${NC}"
    echo -e "   K8s Apps: ${YELLOW}http://localhost:8081${NC}"
    echo ""
    echo -e "${BLUE}📋 Next Steps:${NC}"
    echo -e "   1. Get Jenkins password (command above)"
    echo -e "   2. Build base images: ${YELLOW}Whanos base images/Build all base images${NC}"
    echo -e "   3. Link projects: ${YELLOW}link-project${NC} job"
    echo ""
    echo -e "${BLUE}🔧 To destroy:${NC}"
    echo -e "   ${YELLOW}./scripts/destroy.sh${NC}"
    echo ""
}

# Main
main() {
    echo ""
    echo -e "${BLUE}╔════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${BLUE}║                                                                ║${NC}"
    echo -e "${BLUE}║          WHANOS TERRAFORM DEPLOYMENT (Localhost)               ║${NC}"
    echo -e "${BLUE}║                                                                ║${NC}"
    echo -e "${BLUE}║  Infrastructure as Code: Terraform with Docker Provider        ║${NC}"
    echo -e "${BLUE}║                                                                ║${NC}"
    echo -e "${BLUE}╚════════════════════════════════════════════════════════════════╝${NC}"
    echo ""
    
    check_prerequisites
    install_kubectl
    install_k3d
    create_k3d_cluster
    deploy_terraform
    wait_for_jenkins
    create_k8s_namespace
    show_outputs
}

main "$@"