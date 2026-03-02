# Documentation Mobile - AREA Application

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
11. [Configuration et secrets](#configuration-et-secrets)
12. [Workflow de développement](#workflow-de-développement)

---

## Vue d'ensemble

### Qu'est-ce que l'application mobile AREA ?

L'application mobile AREA est le client natif (Android/iOS) de la plateforme d'automatisation. Elle offre une expérience utilisateur optimisée pour gérer ses workflows en déplacement, similaire à l'interface web mais adaptée aux usages tactiles.

### Objectif du mobile

L'application permet de :
1. **S'authentifier** via Email/Password ou OAuth2 (Google, GitLab, GitHub, LinkedIn, Dropbox).
2. **Gérer les services** connectés (lier/délier des comptes).
3. **Créer des AREAs** (Action-REAction) via un builder intuitif.
4. **Visualiser et piloter** ses automations existantes à partir d'un dashboard.

---

## Architecture générale

### Architecture Client-Serveur

L'architecture suit strictement le même modèle que le frontend web, utilisant l'API REST Django comme source unique de vérité.

```
┌─────────────────────────────────────────┐
│           MOBILE (Client Flutter)       │
│  ┌───────────────────────────────────┐  │
│  │     Flutter App (Android/iOS)     │  │
│  │  - Gère l'UI (Widgets Material)   │  │
│  │  - Stocke le token (Secure St.)   │  │
│  │  - Appelle l'API REST             │  │
│  └───────────────────────────────────┘  │
└─────────────────────────────────────────┘
              ↕ HTTP/REST API
┌─────────────────────────────────────────┐
│          SERVEUR (Backend)              │
│  ┌───────────────────────────────────┐  │
│  │     Django API (Python)           │  │
│  │  - Authentification (Token)       │  │
│  │  - OAuth Flows                    │  │
│  │  - Logique métier des AREAs       │  │
│  └───────────────────────────────────┘  │
└─────────────────────────────────────────┘
```

---

## Stack technique

### Technologies principales

#### 1. **Flutter SDK** & **Dart**
- **Framework UI** : Permet de créer une application native performante compilée pour Android et iOS à partir d'une seule base de code.
- **Langage** : Dart, fortement typé, offrant sécurité et performance (AOT compilation).

#### 2. **Provider** - Gestion d'état
- Équivalent léger de Redux ou Context API (React).
- Permet d'injecter des dépendances (`AuthProvider`, `ServerProvider`) dans l'arbre des widgets.
- Utilise le pattern `ChangeNotifier` pour notifier les widgets des mises à jour.

#### 3. **http** - Client HTTP
- Gère toutes les requêtes REST vers le backend Django.
- Intercepteurs (via wrappers) pour ajouter les tokens d'authentification.

#### 4. **google_sign_in** - OAuth Natif
- Utilise les SDKs natifs (Android/iOS) pour l'authentification Google.
- Plus fluide qu'une WebView pour Google.

#### 5. **webview_flutter** - OAuth Autres Services
- Gère les flux OAuth2 pour les services ne disposant pas de SDK natif intégré ou configuré (GitLab, GitHub, LinkedIn, Dropbox).
- Affiche la page de login du service dans une vue web intégrée.

#### 6. **shared_preferences** - Stockage local
- Stocke de manière persistante le token d'authentification et l'URL du serveur.

---

## Structure du projet

```
mobile/
├── lib/
│   ├── main.dart               # Point d'entrée, MultiProvider
│   ├── config/
│   │   └── secrets.dart        # Clés API et IDs clients (GitLab, etc.)
│   ├── models/
│   │   └── area.dart           # Modèles de données (Area, Service, etc.)
│   ├── providers/
│   │   ├── auth_provider.dart  # État d'authentification (User, Token)
│   │   ├── area_provider.dart  # État des AREAs et Services
│   │   └── server_provider.dart # Configuration URL serveur (Dev/Prod)
│   ├── screens/
│   │   ├── login_screen.dart   # Écran de connexion
│   │   ├── home_screen.dart    # Dashboard principal
│   │   ├── services_screen.dart # Gestion des services connectés
│   │   ├── area_builder_screen.dart # Création d'automations
│   │   └── *_auth_screen.dart  # WebViews pour OAuth (GitLab, GitHub...)
│   ├── services/
│   │   └── api_service.dart    # Centralisation des appels API REST
│   └── widgets/
│       └── ...                 # Widgets réutilisables (Cards, Inputs)
├── pubspec.yaml                # Dépendances Flutter
└── android/                    # Projet natif Android
```

### Organisation des dossiers
- **Providers** : Contiennent la logique métier et l'état global.
- **Screens** : Les pages complètes de l'application.
- **Services** : La couche de communication avec l'extérieur (API).
- **Config** : Les constantes sensibles (ignoré par Git en prod, mais structure présente).

---

## Composants principaux

### 1. main.dart - L'Initialiseur

Point d'entrée qui configure les Providers globaux et le système de routing.

```dart
void main() {
  runApp(
    MultiProvider(
      providers: [
        ChangeNotifierProvider(create: (_) => ServerProvider()),
        ChangeNotifierProvider(create: (_) => AuthProvider()),
        // ...
      ],
      child: const MyApp(),
    ),
  );
}
```

### 2. AuthProvider - Le Gestionnaire de Session

Similaire à `AuthContext` en React, il gère :
- Le stockage du Token JWT.
- L'état de connexion (`isAuthenticated`).
- Les méthodes de login/logout (Google, Email, GitLab...).

### 3. ApiService - Le Client REST

Centralise tous les appels HTTP pour éviter la duplication de code et faciliter la maintenance.

```dart
class ApiService {
  final String baseUrl;

  // Appel générique authentifié
  Future<Map<String, dynamic>> get(String endpoint, String token) async {
    final response = await http.get(
      Uri.parse('$baseUrl$endpoint'),
      headers: {
        'Authorization': 'Token $token',
        'Content-Type': 'application/json',
      },
    );
    // Gestion des erreurs...
  }
}
```

### 4. ServicesScreen - Le Hub de Connexion

Permet de lier des comptes externes (GitLab, Google, etc.).
- Affiche l'état "Connecté/Non connecté" pour chaque service.
- Gère le flux OAuth spécifique à chaque service (Redirection vers `WebView` ou SDK natif).

---

## Gestion de l'état (Provider)

L'application utilise **Provider** pour la gestion d'état, qui est le standard recommandé par Google pour les applications Flutter de taille moyenne.

### Flux de données
1. **Action UI** (ex: Clic sur "Login") → Appelle une méthode du **Provider**.
2. **Provider** → Appelle **ApiService**.
3. **ApiService** → Retourne les données du backend.
4. **Provider** → Met à jour ses variables internes (`_user`, `_token`) et appelle `notifyListeners()`.
5. **Widget** (Consumer) → Se reconstruit automatiquement avec les nouvelles données.

Exemple :
```dart
// Dans AuthProvider
void login(String email, String password) async {
  _isLoading = true;
  notifyListeners(); // Affiche un loader
  
  try {
    _token = await apiService.login(email, password);
    _isAuthenticated = true;
  } finally {
    _isLoading = false;
    notifyListeners(); // Cache le loader et change d'écran
  }
}
```

---

## Authentification

### Méthodes supportées

1. **Email/Password** : Appel direct API `/api/auth/login/`.
2. **Google OAuth (Natif)** :
   - Utilise `google_sign_in`.
   - Récupère un `serverAuthCode`.
   - L'envoie au backend qui l'échange contre un token refresh/access.
3. **Autres OAuth (GitLab, GitHub, LinkedIn, Dropbox)** :
   - Utilise **WebView** (`webview_flutter`).
   - Ouvre l'URL d'autorisation du service.
   - Intercepte l'URL de redirection (`redirect_uri`) pour extraire le `code`.
   - Envoie le `code` au backend pour finaliser l'échange.

### Spécificité Mobile (Redirect URI)
Pour les flux OAuth WebView (comme GitLab), l'URL de redirection est interceptée par la WebView mobile avant d'atteindre le navigateur.
- **URI Backend** : Le backend s'attend souvent à une URI web.
- **Solution** : Le mobile envoie son URI de redirection (ou l'URI par défaut) au backend lors de l'échange pour garantir la correspondance avec la requête initiale (`invalid_grant` prevention).

---

## Routing et navigation

Flutter utilise ici la navigation impérative simple (`Navigator.push`, `Navigator.pop`).

### Définition des routes (main.dart)
```dart
routes: {
  '/login': (context) => const LoginScreen(),
  '/register': (context) => const RegisterScreen(),
  '/home': (context) => const HomeScreen(),
  '/settings': (context) => const SettingsScreen(),
},
```

### Navigation
```dart
// Aller vers une page
Navigator.pushNamed(context, '/home');

// Retour en arrière
Navigator.pop(context);

// Ouvrir une WebView pour Auth (attendre un résultat)
final code = await Navigator.push(
  context,
  MaterialPageRoute(builder: (_) => GitLabAuthScreen(...)),
);
```

---

## Communication avec le backend

### Environment Android Emulateur
Particularité importante : Sur l'émulateur Android, `localhost` désigne l'émulateur lui-même. Pour atteindre le serveur hôte (votre PC), il faut utiliser l'IP spéciale :
- **URL** : `http://10.0.2.2:8080`

Le `ServerProvider` permet de configurer cette URL dynamiquement pour passer facilement du développement local à la production.

---

## Styling et UI

Contrairement au web (CSS/Tailwind), Flutter utilise des widgets **composables**.

- **Thème** : Les couleurs (Dark Mode par défaut ici) sont définies dans les widgets ou via `ThemeData`.
- **Flexibilité** : Utilisation de `Row`, `Column`, `Stack`, et `Container` pour la mise en page.
- **Responsive** : Les layouts s'adaptent, mais sont pensés "Mobile First".

Exemple de style (Bouton GitLab) :
```dart
ElevatedButton(
  style: ElevatedButton.styleFrom(
    backgroundColor: const Color(0xFFFC6D26), // Orange GitLab
    shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(8)),
  ),
  onPressed: _loginGitLab,
  child: const Text('Continue with GitLab'),
)
```

---

## Configuration et secrets

Le fichier `lib/config/secrets.dart` contient les identifiants OAuth publics (Client IDs).
**Important** : Ce fichier ne doit pas contenir de `Client Secret` (le mobile est un client public, il ne peut pas garder de secret). Les secrets sont gérés par le Backend.

```dart
class Secrets {
  static const String gitlabClientId = '...';
  // Note: mobile redirect URIs peuvent être différentes du web
  static const String gitlabRedirectUri = 'http://localhost:8080/api/auth/gitlab/callback/';
}
```

---

## Workflow de développement

### Lancer l'application
```bash
# Lancer sur l'émulateur ou device connecté
flutter run

# Mode verbeuse pour le debug
flutter run -v
```

### Android Emulator Setup
Assurez-vous que l'émulateur a accès au réseau/internet pour contacter :
1. Le backend (`10.0.2.2`).
2. Les serveurs OAuth (Google, GitLab, etc.).

### Build Production
```bash
# Générer un APK
flutter build apk --release
```
