import { API_BASE_URL, getToken, getAuthHeaders, devWarn } from '@/lib/api.config';

export type User = {
  id: string;
  email: string;
  username: string;
  displayName?: string;
  avatarUrl?: string;
  birthYear?: number;
  country?: string;
  roles: string[];
  createdAt: string;
  updatedAt: string;
};

export type UpdateUserInput = Partial<Pick<User, 'displayName' | 'avatarUrl' | 'birthYear' | 'country'>>;
export type PreferencesInput = Partial<{ ageGroup: 'CHILD'|'TEEN'|'ADULT'; defaultLevel: 'EASY'|'MEDIUM'|'HARD'; favoriteTopics: string[]; trickyAnswersEnabled: boolean }>;

export const usersService = {
  async list(): Promise<User[]> {
    const res = await fetch(`${API_BASE_URL}/api/users`, { headers: getAuthHeaders() });
    if (!res.ok) throw new Error('Erreur lors du chargement des utilisateurs');
    return res.json();
  },
  async getById(id: string): Promise<User> {
    const res = await fetch(`${API_BASE_URL}/api/users/${id}`, {
      method: 'GET',
      headers: getAuthHeaders(),
    });
    if (!res.ok) throw new Error('Erreur lors du chargement du profil');
    return res.json();
  },
  async updatePreferences(id: string, prefs: PreferencesInput): Promise<User> {
    const payload: any = { ...prefs };
    if (payload.favoriteTopics && Array.isArray(payload.favoriteTopics)) {
      payload.favoriteTopics = payload.favoriteTopics.slice(0, 2);
    }
    const res = await fetch(`${API_BASE_URL}/api/users/${id}`, {
      method: 'PATCH',
      headers: getAuthHeaders(),
      body: JSON.stringify(payload),
    });
    if (!res.ok) {
      let msg = 'Erreur lors de la mise à jour des préférences';
      try { const j = await res.json(); msg = j.message || msg; } catch {}
      throw new Error(msg);
    }
    return res.json();
  },
  async delete(id: string): Promise<void> {
    const res = await fetch(`${API_BASE_URL}/api/users/${id}`, {
      method: 'DELETE',
      headers: getAuthHeaders(),
    });
    if (!res.ok) {
      let msg = 'Erreur lors de la suppression du compte';
      try { const j = await res.json(); msg = j.message || msg; } catch {}
      throw new Error(msg);
    }
  },

  async update(id: string, data: UpdateUserInput): Promise<User> {
    const toSend: any = { ...data };
    if (typeof toSend.birthYear === 'string') {
      if (toSend.birthYear.trim() === '') delete toSend.birthYear; else toSend.birthYear = Number(toSend.birthYear);
    }
    const res = await fetch(`${API_BASE_URL}/api/users/${id}`, {
      method: 'PATCH',
      headers: getAuthHeaders(),
      body: JSON.stringify(toSend),
    });
    if (!res.ok) {
      let msg = 'Erreur lors de la mise à jour du profil';
      try { const j = await res.json(); msg = j.message || msg; } catch {}
      throw new Error(msg);
    }
    return res.json();
  },
  async changePassword(id: string, currentPassword: string, newPassword: string): Promise<void> {
    const res = await fetch(`${API_BASE_URL}/api/users/${id}/password`, {
      method: 'PATCH',
      headers: getAuthHeaders(),
      body: JSON.stringify({ currentPassword, newPassword }),
    });
    if (!res.ok) {
      let msg = 'Erreur lors du changement de mot de passe';
      try { const j = await res.json(); msg = j.message || msg; } catch {}
      throw new Error(msg);
    }
  },
  async adminSetPassword(id: string, newPassword: string): Promise<void> {
    const res = await fetch(`${API_BASE_URL}/api/users/${id}/password`, {
      method: 'PATCH',
      headers: getAuthHeaders(),
      body: JSON.stringify({ currentPassword: '', newPassword }),
    });
    if (!res.ok) {
      let msg = 'Erreur lors de la mise à jour du mot de passe';
      try { const j = await res.json(); msg = j.message || msg; } catch {}
      throw new Error(msg);
    }
  },
};

export function getUserIdFromToken(): string | null {
  const token = getToken();
  if (!token) return null;
  try {
    const [, payloadB64] = token.split('.');
    const json = JSON.parse(atob(payloadB64.replace(/-/g, '+').replace(/_/g, '/')));
    return typeof json.sub === 'string' ? json.sub : null;
  } catch {
    return null;
  }
}

export function getRolesFromToken(): string[] {
  const token = getToken();
  if (!token) return [];
  try {
    const parts = token.split('.');
    if (parts.length < 2) return [];
    let payloadB64 = parts[1].replace(/-/g, '+').replace(/_/g, '/');
    while (payloadB64.length % 4 !== 0) payloadB64 += '=';
    const jsonStr = atob(payloadB64);
    const json = JSON.parse(jsonStr);
    const roles = json.roles;
    return Array.isArray(roles) ? roles : [];
  } catch (e) {
    devWarn('Failed to decode roles from token:', e);
    return [];
  }
}