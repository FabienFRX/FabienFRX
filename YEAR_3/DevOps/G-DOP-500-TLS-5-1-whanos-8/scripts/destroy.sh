#!/bin/bash
#
# Whanos Terraform Destroy Script
# Destroys infrastructure created by Terraform with Docker provider
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

# Confirm destruction
confirm_destruction() {
    echo ""
    echo -e "${RED}╔════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${RED}║                                                                ║${NC}"
    echo -e "${RED}║                   ⚠ WARNING ⚠                                 ║${NC}"
    echo -e "${RED}║                                                                ║${NC}"
    echo -e "${RED}║     This will DESTROY all Whanos infrastructure:              ║${NC}"
    echo -e "${RED}║       - Terraform-managed Docker containers                    ║${NC}"
    echo -e "${RED}║       - Docker volumes and networks                            ║${NC}"
    echo -e "${RED}║       - k3d Kubernetes cluster                                 ║${NC}"
    echo -e "${RED}║       - All Jenkins data                                       ║${NC}"
    echo -e "${RED}║                                                                ║${NC}"
    echo -e "${RED}╚════════════════════════════════════════════════════════════════╝${NC}"
    echo ""
    
    read -p "Are you sure you want to continue? (yes/no): " -r
    echo
    if [[ ! $REPLY =~ ^[Yy][Ee][Ss]$ ]]; then
        log_info "Destruction cancelled"
        exit 0
    fi
}

# Stop and remove Docker containers before destroy
stop_containers() {
    log_info "Stopping and removing Docker containers managed by Terraform..."
    
    # Stop and remove whanos containers
    if docker ps -a --format '{{.Names}}' | grep -q "whanos"; then
        docker ps -a --format '{{.Names}}' | grep "whanos" | xargs docker rm -f 2>/dev/null || true
        log_success "Removed whanos containers"
    fi
}

# Destroy with Terraform
destroy_terraform() {
    log_info "Destroying Terraform-managed resources..."
    
    if [ ! -d "$TERRAFORM_DIR" ]; then
        log_warning "Terraform directory not found"
        return 0
    fi
    
    cd "$TERRAFORM_DIR" || exit 1
    
    if [ ! -f ".terraform.lock.hcl" ]; then
        log_warning "Terraform not initialized"
        cd - >/dev/null || exit 1
        return 0
    fi
    
    log_info "Terraform destroy..."
    terraform destroy -auto-approve
    
    cd - >/dev/null || exit 1
    
    log_success "Terraform resources destroyed"
}

# Delete k3d cluster
delete_k3d_cluster() {
    if ! command -v k3d >/dev/null 2>&1; then
        log_warning "k3d not installed, skipping cluster deletion"
        return 0
    fi
    
    log_info "Deleting k3d cluster..."
    
    if k3d cluster list | grep -q "$CLUSTER_NAME"; then
        k3d cluster delete "$CLUSTER_NAME"
        log_success "k3d cluster deleted"
    else
        log_warning "k3d cluster not found"
    fi
    
    # Delete k3d registry if exists
    if docker ps -a --format '{{.Names}}' | grep -q "^k3d-whanos-registry$"; then
        docker rm -f k3d-whanos-registry
        log_success "k3d registry deleted"
    fi
}

# Clean up Docker artifacts
cleanup_docker() {
    log_info "Cleaning up Docker artifacts..."
    
    # Remove any remaining whanos containers
    if docker ps -a --format '{{.Names}}' | grep -q "whanos"; then
        docker ps -a --format '{{.Names}}' | grep "whanos" | xargs docker rm -f 2>/dev/null || true
        log_success "Removed remaining whanos containers"
    fi
    
    # Remove whanos networks
    if docker network ls --format '{{.Name}}' | grep -q "whanos"; then
        docker network ls --format '{{.Name}}' | grep "whanos" | xargs docker network rm 2>/dev/null || true
        log_success "Removed whanos networks"
    fi
    
    # Remove whanos volumes (optional - ask user)
    if docker volume ls --format '{{.Name}}' | grep -q "whanos"; then
        echo ""
        read -p "Do you want to remove whanos volumes (this will delete all data)? (yes/no): " -r
        echo
        if [[ $REPLY =~ ^[Yy][Ee][Ss]$ ]]; then
            docker volume ls --format '{{.Name}}' | grep "whanos" | xargs docker volume rm 2>/dev/null || true
            log_success "Removed whanos volumes"
        else
            log_info "Keeping whanos volumes"
        fi
    fi
}

# Show completion
show_completion() {
    echo ""
    echo -e "${GREEN}╔════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║                                                                ║${NC}"
    echo -e "${GREEN}║     ✓ WHANOS INFRASTRUCTURE DESTROYED!                         ║${NC}"
    echo -e "${GREEN}║                                                                ║${NC}"
    echo -e "${GREEN}╚════════════════════════════════════════════════════════════════╝${NC}"
    echo ""
    
    echo -e "${BLUE}📋 Cleanup Summary:${NC}"
    echo -e "   ✓ Terraform resources destroyed"
    echo -e "   ✓ k3d cluster deleted"
    echo -e "   ✓ Docker artifacts cleaned"
    echo ""
    echo -e "${BLUE}🔧 To redeploy:${NC}"
    echo -e "   ${YELLOW}./scripts/deploy.sh${NC}"
    echo ""
}

# Main
main() {
    confirm_destruction
    stop_containers
    destroy_terraform
    delete_k3d_cluster
    cleanup_docker
    show_completion
}

main "$@"