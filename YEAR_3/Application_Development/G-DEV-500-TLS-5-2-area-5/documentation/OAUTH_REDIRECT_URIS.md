# Configuration des OAuth Redirect URIs

Pour que l'authentification OAuth fonctionne correctement, vous devez enregistrer les redirect URIs suivants dans les consoles des providers :

## ✅ Google Cloud Console
**URL:** https://console.cloud.google.com/apis/credentials

1. Sélectionnez votre projet
2. Allez dans "APIs & Services" > "Credentials"
3. Cliquez sur votre "OAuth 2.0 Client ID"
4. Dans "Authorized redirect URIs", ajoutez :
   ```
   http://localhost:8080/api/auth/google/callback/
   ```
5. Cliquez sur "Save"

## 🔵 LinkedIn Developer Portal
**URL:** https://www.linkedin.com/developers/apps

1. Sélectionnez votre application
2. Allez dans l'onglet "Auth"
3. Dans "OAuth 2.0 settings" > "Redirect URLs", ajoutez :
   ```
   http://localhost:8080/api/auth/linkedin/callback/
   ```
4. Cliquez sur "Update"

## 🐙 GitHub Developer Settings
**URL:** https://github.com/settings/developers

1. Sélectionnez votre OAuth App
2. Dans "Authorization callback URL", remplacez par :
   ```
   http://localhost:8080/api/auth/github/callback/
   ```
3. Cliquez sur "Update application"

## 🧪 Test après configuration

Une fois les redirect URIs configurés dans toutes les consoles :

1. Redémarrez les services si nécessaire :
   ```bash
   docker-compose restart server
   ```

2. Testez chaque provider :
   - Allez sur http://localhost:8081/login
   - Cliquez sur "Continue with Google/LinkedIn/GitHub"
   - Vous devriez être redirigé vers le dashboard après authentification

## ⚠️ Notes importantes

- Les redirect URIs doivent correspondre **EXACTEMENT** (protocole, domaine, port, path)
- Pour la production, remplacez `http://localhost:8080` par votre domaine réel
- N'oubliez pas d'ajouter aussi vos domaines de production dans les consoles OAuth

## 📝 Variables d'environnement actuelles

Dans `backend/.env` :
```env
BACKEND_URL=http://localhost:8080
FRONTEND_URL=http://localhost:8081
GOOGLE_OAUTH2_CLIENT_ID=736605041162-l9hfp01tnje5372th5bm3jbnbsgh0cfp.apps.googleusercontent.com
GITHUB_CLIENT_ID=Ov23liL8bhCWdMXAF2dx
LINKEDIN_CLIENT_ID=7728p8w4txpoyj
```

Si vous changez `BACKEND_URL`, pensez à mettre à jour les redirect URIs dans les consoles OAuth !
