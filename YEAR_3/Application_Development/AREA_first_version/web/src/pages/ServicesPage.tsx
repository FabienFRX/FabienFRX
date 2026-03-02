import React, { useEffect, useState } from 'react';
import { useAuth } from '../contexts/AuthContext';
import { servicesAPI, ServiceWithDetails, Action, Reaction } from '../services/api';
import { useGoogleLogin } from '@react-oauth/google';
import { CURRENT_GOOGLE_SCOPES } from '../config/constants';

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
  const [connectedServices, setConnectedServices] = useState<ConnectedService[]>([]);
  const [availableServices, setAvailableServices] = useState<ServiceWithDetails[]>([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [success, setSuccess] = useState<string | null>(null);

  useEffect(() => {
    loadConnectedServices();
    loadAvailableServices();
  }, []);

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

  const handleGoogleConnect = useGoogleLogin({
    onSuccess: async (tokenResponse) => {
      setError(null);
      setSuccess(null);
      setLoading(true);
      
      try {
        // Get user info from Google
        const userInfoResponse = await fetch('https://www.googleapis.com/oauth2/v2/userinfo', {
          headers: { Authorization: `Bearer ${tokenResponse.access_token}` },
        });
        const userInfo = await userInfoResponse.json();

        // Connect service
        await servicesAPI.connectGoogleService(
          token!, 
          tokenResponse.access_token, 
          userInfo,
          tokenResponse.expires_in
        );
        setSuccess('Google service connected successfully!');
        loadConnectedServices();
      } catch (err: any) {
        setError(err.message || 'Failed to connect Google service');
      } finally {
        setLoading(false);
      }
    },
    onError: () => {
      setError('Google connection was cancelled or failed');
    },
    scope: CURRENT_GOOGLE_SCOPES,
  });

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
                  if (googleService) handleDisconnect(googleService.service_id);
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
