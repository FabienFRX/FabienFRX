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
  date_joined: string;
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

  async loginWithLinkedIn(code: string, redirectUri?: string): Promise<AuthResponse> {
    const response = await fetch(`${API_BASE_URL}/api/auth/linkedin/login/`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        code,
        redirect_uri: redirectUri
      }),
    });

    const responseData = await response.json();

    if (!response.ok) {
      throw new Error(responseData.error || 'LinkedIn login failed');
    }

    return responseData;
  },

  async loginWithGitHub(code: string, redirectUri?: string): Promise<AuthResponse> {
    const response = await fetch(`${API_BASE_URL}/api/auth/github/login/`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        code,
        redirect_uri: redirectUri
      }),
    });

    const responseData = await response.json();

    if (!response.ok) {
      throw new Error(responseData.error || 'GitHub login failed');
    }

    return responseData;
  },

  async loginWithGitLab(code: string, redirectUri?: string): Promise<AuthResponse> {
    const response = await fetch(`${API_BASE_URL}/api/auth/gitlab/login/`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        code,
        redirect_uri: redirectUri
      }),
    });

    const responseData = await response.json();

    if (!response.ok) {
      throw new Error(responseData.error || 'GitLab login failed');
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
  id: number;
  name: string;
  description: string;
  actions: Action[];
  reactions: Reaction[];
}

export const servicesAPI = {
  async getAvailableServices(token: string): Promise<{ services: ServiceWithDetails[] }> {
    console.log('Fetching services from:', `${API_BASE_URL}/services/`);

    // Only include Authorization header if token is valid and not empty
    const headers: Record<string, string> = {};
    if (token && token.trim() !== '' && token !== 'null' && token !== 'undefined') {
      headers['Authorization'] = `Token ${token}`;
    }

    const response = await fetch(`${API_BASE_URL}/services/`, {
      headers,
    });

    console.log('Response status:', response.status);

    if (!response.ok) {
      const errorText = await response.text();
      console.error('Error response:', errorText);
      throw new Error('Failed to fetch services');
    }

    const data = await response.json();
    console.log('Services data received:', data);
    return data;
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
    authorizationCode: string
  ) {
    const response = await fetch(`${API_BASE_URL}/api/auth/google/connect/`, {
      method: 'POST',
      headers: {
        'Authorization': `Token ${token}`,
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        code: authorizationCode,
      }),
    });

    const responseData = await response.json();

    if (!response.ok) {
      throw new Error(responseData.error || 'Failed to connect Google service');
    }

    return responseData;
  },

  async connectLinkedInService(
    token: string,
    code: string,
    redirectUri: string
  ): Promise<any> {
    const response = await fetch(`${API_BASE_URL}/api/auth/linkedin/connect/`, {
      method: 'POST',
      headers: {
        'Authorization': `Token ${token}`,
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        code,
        redirect_uri: redirectUri
      }),
    });

    const responseData = await response.json();

    if (!response.ok) {
      throw new Error(responseData.error || 'Failed to connect LinkedIn service');
    }

    return responseData;
  },

  async disconnectService(token: string, serviceId: number) {
    const response = await fetch(`${API_BASE_URL}/api/auth/services/${serviceId}/`, {
      method: 'DELETE',
      headers: {
        'Authorization': `Token ${token}`,
        'Content-Type': 'application/json',
      },
    });

    if (!response.ok) {
      throw new Error('Failed to disconnect service');
    }

    return response.json();
  },

  async connectGitHubService(
    token: string,
    code: string,
    redirectUri: string
  ): Promise<any> {
    const response = await fetch(`${API_BASE_URL}/api/auth/github/connect/`, {
      method: 'POST',
      headers: {
        'Authorization': `Token ${token}`,
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        code,
        redirect_uri: redirectUri
      }),
    });

    const responseData = await response.json();

    if (!response.ok) {
      throw new Error(responseData.error || 'Failed to connect GitHub service');
    }

    return responseData;
  },
};

// AREA API
export const areaAPI = {
  async getAreas(token: string) {
    const response = await fetch(`${API_BASE_URL}/areas/`, {
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
    const response = await fetch(`${API_BASE_URL}/areas/`, {
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
    const response = await fetch(`${API_BASE_URL}/areas/${areaId}/`, {
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
    const response = await fetch(`${API_BASE_URL}/areas/${areaId}/`, {
      method: 'PATCH',
      headers: {
        'Authorization': `Token ${token}`,
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ enabled: isActive }),
    });

    if (!response.ok) {
      throw new Error('Failed to toggle area');
    }

    return response.json();
  },

  // Workflow Executor API
  async getExecutorStatus(token: string) {
    const response = await fetch(`${API_BASE_URL}/executor/`, {
      headers: {
        'Authorization': `Token ${token}`,
      },
    });

    if (!response.ok) {
      throw new Error('Failed to get executor status');
    }

    return response.json();
  },

  async startExecutor(token: string) {
    const response = await fetch(`${API_BASE_URL}/executor/`, {
      method: 'POST',
      headers: {
        'Authorization': `Token ${token}`,
      },
    });

    if (!response.ok) {
      throw new Error('Failed to start executor');
    }

    return response.json();
  },

  async stopExecutor(token: string) {
    const response = await fetch(`${API_BASE_URL}/executor/`, {
      method: 'DELETE',
      headers: {
        'Authorization': `Token ${token}`,
      },
    });

    if (!response.ok) {
      throw new Error('Failed to stop executor');
    }

    return response.json();
  },

  async connectGitLabService(
    token: string,
    code: string,
    redirectUri: string
  ): Promise<any> {
    const response = await fetch(`${API_BASE_URL}/api/auth/gitlab/connect/`, {
      method: 'POST',
      headers: {
        'Authorization': `Token ${token}`,
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        code,
        redirect_uri: redirectUri
      }),
    });

    const responseData = await response.json();

    if (!response.ok) {
      throw new Error(responseData.error || 'Failed to connect GitLab service');
    }

    return responseData;
  },
};
