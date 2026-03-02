# 🚀 AREA - Action REAction Platform

Plateforme d'automatisation type IFTTT/Zapier avec client web et mobile.

**MVP fonctionnel respectant les exigences de défense :**
- ✅ Connexion/Inscription classique (MUST)
- ✅ OAuth2 Google (MUST)
- ✅ Workflow fonctionnel Action → REAction (MUST)
- ✅ Dockerisation complète + APK (SHOULD)
- ⚠️ Tests automatisés (SHOULD - en cours)

## 📦 Architecture

```
├── backend/        # Django REST API (port 8082)
│   ├── api/        # Models, Views, Serializers
│   ├── OAuth2      # Google authentication
│   └── PostgreSQL  # Database
├── web/            # React + TypeScript (port 8083)
│   ├── Login/Register
│   ├── Services (OAuth2)
│   ├── AREA Builder
│   └── My AREAs
├── mobile/         # Flutter Android APK
│   ├── 8 screens (Login, Builder, AREAs...)
│   ├── State management (Provider)
│   └── API integration
└── docker-compose.yml  # 4 services orchestration
```

## 🐳 Lancement via Docker (Production)

```bash
# Build tous les services (backend, web, mobile)
docker-compose build

# Démarrer tous les services
docker-compose up

# En arrière-plan
docker-compose up -d
```

### Services disponibles :
- **Backend API** : http://localhost:8082
- **Client Web** : http://localhost:8083
- **APK Mobile** : http://localhost:8083/apk/client.apk

## 🛠️ Développement

### Web (dev)
```bash
cd web
npm install
npm run dev
# → http://localhost:5173
```

### Mobile (dev)
```bash
cd mobile
flutter pub get
flutter run
# Ou build APK : flutter build apk --release
```

### Backend (dev)
```bash
cd backend
# Suivre les instructions du README backend
```

## 📱 Architecture Mobile APK

Le client mobile Flutter est buildé dans Docker et l'APK est exposé via le client web :

1. **`client_mobile`** service :
   - Build l'APK Flutter Android
   - Copie l'APK dans le volume partagé `/apk/client.apk`

2. **`client_web`** service :
   - Monte le volume partagé en lecture seule
   - Expose l'APK sur `http://localhost:8083/apk/client.apk`

### Télécharger l'APK

```bash
# Via navigateur
open http://localhost:8083/apk/client.apk

# Via curl
curl -O http://localhost:8083/apk/client.apk
```

## 🔧 Configuration

### Variables d'environnement (backend)
```
DB_HOST=database
DB_PORT=5432
DB_NAME=area_db
DB_USER=area_user
DB_PASSWORD=area_password
```

### Ports utilisés
- `5433` : PostgreSQL
- `8082` : Backend API
- `8083` : Client Web + APK mobile

## 📚 Documentation

Voir les README spécifiques :
- [Backend](./backend/README.md)
- [Web](./web/README.md)
- [Mobile](./mobile/README.md)