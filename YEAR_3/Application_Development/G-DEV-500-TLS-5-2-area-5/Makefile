# AREA Project Makefile

.PHONY: help build up down restart rebuild logs clean test migrate dev hook-test celery-logs

# Default command
help:
	@echo "Available commands:"
	@echo "  make build         - Build Docker containers"
	@echo "  make up            - Start all containers"
	@echo "  make down          - Stop all containers"
	@echo "  make restart       - Restart all containers"
	@echo "  make rebuild       - Stop, rebuild, and start containers"
	@echo "  make logs          - Show logs of all containers"
	@echo "  make logs-web      - Show web container logs"
	@echo "  make logs-api      - Show API container logs"
	@echo "  make logs-db       - Show database container logs"
	@echo "  make logs-celery   - Show Celery worker logs"
	@echo "  make logs-beat     - Show Celery beat logs"
	@echo "  make celery-logs   - Show both Celery worker and beat logs"
	@echo "  make clean         - Remove all containers and volumes"
	@echo "  make migrate       - Run Django migrations"
	@echo "  make shell         - Open Django shell"
	@echo "  make test          - Run tests"
	@echo "  make hook-test     - Test the hook system end-to-end"
	@echo "  make dev           - Start development servers"
	@echo "  make install       - Install dependencies"

# Docker commands
build:
	docker-compose build

up:
	docker-compose up -d

down:
	docker-compose down

restart:
	docker-compose restart

rebuild:
	@echo "🔄 Rebuilding containers..."
	docker-compose down
	docker-compose build --no-cache
	docker-compose up -d
	@echo "✅ Rebuild complete!"

# Logs
logs:
	docker-compose logs -f

logs-web:
	docker-compose logs -f client_web

logs-api:
	docker-compose logs -f server

logs-db:
	docker-compose logs -f database

logs-celery:
	docker-compose logs -f celery_worker

logs-beat:
	docker-compose logs -f celery_beat

celery-logs:
	@echo "📊 Showing Celery Worker and Beat logs..."
	docker-compose logs -f celery_worker celery_beat

# Hook system
hook-test:
	@echo "🧪 Testing hook system..."
	./test-hook-system.sh

logs-api:
	docker-compose logs -f server

logs-db:
	docker-compose logs -f database

# Database and Django
migrate:
	@echo "🔄 Running migrations..."
	docker-compose exec server python manage.py migrate
	@echo "✅ Migrations complete!"

makemigrations:
	@echo "🔄 Creating migrations..."
	docker-compose exec server python manage.py makemigrations
	@echo "✅ Migrations created!"

shell:
	docker-compose exec server python manage.py shell

loaddata:
	@echo "🔄 Loading sample data..."
	docker-compose exec server python manage.py load_sample_data
	@echo "✅ Sample data loaded!"

# Development commands
dev:
	@echo "🚀 Starting development servers..."
	@echo "Backend: http://localhost:8080"
	@echo "Frontend: http://localhost:5173"
	cd backend && source venv/bin/activate && python manage.py runserver 8080 &
	cd web && npm run dev &
	wait

install:
	@echo "🔄 Installing dependencies..."
	cd backend && pip install -r requirements.txt
	cd web && npm install
	@echo "✅ Dependencies installed!"

# Testing
test:
	@echo "🧪 Running tests..."
	cd backend && source venv/bin/activate && python manage.py test
	cd web && npm run test
	@echo "✅ Tests complete!"

# Cleanup
clean:
	@echo "🧹 Cleaning up containers and volumes..."
	docker-compose down -v --remove-orphans
	docker system prune -f
	@echo "✅ Cleanup complete!"

# Quick development workflow
quick-start: down build up logs

# Full reset (for when things break)
reset: clean build migrate loaddata up
	@echo "🎉 Full reset complete! Your environment is fresh."

# Update workflow
update: down rebuild migrate loaddata
	@echo "🎉 Update complete!"