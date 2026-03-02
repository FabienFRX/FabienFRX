# AREA benchmark mobile

## Critères de l'AREA

- Une application mobile native/hybride performante et réactive
- Une architecture modulaire avec de nombreux écrans et états UI
- Un écosystème riche (gestion d'état, UI components, intégrations natives)
- Temps de développement limité
- Réutilisation possible avec le frontend web (React)
- Packaging facile (APK/IPA generation)

## Qu'est-ce que Flutter ?

**Flutter** est un framework open source créé par Google pour développer des applications **natives multiplateformes** (iOS, Android, Web, Desktop) à partir d'une seule base de code. Il utilise le langage **Dart** et propose son propre moteur de rendu pour offrir des performances proches du natif.

Flutter s'appuie sur un système de **widgets** (composants UI) personnalisables et un **hot reload** ultra-rapide qui permet de voir les changements instantanément pendant le développement.

## Pourquoi utiliser Flutter ?

- **Performance native** : compilation en code natif (ARM), pas de bridge JavaScript → 60 fps garantis.
- **Hot Reload rapide** : modifications visibles en < 1 seconde, gain de productivité énorme.
- **UI riche et flexible** : widgets Material Design et Cupertino (iOS), animations fluides, contrôle total sur le rendu.
- **Single codebase** : iOS + Android + Web depuis le même code Dart.
- **Écosystème mature** : pub.dev avec 30 000+ packages (HTTP, état, OAuth2, Firebase, etc.).
- **Soutenu par Google** : utilisé en interne (Google Ads, Stadia), maintenance continue, communauté massive.

## Qui utilise Flutter ?

Flutter est adopté par des milliers d'entreprises dans le monde :

1. Google (Google Ads, Stadia, Assistant)
2. Alibaba
3. BMW
4. eBay
5. Groupon
6. Philips Hue
7. Reflectly
8. Tencent
9. The New York Times
10. Nubank

________________________________________________________________________

## Qu'est-ce qu'Ionic ?

**Ionic** est un framework open source pour créer des applications **hybrides** multiplateformes (iOS, Android, Web). Il s'appuie sur des **technologies web** (HTML, CSS, JavaScript/TypeScript) et utilise **Capacitor** ou **Cordova** pour accéder aux fonctionnalités natives.

Ionic permet de réutiliser directement du code web existant (Angular, React, Vue) et de déployer sur mobile via une **WebView**. C'est une approche "write once, run everywhere" axée sur la productivité.

## Pourquoi utiliser Ionic ?

- **Technologies web familières** : HTML/CSS/JS → aucune courbe d'apprentissage pour dev web.
- **Réutilisation de code** : partage possible avec frontend React → gain de temps énorme.
- **Développement rapide** : prototypage ultra-rapide, hot reload, debugging Chrome DevTools.
- **Composants UI prêts** : bibliothèque de composants Material/iOS intégrée.
- **Capacitor moderne** : accès natif (caméra, GPS, Bluetooth) sans plugin lourd.
- **PWA-first** : déploiement web instantané + optionnellement APK/IPA.

## Qui utilise Ionic ?

Plus de **5 millions d'applications** ont été créées avec Ionic, dont :

1. MarketWatch
2. Sworkit
3. JustWatch
4. Nationwide
5. McDonald's Türkiye
6. Pacifica
7. Untappd
8. Diesel
9. T-Mobile
10. NHS (UK Healthcare)

________________________________________________________________________

## Qu'est-ce que Kotlin (Multiplatform) ?

**Kotlin** est un langage moderne développé par JetBrains, devenu langage officiel pour Android depuis 2017. **Kotlin Multiplatform (KMP)** permet de partager la logique métier entre iOS, Android, Web et Desktop tout en gardant des UI natives pour chaque plateforme.

Contrairement à Flutter ou Ionic, Kotlin privilégie une approche **"shared logic, native UI"** : le code métier (API, état, business rules) est partagé, mais l'UI reste 100% native (Jetpack Compose pour Android, SwiftUI pour iOS).

## Pourquoi utiliser Kotlin ?

- **Langage moderne et sûr** : null-safety, coroutines, DSL élégants, interop Java.
- **Performance native pure** : aucune WebView, aucun bridge → performance maximale.
- **UI vraiment native** : Jetpack Compose (Android) / SwiftUI (iOS) → look & feel parfait.
- **Écosystème Android first-class** : intégration parfaite avec Android SDK, Gradle, Android Studio.
- **Soutien de Google & JetBrains** : langage officiel Android, maintenance garantie.
- **Adoption croissante** : KMP utilisé par Netflix, VMware, Philips, Cash App.

## Qui utilise Kotlin ?

Kotlin est massivement adopté dans l'écosystème Android et au-delà :

1. Google (Android OS, Google Home)
2. Netflix
3. Uber
4. Trello
5. Pinterest
6. Slack
7. Coursera
8. Evernote
9. Cash App (Square)
10. VMware

________________________________________________________________________

## Comparaison :

### **Flutter vs Ionic**

| **Besoin AREA**                      | **Flutter**                            | **Ionic**                        | **Avantage**                                                        |
| ------------------------------------ | -------------------------------------- | -------------------------------- | ------------------------------------------------------------------- |
| **Performance UI**                   | ==Native (60fps garanti)==             | WebView (limitations animations) | **Flutter** : rendu natif, pas de WebView                           |
| **Réutilisation code frontend**      | Non (Dart ≠ React)                     | ==Oui (React direct)==           | **Ionic** : partage composants React → gain temps                   |
| **Écosystème OAuth2**                | ==20+ packages pub.dev==               | 15+ packages npm                 | **Flutter** : packages matures (oauth2, flutter_appauth, etc.)      |
| **Temps de développement (scratch)** | ~3-5 jours (apprentissage Dart)        | ==~1-2 jours (React existant)==  | **Ionic** : si frontend React déjà fait                             |
| **Accès fonctions natives**          | ==Plugins Flutter officiels (camera)== | Capacitor plugins                | **Flutter** : intégration native plus profonde                      |
| **Bundle APK size**                  | ~15-20 MB (release)                    | ==~8-12 MB==                     | **Ionic** : plus léger (mais WebView Android ~30MB système requis)  |

### **Flutter vs Kotlin**

| **Critère**                          | **Flutter**                   | **Kotlin (Multiplatform)**         | **Impact**                                          |
| ------------------------------------ | ----------------------------- | ---------------------------------- | --------------------------------------------------- |
| **Courbe d'apprentissage**           | ==Rapide (Dart simple)==      | Moyenne (KMP + Swift pour iOS)     | **Flutter** : single langage Dart vs Dart+Swift    |
| **UI cross-platform**                | ==Widgets identiques partout== | UI native séparée (Compose/SwiftUI)| **Flutter** : cohérence visuelle garantie          |
| **Performance**                      | Native ARM (excellent)        | ==100% natif (optimal)==           | **Kotlin** : léger avantage perf pure              |
| **Time-to-market**                   | ==Plus rapide (UI partagée)== | Plus long (2 UI à développer)      | **Flutter** : UI unique = développement plus rapide|
| **Écosystème mobile**                | ==30 000+ packages pub.dev==  | Limité (KMP récent)                | **Flutter** : écosystème plus mature               |

### **Ionic vs Kotlin**

| **Critère**                      | **Ionic**                        | **Kotlin**                     | **Impact**                                  |
| -------------------------------- | -------------------------------- | ------------------------------ | ------------------------------------------- |
| **Réutilisation code web**       | ==100% (même React)==            | 0% (logique partagée seulement)| **Ionic** : énorme gain si frontend React  |
| **Performance animations**       | Limitée (WebView 30-60fps)       | ==60fps natif garanti==        | **Kotlin** : meilleure pour UI complexe    |
| **Tooling & debug**              | ==Chrome DevTools==              | Android Studio / Xcode         | **Ionic** : debugging web familier         |
| **Build time**                   | ==~30 secondes==                 | ~2-3 minutes (Gradle)          | **Ionic** : itération plus rapide          |

________________________________________________________________________

## Popularité :

Allons sur Google Trends pour voir la popularité en termes de recherches (12 derniers mois, France) :

[Google Trends Flutter vs Ionic vs Kotlin](assets/google-trend_flutter_vs_ionic_vs_kotlin.png)

Passons à GitHub qui est l'endroit où la plupart des projets open source mettent leur code et où les développeurs trouvent les informations sur les frameworks :

[GitHub Comparaison](assets/github_mobile_comparaison.png)

**Statistiques GitHub (Nov 2025)** :
- **Flutter** : 174 000 stars, 29 500 forks, 12 400 issues ouvertes
- **Ionic** : 52 200 stars, 13 400 forks, 620 issues ouvertes
- **Kotlin** : 51 800 stars, 6 100 forks, 194 issues ouvertes

À noter qu'une forte popularité signifie plus d'exemples en ligne et d'aide disponible.

________________________________________________________________________

## + vs -

### **Flutter**

**Plus :**
- Performance native (60fps) sans compromis
- Hot reload ultra-rapide (< 1s)
- UI riche et personnalisable à l'infini
- Single codebase (iOS + Android + Web)
- Écosystème mature (30 000+ packages)
- Fort soutien de Google et communauté massive

**Moins :**
- Langage Dart (courbe d'apprentissage si équipe JS/TS)
- APK size plus lourde (~15-20 MB minimum)
- Pas de réutilisation directe du code React

### **Ionic**

**Plus :**
- Réutilisation totale du code React/Vue/Angular
- Développement ultra-rapide (technologies web)
- Debugging facile (Chrome DevTools)
- Bundle léger (~8-12 MB)
- PWA-first (déploiement web instantané)

**Moins :**
- Performance limitée par WebView (animations complexes)
- Moins "native feeling" que Flutter/Kotlin
- Dépendance aux plugins Capacitor pour fonctions natives

### **Kotlin (Multiplatform)**

**Plus :**
- Performance 100% native (la meilleure)
- UI vraiment native (Material/Cupertino)
- Langage officiel Android (Google)
- Interop parfaite avec Java/Android SDK

**Moins :**
- UI non partagée (Compose + SwiftUI séparés)
- Courbe d'apprentissage (Kotlin + Swift nécessaires)
- Écosystème KMP encore jeune
- Time-to-market plus long

________________________________________________________________________

## Pour conclure

Flutter est le choix optimal pour le projet AREA grâce à son équilibre entre **performance native** (60fps), **productivité** (hot reload, single codebase), et **écosystème mature** (30 000+ packages). Il combine développement rapide et performances proches du natif, essentiel pour respecter nos deadlines tout en livrant une application mobile professionnelle.
