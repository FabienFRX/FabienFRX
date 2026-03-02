##
## EPITECH PROJECT, 2025
## G-DOP-500-TLS-5-1-bernstein-4 [WSL: Debian]
## File description:
## Makefile
##
.PHONY: help apply delete clean status logs init-db setup-dns tunnel all

# Default target
help:
	@echo "Bernstein Kubernetes Project Makefile"
	@echo "=============================="
	@echo "Available targets:"
	@echo "  make apply      - Deploy all Kubernetes resources"
	@echo "  make delete     - Delete all Kubernetes resources"
	@echo "  make clean      - Complete cleanup (delete + remove PVs)"
	@echo "  make status     - Show status of all resources"
	@echo "  make logs       - Show logs from all pods"
	@echo "  make init-db    - Initialize PostgreSQL database"
	@echo "  make setup-dns  - Configure local DNS (/etc/hosts)"
	@echo "  make tunnel     - Start minikube tunnel (keep running)"
	@echo "  make all        - Full deployment (apply + init-db + setup-dns)"

# Deploy all resources in correct order
apply:
	@echo "Deploying Bernstein project..."
	@echo "================================"
	
	@echo "Deploying cAdvisor monitoring..."
	kubectl apply -f cadvisor.daemonset.yaml
	
	@echo "Deploying PostgreSQL..."
	kubectl apply -f postgres.secret.yaml \
		-f postgres.configmap.yaml \
		-f postgres.volume.yaml \
		-f postgres.deployment.yaml \
		-f postgres.service.yaml
	
	@echo "Deploying Redis..."
	kubectl apply -f redis.configmap.yaml \
		-f redis.deployment.yaml \
		-f redis.service.yaml
	
	@echo "Deploying application components..."
	kubectl apply -f poll.deployment.yaml \
		-f worker.deployment.yaml \
		-f result.deployment.yaml \
		-f poll.service.yaml \
		-f result.service.yaml \
		-f poll.ingress.yaml \
		-f result.ingress.yaml
	
	@echo "Deploying Traefik load balancer..."
	kubectl apply -f traefik.rbac.yaml \
		-f traefik.deployment.yaml \
		-f traefik.service.yaml
	
	@echo "Deployment complete!"
	@echo ""
	@echo "Run 'make status' to check deployment status"
	@echo "Run 'make init-db' to initialize the database"

# Delete all resources
delete:
	@echo "Deleting Bernstein project resources..."
	@echo "========================================="
	
	@echo "Deleting Traefik..."
	-kubectl delete -f traefik.service.yaml
	-kubectl delete -f traefik.deployment.yaml
	-kubectl delete -f traefik.rbac.yaml
	
	@echo "Deleting application components..."
	-kubectl delete -f result.ingress.yaml
	-kubectl delete -f poll.ingress.yaml
	-kubectl delete -f result.service.yaml
	-kubectl delete -f poll.service.yaml
	-kubectl delete -f result.deployment.yaml
	-kubectl delete -f worker.deployment.yaml
	-kubectl delete -f poll.deployment.yaml
	
	@echo "Deleting Redis..."
	-kubectl delete -f redis.service.yaml
	-kubectl delete -f redis.deployment.yaml
	-kubectl delete -f redis.configmap.yaml
	
	@echo "Deleting PostgreSQL..."
	-kubectl delete -f postgres.service.yaml
	-kubectl delete -f postgres.deployment.yaml
	-kubectl delete -f postgres.volume.yaml
	-kubectl delete -f postgres.configmap.yaml
	-kubectl delete -f postgres.secret.yaml
	
	@echo "Deleting cAdvisor..."
	-kubectl delete -f cadvisor.daemonset.yaml
	
	@echo "Resources deleted!"

# Complete cleanup including PVs
clean: delete
	@echo "Performing complete cleanup..."
	-kubectl delete pv postgres-pv --force --grace-period=0
	@echo "Complete cleanup done!"

