import React, { useEffect, useState } from 'react';
import { useLocation } from 'react-router-dom';
import { useAuth } from '../contexts/AuthContext';
import { servicesAPI, ServiceWithDetails, Action, Reaction } from '../services/api';
import { API_BASE_URL } from '../config/constants';

interface ConnectedService {
  id: number;
  service_id: number;
  service_name: string;
  service_description: string;
  service_username: string;
  is_active: boolean;
}

const ServicesPage: React.FC = () => {
  const { token } = useAuth();
  const location = useLocation();
  const [connectedServices, setConnectedServices] = useState<ConnectedService[]>([]);
  const [availableServices, setAvailableServices] = useState<ServiceWithDetails[]>([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [success, setSuccess] = useState<string | null>(null);

  useEffect(() => {
    loadConnectedServices();
    loadAvailableServices();
    
    // Check for location.state callbacks (GitHub, LinkedIn)
    if (location.state?.success) {
      setSuccess(location.state.success);
      window.history.replaceState({}, document.title);
    }
    if (location.state?.error) {
      setError(location.state.error);
      window.history.replaceState({}, document.title);
    }

    // Check for Dropbox connection callback via URL params
    const urlParams = new URLSearchParams(window.location.search);
    if (urlParams.get('dropbox_connected') === 'true') {
      setSuccess('Dropbox connected successfully!');
      loadConnectedServices();
      // Clean URL
      window.history.replaceState({}, '', '/services');
    } else if (urlParams.get('dropbox_error')) {
      setError(`Dropbox connection failed: ${urlParams.get('dropbox_error')}`);
      window.history.replaceState({}, '', '/services');
    }
  }, [location]);

  const loadConnectedServices = async () => {
    if (!token) return;
    
    try {
      const data = await servicesAPI.getUserServices(token);
      setConnectedServices(data.services || []);
    } catch (err: any) {
      console.error('Failed to load services:', err);
    }
  };

  const loadAvailableServices = async () => {
    if (!token) return;
    
    try {
      const data = await servicesAPI.getAvailableServices(token);
      setAvailableServices(data.services || []);
    } catch (err: any) {
      console.error('Failed to load available services:', err);
    }
  };

  const handleGoogleConnect = () => {
    // Simply redirect to backend OAuth flow (will connect to existing user)
    window.location.href = `${API_BASE_URL}/api/auth/google/init/`;
  };

  const handleLinkedInConnect = () => {
    // Simply redirect to backend OAuth flow
    window.location.href = `${API_BASE_URL}/api/auth/linkedin/init/`;
  };

  const handleDropboxConnect = async () => {
    if (!token) return;
    
    setLoading(true);
    setError(null);
    
    try {
      // Get Dropbox OAuth URL from backend
      const response = await fetch(`${API_BASE_URL}/api/auth/dropbox/init/`, {
        headers: {
          'Authorization': `Token ${token}`
        }
      });
      
      const data = await response.json();
      
      if (data.auth_url) {
        // Redirect to Dropbox OAuth
        window.location.href = data.auth_url;
      } else {
        throw new Error('Failed to get Dropbox authorization URL');
      }
    } catch (err: any) {
      setError(err.message || 'Failed to connect to Dropbox');
      setLoading(false);
    }
  };

  const handleGitHubConnect = () => {
    // Simply redirect to backend OAuth flow
    window.location.href = `${API_BASE_URL}/api/auth/github/init/`;
  };

  const handleGitLabConnect = () => {
    // Simply redirect to backend OAuth flow
    window.location.href = `${API_BASE_URL}/api/auth/gitlab/init/`;
  };

  const handleDisconnect = async (serviceId: number) => {
    if (!token) return;
    
    setError(null);
    setSuccess(null);
    
    try {
      await servicesAPI.disconnectService(token, serviceId);
      setSuccess('Service disconnected successfully!');
      loadConnectedServices();
    } catch (err: any) {
      setError(err.message || 'Failed to disconnect service');
    }
  };

  const isGoogleConnected = connectedServices.some(s => s.service_name === 'google');

  return (
    <main className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
      <div className="mb-8">
        <h1 className="text-3xl font-bold text-gray-900 dark:text-white mb-2">Services</h1>
        <p className="text-gray-600 dark:text-gray-400">
          Connect services to unlock Actions and REActions
        </p>
      </div>

      {/* Info message pour le développement */}
      <div className="mb-6 p-4 bg-blue-50 border border-blue-200 rounded-lg dark:bg-blue-900 dark:border-blue-700">
        <div className="flex items-start gap-3">
          <svg className="w-5 h-5 text-blue-600 dark:text-blue-400 mt-0.5" fill="currentColor" viewBox="0 0 20 20">
            <path fillRule="evenodd" d="M18 10a8 8 0 11-16 0 8 8 0 0116 0zm-7-4a1 1 0 11-2 0 1 1 0 012 0zM9 9a1 1 0 000 2v3a1 1 0 001 1h1a1 1 0 100-2v-3a1 1 0 00-1-1H9z" clipRule="evenodd" />
          </svg>
          <div className="flex-1">
            <h3 className="text-sm font-semibold text-blue-800 dark:text-blue-200 mb-1">Mode Développement</h3>
            <p className="text-sm text-blue-700 dark:text-blue-300">
              Actuellement, seuls les scopes de base Google sont activés. Pour accéder aux fonctionnalités Gmail/Calendar en production, l'application devra être validée par Google ou vous devez être ajouté comme testeur.
            </p>
          </div>
        </div>
      </div>

      {error && (
        <div className="mb-4 p-4 bg-red-100 border border-red-400 text-red-700 rounded dark:bg-red-900 dark:border-red-700 dark:text-red-200">
          {error}
        </div>
      )}

      {success && (
        <div className="mb-4 p-4 bg-green-100 border border-green-400 text-green-700 rounded dark:bg-green-900 dark:border-green-700 dark:text-green-200">
          {success}
        </div>
      )}

      {/* Available Services */}
      <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
        {/* Google Service Card */}
        {(() => {
          const googleService = availableServices.find(s => s.name === 'google');
          const service = googleService || { 
            name: 'google', 
            actions: [], 
            reactions: [] 
          };
          return (
          <div key={service.name} className="bg-white dark:bg-gray-800 rounded-lg shadow-lg p-6 border-2 border-gray-200 dark:border-gray-700">
            <div className="flex items-start justify-between mb-4">
              <div className="flex items-center gap-3">
                <div className="w-12 h-12 bg-gradient-to-r from-blue-500 to-red-500 rounded-lg flex items-center justify-center">
                  <svg className="w-8 h-8 text-white" viewBox="0 0 24 24">
                    <path fill="currentColor" d="M22.56 12.25c0-.78-.07-1.53-.2-2.25H12v4.26h5.92c-.26 1.37-1.04 2.53-2.21 3.31v2.77h3.57c2.08-1.92 3.28-4.74 3.28-8.09z"/>
                    <path fill="currentColor" d="M12 23c2.97 0 5.46-.98 7.28-2.66l-3.57-2.77c-.98.66-2.23 1.06-3.71 1.06-2.86 0-5.29-1.93-6.16-4.53H2.18v2.84C3.99 20.53 7.7 23 12 23z"/>
                    <path fill="currentColor" d="M5.84 14.09c-.22-.66-.35-1.36-.35-2.09s.13-1.43.35-2.09V7.07H2.18C1.43 8.55 1 10.22 1 12s.43 3.45 1.18 4.93l2.85-2.22.81-.62z"/>
                    <path fill="currentColor" d="M12 5.38c1.62 0 3.06.56 4.21 1.64l3.15-3.15C17.45 2.09 14.97 1 12 1 7.7 1 3.99 3.47 2.18 7.07l3.66 2.84c.87-2.6 3.3-4.53 6.16-4.53z"/>
                  </svg>
                </div>
                <div>
                  <h3 className="text-lg font-semibold text-gray-900 dark:text-white">Google</h3>
                  {isGoogleConnected && (
                    <span className="inline-flex items-center px-2 py-1 text-xs font-medium text-green-700 bg-green-100 rounded-full dark:bg-green-900 dark:text-green-200">
                      Connected
                    </span>
                  )}
                </div>
              </div>
            </div>

            <p className="text-sm text-gray-600 dark:text-gray-400 mb-4">
              Access Gmail, Calendar and more for powerful automation
            </p>

            {service.actions && service.actions.length > 0 && (
              <div className="space-y-2 mb-4">
                <h4 className="text-xs font-semibold text-gray-500 dark:text-gray-400 uppercase">Available Actions:</h4>
                <ul className="text-sm text-gray-700 dark:text-gray-300 space-y-1">
                  {service.actions.map((action) => (
                    <li key={action.name} title={action.description}>
                      • {action.description}
                    </li>
                  ))}
                </ul>
              </div>
            )}

            {service.reactions && service.reactions.length > 0 && (
              <div className="space-y-2 mb-6">
                <h4 className="text-xs font-semibold text-gray-500 dark:text-gray-400 uppercase">Available REActions:</h4>
                <ul className="text-sm text-gray-700 dark:text-gray-300 space-y-1">
                  {service.reactions.map((reaction) => (
                    <li key={reaction.name} title={reaction.description}>
                      • {reaction.description}
                    </li>
                  ))}
                </ul>
              </div>
            )}

            {!isGoogleConnected ? (
              <button
                onClick={() => handleGoogleConnect()}
                disabled={loading}
                className="w-full px-4 py-2 bg-indigo-600 text-white rounded-md hover:bg-indigo-700 transition disabled:opacity-50 disabled:cursor-not-allowed"
              >
                {loading ? 'Connecting...' : 'Connect Google'}
              </button>
            ) : (
              <button
                onClick={() => {
                  const googleService = connectedServices.find(s => s.service_name === 'google');
                  if (googleService) handleDisconnect(googleService.id);
                }}
                className="w-full px-4 py-2 bg-red-600 text-white rounded-md hover:bg-red-700 transition"
              >
                Disconnect
              </button>
            )}
          </div>
        );
        })()}

        {/* GitHub Service Card */}
        {(() => {
          const githubService = availableServices.find(s => s.name === 'github');
          const service = githubService || {
            name: 'github',
            actions: [],
            reactions: []
          };
          const isGitHubConnected = connectedServices.some(s => s.service_name === 'github');

          return (
          <div key={service.name} className="bg-white dark:bg-gray-800 rounded-lg shadow-lg p-6 border-2 border-gray-200 dark:border-gray-700">
            <div className="flex items-start justify-between mb-4">
              <div className="flex items-center gap-3">
                <div className="w-12 h-12 bg-gradient-to-r from-gray-700 to-gray-900 rounded-lg flex items-center justify-center">
                  <svg className="w-8 h-8 text-white fill-current" viewBox="0 0 24 24">
                    <path d="M12 0C5.37 0 0 5.37 0 12c0 5.31 3.435 9.795 8.205 11.385.6.105.825-.255.825-.57 0-.285-.015-1.23-.015-2.235-3.015.555-3.795-.735-4.035-1.41-.135-.345-.72-1.41-1.23-1.695-.42-.225-1.02-.78-.015-.795.945-.015 1.62.87 1.845 1.23 1.08 1.815 2.805 1.305 3.495.99.105-.78.42-1.305.765-1.605-2.67-.3-5.46-1.335-5.46-5.925 0-1.305.465-2.385 1.23-3.225-.12-.3-.54-1.53.12-3.18 0 0 1.005-.315 3.3 1.23.96-.27 1.98-.405 3-.405s2.04.135 3 .405c2.295-1.56 3.3-1.23 3.3-1.23.66 1.65.24 2.88.12 3.18.765.84 1.23 1.905 1.23 3.225 0 4.605-2.805 5.625-5.475 5.925.435.375.81 1.095.81 2.22 0 1.605-.015 2.895-.015 3.3 0 .315.225.69.825.57A12.02 12.02 0 0024 12c0-6.63-5.37-12-12-12z"/>
                  </svg>
                </div>
                <div>
                  <h3 className="text-lg font-semibold text-gray-900 dark:text-white">GitHub</h3>
                  {isGitHubConnected && (
                    <span className="inline-flex items-center px-2 py-1 text-xs font-medium text-green-700 bg-green-100 rounded-full dark:bg-green-900 dark:text-green-200">
                      Connected
                    </span>
                  )}
                </div>
              </div>
            </div>

            <p className="text-sm text-gray-600 dark:text-gray-400 mb-4">
              Manage repositories, issues, and pull requests
            </p>

            {service.actions && service.actions.length > 0 && (
              <div className="space-y-2 mb-4">
                <h4 className="text-xs font-semibold text-gray-500 dark:text-gray-400 uppercase">Available Actions:</h4>
                <ul className="text-sm text-gray-700 dark:text-gray-300 space-y-1">
                  {service.actions.map((action) => (
                    <li key={action.name} title={action.description}>
                      • {action.description}
                    </li>
                  ))}
                </ul>
              </div>
            )}

            {service.reactions && service.reactions.length > 0 && (
              <div className="space-y-2 mb-6">
                <h4 className="text-xs font-semibold text-gray-500 dark:text-gray-400 uppercase">Available REActions:</h4>
                <ul className="text-sm text-gray-700 dark:text-gray-300 space-y-1">
                  {service.reactions.map((reaction) => (
                    <li key={reaction.name} title={reaction.description}>
                      • {reaction.description}
                    </li>
                  ))}
                </ul>
              </div>
            )}

            {isGitHubConnected ? (
              <div className="space-y-2">
                <div className="px-4 py-2 bg-green-50 border border-green-200 rounded-md dark:bg-green-900 dark:border-green-700">
                  <p className="text-sm text-green-700 dark:text-green-200">
                    Already connected via login
                  </p>
                </div>
                <button
                  onClick={() => {
                    const githubService = connectedServices.find(s => s.service_name === 'github');
                    if (githubService) handleDisconnect(githubService.id);
                  }}
                  className="w-full px-4 py-2 bg-red-600 text-white rounded-md hover:bg-red-700 transition"
                >
                  Disconnect
                </button>
              </div>
            ) : (
              <button
                onClick={handleGitHubConnect}
                disabled={loading}
                className="w-full px-4 py-2 bg-gray-900 text-white rounded-md hover:bg-gray-800 transition disabled:opacity-50 disabled:cursor-not-allowed"
              >
                {loading ? 'Connecting...' : 'Connect GitHub'}
              </button>
            )}
          </div>
          );
        })()}

        {/* GitLab Service Card */}
        {(() => {
          const gitlabService = availableServices.find(s => s.name === 'gitlab');
          const service = gitlabService || {
            name: 'gitlab',
            actions: [],
            reactions: []
          };
          const isGitLabConnected = connectedServices.some(s => s.service_name === 'gitlab');

          return (
          <div key={service.name} className="bg-white dark:bg-gray-800 rounded-lg shadow-lg p-6 border-2 border-gray-200 dark:border-gray-700">
            <div className="flex items-start justify-between mb-4">
              <div className="flex items-center gap-3">
                <div className="w-12 h-12 bg-gradient-to-r from-orange-600 to-orange-500 rounded-lg flex items-center justify-center">
                  <svg className="w-8 h-8 text-white fill-current" viewBox="0 0 24 24">
                    <path d="M23.955 13.587l-1.342-4.135-2.664-8.189a.455.455 0 00-.867 0L16.418 9.45H7.582L4.919 1.263a.455.455 0 00-.867 0L1.388 9.452.045 13.587a.924.924 0 00.331 1.03l11.625 8.443 11.625-8.443a.92.92 0 00.329-1.03"/>
                  </svg>
                </div>
                <div>
                  <h3 className="text-lg font-semibold text-gray-900 dark:text-white">GitLab</h3>
                  {isGitLabConnected && (
                    <span className="inline-flex items-center px-2 py-1 text-xs font-medium text-green-700 bg-green-100 rounded-full dark:bg-green-900 dark:text-green-200">
                      Connected
                    </span>
                  )}
                </div>
              </div>
            </div>

            <p className="text-sm text-gray-600 dark:text-gray-400 mb-4">
              Manage projects, issues, and merge requests
            </p>

            {service.actions && service.actions.length > 0 && (
              <div className="space-y-2 mb-4">
                <h4 className="text-xs font-semibold text-gray-500 dark:text-gray-400 uppercase">Available Actions:</h4>
                <ul className="text-sm text-gray-700 dark:text-gray-300 space-y-1">
                  {service.actions.map((action) => (
                    <li key={action.name} title={action.description}>
                      • {action.description}
                    </li>
                  ))}
                </ul>
              </div>
            )}

            {service.reactions && service.reactions.length > 0 && (
              <div className="space-y-2 mb-6">
                <h4 className="text-xs font-semibold text-gray-500 dark:text-gray-400 uppercase">Available REActions:</h4>
                <ul className="text-sm text-gray-700 dark:text-gray-300 space-y-1">
                  {service.reactions.map((reaction) => (
                    <li key={reaction.name} title={reaction.description}>
                      • {reaction.description}
                    </li>
                  ))}
                </ul>
              </div>
            )}

            {isGitLabConnected ? (
              <div className="space-y-2">
                <div className="px-4 py-2 bg-green-50 border border-green-200 rounded-md dark:bg-green-900 dark:border-green-700">
                  <p className="text-sm text-green-700 dark:text-green-200">
                    Already connected via login
                  </p>
                </div>
                <button
                  onClick={() => {
                    const gitlabService = connectedServices.find(s => s.service_name === 'gitlab');
                    if (gitlabService) handleDisconnect(gitlabService.id);
                  }}
                  className="w-full px-4 py-2 bg-red-600 text-white rounded-md hover:bg-red-700 transition"
                >
                  Disconnect
                </button>
              </div>
            ) : (
              <button
                onClick={handleGitLabConnect}
                disabled={loading}
                className="w-full px-4 py-2 bg-gradient-to-r from-orange-600 to-orange-500 text-white rounded-md hover:from-orange-700 hover:to-orange-600 transition disabled:opacity-50 disabled:cursor-not-allowed"
              >
                {loading ? 'Connecting...' : 'Connect GitLab'}
              </button>
            )}
          </div>
          );
        })()}

        {/* LinkedIn Service Card */}
        {(() => {
          const linkedinService = availableServices.find(s => s.name === 'linkedin');
          const service = linkedinService || {
            name: 'linkedin',
            actions: [],
            reactions: []
          };
          const isLinkedInConnected = connectedServices.some(s => s.service_name === 'linkedin');

          return (
          <div key={service.name} className="bg-white dark:bg-gray-800 rounded-lg shadow-lg p-6 border-2 border-gray-200 dark:border-gray-700">
            <div className="flex items-start justify-between mb-4">
              <div className="flex items-center gap-3">
                <div className="w-12 h-12 bg-blue-600 rounded-lg flex items-center justify-center">
                  <svg className="w-8 h-8 text-white" viewBox="0 0 24 24">
                     <path fill="currentColor" d="M19 0h-14c-2.761 0-5 2.239-5 5v14c0 2.761 2.239 5 5 5h14c2.762 0 5-2.239 5-5v-14c0-2.761-2.238-5-5-5zm-11 19h-3v-11h3v11zm-1.5-12.268c-.966 0-1.75-.79-1.75-1.764s.784-1.764 1.75-1.764 1.75.79 1.75 1.764-.783 1.764-1.75 1.764zm13.5 12.268h-3v-5.604c0-3.368-4-3.113-4 0v5.604h-3v-11h3v1.765c1.396-2.586 7-2.777 7 2.476v6.759z"/>
                  </svg>
                </div>
                <div>
                  <h3 className="text-lg font-semibold text-gray-900 dark:text-white">LinkedIn</h3>
                  {isLinkedInConnected && (
                    <span className="inline-flex items-center px-2 py-1 text-xs font-medium text-green-700 bg-green-100 rounded-full dark:bg-green-900 dark:text-green-200">
                      Connected
                    </span>
                  )}
                </div>
              </div>
            </div>

            <p className="text-sm text-gray-600 dark:text-gray-400 mb-4">
              Connect to your professional network and share updates
            </p>

            {service.actions && service.actions.length > 0 && (
              <div className="space-y-2 mb-4">
                <h4 className="text-xs font-semibold text-gray-500 dark:text-gray-400 uppercase">Available Actions:</h4>
                <ul className="text-sm text-gray-700 dark:text-gray-300 space-y-1">
                  {service.actions.map((action) => (
                    <li key={action.name} title={action.description}>
                      • {action.description}
                    </li>
                  ))}
                </ul>
              </div>
            )}

            {service.reactions && service.reactions.length > 0 && (
              <div className="space-y-2 mb-6">
                <h4 className="text-xs font-semibold text-gray-500 dark:text-gray-400 uppercase">Available REActions:</h4>
                <ul className="text-sm text-gray-700 dark:text-gray-300 space-y-1">
                  {service.reactions.map((reaction) => (
                    <li key={reaction.name} title={reaction.description}>
                      • {reaction.description}
                    </li>
                  ))}
                </ul>
              </div>
            )}

            {!isLinkedInConnected ? (
              <button
                onClick={handleLinkedInConnect}
                disabled={loading}
                className="w-full px-4 py-2 bg-blue-700 text-white rounded-md hover:bg-blue-800 transition disabled:opacity-50 disabled:cursor-not-allowed"
              >
                {loading ? 'Connecting...' : 'Connect LinkedIn'}
              </button>
            ) : (
              <button
                onClick={() => {
                  const linkedinService = connectedServices.find(s => s.service_name === 'linkedin');
                  if (linkedinService) handleDisconnect(linkedinService.id);
                }}
                className="w-full px-4 py-2 bg-red-600 text-white rounded-md hover:bg-red-700 transition"
              >
                Disconnect
              </button>
            )}
          </div>
          );
        })()}

        {/* Dropbox Service Card */}
        {(() => {
          const dropboxService = availableServices.find(s => s.name === 'dropbox');
          const service = dropboxService || {
            name: 'dropbox',
            actions: [],
            reactions: []
          };
          const isDropboxConnected = connectedServices.some(s => s.service_name === 'dropbox');

          return (
          <div key={service.name} className="bg-white dark:bg-gray-800 rounded-lg shadow-lg p-6 border-2 border-gray-200 dark:border-gray-700">
            <div className="flex items-start justify-between mb-4">
              <div className="flex items-center gap-3">
                <div className="w-12 h-12 bg-gradient-to-r from-blue-600 to-blue-400 rounded-lg flex items-center justify-center">
                  <svg className="w-8 h-8 text-white" viewBox="0 0 24 24">
                    <path fill="currentColor" d="M6 1.807L0 5.63l6 3.822 6-3.822-6-3.822zM18 1.807l-6 3.822 6 3.822 6-3.822-6-3.822zM0 13.274l6 3.822 6-3.822-6-3.823-6 3.823zM18 9.451l-6 3.823 6 3.822 6-3.822-6-3.823zM6 18.371l6-3.823-6-3.823-6 3.823 6 3.823zm6 0l6-3.823-6-3.823-6 3.823 6 3.823z"/>
                  </svg>
                </div>
                <div>
                  <h3 className="text-lg font-semibold text-gray-900 dark:text-white">Dropbox</h3>
                  {isDropboxConnected && (
                    <span className="inline-flex items-center px-2 py-1 text-xs font-medium text-green-700 bg-green-100 rounded-full dark:bg-green-900 dark:text-green-200">
                      Connected
                    </span>
                  )}
                </div>
              </div>
            </div>

            <p className="text-sm text-gray-600 dark:text-gray-400 mb-4">
              Manage and automate your Dropbox files and folders
            </p>

            {service.actions && service.actions.length > 0 && (
              <div className="space-y-2 mb-4">
                <h4 className="text-xs font-semibold text-gray-500 dark:text-gray-400 uppercase">Available Actions:</h4>
                <ul className="text-sm text-gray-700 dark:text-gray-300 space-y-1">
                  {service.actions.map((action) => (
                    <li key={action.name} title={action.description}>
                      • {action.description}
                    </li>
                  ))}
                </ul>
              </div>
            )}

            {service.reactions && service.reactions.length > 0 && (
              <div className="space-y-2 mb-6">
                <h4 className="text-xs font-semibold text-gray-500 dark:text-gray-400 uppercase">Available REActions:</h4>
                <ul className="text-sm text-gray-700 dark:text-gray-300 space-y-1">
                  {service.reactions.map((reaction) => (
                    <li key={reaction.name} title={reaction.description}>
                      • {reaction.description}
                    </li>
                  ))}
                </ul>
              </div>
            )}

            {!isDropboxConnected ? (
              <button
                onClick={() => handleDropboxConnect()}
                disabled={loading}
                className="w-full px-4 py-2 bg-blue-600 text-white rounded-md hover:bg-blue-700 transition disabled:opacity-50 disabled:cursor-not-allowed"
              >
                {loading ? 'Connecting...' : 'Connect Dropbox'}
              </button>
            ) : (
              <button
                onClick={() => {
                  const dropboxService = connectedServices.find(s => s.service_name === 'dropbox');
                  if (dropboxService) handleDisconnect(dropboxService.id);
                }}
                className="w-full px-4 py-2 bg-red-600 text-white rounded-md hover:bg-red-700 transition"
              >
                Disconnect
              </button>
            )}
          </div>
          );
        })()}

        {/* Placeholder for other services */}
        <div className="bg-white dark:bg-gray-800 rounded-lg shadow-lg p-6 border-2 border-dashed border-gray-300 dark:border-gray-600 opacity-50">
          <div className="flex items-center gap-3 mb-4">
            <div className="w-12 h-12 bg-gray-300 dark:bg-gray-600 rounded-lg"></div>
            <h3 className="text-lg font-semibold text-gray-500 dark:text-gray-400">More services coming soon...</h3>
          </div>
        </div>
      </div>

      {/* Connected Services List */}
      {connectedServices.length > 0 && (
        <div className="mt-8">
          <h2 className="text-2xl font-bold text-gray-900 dark:text-white mb-4">Connected Services</h2>
          <div className="bg-white dark:bg-gray-800 rounded-lg shadow overflow-hidden">
            <ul className="divide-y divide-gray-200 dark:divide-gray-700">
              {connectedServices.map((userService) => (
                <li key={userService.id} className="p-4 hover:bg-gray-50 dark:hover:bg-gray-700">
                  <div className="flex items-center justify-between">
                    <div>
                      <p className="text-sm font-medium text-gray-900 dark:text-white capitalize">
                        {userService.service_name || 'Unknown Service'}
                      </p>
                      <p className="text-sm text-gray-500 dark:text-gray-400">
                        Connected as: {userService.service_username}
                      </p>
                    </div>
                    <span className="text-xs text-gray-500 dark:text-gray-400">
                      {userService.is_active ? 'Active' : 'Inactive'}
                    </span>
                  </div>
                </li>
              ))}
            </ul>
          </div>
        </div>
      )}
    </main>
  );
};

export default ServicesPage;
