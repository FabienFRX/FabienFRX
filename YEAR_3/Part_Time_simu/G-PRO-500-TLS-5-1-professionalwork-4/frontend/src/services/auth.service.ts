import { API_BASE_URL, getToken as getTokenFromConfig, getAuthHeaders, devLog, devError } from '@/lib/api.config';

export interface RegisterData {
  email: string;
  username: string;
  password: string;
  displayName?: string;
}

export interface LoginData {
  email: string;
  password: string;
}

export interface AuthResponse {
  access_token: string;
}

export interface UserInfo {
  id: string;
  email: string;
  username: string;
  displayName?: string;
  avatarUrl?: string;
}

class AuthService {
  async register(data: RegisterData): Promise<AuthResponse> {
    const response = await fetch(`${API_BASE_URL}/api/auth/register`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(data),
    });

    if (!response.ok) {
      const errorData = await response.json();
      throw new Error(errorData.message || 'Erreur lors de l\'inscription');
    }

    return response.json();
  }

  async login(data: LoginData): Promise<AuthResponse> {
    const response = await fetch(`${API_BASE_URL}/api/auth/login`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(data),
    });

    if (!response.ok) {
      const errorData = await response.json();
      throw new Error(errorData.message || 'Erreur lors de la connexion');
    }

    return response.json();
  }

  setToken(token: string): void {
    localStorage.setItem('auth_token', token);
  }

  getToken(): string | null {
    return getTokenFromConfig();
  }

  removeToken(): void {
    localStorage.removeItem('auth_token');
  }

  isAuthenticated(): boolean {
    return !!this.getToken();
  }

  async getCurrentUser(): Promise<UserInfo | null> {
    const token = this.getToken();
    devLog('getCurrentUser - Token:', token ? token.substring(0, 20) + '...' : 'null');
    
    if (!token) {
      devLog('Pas de token, utilisateur non connecté');
      return null;
    }

    try {
      devLog('Appel API /auth/me...');
      const response = await fetch(`${API_BASE_URL}/api/auth/me`, {
        headers: getAuthHeaders(),
      });

      devLog('Réponse API status:', response.status);
      
      if (!response.ok) {
        devLog('Réponse API non OK');
        return null;
      }

      const userData = await response.json();
      devLog('Données utilisateur reçues:', userData);
      return userData;
    } catch (error) {
      devError('Erreur lors de la récupération de l\'utilisateur:', error);
      return null;
    }
  }

  logout(): void {
    this.removeToken();
  }
}

export const authService = new AuthService();