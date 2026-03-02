# 🧠 QuizLab

**QuizLab** est une plateforme d'entraînement à la culture générale avec quiz solo et multijoueur. Ce projet propose une API REST NestJS (backend), une interface React/Vite (frontend) et une base PostgreSQL.

---

## 🚀 Démarrage rapide

### Mode Production (Utilisateurs finaux)

Pour lancer l'application complète (DB + Backend + Frontend) en production :

```bash
# Cloner le repository
git clone <YOUR_GIT_URL>
cd QuizLab

# Lancer tous les services avec Docker
docker-compose up -d --build
```

**Ce qui se passe :**
- 🗄️ PostgreSQL démarre sur le port interne (pas exposé)
- 🔧 Backend (NestJS) build, migrations automatiques, disponible sur `http://localhost:3000`
- 🎨 Frontend (React+Vite) build optimisé avec Nginx, disponible sur `http://localhost:8080`

**Pour arrêter :**
```bash
docker-compose down
```

**Pour supprimer les volumes (données DB) :**
```bash
docker-compose down -v
```

---

### Mode Développement (Contributeurs)

Pour travailler sur le code avec hot-reload :

```bash
# Cloner le repository
git clone <YOUR_GIT_URL>
cd QuizLab

# Lancer tous les services en mode dev
docker-compose -f docker-compose.dev.yml up -d --build
```

**Ce qui se passe :**
- 🗄️ PostgreSQL disponible sur `localhost:5434`
- 🔍 pgAdmin disponible sur `http://localhost:5051` (admin@example.com / admin)
- 🔧 Backend avec hot-reload (NestJS watch mode) sur `http://localhost:3000`
- 🎨 Frontend avec hot-reload (Vite HMR) sur `http://localhost:8081`
- 📂 Vos modifications locales sont synchronisées dans les conteneurs

**Logs en temps réel :**
```bash
docker-compose -f docker-compose.dev.yml logs -f
```

**Rebuild sans cache (si nécessaire) :**
```bash
docker-compose -f docker-compose.dev.yml up -d --build --force-recreate
```

**Arrêter :**
```bash
docker-compose -f docker-compose.dev.yml down
```

---

## 🛠️ Installation manuelle (sans Docker)

Si vous préférez lancer les services individuellement :

### Backend

```bash
cd backend

# Installer les dépendances
npm install

# Créer le fichier .env
cat > .env << EOF
DATABASE_URL="postgresql://user:user@localhost:5433/quizlab?schema=public"
JWT_SECRET="dev-secret-key-not-for-production"
PORT=3000
NODE_ENV=development
EOF

# Lancer PostgreSQL uniquement
docker compose up -d

# Générer Prisma Client et appliquer les migrations
npx prisma generate
npx prisma migrate deploy

# Lancer le backend
cd api
npm run start:dev
```

### Frontend

```bash
cd frontend

# Installer les dépendances
npm install

# Lancer le serveur de développement
npm run dev
```

---

## ⚙️ Technologies principales

### Backend
| Outil / Librairie | Rôle |
|--------------------|------|
| **NestJS** | Framework Node.js modulaire et typé |
| **Prisma ORM** | Gestion des données et migrations PostgreSQL |
| **PostgreSQL** | Base de données relationnelle |
| **JWT** | Authentification sécurisée |
| **bcrypt** | Hash des mots de passe |
| **class-validator** | Validation des entrées utilisateurs |

### Frontend
| Outil / Librairie | Rôle |
|--------------------|------|
| **Vite** | Build tool ultra-rapide |
| **React** | Bibliothèque UI |
| **TypeScript** | Typage statique |
| **shadcn-ui** | Composants UI modernes |
| **Tailwind CSS** | Framework CSS utility-first |

---

## 📁 Structure du projet

```
QuizLab/
├── docker-compose.yml          # Configuration production
├── docker-compose.dev.yml      # Configuration développement
├── backend/
│   ├── Dockerfile              # Build production backend
│   ├── Dockerfile.dev          # Build développement backend
│   ├── api/                    # Code source NestJS
│   ├── prisma/                 # Schéma et migrations
│   └── docker-compose.yml      # DB locale (legacy)
└── frontend/
    ├── Dockerfile              # Build production frontend
    ├── Dockerfile.dev          # Build développement frontend
    ├── nginx.conf              # Config Nginx pour production
    └── src/                    # Code source React
```

