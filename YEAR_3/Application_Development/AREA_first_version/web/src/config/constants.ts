export const GOOGLE_CLIENT_ID = import.meta.env.VITE_GOOGLE_CLIENT_ID || '';
export const API_BASE_URL = 'http://localhost:8080';

// Google OAuth Scopes
// En développement : scopes de base seulement
// En production : ajouter les scopes Gmail/Calendar après validation Google
export const GOOGLE_SCOPES = {
  // Scopes de base (toujours disponibles)
  basic: 'https://www.googleapis.com/auth/userinfo.email https://www.googleapis.com/auth/userinfo.profile',
  // Scopes complets (nécessitent validation Google)
  full: 'https://www.googleapis.com/auth/userinfo.email https://www.googleapis.com/auth/userinfo.profile https://www.googleapis.com/auth/gmail.readonly https://www.googleapis.com/auth/gmail.send https://www.googleapis.com/auth/calendar',
};

// Utiliser les scopes de base en développement
export const CURRENT_GOOGLE_SCOPES = GOOGLE_SCOPES.basic;
