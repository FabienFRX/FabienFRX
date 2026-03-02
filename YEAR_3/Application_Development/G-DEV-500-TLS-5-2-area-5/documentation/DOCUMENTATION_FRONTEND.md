# Documentation Frontend - AREA Application

## 📚 Table des matières

1. [Vue d'ensemble](#vue-densemble)
2. [Architecture générale](#architecture-générale)
3. [Stack technique](#stack-technique)
4. [Structure du projet](#structure-du-projet)
5. [Composants principaux](#composants-principaux)
6. [Gestion de l'état](#gestion-de-létat)
7. [Authentification](#authentification)
8. [Routing et navigation](#routing-et-navigation)
9. [Communication avec le backend](#communication-avec-le-backend)
10. [Styling et UI](#styling-et-ui)
11. [Configuration et variables d'environnement](#configuration-et-variables-denvironnement)
12. [Workflow de développement](#workflow-de-développement)

---

## Vue d'ensemble

### Qu'est-ce que cette application ?

AREA est une plateforme d'automatisation (type IFTTT/Zapier) qui permet aux utilisateurs de créer des workflows automatisés appelés **AREAs** :
- **ACTION** = Déclencheur (ex: "nouveau event ajouté au calendrier")
- **REACTION** = Réponse automatique (ex: "Envoyer un email")

### Objectif du frontend

Le frontend est une **Single Page Application (SPA)** qui permet de :
1. S'authentifier (login classique ou Google OAuth2)
2. Connecter des services externes (Google, etc.)
3. Créer et gérer des AREAs
4. Visualiser un dashboard avec les statistiques

---

## Architecture générale

### Pourquoi une SPA ?

Une Single Page Application charge une seule page HTML et met à jour dynamiquement le contenu sans recharger la page entière. 

**Avantages :**
- Navigation ultra-rapide (pas de rechargement complet)
- Expérience utilisateur fluide
- Séparation claire frontend/backend (architecture moderne)

**Comment ça marche :**
```
Utilisateur clique → React Router intercepte → 
Change les composants affichés → Pas de requête serveur
```

### Architecture Client-Serveur

```
┌─────────────────────────────────────────┐
│           NAVIGATEUR (Client)           │
│  ┌───────────────────────────────────┐  │
│  │     React Application (SPA)       │  │
│  │  - Gère l'interface utilisateur   │  │
│  │  - Stocke le token d'auth         │  │
│  │  - Fait des appels API REST       │  │
│  └───────────────────────────────────┘  │
└─────────────────────────────────────────┘
              ↕ HTTP/REST API
┌─────────────────────────────────────────┐
│          SERVEUR (Backend)              │
│  ┌───────────────────────────────────┐  │
│  │     Django API (Python)           │  │
│  │  - Authentification (Token)       │  │
│  │  - Gestion des services OAuth     │  │
│  │  - CRUD des AREAs                 │  │
│  │  - Base de données PostgreSQL     │  │
│  └───────────────────────────────────┘  │
└─────────────────────────────────────────┘
```

---

## Stack technique

### Technologies principales

#### 1. **React 18** - Bibliothèque UI
**Pourquoi React ?**
- Composants réutilisables
- État réactif (quand les données changent, l'UI se met à jour automatiquement)
- Écosystème riche
- Performance optimale avec le Virtual DOM

**Concepts clés :**
```typescript
// Composant fonctionnel avec hooks
const MonComposant: React.FC = () => {
  // useState = état local du composant
  const [count, setCount] = useState(0);
  
  // useEffect = effet de bord (comme charger des données)
  useEffect(() => {
    console.log('Composant monté');
  }, []); // [] = exécuté une seule fois
  
  return <div>Count: {count}</div>;
};
```

#### 2. **TypeScript** - Langage de programmation
**Pourquoi TypeScript au lieu de JavaScript ?**
- **Typage statique** = moins d'erreurs à l'exécution
- **Autocomplétion** meilleure dans l'IDE
- **Documentation intégrée** via les types

**Exemple :**
```typescript
// Sans TypeScript (JavaScript)
const user = { name: "John" };
user.age = 30; // Pas d'erreur, mais age n'était pas prévu

// Avec TypeScript
interface User {
  name: string;
  email: string;
}
const user: User = { name: "John" }; // ❌ Erreur: email manquant
```

#### 3. **Vite 5** - Build tool
**Pourquoi Vite ?**
- **Hot Module Replacement (HMR)** ultra-rapide
  - Tu modifies le code → Changement instantané dans le navigateur
  - Pas besoin de recharger toute la page
- Build de production optimisé
- Configuration minimale

**Comment ça marche :**
```bash
npm run dev     # Lance le serveur de dev (port 5173)
npm run build   # Compile pour la production (dossier dist/)
```

#### 4. **React Router DOM** - Navigation
**Pourquoi React Router ?**
- Gère la navigation côté client (sans recharger la page)
- URLs propres et intuitives
- Protection des routes (authentification)

**Exemple :**
```typescript
<Routes>
  <Route path="/login" element={<LoginPage />} />
  <Route path="/dashboard" element={
    <ProtectedRoute>  {/* Vérifie si l'user est connecté */}
      <Dashboard />
    </ProtectedRoute>
  } />
</Routes>
```

#### 5. **Tailwind CSS** - Styling
**Pourquoi Tailwind ?**
- **Utility-first** = classes CSS pré-définies
- Pas besoin d'écrire du CSS custom
- Design cohérent et rapide

**Exemple :**
```tsx
// Avant (CSS traditionnel)
<div className="my-button">Click me</div>
.my-button { padding: 8px; background: blue; color: white; }

// Avec Tailwind
<div className="px-2 py-2 bg-blue-600 text-white rounded">
  Click me
</div>
```

#### 6. **@react-oauth/google** - OAuth2 Google
**Pourquoi cette lib ?**
- Simplifie l'intégration OAuth2 Google
- Gère automatiquement le popup de connexion
- Retourne directement les tokens

---

## Structure du projet

```
web/
├── src/
│   ├── app/
│   │   └── App.tsx              # Point d'entrée, définit les routes
│   ├── pages/
│   │   ├── LoginPage.tsx        # Page de connexion
│   │   ├── RegisterPage.tsx     # Page d'inscription
│   │   ├── Dashboard.tsx        # Tableau de bord
│   │   ├── ServicesPage.tsx     # Connexion des services
│   │   └── AREABuilderPage.tsx  # Création d'AREAs
│   ├── components/
│   │   ├── Header.tsx           # En-tête avec logout
│   │   └── ProtectedRoute.tsx   # Route protégée par auth
│   ├── contexts/
│   │   └── AuthContext.tsx      # État global d'authentification
│   ├── services/
│   │   └── api.ts               # Fonctions d'appel API
│   ├── config/
│   │   └── constants.ts         # Variables de configuration
│   └── styles/
│       └── index.css            # Styles globaux + Tailwind
├── public/                       # Fichiers statiques
├── index.html                    # Point d'entrée HTML
├── package.json                  # Dépendances npm
├── vite.config.ts               # Config Vite
├── tailwind.config.js           # Config Tailwind
└── tsconfig.json                # Config TypeScript
```

### Pourquoi cette structure ?

#### `/pages` vs `/components`
- **Pages** = Composants associés à une route (URL)
  - Exemple : `/dashboard` → `Dashboard.tsx`
- **Components** = Composants réutilisables partout
  - Exemple : `Header.tsx` utilisé dans toutes les pages

#### `/contexts`
- Contient l'état global partagé entre tous les composants
- Alternative simple à Redux pour les petits projets

#### `/services`
- Logique de communication avec le backend centralisée
- Facilite les tests et la maintenance

---

## Composants principaux

### 1. App.tsx - Le Chef d'orchestre

**Rôle :** Point d'entrée de l'application, définit toutes les routes.

**Pourquoi ce fichier est important :**
```tsx
function App() {
  return (
    <GoogleOAuthProvider clientId={GOOGLE_CLIENT_ID}>  {/* 1️⃣ */}
      <BrowserRouter>  {/* 2️⃣ */}
        <AuthProvider>  {/* 3️⃣ */}
          <Routes>  {/* 4️⃣ */}
            <Route path="/login" element={<LoginPage />} />
            <Route path="/dashboard" element={
              <ProtectedRoute>  {/* 5️⃣ */}
                <Dashboard />
              </ProtectedRoute>
            } />
          </Routes>
        </AuthProvider>
      </BrowserRouter>
    </GoogleOAuthProvider>
  );
}
```

**Explication des layers :**
1. **GoogleOAuthProvider** = Fournit le contexte OAuth Google à toute l'app
2. **BrowserRouter** = Active la navigation côté client (React Router)
3. **AuthProvider** = Fournit l'état d'authentification global
4. **Routes** = Définit les URLs et leurs composants associés
5. **ProtectedRoute** = Vérifie si l'user est connecté avant d'afficher la page

### 2. AuthContext.tsx - La mémoire de l'authentification

**Rôle :** Stocke l'état d'authentification et le rend accessible partout.

**Pourquoi un Context ?**
Sans Context, il faudrait passer les infos user de composant en composant :
```tsx
<App user={user} token={token}>
  <Header user={user} token={token}>
    <Menu user={user} token={token}>
      <Profile user={user} token={token} />
    </Menu>
  </Header>
</App>
```

Avec Context :
```tsx
// N'importe où dans l'app
const { user, token } = useAuth();  // ✅ Direct !
```

**Comment ça fonctionne :**
```typescript
// 1. Créer le Context
const AuthContext = createContext<AuthContextType>();

// 2. Provider qui fournit les valeurs
export const AuthProvider = ({ children }) => {
  const [user, setUser] = useState<User | null>(null);
  const [token, setToken] = useState<string | null>(null);
  
  // Persistance dans localStorage
  useEffect(() => {
    const storedToken = localStorage.getItem('auth_token');
    if (storedToken) {
      setToken(storedToken);
      // Récupérer aussi l'user depuis localStorage
    }
  }, []);
  
  const login = async (email: string, password: string) => {
    const response = await authAPI.login({ email, password });
    setUser(response.user);
    setToken(response.token);
    localStorage.setItem('auth_token', response.token);
  };
  
  return (
    <AuthContext.Provider value={{ user, token, login, ... }}>
      {children}
    </AuthContext.Provider>
  );
};

// 3. Hook personnalisé pour utiliser le Context
export const useAuth = () => {
  const context = useContext(AuthContext);
  if (!context) throw new Error('useAuth doit être dans AuthProvider');
  return context;
};
```

**Pourquoi localStorage ?**
- Persiste les données même si l'user ferme le navigateur
- Évite de redemander la connexion à chaque visite

### 3. ProtectedRoute.tsx - Le gardien

**Rôle :** Empêche l'accès aux pages si l'user n'est pas connecté.

**Comment ça marche :**
```typescript
const ProtectedRoute = ({ children }) => {
  const { isAuthenticated, isLoading } = useAuth();
  
  // Pendant le chargement
  if (isLoading) {
    return <div>Loading...</div>;
  }
  
  // Si pas connecté → redirection vers login
  if (!isAuthenticated) {
    return <Navigate to="/login" replace />;
  }
  
  // Si connecté → afficher la page demandée
  return <>{children}</>;
};
```

**Pourquoi c'est important :**
Sans ça, n'importe qui pourrait accéder à `/dashboard` en tapant l'URL directement.

### 4. LoginPage.tsx - La porte d'entrée

**Rôle :** Authentifier l'utilisateur (email/password ou Google).

**Deux méthodes de connexion :**

#### A. Connexion classique (email/password)
```typescript
const handleSubmit = async (e: React.FormEvent) => {
  e.preventDefault();  // Empêche le rechargement de la page
  
  try {
    await login(formData.email, formData.password);  // Appel API
    navigate('/dashboard');  // Redirection après succès
  } catch (err) {
    setError('Invalid credentials');
  }
};
```

#### B. Connexion Google OAuth2
```typescript
const handleGoogleLogin = useGoogleLogin({
  onSuccess: async (tokenResponse) => {
    // tokenResponse contient l'access_token Google
    await loginWithGoogle(
      tokenResponse.access_token,
      tokenResponse.expires_in
    );
    navigate('/dashboard');
  },
  onError: () => {
    setError('Google login failed');
  },
});
```

**Flux OAuth2 Google :**
```
1. User clique "Sign in with Google"
2. Popup Google s'ouvre
3. User autorise l'application
4. Google retourne un access_token
5. Frontend envoie le token au backend
6. Backend vérifie le token auprès de Google
7. Backend crée/récupère l'user et retourne un token Django
8. Frontend stocke le token Django
9. Redirection vers /dashboard
```

### 5. ServicesPage.tsx - La connexion des services

**Rôle :** Permettre à l'user de connecter des services externes (Google, etc.).

**Architecture de la page :**
```
ServicesPage
├── Message d'info (mode développement)
├── Cartes de services disponibles
│   └── Carte Google
│       ├── Logo et statut (Connected/Not Connected)
│       ├── Description
│       ├── Liste des Actions disponibles
│       ├── Liste des REActions disponibles
│       └── Bouton Connect/Disconnect
└── Liste des services connectés
```

**Pourquoi deux appels API ?**
```typescript
useEffect(() => {
  loadConnectedServices();    // GET /api/auth/services/
  loadAvailableServices();    // GET /api/services/
}, []);
```
- `loadConnectedServices()` = Services que TU as connectés
- `loadAvailableServices()` = Tous les services existants (avec leurs actions/réactions)

**Connexion d'un service :**
```typescript
const handleGoogleConnect = useGoogleLogin({
  onSuccess: async (tokenResponse) => {
    // 1. Récupérer les infos user depuis Google
    const userInfo = await fetch('https://www.googleapis.com/oauth2/v2/userinfo', {
      headers: { Authorization: `Bearer ${tokenResponse.access_token}` }
    });
    
    // 2. Envoyer au backend pour stockage
    await servicesAPI.connectGoogleService(
      token,                           // Token Django
      tokenResponse.access_token,      // Token Google
      userInfo,                        // Infos Google
      tokenResponse.expires_in         // Expiration
    );
    
    // 3. Recharger la liste des services
    loadConnectedServices();
  },
  scope: CURRENT_GOOGLE_SCOPES,  // Permissions demandées
});
```

**Pourquoi stocker le token Google côté backend ?**
- Pour que le backend puisse utiliser l'API Gmail/Calendar plus tard
- L'user n'aura pas besoin de se reconnecter à chaque AREA

### 6. AREABuilderPage.tsx - Le créateur d'automatisations

**Rôle :** Créer des workflows Action → REAction.

**Architecture :**
```
AREABuilderPage
├── Vérification (au moins 1 service connecté)
├── Header avec bouton "Create New AREA"
├── Formulaire de création (si showCreateForm = true)
│   ├── Nom et description
│   ├── Section ACTION
│   │   ├── Sélection du service (Google)
│   │   └── Sélection de l'action (new_email_received, etc.)
│   └── Section REACTION
│       ├── Sélection du service (Google)
│       └── Sélection de la réaction (send_email, etc.)
└── Grille des AREAs existantes
    └── Carte AREA
        ├── Nom et description
        ├── Action → Reaction
        ├── Toggle Active/Inactive
        └── Bouton Delete
```

**Chargement dynamique des actions/réactions :**
```typescript
// Récupère les actions disponibles pour le service sélectionné
const getAvailableActions = (): Action[] => {
  const service = availableServices.find(
    s => s.name === formData.action_service
  );
  return service?.actions || [];
};
```

**Pourquoi dynamique ?**
- Si on ajoute un nouveau service au backend, le frontend l'affiche automatiquement
- Pas besoin de hardcoder la liste des actions

**Création d'une AREA :**
```typescript
const handleCreateArea = async (e: React.FormEvent) => {
  e.preventDefault();
  
  try {
    // Envoie les données au backend
    await areaAPI.createArea(token, {
      name: "Auto-reply emails",
      description: "Répond automatiquement aux emails",
      action_service: "google",
      action_type: "new_email_received",
      reaction_service: "google",
      reaction_type: "send_email"
    });
    
    // Recharge la liste des AREAs
    loadAreas();
    
    // Ferme le formulaire
    setShowCreateForm(false);
  } catch (err) {
    setError('Failed to create AREA');
  }
};
```

---

## Gestion de l'état

### États locaux (useState)

**Quand utiliser :**
- Données spécifiques à UN composant
- Formulaires
- UI states (loading, error, etc.)

**Exemple :**
```typescript
const [formData, setFormData] = useState({
  email: '',
  password: ''
});

const [loading, setLoading] = useState(false);
const [error, setError] = useState<string | null>(null);
```

### État global (Context)

**Quand utiliser :**
- Données nécessaires dans PLUSIEURS composants
- Authentification (user, token)
- Thème (dark mode)

**Pourquoi ne pas mettre TOUT dans le Context ?**
- Performance : quand le Context change, TOUS les composants qui l'utilisent se re-rendent
- Complexité : trop d'états globaux = code difficile à maintenir

### Effets (useEffect)

**Rôle :** Exécuter du code en réponse à des changements.

**Cas d'usage courants :**

#### 1. Chargement initial de données
```typescript
useEffect(() => {
  loadAreas();      // Charger les AREAs
  loadServices();   // Charger les services
}, []);  // [] = exécuté UNE SEULE FOIS au montage
```

#### 2. Réagir aux changements
```typescript
useEffect(() => {
  if (user) {
    console.log(`Bienvenue ${user.email}`);
  }
}, [user]);  // Se déclenche quand 'user' change
```

#### 3. Cleanup (nettoyage)
```typescript
useEffect(() => {
  const timer = setInterval(() => {
    console.log('Tick');
  }, 1000);
  
  // Fonction de cleanup
  return () => {
    clearInterval(timer);  // Arrête le timer quand le composant est démonté
  };
}, []);
```

---

## Authentification

### Flow complet

#### 1. Login classique
```
User entre email/password
      ↓
Frontend → POST /api/auth/login/ { email, password }
      ↓
Backend vérifie les credentials
      ↓
Backend retourne { user, token }
      ↓
Frontend stocke dans:
  - useState (pour l'UI)
  - localStorage (pour la persistance)
      ↓
Redirection vers /dashboard
```

#### 2. Login Google
```
User clique "Sign in with Google"
      ↓
Popup Google s'ouvre
      ↓
User autorise l'app
      ↓
Google retourne access_token
      ↓
Frontend → POST /api/auth/google/login/ { access_token, expires_in }
      ↓
Backend vérifie le token auprès de Google
      ↓
Backend crée/récupère l'user
      ↓
Backend crée un UserService (connexion auto du service Google)
      ↓
Backend retourne { user, token, google_service_connected: true }
      ↓
Frontend stocke le token Django
      ↓
Redirection vers /dashboard
```

### Sécurité

#### Token Storage
**Où stocker le token ?**
- ❌ **Cookies** : Vulnérables aux attaques CSRF
- ✅ **localStorage** : Simple et sécurisé pour notre cas
- ⚠️ **sessionStorage** : Perdu quand l'onglet se ferme

**Notre implémentation :**
```typescript
// Stockage
localStorage.setItem('auth_token', token);
localStorage.setItem('user', JSON.stringify(user));

// Récupération au chargement
const storedToken = localStorage.getItem('auth_token');
const storedUser = localStorage.getItem('user');

// Suppression au logout
localStorage.removeItem('auth_token');
localStorage.removeItem('user');
```

#### Protection des routes
```typescript
// Toutes les routes sensibles sont protégées
<Route path="/dashboard" element={
  <ProtectedRoute>
    <Dashboard />
  </ProtectedRoute>
} />
```

#### Envoi du token dans les requêtes
```typescript
fetch(`${API_BASE_URL}/api/areas/`, {
  headers: {
    'Authorization': `Token ${token}`,  // Format Django Token Auth
    'Content-Type': 'application/json'
  }
});
```

---

## Routing et navigation

### React Router DOM

**Concepts clés :**

#### 1. BrowserRouter
Utilise l'API History HTML5 pour des URLs propres :
- ✅ `/dashboard`
- ❌ `/#/dashboard` (HashRouter - ancien style)

#### 2. Routes et Route
```typescript
<Routes>
  <Route path="/login" element={<LoginPage />} />
  <Route path="/dashboard" element={<Dashboard />} />
</Routes>
```

#### 3. Navigation programmatique
```typescript
import { useNavigate } from 'react-router-dom';

const MyComponent = () => {
  const navigate = useNavigate();
  
  const handleSuccess = () => {
    navigate('/dashboard');  // Redirection
  };
};
```

#### 4. Liens déclaratifs
```typescript
import { Link } from 'react-router-dom';

<Link to="/services" className="btn">
  Go to Services
</Link>
```

**Pourquoi Link au lieu de <a> ?**
- `<a href="/services">` → Rechargement complet de la page ❌
- `<Link to="/services">` → Navigation côté client ✅

### Configuration des redirections

```typescript
// Redirection par défaut
<Route path="/" element={<Navigate to="/dashboard" replace />} />

// 404 - Route non trouvée
<Route path="*" element={<Navigate to="/dashboard" replace />} />
```

---

## Communication avec le backend

### Architecture de api.ts

**Pourquoi centraliser les appels API ?**
- **DRY (Don't Repeat Yourself)** : Pas de duplication
- **Maintenance** : Changer l'URL du backend à UN seul endroit
- **Testabilité** : Facile à mocker pour les tests
- **Typage** : Interfaces TypeScript centralisées

**Structure :**
```typescript
// 1. Configuration de base
const API_BASE_URL = 'http://localhost:8080';

// 2. Interfaces TypeScript
export interface User {
  id: number;
  email: string;
  // ...
}

// 3. Modules API organisés par fonctionnalité
export const authAPI = {
  login: async (data) => { /* ... */ },
  register: async (data) => { /* ... */ }
};

export const servicesAPI = {
  getUserServices: async (token) => { /* ... */ }
};

export const areaAPI = {
  getAreas: async (token) => { /* ... */ },
  createArea: async (token, data) => { /* ... */ }
};
```

### Gestion des erreurs

**Pattern utilisé :**
```typescript
async function callAPI() {
  const response = await fetch(url, options);
  
  // 1. Parser la réponse JSON
  const data = await response.json();
  
  // 2. Vérifier le statut HTTP
  if (!response.ok) {
    throw new Error(data.error || 'Request failed');
  }
  
  // 3. Retourner les données
  return data;
}
```

**Dans le composant :**
```typescript
try {
  const data = await authAPI.login({ email, password });
  // Succès
} catch (err: any) {
  setError(err.message);  // Afficher l'erreur à l'user
}
```

### Authentification des requêtes

**Format Django Token :**
```typescript
headers: {
  'Authorization': `Token ${token}`,  // "Token" est le préfixe Django
  'Content-Type': 'application/json'
}
```

**Pourquoi passer le token dans chaque requête ?**
- Le backend vérifie le token pour identifier l'utilisateur
- Chaque requête est indépendante (stateless)

---

## Styling et UI

### Tailwind CSS

**Philosophie Utility-First :**

Au lieu d'écrire :
```css
/* styles.css */
.card {
  background-color: white;
  border-radius: 8px;
  padding: 16px;
  box-shadow: 0 2px 4px rgba(0,0,0,0.1);
}
```

Tu utilises directement les classes Tailwind :
```tsx
<div className="bg-white rounded-lg p-4 shadow-md">
  {/* Contenu */}
</div>
```

**Avantages :**
- Pas de fichiers CSS séparés
- Design cohérent (espacement, couleurs)
- Responsive intégré : `md:`, `lg:`, etc.
- Dark mode : `dark:` prefix

**Classes courantes :**
```tsx
{/* Layout */}
<div className="flex items-center justify-between">

{/* Espacement */}
<div className="p-4 m-2">  {/* padding: 16px, margin: 8px */}

{/* Couleurs */}
<div className="bg-blue-600 text-white">

{/* Responsive */}
<div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3">

{/* Dark mode */}
<div className="bg-white dark:bg-gray-800">
```

### Composants réutilisables

**Exemple : Bouton**
```tsx
const Button = ({ children, onClick, variant = 'primary' }) => {
  const baseClasses = "px-4 py-2 rounded-md font-medium transition";
  
  const variantClasses = {
    primary: "bg-indigo-600 text-white hover:bg-indigo-700",
    danger: "bg-red-600 text-white hover:bg-red-700",
    secondary: "bg-gray-200 text-gray-800 hover:bg-gray-300"
  };
  
  return (
    <button 
      className={`${baseClasses} ${variantClasses[variant]}`}
      onClick={onClick}
    >
      {children}
    </button>
  );
};
```

**Utilisation :**
```tsx
<Button variant="primary" onClick={handleSave}>Save</Button>
<Button variant="danger" onClick={handleDelete}>Delete</Button>
```

---

## Configuration et variables d'environnement

### constants.ts

**Pourquoi un fichier de constants ?**
- Centraliser toutes les configs
- Faciliter le switch dev/prod
- Variables d'environnement typées

**Structure :**
```typescript
// Google OAuth Client ID (depuis .env)
export const GOOGLE_CLIENT_ID = import.meta.env.VITE_GOOGLE_CLIENT_ID || '';

// URL du backend
export const API_BASE_URL = 'http://localhost:8080';

// Scopes OAuth Google
export const GOOGLE_SCOPES = {
  basic: 'email profile',
  full: 'email profile gmail.readonly gmail.send'
};

export const CURRENT_GOOGLE_SCOPES = GOOGLE_SCOPES.basic;
```

### Variables d'environnement (.env)

**Fichier `.env` (à la racine de /web) :**
```env
VITE_GOOGLE_CLIENT_ID=736605041162-xxx.apps.googleusercontent.com
```

**Accès dans le code :**
```typescript
import.meta.env.VITE_GOOGLE_CLIENT_ID
```

**⚠️ Important :**
- Préfixe `VITE_` obligatoire pour Vite
- Variables exposées côté client (pas de secrets !)
- `.env` dans `.gitignore`

---

## Workflow de développement

### Lancer l'application

**1. Installation des dépendances**
```bash
cd web/
npm install
```

**2. Lancer le dev server**
```bash
npm run dev
```
- Démarre sur http://localhost:5173
- Hot reload activé

**3. Avec Docker (production-like)**
```bash
docker-compose up --build
```
- Frontend accessible sur http://localhost:8081

### Structure d'un composant type

```typescript
// 1. Imports
import React, { useState, useEffect } from 'react';
import { useAuth } from '../contexts/AuthContext';
import { myAPI } from '../services/api';

// 2. Interfaces TypeScript
interface MyComponentProps {
  title: string;
  onSave?: () => void;
}

interface MyData {
  id: number;
  name: string;
}

// 3. Composant
const MyComponent: React.FC<MyComponentProps> = ({ title, onSave }) => {
  // 4. Hooks (toujours en haut, avant toute logique)
  const { token } = useAuth();
  const [data, setData] = useState<MyData[]>([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);
  
  // 5. Effects
  useEffect(() => {
    loadData();
  }, []);
  
  // 6. Fonctions handlers
  const loadData = async () => {
    if (!token) return;
    
    setLoading(true);
    setError(null);
    
    try {
      const result = await myAPI.getData(token);
      setData(result.data);
    } catch (err: any) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };
  
  const handleClick = () => {
    onSave?.();  // Appelle onSave si défini
  };
  
  // 7. Rendering conditionnel
  if (loading) {
    return <div>Loading...</div>;
  }
  
  if (error) {
    return <div className="text-red-600">{error}</div>;
  }
  
  // 8. JSX principal
  return (
    <div className="container mx-auto p-4">
      <h1 className="text-2xl font-bold mb-4">{title}</h1>
      
      {data.length === 0 ? (
        <p>No data available</p>
      ) : (
        <ul>
          {data.map(item => (
            <li key={item.id}>{item.name}</li>
          ))}
        </ul>
      )}
      
      <button onClick={handleClick} className="btn-primary">
        Save
      </button>
    </div>
  );
};

// 9. Export
export default MyComponent;
```

### Bonnes pratiques

#### 1. Typage TypeScript
```typescript
// ✅ Bon
interface User {
  id: number;
  email: string;
}
const [user, setUser] = useState<User | null>(null);

// ❌ À éviter
const [user, setUser] = useState<any>(null);
```

#### 2. Gestion des erreurs
```typescript
// ✅ Bon
try {
  await api.call();
} catch (err: any) {
  setError(err.message || 'Unknown error');
}

// ❌ À éviter
try {
  await api.call();
} catch (err) {
  console.log(err);  // User ne voit rien
}
```

#### 3. Nettoyage des effets
```typescript
useEffect(() => {
  const timer = setTimeout(() => {
    setSuccess(null);
  }, 3000);
  
  return () => clearTimeout(timer);  // ✅ Cleanup
}, []);
```

#### 4. Dépendances useEffect
```typescript
// ✅ Bon
useEffect(() => {
  loadData(userId);
}, [userId]);  // Se réexécute quand userId change

// ❌ À éviter
useEffect(() => {
  loadData(userId);
}, []);  // userId change mais loadData ne se réexécute pas !
```

---

## Debugging et outils

### React DevTools

**Installation :**
Extension Chrome/Firefox "React Developer Tools"

**Fonctionnalités :**
- Inspecter l'arbre des composants
- Voir les props et state en temps réel
- Profiler les performances

### Console du navigateur

**Logs stratégiques :**
```typescript
console.log('User logged in:', user);
console.error('API Error:', error);
console.table(areas);  // Affiche un tableau
```

### Network Tab

**Inspecter les requêtes API :**
1. F12 → Onglet Network
2. Filter : XHR/Fetch
3. Voir les requêtes, headers, body, réponses

---

## Résumé des flux principaux

### 1. Flux d'authentification
```
User ouvre http://localhost:8081
    ↓
App.tsx charge
    ↓
AuthProvider vérifie localStorage
    ↓
Si token trouvé → restaure user → accès aux routes protégées
Si pas de token → redirection vers /login
    ↓
User se connecte (email/password ou Google)
    ↓
Token stocké dans localStorage + state
    ↓
Accès à /dashboard et autres pages protégées
```

### 2. Flux de connexion de service
```
User va sur /services
    ↓
ServicesPage charge:
  - Services disponibles (GET /api/services/)
  - Services connectés (GET /api/auth/services/)
    ↓
User clique "Connect Google"
    ↓
Popup OAuth Google
    ↓
User autorise → Google retourne access_token
    ↓
Frontend → POST /api/auth/google/connect/ {
  access_token,
  user_info,
  expires_in
}
    ↓
Backend stocke le token Google
    ↓
Frontend recharge les services connectés
    ↓
Carte Google affiche "Connected"
```

### 3. Flux de création d'AREA
```
User va sur /areas
    ↓
AREABuilderPage charge:
  - AREAs existantes (GET /api/areas/)
  - Services disponibles (GET /api/services/)
  - Services connectés (GET /api/auth/services/)
    ↓
User clique "Create New AREA"
    ↓
Formulaire s'affiche avec:
  - Actions disponibles (depuis services disponibles)
  - REActions disponibles (depuis services disponibles)
    ↓
User remplit:
  - Nom: "Auto-reply"
  - Action: "New email received"
  - REAction: "Send email"
    ↓
User clique "Create AREA"
    ↓
Frontend → POST /api/areas/ {
  name,
  description,
  action_service,
  action_type,
  reaction_service,
  reaction_type
}
    ↓
Backend crée l'AREA en base
    ↓
Frontend recharge la liste des AREAs
    ↓
Nouvelle AREA s'affiche dans la grille
```

---

## Points techniques avancés

### Pourquoi React vs Vue/Angular ?

**React :**
- ✅ Flexibilité (tu choisis tes libs)
- ✅ Communauté massive
- ✅ JSX (HTML dans JS) intuitif
- ⚠️ Nécessite d'apprendre l'écosystème

**Vue :**
- ✅ Plus simple pour débutants
- ✅ Template HTML classique
- ⚠️ Écosystème plus petit

**Angular :**
- ✅ Framework complet (tout inclus)
- ⚠️ Courbe d'apprentissage élevée
- ⚠️ Plus verbeux

### Virtual DOM

**Problème :**
Manipuler le DOM directement est lent :
```javascript
document.getElementById('user-name').textContent = 'John';
```

**Solution React :**
1. React maintient une copie virtuelle du DOM en mémoire
2. Quand l'état change, React compare l'ancien et le nouveau Virtual DOM
3. React calcule le minimum de changements nécessaires
4. React applique uniquement ces changements au vrai DOM

**Résultat :**
Performance optimale, même avec des UIs complexes.

### Reconciliation et Keys

**Pourquoi les keys dans les listes ?**
```tsx
// ❌ Sans key
{areas.map(area => <AreaCard area={area} />)}

// ✅ Avec key
{areas.map(area => <AreaCard key={area.id} area={area} />)}
```

Sans key, React ne sait pas quel élément a changé et re-rend toute la liste.
Avec key, React identifie précisément l'élément modifié.

### Closure dans useEffect

**Problème classique :**
```typescript
const [count, setCount] = useState(0);

useEffect(() => {
  const timer = setInterval(() => {
    console.log(count);  // Toujours 0 !
  }, 1000);
  
  return () => clearInterval(timer);
}, []);  // [] = closure capture count = 0
```

**Solution :**
```typescript
useEffect(() => {
  const timer = setInterval(() => {
    console.log(count);  // Valeur à jour
  }, 1000);
  
  return () => clearInterval(timer);
}, [count]);  // Se réexécute quand count change
```

---

## Conclusion

### Ce que tu dois retenir

1. **React** = Bibliothèque pour construire des UIs avec des composants
2. **Context** = État global partagé (authentification)
3. **useState** = État local d'un composant
4. **useEffect** = Effets de bord (chargement données, cleanup)
5. **React Router** = Navigation côté client
6. **TypeScript** = JavaScript typé pour moins d'erreurs
7. **Tailwind** = CSS utility-first
8. **Vite** = Build tool rapide

### Architecture en un schéma

```
┌──────────────────────────────────────────────────┐
│                 NAVIGATEUR                       │
│                                                  │
│  ┌────────────────────────────────────────────┐ │
│  │            App.tsx (Routes)               │ │
│  │  ┌──────────────────────────────────────┐ │ │
│  │  │   GoogleOAuthProvider                │ │ │
│  │  │  ┌────────────────────────────────┐  │ │ │
│  │  │  │  BrowserRouter (React Router) │  │ │ │
│  │  │  │ ┌───────────────────────────┐  │  │ │ │
│  │  │  │ │  AuthProvider (Context)   │  │  │ │ │
│  │  │  │ │ ┌──────────────────────┐  │  │  │ │ │
│  │  │  │ │ │ Pages (LoginPage,   │  │  │  │ │ │
│  │  │  │ │ │ Dashboard, etc.)    │  │  │  │ │ │
│  │  │  │ │ │                      │  │  │  │ │ │
│  │  │  │ │ │ useAuth() hook       │  │  │  │ │ │
│  │  │  │ │ │ api.ts functions     │  │  │  │ │ │
│  │  │  │ │ └──────────────────────┘  │  │  │ │ │
│  │  │  │ └───────────────────────────┘  │  │ │ │
│  │  │  └────────────────────────────────┘  │ │ │
│  │  └──────────────────────────────────────┘ │ │
│  └────────────────────────────────────────────┘ │
│                                                  │
│  localStorage: { auth_token, user }              │
└──────────────────────────────────────────────────┘
                    ↕ REST API
┌──────────────────────────────────────────────────┐
│              BACKEND (Django)                    │
│  /api/auth/login/                                │
│  /api/auth/google/login/                         │
│  /api/services/                                  │
│  /api/areas/                                     │
└──────────────────────────────────────────────────┘
```

### Prochaines étapes pour approfondir

1. **Tests** : Jest + React Testing Library
2. **State Management avancé** : Redux Toolkit
3. **Performance** : React.memo, useMemo, useCallback
4. **Formulaires** : React Hook Form
5. **Animations** : Framer Motion

---

**Auteur :** Documentation générée pour le projet AREA
**Date :** Décembre 2025
**Version :** 1.0