---

## 🔑 Variables d'environnement

### Backend (.env dans backend/)
```dotenv
DATABASE_URL="postgresql://user:user@localhost:5433/quizlab?schema=public"
JWT_SECRET="change-me-in-production"
PORT=3000
NODE_ENV=development
```

### Docker Compose (production)
Les variables sont définies directement dans `docker-compose.yml` :
- `DATABASE_URL` : connexion interne entre conteneurs
- `JWT_SECRET` : **À changer en production !**

---

## 🧱 Commandes Prisma utiles

```bash
# Générer le client Prisma
npx prisma generate

# Créer une nouvelle migration
npx prisma migrate dev --name nom_de_la_migration

# Appliquer les migrations (production)
npx prisma migrate deploy

# Ouvrir Prisma Studio (interface graphique)
npx prisma studio

# Seed la base de données
npm run db:seed
```

---

## 🔐 Endpoints principaux

| Méthode | Route | Description |
|----------|--------|-------------|
| `POST /auth/register` | Crée un compte utilisateur |
| `POST /auth/login` | Connecte un utilisateur et renvoie un JWT |
| `GET /users` | Liste tous les utilisateurs (JWT requis) |
| `GET /users/:id` | Récupère un utilisateur spécifique |
| `PATCH /users/:id` | Met à jour un utilisateur |
| `PATCH /users/:id/password` | Change le mot de passe |
| `DELETE /users/:id` | Supprime (soft delete) un utilisateur |

---

## 🧪 Exemple rapide (via `curl`)

### Enregistrement
```bash
curl -X POST http://localhost:3000/auth/register \
  -H "Content-Type: application/json" \
  -d '{"email":"test@example.com","username":"testuser","password":"password123"}'
```

### Connexion
```bash
curl -X POST http://localhost:3000/auth/login \
  -H "Content-Type: application/json" \
  -d '{"email":"test@example.com","password":"password123"}'
```

---

## 🎯 Fonctionnalités

- ✅ Authentification JWT avec rôles (USER, ADMIN)
- ✅ Gestion des utilisateurs (CRUD, soft delete)
- ✅ Préférences d'apprentissage (tranche d'âge, niveau, thèmes favoris)
- ✅ Panel admin pour gérer les utilisateurs
- ✅ Sélection d'avatars personnalisés
- ✅ Changement de mot de passe sécurisé

---

## 👤 Créer un compte administrateur

```bash
# Se connecter au container backend en mode dev
docker-compose -f docker-compose.dev.yml exec backend sh

# Exécuter le script
npx ts-node scripts/set-admin-role.ts your_admin@example.com
```

Ou en mode manuel :
```bash
cd backend
npx ts-node scripts/set-admin-role.ts admin@quizlab.com
```

---

## 🐛 Dépannage

### Les conteneurs ne démarrent pas
```bash
# Vérifier les logs
docker-compose logs -f

# En mode dev
docker-compose -f docker-compose.dev.yml logs -f
```

### Erreur de connexion à la base de données
```bash
# Vérifier que PostgreSQL est bien démarré
docker ps

# Redémarrer la DB
docker-compose restart db
```

### Le backend ne trouve pas Prisma Client
```bash
# Régénérer le client
docker-compose exec backend npx prisma generate

# Ou reconstruire l'image
docker-compose up -d --build backend
```

### Hot reload ne fonctionne pas en mode dev
```bash
# Vérifier que les volumes sont bien montés
docker-compose -f docker-compose.dev.yml config

# Forcer la reconstruction
docker-compose -f docker-compose.dev.yml up -d --build --force-recreate
```

---

## 📝 Ports utilisés

| Service | Mode Production | Mode Développement |
|---------|----------------|-------------------|
| Frontend | 8080 | 8081 |
| Backend | 3000 | 3000 |
| PostgreSQL | N/A (interne) | 5434 |
| pgAdmin | N/A | 5051 |

---

## ✅ Prêt pour la production

Le mode production utilise :
- Images Docker optimisées multi-stage
- Nginx pour servir le frontend (cache, compression, routing SPA)
- Migrations automatiques au démarrage du backend
- Variables d'environnement sécurisées

**N'oubliez pas de changer `JWT_SECRET` en production !**

---

**Projet prêt à être déployé et développé.** 🚀
