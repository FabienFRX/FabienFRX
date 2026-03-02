# AREA Project Makefile

COMPOSE_FILE ?= docker-compose.yml
BACKEND_SERVICE = server
DB_SERVICE = database
WEB_SERVICE = client_web
MOBILE_SERVICE = client_mobile

.PHONY: help build up down restart logs clean test migrate shell admin install

all: help

help: ## Show this help message
	@echo "AREA Project - Action-Reaction Platform"
	@echo "====================================="
	@echo ""
	@awk 'BEGIN {FS = ":.*##"; printf "Usage: make <target>\n\nTargets:\n"} /^[a-zA-Z_-]+:.*?##/ { printf "  %-15s %s\n", $$1, $$2 }' $(MAKEFILE_LIST)

install: ## Install dependencies and setup environment
	@echo "Installing dependencies..."
	@if [ ! -f .env ]; then \
		echo "Creating .env file..."; \
		cp .env.example .env 2>/dev/null || echo "# Configure your .env file" > .env; \
	fi
	@echo "Dependencies installed"

build: ## Build all Docker services
	@echo "Building Docker services..."
	@docker-compose -f $(COMPOSE_FILE) build
	@echo "Build completed"

up: ## Start all services
	@echo "Starting all services..."
	@docker-compose -f $(COMPOSE_FILE) up -d
	@echo "All services started"
	@echo "Web Client: http://localhost:8081"
	@echo "API Server: http://localhost:8080"
	@echo "Admin Panel: http://localhost:8080/admin"

down: ## Stop all services
	@echo "Stopping all services..."
	@docker-compose -f $(COMPOSE_FILE) down
	@echo "All services stopped"

restart: down up ## Restart all services

migrate: ## Run database migrations
	@echo "Running migrations..."
	@docker-compose -f $(COMPOSE_FILE) exec -T $(BACKEND_SERVICE) python manage.py makemigrations
	@docker-compose -f $(COMPOSE_FILE) exec -T $(BACKEND_SERVICE) python manage.py migrate
	@echo "Migrations completed"

seed: ## Load sample data
	@echo "Loading sample data..."
	@docker-compose -f $(COMPOSE_FILE) exec -T $(BACKEND_SERVICE) python manage.py load_sample_data
	@echo "Sample data loaded"

shell: ## Access Django shell
	@echo "Opening Django shell..."
	@docker-compose -f $(COMPOSE_FILE) exec $(BACKEND_SERVICE) python manage.py shell

dbshell: ## Access database shell
	@echo "Opening database shell..."
	@docker-compose -f $(COMPOSE_FILE) exec $(DB_SERVICE) psql -U area_user area_db

admin: ## Create Django superuser
	@echo "Creating Django superuser..."
	@docker-compose -f $(COMPOSE_FILE) exec $(BACKEND_SERVICE) python manage.py createsuperuser

logs: ## View logs for all services
	@docker-compose -f $(COMPOSE_FILE) logs -f

logs-api: ## View API server logs
	@docker-compose -f $(COMPOSE_FILE) logs -f $(BACKEND_SERVICE)

logs-db: ## View database logs
	@docker-compose -f $(COMPOSE_FILE) logs -f $(DB_SERVICE)

status: ## Show services status
	@echo "Services Status:"
	@docker-compose -f $(COMPOSE_FILE) ps

test: ## Run tests
	@echo "Running tests..."
	@docker-compose -f $(COMPOSE_FILE) exec -T $(BACKEND_SERVICE) python manage.py test
	@echo "Tests completed"

test-api: ## Test API endpoints
	@echo "Testing API endpoints..."
	@curl -s http://localhost:8080/about.json | head -c 200 && echo "..."
	@echo ""
	@curl -s http://localhost:8080/hello/ || echo "API not responding"
	@echo "API tests completed"

clean: ## Clean up Docker resources
	@echo "Cleaning up Docker resources..."
	@docker-compose -f $(COMPOSE_FILE) down -v
	@docker system prune -f
	@echo "Cleanup completed"

backup-db: ## Backup database
	@echo "Creating database backup..."
	@mkdir -p backups
	@docker-compose -f $(COMPOSE_FILE) exec -T $(DB_SERVICE) pg_dump -U area_user area_db > backups/backup_$(shell date +%Y%m%d_%H%M%S).sql
	@echo "Database backup created"

quick-start: build up migrate seed ## Quick start: build, run, migrate, and seed
	@echo "AREA Platform ready!"
	@echo "Access: http://localhost:8081"

dev: ## Start development environment
	@echo "Starting development environment..."
	@make build
	@make up
	@sleep 5
	@make migrate
	@make test-api
	@echo "Development environment ready!"