# Show status of all resources
status:
	@echo "Bernstein Project Status"
	@echo "=========================="
	@echo ""
	@echo "Nodes:"
	kubectl get nodes
	@echo ""
	@echo "Pods (all namespaces):"
	kubectl get pods --all-namespaces | grep -E "(default|kube-public|kube-system|NAME)" | grep -E "(poll|result|worker|postgres|redis|traefik|cadvisor|NAME)"
	@echo ""
	@echo "Services:"
	kubectl get svc --all-namespaces | grep -E "(default|kube-public|NAME)" | grep -E "(poll|result|postgres|redis|traefik|NAME)"
	@echo ""
	@echo "Ingresses:"
	kubectl get ingress
	@echo ""
	@echo "Persistent Volumes:"
	kubectl get pv,pvc

# Show logs from all application pods
logs:
	@echo "Application Logs"
	@echo "=================="
	@echo ""
	@echo "Poll logs:"
	-kubectl logs -l app=poll --tail=20
	@echo ""
	@echo "Result logs:"
	-kubectl logs -l app=result --tail=20
	@echo ""
	@echo "Worker logs:"
	-kubectl logs -l app=worker --tail=20
	@echo ""
	@echo "PostgreSQL logs:"
	-kubectl logs -l app=postgres --tail=20
	@echo ""
	@echo "Redis logs:"
	-kubectl logs -l app=redis --tail=20

# Initialize PostgreSQL database
init-db:
	@echo "Initializing PostgreSQL database..."
	@echo "Waiting for PostgreSQL to be ready..."
	@kubectl wait --for=condition=ready pod -l app=postgres --timeout=60s 2>/dev/null || echo "PostgreSQL might not be ready yet"
	@sleep 5
	@POSTGRES_POD=$$(kubectl get pods -l app=postgres -o jsonpath='{.items[0].metadata.name}' 2>/dev/null); \
	if [ -n "$$POSTGRES_POD" ]; then \
		echo "Creating votes table in pod: $$POSTGRES_POD"; \
		echo "CREATE TABLE IF NOT EXISTS votes (id text PRIMARY KEY, vote text NOT NULL);" | \
		kubectl exec -i $$POSTGRES_POD -- psql -U postgres -d votes 2>/dev/null || \
		echo "Note: Table might already exist or database is still initializing"; \
	else \
		echo "PostgreSQL pod not found. Please run 'make apply' first."; \
	fi
	@echo "Database initialization complete!"

# Setup local DNS entries
setup-dns:
	@echo "Setting up local DNS entries..."
	@MINIKUBE_IP=$$(minikube ip 2>/dev/null); \
	if [ -n "$$MINIKUBE_IP" ]; then \
		echo "Minikube IP: $$MINIKUBE_IP"; \
		if grep -q "poll.dop.io\|result.dop.io" /etc/hosts; then \
			echo "DNS entries already exist in /etc/hosts"; \
		else \
			echo "Adding DNS entries to /etc/hosts (requires sudo)..."; \
			echo "$$MINIKUBE_IP poll.dop.io result.dop.io" | sudo tee -a /etc/hosts; \
		fi; \
	else \
		echo "Minikube not running. Please start minikube first."; \
	fi
	@echo "DNS setup complete!"
	@echo ""
	@echo "You can now access:"
	@echo "  Poll:    http://poll.dop.io:30021"
	@echo "  Result:  http://result.dop.io:30021"
	@echo "  Traefik: http://localhost:30042"

# Start minikube tunnel (needs to keep running)
tunnel:
	@echo "Starting Minikube tunnel..."
	@echo "Keep this terminal open for the tunnel to work!"
	@echo "Press Ctrl+C to stop the tunnel"
	sudo minikube tunnel

# Full deployment
all: apply
	@sleep 10
	@$(MAKE) init-db
	@$(MAKE) setup-dns
	@echo ""
	@echo "Full deployment complete!"
	@echo ""
	@$(MAKE) status