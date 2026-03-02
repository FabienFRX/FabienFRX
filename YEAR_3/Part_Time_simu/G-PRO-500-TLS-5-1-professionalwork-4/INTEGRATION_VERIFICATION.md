# Vérification d'Intégration - Daily Challenge & Messages

## 📋 Changements Effectués

### ✅ Backend

#### 1. **jwt.strategy.ts** - Ajout de `avatarUrl`
- **Fichier**: `backend/api/src/auth/jwt.strategy.ts`
- **Changement**: Ajout de `avatarUrl: true` dans le select
- **Impact**: L'endpoint `/api/auth/me` retourne maintenant l'avatar utilisateur
- **Raison**: Nécessaire pour afficher les avatars dans la messagerie

### ✅ Frontend

#### 1. **auth.service.ts** - Correction des types
- **Fichier**: `frontend/src/services/auth.service.ts`
- **Changement**: 
  - `id: number` → `id: string` (compatibilité UUID PostgreSQL)
  - Ajout de `avatarUrl?: string` dans interface `UserInfo`
- **Impact**: Compatible avec les UUID du backend
- **Raison**: Éviter les erreurs de type entre frontend/backend

#### 2. **messages.service.ts** - Configuration API centralisée
- **Fichier**: `frontend/src/services/messages.service.ts`
- **Changement**: Utilisation de `API_BASE_URL` depuis `api.config.ts`
- **Avant**: `const API_URL = import.meta.env.VITE_API_URL || 'http://localhost:3000/api'`
- **Après**: `const API_URL = \`${API_BASE_URL}/api\``
- **Impact**: Configuration API cohérente à travers l'application
- **Raison**: Éviter la duplication et les erreurs de configuration

## 🔍 Points de Vérification

### 1. **Système de Messagerie** 

#### Backend (NestJS)
- [ ] Le backend démarre sans erreur
- [ ] Les endpoints sont accessibles:
  - `GET /api/messages/conversations` - Liste des conversations
  - `POST /api/messages` - Envoi de message
  - `GET /api/messages/conversations/:id` - Messages d'une conversation
  - `PATCH /api/messages/conversations/:id/read` - Marquer comme lu
  - `DELETE /api/messages/conversations/:id` - Supprimer conversation
  - `GET /api/messages/search?query=xxx` - Recherche utilisateurs
  - `POST /api/messages/conversations` - Créer conversation
- [ ] WebSocket Gateway fonctionne sur namespace `/messages`
- [ ] CORS configuré pour localhost:8080, 8081, 8082, 5173

#### Frontend (React)
- [ ] Page `/messages` se charge correctement
- [ ] Liste des conversations s'affiche
- [ ] Envoi de message fonctionne (HTTP et WebSocket)
- [ ] Réception temps réel fonctionne
- [ ] Indicateur "typing..." s'affiche
- [ ] Avatars s'affichent correctement
- [ ] Recherche d'utilisateurs fonctionne
- [ ] Suppression de conversation fonctionne

#### Base de Données
- [ ] Migration `20251211124823_add_messaging_system` appliquée
- [ ] Tables créées:
  - `Conversation`
  - `ConversationParticipant`
  - `Message`
- [ ] Relations correctes entre User et Messages

### 2. **Daily Challenge**

#### Backend
- [ ] Endpoints fonctionnels:
  - `GET /api/daily-challenge/current` - Challenge du jour
  - `POST /api/daily-challenge/submit` - Soumettre réponses
  - `GET /api/daily-challenge/status` - Statut utilisateur
  - `GET /api/daily-challenge/history` - Historique
  - `GET /api/daily-challenge/leaderboard` - Classement

#### Frontend
- [ ] Page `/daily-challenge` affiche le Header/Footer
- [ ] Timer compte à rebours correctement (format: "Xh Ym Zs")
- [ ] Timer se met à jour toutes les secondes
- [ ] Réinitialisation à minuit fonctionne
- [ ] Card sur la page d'accueil affiche le bon timer
- [ ] Page `/daily-challenge/history` affiche le Header/Footer
- [ ] Navigation cohérente entre les pages

### 3. **Intégration Friends → Messages**

