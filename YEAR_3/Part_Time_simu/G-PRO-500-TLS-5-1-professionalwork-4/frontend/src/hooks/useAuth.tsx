import { useState, useContext, createContext, ReactNode, useEffect } from 'react';
import { authService, LoginData, RegisterData, UserInfo } from '@/services/auth.service';
import { toast } from '@/hooks/use-toast';
import { devLog, devError } from '@/lib/api.config';

interface AuthContextType {
  isAuthenticated: boolean;
  user: UserInfo | null;
  login: (data: LoginData) => Promise<boolean>;
  register: (data: RegisterData) => Promise<boolean>;
  logout: () => void;
  loading: boolean;
}

const AuthContext = createContext<AuthContextType | undefined>(undefined);

export function AuthProvider({ children }: { children: ReactNode }) {
  const [isAuthenticated, setIsAuthenticated] = useState(false);
  const [user, setUser] = useState<UserInfo | null>(null);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    const loadUser = async () => {
      devLog('[AuthProvider] Chargement initial...');
      setLoading(true);
      if (authService.isAuthenticated()) {
        devLog('[AuthProvider] Token trouvé, récupération des données utilisateur...');
        const userData = await authService.getCurrentUser();
        if (userData) {
          devLog('[AuthProvider] Utilisateur chargé:', userData);
          setUser(userData);
          setIsAuthenticated(true);
        } else {
          devLog('[AuthProvider] Token invalide, déconnexion');
          authService.logout();
          setIsAuthenticated(false);
          setUser(null);
        }
      } else {
        devLog('[AuthProvider] Aucun token trouvé');
      }
      setLoading(false);
    };
    loadUser();
  }, []);

  const login = async (data: LoginData): Promise<boolean> => {
    try {
      devLog('[AuthProvider] Login en cours...');
      setLoading(true);
      const response = await authService.login(data);
      devLog('[AuthProvider] Token reçu:', response.access_token.substring(0, 20) + '...');
      authService.setToken(response.access_token);
      
      const userData = await authService.getCurrentUser();
      devLog('[AuthProvider] Login réussi, données utilisateur:', userData);
      setUser(userData);
      setIsAuthenticated(true);
      
      toast({
        title: "Connexion réussie",
        description: `Bienvenue ${userData?.username || ''} !`,
      });
      
      return true;
    } catch (error) {
      devError('[AuthProvider] Erreur de connexion:', error);
      toast({
        title: "Erreur de connexion",
        description: error instanceof Error ? error.message : "Une erreur est survenue",
        variant: "destructive",
      });
      return false;
    } finally {
      setLoading(false);
    }
  };

  const register = async (data: RegisterData): Promise<boolean> => {
    try {
      devLog('[AuthProvider] Inscription en cours...');
      setLoading(true);
      const response = await authService.register(data);
      devLog('[AuthProvider] Token reçu:', response.access_token.substring(0, 20) + '...');
      authService.setToken(response.access_token);
      
      const userData = await authService.getCurrentUser();
      devLog('[AuthProvider] Inscription réussie, données utilisateur:', userData);
      setUser(userData);
      setIsAuthenticated(true);
      
      toast({
        title: "Inscription réussie",
        description: `Bienvenue ${userData?.username || ''} !`,
      });
      
      return true;
    } catch (error) {
      devError('[AuthProvider] Erreur d\'inscription:', error);
      toast({
        title: "Erreur d'inscription",
        description: error instanceof Error ? error.message : "Une erreur est survenue",
        variant: "destructive",
      });
      return false;
    } finally {
      setLoading(false);
    }
  };

  const logout = () => {
    authService.logout();
    setUser(null);
    setIsAuthenticated(false);
    toast({
      title: "Déconnexion",
      description: "À bientôt !",
    });
  };

  return (
    <AuthContext.Provider value={{ isAuthenticated, user, login, register, logout, loading }}>
      {children}
    </AuthContext.Provider>
  );
}

export function useAuth(): AuthContextType {
  const context = useContext(AuthContext);
  if (!context) {
    throw new Error('useAuth must be used within an AuthProvider');
  }
  return context;
}