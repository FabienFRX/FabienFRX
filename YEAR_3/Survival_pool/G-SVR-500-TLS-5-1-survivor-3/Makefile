# Makefile pour le projet JEB Incubator Platform
# Utilisation: make <commande>

.PHONY: help install dev build up down logs shell migrate superuser test clean frontend

# Variables
DOCKER_COMPOSE = docker-compose
PYTHON = python
MANAGE = python manage.py

# Aide - affiche toutes les commandes disponibles
help:
	@echo "=== Commandes disponibles ==="
	@echo "Développement:"
	@echo "  make dev          - Démarrer l'environnement complet (backend + frontend Docker)"
	@echo "  make dev-backend  - Démarrer seulement le backend en Docker"
	@echo "  make dev-frontend - Démarrer le frontend en local (plus rapide pour le dev)"
	@echo "  make dev-full     - Démarrer backend + frontend en Docker"
	@echo "  make build        - Construire les images Docker"
	@echo "  make up           - Démarrer les conteneurs"
	@echo "  make down         - Arrêter les conteneurs"
	@echo "  make logs         - Voir les logs des conteneurs"
	@echo "  make shell        - Ouvrir un shell dans le conteneur backend"
	@echo ""
	@echo "Frontend:"
	@echo "  make frontend     - Démarrer seulement le frontend en Docker"
	@echo "  make frontend-dev - Démarrer le frontend en mode dev local"
	@echo ""
	@echo "Base de données:"
	@echo "  make migrate      - Appliquer les migrations"
	@echo "  make makemigrations - Créer de nouvelles migrations"
	@echo "  make superuser    - Créer un superutilisateur"
	@echo "  make sync-data    - Synchroniser les données JEB"
	@echo ""
	@echo "Tests et qualité:"
	@echo "  make test         - Lancer les tests"
	@echo "  make lint         - Vérifier le code avec flake8"
	@echo ""
	@echo "Maintenance:"
	@echo "  make clean        - Nettoyer les conteneurs et volumes"
	@echo "  make install      - Installer les dépendances localement"
	@echo "  make requirements - Mettre à jour requirements.txt"

# Installation des dépendances localement
install:
	@echo "Installation des dépendances Python..."
	pip install -r requirements.txt

# Mise à jour des requirements
requirements:
	@echo "Mise à jour de requirements.txt..."
	pip freeze > requirements.txt

# Développement - démarre l'environnement complet
dev: build up
	@echo "Environnement de développement démarré !"
	@echo "Backend Django: http://localhost:8000"
	@echo "Frontend React: http://localhost:5173"
	@echo "Admin Django: http://localhost:8000/admin"

# Développement backend seulement
dev-backend:
	@echo "Démarrage du backend seulement..."
	$(DOCKER_COMPOSE) up -d web
	@echo "Backend disponible sur: http://localhost:8000"
	@echo "Pour le frontend, utilisez: make dev-frontend"

# Développement frontend en local (recommandé pour le dev)
dev-frontend:
	@echo "Démarrage du frontend en local..."
	@echo "Assurez-vous que le backend est démarré avec: make dev-backend"
	@echo "Installation des dépendances Node.js..."
	cd frontend && npm install
	@echo "Démarrage du serveur de développement..."
	cd frontend && npm run dev

# Développement complet en Docker
dev-full:
	@echo "Démarrage complet (backend + frontend Docker)..."
	$(DOCKER_COMPOSE) up -d
	@echo "Backend: http://localhost:8000"
	@echo "Frontend: http://localhost:5173"

# Construction des images Docker
build:
	@echo "Construction des images Docker..."
	$(DOCKER_COMPOSE) build

# Démarrage des conteneurs
up:
	@echo "Démarrage des conteneurs..."
	$(DOCKER_COMPOSE) up -d

# Démarrage avec logs en temps réel
up-logs:
	@echo "Démarrage des conteneurs avec logs..."
	$(DOCKER_COMPOSE) up

# Arrêt des conteneurs
down:
	@echo "Arrêt des conteneurs..."
	$(DOCKER_COMPOSE) down

# Voir les logs
logs:
	@echo "Affichage des logs..."
	$(DOCKER_COMPOSE) logs -f

# Shell dans le conteneur backend
shell:
	@echo "Ouverture d'un shell dans le conteneur backend..."
	$(DOCKER_COMPOSE) exec web bash

# Frontend
frontend:
	@echo "Démarrage du frontend..."
	$(DOCKER_COMPOSE) up -d frontend

frontend-dev:
	@echo "Démarrage du frontend en mode développement local..."
	cd frontend && npm install && npm run dev

# Commandes Django dans le conteneur
migrate:
	@echo "Application des migrations..."
	$(DOCKER_COMPOSE) exec web $(MANAGE) migrate

makemigrations:
	@echo "Création des migrations..."
	$(DOCKER_COMPOSE) exec web $(MANAGE) makemigrations

superuser:
	@echo "Création d'un superutilisateur..."
	$(DOCKER_COMPOSE) exec web $(MANAGE) createsuperuser

sync-data:
	@echo "Synchronisation des données JEB..."
	$(DOCKER_COMPOSE) exec web $(MANAGE) sync_jeb_data

# Tests
test:
	@echo "Lancement des tests..."
	$(DOCKER_COMPOSE) exec web $(MANAGE) test

# Vérification du code (nécessite flake8)
lint:
	@echo "Vérification du code avec flake8..."
	$(DOCKER_COMPOSE) exec web flake8 .

# Nettoyage complet
clean:
	@echo "Nettoyage des conteneurs et volumes..."
	$(DOCKER_COMPOSE) down --volumes --remove-orphans
	docker system prune -f

# Nettoyage des images Docker
clean-images:
	@echo "Nettoyage des images Docker..."
	docker image prune -f

# Redémarrage complet
restart: down up
	@echo "Redémarrage complet effectué !"

# Statut des conteneurs
status:
	@echo "Statut des conteneurs:"
	$(DOCKER_COMPOSE) ps

# Commandes de développement rapide
quick-start: build up migrate superuser
	@echo "Démarrage rapide terminé !"
	@echo "Backend: http://localhost:8000"
	@echo "Frontend: http://localhost:5173"

# Sauvegarde de la base de données (SQLite)
backup:
	@echo "Sauvegarde de la base de données..."
	cp survival/db.sqlite3 survival/db.sqlite3.backup.$(shell date +%Y%m%d_%H%M%S)

# Restauration de la base de données
restore:
	@echo "Restauration de la base de données..."
	@read -p "Nom du fichier de sauvegarde: " file; \
	cp $$file survival/db.sqlite3

# Commandes par défaut
.DEFAULT_GOAL := help 