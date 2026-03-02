# 🚀 PoC Flutter - Application Mobile AREA

## Objectif
Tester Flutter pour une application mobile native cross-platform : un seul code pour Android, iOS et Web avec Material Design 3.

## 🛠️ Installation rapide (Développement)

```bash
# Aller dans le dossier du PoC
cd poc_mobile_flutter

# Installer les dépendances
flutter pub get

# Lancer l'application sur Chrome (mode mobile)
flutter run -d chrome --web-port 8200

# Ou sur un émulateur Android/iOS
flutter run
```

## 🐳 Build avec Docker (Production APK)

```bash
# Builder l'image Docker et générer l'APK
./build-apk.sh

# Lancer le serveur pour servir l'APK
docker run -d -p 8081:8081 --name area-mobile flutter-mobile-client

# Télécharger l'APK
curl http://localhost:8081/client.apk -o area-mobile.apk

# Ou directement dans le navigateur
# http://localhost:8081/client.apk
```

## 📦 Intégration Docker Compose

Pour intégrer dans l'architecture AREA complète :

```yaml
mobile_client:
  build: ./poc_mobile_flutter
  ports:
    - "8081:8081"
```

L'APK sera accessible sur `http://localhost:8081/client.apk`

## 📱 Fonctionnalités
- Material Design 3 natif
- Navigation avec Navigator
- Widgets Flutter (Card, DropdownButton, etc.)
- Hot reload instantané (~0.5s)
- Cross-platform 100% (Android, iOS, Web)
- Performance native avec Dart
- Build APK via Docker pour distribution