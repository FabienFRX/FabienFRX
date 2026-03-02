// Backend API URL - The ONLY configuration needed by the frontend
export const API_BASE_URL = 'http://localhost:8080';

// OAuth redirect URIs
export const GITHUB_REDIRECT_URI = 'http://localhost:8081/github/callback';
export const GITLAB_REDIRECT_URI = 'http://localhost:8081/gitlab/callback';
export const LINKEDIN_REDIRECT_URI = 'http://localhost:8081/linkedin/callback';

// OAuth endpoints - All OAuth logic is handled by the backend
export const OAUTH_ENDPOINTS = {
  google: {
    init: `${API_BASE_URL}/api/auth/google/init/`,
  },
  linkedin: {
    init: `${API_BASE_URL}/api/auth/linkedin/init/`,
  },
  github: {
    init: `${API_BASE_URL}/api/auth/github/init/`,
  },
  gitlab: {
    init: `${API_BASE_URL}/api/auth/gitlab/init/`,
  },
};
