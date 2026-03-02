// Service pour gérer les images de l'API JEB (redirection directe)
const API_BASE_URL = import.meta.env.VITE_API_URL || 'http://jeb-api.local:8000/api';

export const getImageUrl = (endpoint: string, id: number, subId?: number): string => {
  const baseUrl = API_BASE_URL.replace('/api', ''); // Enlever /api car on va l'ajouter dans l'endpoint
  
  switch (endpoint) {
    case 'news':
      return `${baseUrl}/api/news/${id}/image`;
    case 'events':
      return `${baseUrl}/api/events/${id}/image`;
    case 'users':
      return `${baseUrl}/api/users/${id}/image`;
    case 'startups':
      return `${baseUrl}/api/startups/${id}/image`;
    case 'founders':
      if (!subId) throw new Error('Founder ID is required for founder images');
      return `${baseUrl}/api/startups/${id}/founders/${subId}/image`;
    default:
      throw new Error(`Unknown image endpoint: ${endpoint}`);
  }
};

// Fonction pour vérifier si une image existe (optionnel)
export const checkImageExists = async (url: string): Promise<boolean> => {
  try {
    const response = await fetch(url, { method: 'HEAD' });
    return response.ok;
  } catch {
    return false;
  }
};

// Fonction pour obtenir une image avec fallback
export const getImageWithFallback = async (
  endpoint: string, 
  id: number, 
  subId?: number,
  fallbackUrl: string = '/logo.png'
): Promise<string> => {
  try {
    const imageUrl = getImageUrl(endpoint, id, subId);
    // Pour les redirections, on fait confiance que l'image existe
    // Si elle n'existe pas, le navigateur affichera l'image de fallback
    return imageUrl;
  } catch {
    return fallbackUrl;
  }
};
