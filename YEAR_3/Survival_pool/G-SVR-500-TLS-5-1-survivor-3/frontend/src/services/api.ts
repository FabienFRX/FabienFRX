import axios from 'axios';
import { perfMonitor } from '../utils/performance';

// Environment variables with fallbacks
const API_BASE_URL = import.meta.env.VITE_API_BASE_URL || 'http://localhost:8000/api';
const API_TIMEOUT = parseInt(import.meta.env.VITE_API_TIMEOUT || '10000');

// Types
export interface Founder {
  id: number;
  jeb_id: number;
  name: string;
  image?: string;
  image_url?: string;
}

export interface Startup {
  id: number;
  name: string;
  description: string;
  sector: string;
  maturity: string;
  legal_status: string;
  address: string;
  email: string;
  phone: string;
  website_url: string;
  social_media_url: string;
  project_status: string;
  needs: string;
  progress: string;
  founders: Founder[];
  created_at: string;
  updated_at: string;
  jeb_id: number;
}

export interface Event {
  id: number;
  name: string;
  description: string;
  event_type: string;
  dates: string;
  location: string;
  target_audience: string;
  max_participants: number;
  created_at: string;
  updated_at: string;
  jeb_id: number;
}

export interface News {
  id: number;
  title: string;
  description: string;
  content: string;
  category: string;
  news_date: string;
  location: string;
  created_at: string;
  updated_at: string;
  jeb_id: number;
}

export interface Investor {
  id: number;
  name: string;
  description: string;
  investor_type: string;
  investment_focus: string;
  legal_status: string;
  address: string;
  email: string;
  phone: string;
  created_at: string;
  updated_at: string;
  jeb_id: number;
}

export interface Partner {
  id: number;
  name: string;
  description: string;
  partner_type: string;
  legal_status: string;
  address: string;
  email: string;
  phone: string;
  created_at: string;
  updated_at: string;
  jeb_id: number;
}

export interface User {
  id: number;
  username: string;
  email: string;
  first_name: string;
  last_name: string;
  created_at: string;
  updated_at: string;
  jeb_id: number;
}

// API client
const apiClient = axios.create({
  baseURL: API_BASE_URL,
  timeout: API_TIMEOUT,
  headers: {
    'Content-Type': 'application/json',
  },
});

// Add auth token to requests
apiClient.interceptors.request.use((config) => {
  const token = localStorage.getItem('token');
  if (token) {
    config.headers.Authorization = `Token ${token}`;
  }
  return config;
});

// Add response interceptor for better error handling
apiClient.interceptors.response.use(
  (response) => response,
  (error) => {
    if (error.code === 'ECONNABORTED') {
      console.error('Request timeout');
      error.message = 'Request timed out. Please try again.';
    } else if (!error.response) {
      console.error('Network error');
      error.message = 'Network error. Please check your connection.';
    }
    return Promise.reject(error);
  }
);

// Services with better error handling and performance monitoring
export const startupService = {
  getAll: async (): Promise<Startup[]> => {
    return perfMonitor.measureApiCall('Fetch All Startups', async () => {
      try {
        const response = await apiClient.get('/startups/');
        return response.data;
      } catch (error) {
        console.error('Failed to fetch startups:', error);
        throw new Error('Failed to load startups. Please try again.');
      }
    });
  },
  getById: async (id: number): Promise<Startup> => {
    return perfMonitor.measureApiCall(`Fetch Startup ${id}`, async () => {
      try {
        const response = await apiClient.get(`/startups/${id}/`);
        return response.data;
      } catch (error) {
        console.error('Failed to fetch startup:', error);
        throw new Error('Failed to load startup details. Please try again.');
      }
    });
  },
};

export const eventService = {
  getAll: async (): Promise<Event[]> => {
    const response = await apiClient.get('/events/');
    return response.data;
  },
  getById: async (id: number): Promise<Event> => {
    const response = await apiClient.get(`/events/${id}/`);
    return response.data;
  },
};

export const newsService = {
  getAll: async (): Promise<News[]> => {
    const response = await apiClient.get('/news/');
    return response.data;
  },
  getById: async (id: number): Promise<News> => {
    const response = await apiClient.get(`/news/${id}/`);
    return response.data;
  },
};

export const investorService = {
  getAll: async (): Promise<Investor[]> => {
    const response = await apiClient.get('/investors/');
    return response.data;
  },
  getById: async (id: number): Promise<Investor> => {
    const response = await apiClient.get(`/investors/${id}/`);
    return response.data;
  },
};

export const partnerService = {
  getAll: async (): Promise<Partner[]> => {
    const response = await apiClient.get('/partners/');
    return response.data;
  },
  getById: async (id: number): Promise<Partner> => {
    const response = await apiClient.get(`/partners/${id}/`);
    return response.data;
  },
};

export const userService = {
  getAll: async (): Promise<User[]> => {
    const response = await apiClient.get('/users/');
    return response.data;
  },
  getById: async (id: number): Promise<User> => {
    const response = await apiClient.get(`/users/${id}/`);
    return response.data;
  },
};

// Image URL helpers
export const getImageUrl = {
  startup: (jebId: number) => `http://jeb-api.local/api/startups/${jebId}/image/`,
  event: (jebId: number) => `http://jeb-api.local/api/events/${jebId}/image/`,
  news: (jebId: number) => `http://jeb-api.local/api/news/${jebId}/image/`,
  user: (jebId: number) => `http://jeb-api.local/api/users/${jebId}/image/`,
};

// Export the api client for direct use
export const api = apiClient;

// JEB User service (alias for userService)
export const jebUserService = userService;

// JEB User interface (alias for User)
export interface JEBUser extends User {}
