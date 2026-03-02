# AREA - ACTION-REACTION Platform

> **Plateforme d'automatisation similaire à IFTTT/Zapier** permettant de créer des workflows automatiques entre différents services (Google, Timer, Spotify, etc.)

## 🚀 Démarrage Rapide

```bash
# Démarrer l'infrastructure complète
docker-compose up -d --build

# Tester le système de hook
make hook-test
```

**Services disponibles** :
- 🌐 Web Interface: http://localhost:8081
- 🔌 API Backend: http://localhost:8080
- 📊 API Documentation: http://localhost:8080/api/about.json

## ✨ Fonctionnalités

### ✅ Système de Hook Automatique

Le projet utilise **Celery + Redis** pour exécuter automatiquement les workflows :
- ⏰ Vérification automatique toutes les 30 secondes
- 🔄 Exécution asynchrone des réactions
- 📝 Historique complet des exécutions
- 🛡️ Retry automatique en cas d'échec

### ✅ Services Intégrés

| Service | Actions | Réactions |
|---------|---------|-----------|
| **Timer** | Intervalles (secondes, minutes, heures), Heure quotidienne | - |
| **Google Gmail** | Nouvel email, Email d'un expéditeur, Email avec label | Envoyer email, Transférer, Marquer comme lu |
| **Google Calendar** | Événement créé, Événement à venir | Créer événement, Supprimer événement |
| **Google Drive** | Fichier uploadé, Fichier partagé | Sauvegarder fichier, Créer dossier, Partager |

### ✅ Architecture REST

- 🔐 Authentification Token + OAuth2
- 📡 API REST complète (Django REST Framework)
- 🌍 CORS configuré
- 📱 Support Web + Mobile

## 📋 Prérequis

- Docker & Docker Compose
- Ports : 8080 (API), 8081 (Web), 5432 (PostgreSQL), 6379 (Redis)

## 🏗️ Architecture

```
┌─────────────────────────────────────────────────────────┐
│                  INFRASTRUCTURE                         │
├─────────────┬────────────┬─────────────┬────────────────┤
│ PostgreSQL  │   Redis    │  Celery     │  Celery Beat   │
│ (Database)  │  (Broker)  │  (Worker)   │  (Scheduler)   │
└─────────────┴────────────┴─────────────┴────────────────┘
                     ↕ (Tâches asynchrones)
┌─────────────────────────────────────────────────────────┐
│              DJANGO BACKEND (Port 8080)                 │
│  - REST API (DRF)                                       │
│  - Workflow Executor                                    │
│  - Service Integrations (Google, Timer, etc.)           │
└─────────────────────────────────────────────────────────┘
                     ↕ (HTTP/REST)
┌─────────────────────────────────────────────────────────┐
│           CLIENTS (Web 8081 + Mobile APK)               │
│  - React + TypeScript (Web)                             │
│  - Flutter (Mobile)                                     │
└─────────────────────────────────────────────────────────┘
```

## 🔧 Installation et Démarrage

### Docker Compose (Recommandé)

```bash
# Cloner le projet
git clone <repo-url>
cd G-DEV-500-TLS-5-2-area-5

# Configurer les variables d'environnement
cp backend/.env.example backend/.env
# Éditer backend/.env pour ajouter vos credentials Google OAuth2

# Démarrer tous les services (y compris la compilation de l'APK mobile)
docker-compose up -d --build

# Vérifier que tout fonctionne
docker-compose ps

### 📱 Client Mobile

#### Option A : Via Docker (Génération de l'APK)
L'APK est généré automatiquement lors du `docker-compose up` via le service `client_mobile`.
Il est accessible dans le volume partagé ou peut être copié depuis le conteneur :
```bash
docker cp $(docker-compose ps -q client_mobile):/apk/client.apk ./client.apk
```

#### Option B : Développement Local (Flutter)
Pour lancer l'application mobile en mode développement sur votre machine (nécessite Flutter SDK) :

1. Aller dans le dossier mobile
```bash
cd mobile
```

2. Installer les dépendances
```bash
flutter pub get
```

3. (Optionnel) Lancer un émulateur Android
Si vous n'avez pas de device physique, vous pouvez lancer un émulateur :
```bash
# Lister les émulateurs disponibles
flutter emulators

# Lancer un émulateur spécifique
flutter emulators --launch Medium_Phone_API_36.1
```

4. Lancer sur un émulateur ou device connecté
```bash
# Lancer sur l'émulateur par défaut
flutter run

# Lancer sur le web (pour debug rapide)
flutter run -d chrome

# Générer l'APK manuellement
flutter build apk --release
```

> **Note**: L'application est configurée pour se connecter à `http://10.0.2.2:8080` (alias localhost pour l'émulateur Android).

Pour plus de détails, voir [DOCUMENTATION_MOBILE.md](documentation/DOCUMENTATION_MOBILE.md).
```

## 📖 Documentation

- 🔧 [**HOOK_SYSTEM.md**](documentation/HOOK_SYSTEM.md) - Documentation complète du système de hook
- 🏗️ [**AREA_API_documentation.md**](documentation/AREA_API_documentation.md) - Documentation de l'API
- 🎨 [**DOCUMENTATION_FRONTEND.md**](documentation/DOCUMENTATION_FRONTEND.md) - Architecture du frontend
- 🔄 [**Sequence_Diagram.md**](documentation/Sequence_Diagram.md) - Diagrammes de séquence

## 🎯 Exemples d'Utilisation

### 1. Notification Email Périodique (Sans OAuth)

```json
{
  "name": "Rappel toutes les heures",
  "action_service": "timer",
  "action_type": "every_x_hours",
  "reaction_service": "email",
  "reaction_type": "send_notification_email",
  "action_config": {"hours": 1},
  "reaction_config": {}
}
```

### 2. Email → Calendar Event (Nécessite Google)

```json
{
  "name": "Email du boss → Calendar",
  "action_service": "google",
  "action_type": "new_email_from_sender",
  "reaction_service": "google",
  "reaction_type": "create_calendar_event",
  "action_config": {"sender_email": "boss@company.com"},
  "reaction_config": {
    "title": "Important Email",
    "duration": 30
  }
}
```

## 📊 Monitoring et Debugging

### Endpoints de Monitoring

- `GET /api/executor/` - Statut du workflow executor
- `GET /api/health/celery/` - Santé de Celery
- `GET /api/executions/` - Historique d'exécution
- `GET /api/about.json` - Liste des services disponibles

## 🛠️ Commandes Utiles

```bash
# Démarrage
make build          # Construire les images
make up             # Démarrer les services
make down           # Arrêter les services

# Logs
make logs           # Tous les logs
make celery-logs    # Logs Celery Worker + Beat
make logs-api       # Logs Django Server

# Tests
make hook-test      # Tester le système de hook

# Maintenance
make restart        # Redémarrer tous les services
make rebuild        # Rebuild complet
make clean          # Nettoyer volumes et containers
```

## 🔐 Configuration OAuth2

### Google OAuth2

1. Aller sur https://console.cloud.google.com/
2. Créer un projet
3. Activer les APIs : Gmail, Calendar, Drive
4. Créer des credentials OAuth 2.0
5. Ajouter dans `backend/.env` :

```env
GOOGLE_OAUTH2_CLIENT_ID=your-client-id
GOOGLE_OAUTH2_CLIENT_SECRET=your-client-secret
```

## 📄 License

Ce projet est développé dans un cadre éducatif au sein d'Epitech (TEK3).

## 👥 Équipe

Projet AREA - G-DEV-500-TLS-5-2 - Marceau / Andi / Fabien / Evan

---
