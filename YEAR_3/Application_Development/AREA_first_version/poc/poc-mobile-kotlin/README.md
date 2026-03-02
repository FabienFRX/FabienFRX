# 🚀 PoC Kotlin - Application Mobile AREA

## Objectif
Démontrer la création d'un AREA Builder avec Kotlin et Jetpack Compose pour Android natif.

## 🛠️ Installation rapide

### Prérequis
- Android Studio (latest version)
- JDK 11 ou supérieur
- Android SDK 24+ (Android 7.0+)

### Étapes
1. Ouvrir Android Studio
2. File → Open → Sélectionner `poc-mobile-kotlin/`
3. Attendre le Gradle Sync
4. Créer un émulateur Android (API 24+) ou connecter un appareil physique
5. Cliquer sur Run (▶️)

## 📱 Fonctionnalités

### Architecture
- **Jetpack Compose** : UI déclarative moderne
- **Material Design 3** : Composants UI modernes
- **MVVM Pattern** : ViewModel avec StateFlow
- **Navigation Compose** : Navigation entre écrans
- **100% Kotlin** : Code natif Android

### Pages
1. **HomePage** : Page d'accueil avec navigation vers création et liste
2. **AREABuilderPage** : Création d'AREA avec sélection action/réaction
3. **MyAreasPage** : Liste des AREAs avec toggle actif/inactif et suppression

### Services disponibles
- 📧 **Gmail** : Actions (nouvel email, email avec pièce jointe)
- ☁️ **OneDrive** : Réactions (sauvegarder fichier, créer dossier)
- 🐙 **GitHub** : Actions & Réactions (PR, issues, star)
- 🐦 **Twitter** : Actions & Réactions (tweets, followers, likes)