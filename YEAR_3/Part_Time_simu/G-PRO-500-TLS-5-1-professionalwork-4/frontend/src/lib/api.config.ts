/**
 * Configuration centralisée pour les appels API
 */

export const API_BASE_URL = import.meta.env.VITE_API_URL || 'http://localhost:3000';

/**
 * Récupère le token d'authentification
 */
export function getToken(): string | null {
  return localStorage.getItem('auth_token');
}

/**
 * Génère les headers d'authentification pour les requêtes API
 */
export function getAuthHeaders(): HeadersInit {
  const token = getToken();
  return {
    'Content-Type': 'application/json',
    ...(token && { Authorization: `Bearer ${token}` }),
  };
}

/**
 * Logger conditionnel (uniquement en développement)
 */
export function devLog(...args: any[]): void {
  if (import.meta.env.DEV) {
    console.log('[QuizLab]', ...args);
  }
}

/**
 * Erreur logger conditionnel
 */
export function devError(...args: any[]): void {
  if (import.meta.env.DEV) {
    console.error('[QuizLab Error]', ...args);
  }
}

/**
 * Warning logger conditionnel
 */
export function devWarn(...args: any[]): void {
  if (import.meta.env.DEV) {
    console.warn('[QuizLab Warning]', ...args);
  }
}