- [ ] Page `/friends` charge les vrais utilisateurs depuis `usersService.list()`
- [ ] Bouton "Message" crée une conversation
- [ ] Redirection vers `/messages` avec conversation ouverte
- [ ] Avatar s'affiche dans la liste des amis

## 🧪 Tests à Effectuer

### Test 1: Authentification
```bash
# 1. Démarrer le backend
cd backend/api
npm run start:dev

# 2. Démarrer le frontend
cd frontend
npm run dev

# 3. Se connecter avec un utilisateur
# 4. Vérifier que l'avatar s'affiche dans le Header
```

### Test 2: Messagerie
```bash
# 1. Aller sur /friends
# 2. Cliquer sur "Message" pour un ami
# 3. Vérifier la redirection vers /messages
# 4. Envoyer un message
# 5. Vérifier la réception temps réel (ouvrir dans 2 onglets)
# 6. Tester l'indicateur "typing..."
```

### Test 3: Daily Challenge Timer
```bash
# 1. Aller sur la page d'accueil
# 2. Vérifier que le timer du Daily Challenge compte à rebours
# 3. Aller sur /daily-challenge
# 4. Vérifier que le timer est synchronisé
# 5. Attendre 5 secondes, vérifier la mise à jour
```

## 🔧 Configuration Requise

### Variables d'Environnement

#### Backend `.env`
```env
DATABASE_URL="postgresql://user:password@localhost:5432/dbname"
JWT_SECRET="votre-secret-jwt"
PORT=3000
```

#### Frontend `.env`
```env
VITE_API_URL=http://localhost:3000
```

### Ports Utilisés
- **Backend**: 3000
- **Frontend**: 5173 (ou 8080, 8081, 8082)
- **PostgreSQL**: 5432
- **WebSocket**: 3000 (namespace `/messages`)

## 📊 Structure des Données

### UserInfo (Frontend)
```typescript
interface UserInfo {
  id: string;          // ✅ UUID compatible
  email: string;
  username: string;
  displayName?: string;
  avatarUrl?: string;  // ✅ Nouveau champ
  roles: string[];
}
```

### JWT Payload (Backend)
```typescript
{
  sub: string;        // User ID (UUID)
  roles: string[];
  email: string;
  username: string;
  displayName: string;
  avatarUrl: string;  // ✅ Maintenant inclus
}
```

## 🚀 Commandes de Test

```bash
# Rebuild backend après changements
cd backend/api
npm run build
npm run start:dev

# Rebuild frontend
cd frontend
npm run build
npm run dev

# Vérifier les migrations Prisma
cd backend
npx prisma migrate status
npx prisma studio  # Pour inspecter la DB
```

## ✅ Checklist Finale

- [x] Types frontend/backend compatibles (UUID string)
- [x] API configuration centralisée
- [x] avatarUrl ajouté dans JWT strategy
- [x] Messages service utilise API_BASE_URL
- [x] Daily Challenge service utilise API_BASE_URL
- [ ] Tests end-to-end de la messagerie
- [ ] Tests du timer Daily Challenge
- [ ] Tests de l'intégration Friends → Messages
- [ ] Vérification WebSocket temps réel
- [ ] Vérification des avatars

## 🐛 Problèmes Potentiels

1. **WebSocket ne se connecte pas**
   - Vérifier CORS dans `messages.gateway.ts`
   - Vérifier que le backend écoute sur le bon port
   - Vérifier `VITE_API_URL` dans `.env`

2. **Avatar ne s'affiche pas**
   - Vérifier que `avatarUrl` est bien dans le select JWT
   - Vérifier que l'utilisateur a un `avatarUrl` en DB
   - Redémarrer le backend après modification

3. **Timer ne compte pas**
   - Vérifier que `resetTime` est en millisecondes
   - Vérifier le useEffect dans `DailyChallengeCard.tsx`
   - Vérifier le calcul de minuit dans `DailyChallenge.tsx`

4. **Messages ne s'envoient pas**
   - Vérifier le token dans localStorage
   - Vérifier les headers Authorization
   - Vérifier les logs backend pour les erreurs JWT
