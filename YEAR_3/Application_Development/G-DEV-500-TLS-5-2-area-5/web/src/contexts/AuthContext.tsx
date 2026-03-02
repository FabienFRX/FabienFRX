import React, { createContext, useContext, useState, useEffect, ReactNode } from 'react';
import { authAPI, User } from '../services/api';

interface AuthContextType {
  user: User | null;
  token: string | null;
  isAuthenticated: boolean;
  isLoading: boolean;
  login: (email: string, password: string) => Promise<void>;
  loginWithGoogle: (accessToken: string, expiresIn?: number) => Promise<void>;
  loginWithGoogleAuthCode: (authCode: string) => Promise<any>;
  loginWithLinkedIn: (code: string, redirectUri: string) => Promise<void>;
  loginWithGitHub: (code: string, redirectUri: string) => Promise<void>;
  loginWithGitLab: (code: string, redirectUri: string) => Promise<void>;
  register: (email: string, password: string, firstName?: string, lastName?: string) => Promise<void>;
  logout: () => Promise<void>;
  handleAuthError: () => void;
  setAuthToken: (token: string) => void;
}

const AuthContext = createContext<AuthContextType | undefined>(undefined);

export const useAuth = () => {
  const context = useContext(AuthContext);
  if (!context) {
    throw new Error('useAuth must be used within an AuthProvider');
  }
  return context;
};

interface AuthProviderProps {
  children: ReactNode;
}

export const AuthProvider: React.FC<AuthProviderProps> = ({ children }) => {
  const [user, setUser] = useState<User | null>(null);
  const [token, setToken] = useState<string | null>(null);
  const [isLoading, setIsLoading] = useState(true);

  // Check if user is already logged in on mount
  useEffect(() => {
    const storedToken = localStorage.getItem('auth_token');
    const storedUser = localStorage.getItem('user');

    if (storedToken && storedUser && storedToken !== 'null' && storedToken !== 'undefined') {
      try {
        const parsedUser = JSON.parse(storedUser);
        // Only set if both token and user are valid
        if (parsedUser && typeof parsedUser === 'object') {
          setToken(storedToken);
          setUser(parsedUser);
        } else {
          // Clear invalid data
          localStorage.removeItem('auth_token');
          localStorage.removeItem('user');
        }
      } catch (error) {
        console.error('Failed to parse stored user:', error);
        localStorage.removeItem('auth_token');
        localStorage.removeItem('user');
      }
    } else if (storedToken || storedUser) {
      // Clean up incomplete or invalid stored data
      localStorage.removeItem('auth_token');
      localStorage.removeItem('user');
    }
    setIsLoading(false);
  }, []);

  const login = async (email: string, password: string) => {
    const response = await authAPI.login({ email, password });
    
    // Store token and user
    localStorage.setItem('auth_token', response.token);
    localStorage.setItem('user', JSON.stringify(response.user));
    
    setToken(response.token);
    setUser(response.user);
  };

  const loginWithGoogle = async (accessToken: string, expiresIn?: number) => {
    const response = await authAPI.loginWithGoogle(accessToken, expiresIn);

    // Store token and user
    localStorage.setItem('auth_token', response.token);
    localStorage.setItem('user', JSON.stringify(response.user));

    setToken(response.token);
    setUser(response.user);
  };

  const loginWithGoogleAuthCode = async (authCode: string) => {
    // Exchange authorization code for tokens
    const apiUrl = window.location.hostname === 'localhost' ? 'http://localhost:8080' : 'http://localhost:8080';
    const redirectUri = window.location.origin; // Use the current origin as redirect_uri

    const response = await fetch(`${apiUrl}/api/auth/google/login/`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'Accept': 'application/json'
      },
      mode: 'cors',
      body: JSON.stringify({
        authorization_code: authCode,
        redirect_uri: redirectUri  // Send redirect_uri to backend
      }),
    });

    if (!response.ok) {
      const errorData = await response.json();
      throw new Error(errorData.error || 'Google login failed');
    }

    const data = await response.json();

    // Store token and user
    localStorage.setItem('auth_token', data.token);
    localStorage.setItem('user', JSON.stringify(data.user));

    setToken(data.token);
    setUser(data.user);

    return data;
  };

  const loginWithLinkedIn = async (code: string, redirectUri: string) => {
    const response = await authAPI.loginWithLinkedIn(code, redirectUri);

    // Store token and user
    localStorage.setItem('auth_token', response.token);
    localStorage.setItem('user', JSON.stringify(response.user));

    setToken(response.token);
    setUser(response.user);
  };

  const loginWithGitHub = async (code: string, redirectUri: string) => {
    const response = await authAPI.loginWithGitHub(code, redirectUri);

    // Store token and user
    localStorage.setItem('auth_token', response.token);
    localStorage.setItem('user', JSON.stringify(response.user));

    setToken(response.token);
    setUser(response.user);
  };

  const loginWithGitLab = async (code: string, redirectUri: string) => {
    const response = await authAPI.loginWithGitLab(code, redirectUri);

    // Store token and user
    localStorage.setItem('auth_token', response.token);
    localStorage.setItem('user', JSON.stringify(response.user));

    setToken(response.token);
    setUser(response.user);
  };

  const register = async (email: string, password: string, firstName?: string, lastName?: string) => {
    await authAPI.register({
      email,
      password,
      first_name: firstName,
      last_name: lastName,
    });
    // After registration, user needs to login
  };

  const logout = async () => {
    if (token) {
      try {
        await authAPI.logout(token);
      } catch (error) {
        console.error('Logout error:', error);
      }
    }

    // Clear storage and state
    localStorage.removeItem('auth_token');
    localStorage.removeItem('user');
    setToken(null);
    setUser(null);
  };

  const handleAuthError = () => {
    // Clear invalid tokens when we get 401/403 errors
    console.log('Authentication error - clearing stored credentials');
    localStorage.removeItem('auth_token');
    localStorage.removeItem('user');
    setToken(null);
    setUser(null);
  };

  const setAuthToken = (newToken: string) => {
    setToken(newToken);
    localStorage.setItem('auth_token', newToken);
    
    // Try to load user from localStorage if not already set
    if (!user) {
      const storedUser = localStorage.getItem('user');
      if (storedUser) {
        try {
          setUser(JSON.parse(storedUser));
        } catch (e) {
          console.error('Failed to parse stored user');
        }
      }
    }
  };

  const value: AuthContextType = {
    user,
    token,
    isAuthenticated: !!token && !!user,
    isLoading,
    login,
    loginWithGoogle,
    loginWithGoogleAuthCode,
    loginWithLinkedIn,
    loginWithGitHub,
    loginWithGitLab,
    register,
    logout,
    handleAuthError,
    setAuthToken,
  };

  return <AuthContext.Provider value={value}>{children}</AuthContext.Provider>;
};
