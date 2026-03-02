# 📱 AREA Mobile Client (Flutter - Android Only)

Client mobile Flutter pour le projet AREA (Action → REAction).
**Application Android uniquement** - Les plateformes iOS, Web, Windows, Linux et macOS ont été supprimées.

## 🎯 Objectif

Interface mobile Flutter qui communique avec le serveur AREA via REST API.
L'application ne contient **aucune logique métier** - elle sert uniquement d'interface utilisateur.

## 🏗️ Architecture

```
client_mobile/
├── lib/
│   ├── main.dart                 # Point d'entrée + configuration
│   ├── models/                   # Modèles de données
│   │   ├── area.dart
│   │   └── service.dart
│   ├── providers/                # State management (Provider)
│   │   ├── auth_provider.dart
│   │   ├── server_provider.dart
│   │   └── area_provider.dart
│   ├── screens/                  # Écrans de l'application
│   │   ├── splash_screen.dart
│   │   ├── settings_screen.dart
│   │   ├── login_screen.dart
│   │   ├── register_screen.dart
│   │   ├── home_screen.dart
│   │   ├── services_screen.dart
│   │   ├── area_builder_screen.dart
│   │   └── my_areas_screen.dart
│   └── services/                 # API REST
│       └── api_service.dart
├── android/                      # Configuration Android
└── Dockerfile                    # Build APK dans Docker
```

## 📦 Fonctionnalités

- ✅ **Configuration serveur** - Définir l'URL du serveur (http://localhost:8080)
- ✅ **Authentification** - Login / Register avec le serveur
- ✅ **Gestion des services** - Affichage des services disponibles
- ✅ **Création d'AREA** - Builder avec actions et réactions
- ✅ **Mes AREAs** - Liste, activation/désactivation, suppression
- ✅ **Material Design 3** - Interface moderne et responsive

## 🚀 Installation et lancement

### Option 1 : Développement local

```bash
# Installer les dépendances
flutter pub get

# Lancer sur un émulateur Android ou device connecté
flutter run

# Ou builder l'APK
flutter build apk --release
# APK disponible dans: build/app/outputs/flutter-apk/app-release.apk
```

### Option 2 : Docker (Production)

```bash
# Builder l'image Docker
docker build -t area-mobile-client .

# Lancer avec volume partagé pour récupérer l'APK
docker run --rm -v $(pwd)/output:/apk area-mobile-client

# L'APK sera dans ./output/client.apk
```

### Option 3 : Docker Compose (Intégration complète)

Voir [`docker-compose.example.yml`](docker-compose.example.yml ) pour l'intégration avec le serveur et le client web.

```yaml
services:
  client_mobile:
    build: ./client_mobile
    volumes:
      - apk-volume:/apk
    depends_on:
      - server
```

## 🔧 Configuration

### 1. Configuration du serveur
Au premier lancement, l'app demande l'URL du serveur :
```
http://localhost:8080
ou
http://10.0.2.2:8080  (depuis émulateur Android)
```

### 2. Création de compte / Connexion
- Créer un compte avec email/password
- Ou se connecter avec un compte existant

### 3. Utilisation
- Parcourir les services disponibles
- Créer des AREAs (SI → ALORS)
- Gérer ses automations

## 📡 API REST utilisées

L'application communique avec le serveur via :

```dart
GET  /about.json              # Info serveur + services
POST /auth/register           # Inscription
POST /auth/login              # Connexion
GET  /services                # Liste des services
GET  /areas                   # Liste des AREAs de l'utilisateur
POST /areas                   # Créer une AREA
PATCH /areas/:id/toggle       # Activer/Désactiver
DELETE /areas/:id             # Supprimer
```

## 📱 Plateformes supportées

- ✅ **Android** uniquement (API 21+)
- ❌ iOS - Supprimé
- ❌ Web - Supprimé
- ❌ Windows - Supprimé
- ❌ Linux - Supprimé
- ❌ macOS - Supprimé

## 🎨 Technologies

- **Flutter** 3.0+
- **Dart** 3.0+
- **Provider** - State management
- **http** - API REST calls
- **shared_preferences** - Stockage local
- **Material Design 3** - UI/UX

## 📄 Conformité PDF AREA

Ce client mobile respecte les exigences du PDF G-DEV-500_AREA.pdf :
- ✅ Client Android
- ✅ Interface utilisateur uniquement (pas de logique métier)
- ✅ Communication REST API avec le serveur
- ✅ Configuration de l'URL du serveur
- ✅ Distribution via Docker (APK exposé sur /client.apk)

## 🛠️ Développement

```bash
# Analyser le code
flutter analyze

# Formater le code
flutter format lib/

# Tester
flutter test

# Build APK debug
flutter build apk --debug

# Build APK release
flutter build apk --release
```
