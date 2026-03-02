const API_BASE_URL = 'http://localhost:8080';

interface RegisterData {
  email: string;
  password: string;
  first_name?: string;
  last_name?: string;
}

interface LoginData {
  email: string;
  password: string;
}

export interface User {
  id: number;
  email: string;
  first_name: string;
  last_name: string;
  username: string;
}

interface AuthResponse {
  message: string;
  user: User;
  token: string;
}

export const authAPI = {
  async register(data: RegisterData): Promise<AuthResponse> {
    const response = await fetch(`${API_BASE_URL}/api/auth/signup/`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(data),
    });

    const responseData = await response.json();

    if (!response.ok) {
      throw new Error(responseData.error || 'Registration failed');
    }

    return responseData;
  },

  async login(data: LoginData): Promise<AuthResponse> {
    const response = await fetch(`${API_BASE_URL}/api/auth/login/`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(data),
    });

    const responseData = await response.json();

    if (!response.ok) {
      throw new Error(responseData.error || 'Login failed');
    }

    return responseData;
  },

  async logout(token: string): Promise<void> {
    const response = await fetch(`${API_BASE_URL}/api/auth/logout/`, {
      method: 'POST',
      headers: {
        'Authorization': `Token ${token}`,
        'Content-Type': 'application/json',
      },
    });

    if (!response.ok) {
      throw new Error('Logout failed');
    }
  },

  async loginWithGoogle(accessToken: string, expiresIn?: number): Promise<AuthResponse> {
    const response = await fetch(`${API_BASE_URL}/api/auth/google/login/`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ 
        access_token: accessToken,
        expires_in: expiresIn 
      }),
    });

    const responseData = await response.json();

    if (!response.ok) {
      throw new Error(responseData.error || 'Google login failed');
    }

    return responseData;
  },
};

// Services API
export interface Action {
  name: string;
  description: string;
}

export interface Reaction {
  name: string;
  description: string;
}

export interface ServiceWithDetails {
  name: string;
  actions: Action[];
  reactions: Reaction[];
}

export const servicesAPI = {
  async getAvailableServices(token: string): Promise<{ services: ServiceWithDetails[] }> {
    const response = await fetch(`${API_BASE_URL}/api/services/`, {
      headers: {
        'Authorization': `Token ${token}`,
      },
    });

    if (!response.ok) {
      throw new Error('Failed to fetch services');
    }

    return response.json();
  },

  async getUserServices(token: string) {
    const response = await fetch(`${API_BASE_URL}/api/auth/services/`, {
      headers: {
        'Authorization': `Token ${token}`,
      },
    });

    if (!response.ok) {
      throw new Error('Failed to fetch user services');
    }

    return response.json();
  },

  async connectGoogleService(
    token: string, 
    googleAccessToken: string, 
    userInfo: any, 
    expiresIn?: number, 
    refreshToken?: string
  ) {
    const response = await fetch(`${API_BASE_URL}/api/auth/google/connect/`, {
      method: 'POST',
      headers: {
        'Authorization': `Token ${token}`,
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        access_token: googleAccessToken,
        user_info: userInfo,
        expires_in: expiresIn,
        refresh_token: refreshToken || '',
      }),
    });

    const responseData = await response.json();

    if (!response.ok) {
      throw new Error(responseData.error || 'Failed to connect Google service');
    }

    return responseData;
  },

  async disconnectService(token: string, serviceId: number) {
    const response = await fetch(`${API_BASE_URL}/api/auth/services/`, {
      method: 'DELETE',
      headers: {
        'Authorization': `Token ${token}`,
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ service_id: serviceId }),
    });

    if (!response.ok) {
      throw new Error('Failed to disconnect service');
    }

    return response.json();
  },
};

// AREA API
export const areaAPI = {
  async getAreas(token: string) {
    const response = await fetch(`${API_BASE_URL}/api/areas/`, {
      headers: {
        'Authorization': `Token ${token}`,
      },
    });

    if (!response.ok) {
      throw new Error('Failed to fetch areas');
    }

    return response.json();
  },

  async createArea(token: string, areaData: any) {
    const response = await fetch(`${API_BASE_URL}/api/areas/`, {
      method: 'POST',
      headers: {
        'Authorization': `Token ${token}`,
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(areaData),
    });

    const responseData = await response.json();

    if (!response.ok) {
      throw new Error(responseData.error || 'Failed to create area');
    }

    return responseData;
  },

  async deleteArea(token: string, areaId: number) {
    const response = await fetch(`${API_BASE_URL}/api/areas/${areaId}/`, {
      method: 'DELETE',
      headers: {
        'Authorization': `Token ${token}`,
      },
    });

    if (!response.ok) {
      throw new Error('Failed to delete area');
    }

    return response.json();
  },

  async toggleArea(token: string, areaId: number, isActive: boolean) {
    const response = await fetch(`${API_BASE_URL}/api/areas/${areaId}/`, {
      method: 'PATCH',
      headers: {
        'Authorization': `Token ${token}`,
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ is_active: isActive }),
    });

    if (!response.ok) {
      throw new Error('Failed to toggle area');
    }

    return response.json();
  },
};
