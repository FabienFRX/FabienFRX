import React, { useState, useEffect } from 'react';
import { useAuth } from '../contexts/AuthContext';
import { areaAPI, servicesAPI, ServiceWithDetails, Action, Reaction } from '../services/api';
import { Link } from 'react-router-dom';

interface AREA {
  id: number;
  name: string;
  description: string;
  action_service: string;
  action_type: string;
  reaction_service: string;
  reaction_type: string;
  is_active: boolean;
}

const AREABuilderPage: React.FC = () => {
  const { token } = useAuth();
  const [areas, setAreas] = useState<AREA[]>([]);
  const [connectedServices, setConnectedServices] = useState<any[]>([]);
  const [availableServices, setAvailableServices] = useState<ServiceWithDetails[]>([]);
  const [showCreateForm, setShowCreateForm] = useState(false);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);

  // Form state
  const [formData, setFormData] = useState({
    name: '',
    description: '',
    action_service: 'google',
    action_type: 'new_email_received',
    action_config: {},
    reaction_service: 'google',
    reaction_type: 'send_email',
    reaction_config: {},
  });

  useEffect(() => {
    loadAreas();
    loadConnectedServices();
    loadAvailableServices();
  }, []);

  const loadAreas = async () => {
    if (!token) return;
    
    try {
      const data = await areaAPI.getAreas(token);
      setAreas(data.areas || []);
    } catch (err: any) {
      console.error('Failed to load areas:', err);
    }
  };

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

  const handleCreateArea = async (e: React.FormEvent) => {
    e.preventDefault();
    if (!token) return;

    setError(null);
    setLoading(true);

    try {
      await areaAPI.createArea(token, formData);
      setShowCreateForm(false);
      setFormData({
        name: '',
        description: '',
        action_service: 'google',
        action_type: 'new_email',
        action_config: {},
        reaction_service: 'google',
        reaction_type: 'send_email',
        reaction_config: {},
      });
      loadAreas();
    } catch (err: any) {
      setError(err.message || 'Failed to create AREA');
    } finally {
      setLoading(false);
    }
  };

  const handleToggleArea = async (areaId: number, isActive: boolean) => {
    if (!token) return;

    try {
      await areaAPI.toggleArea(token, areaId, !isActive);
      loadAreas();
    } catch (err: any) {
      setError(err.message || 'Failed to toggle AREA');
    }
  };

  const handleDeleteArea = async (areaId: number) => {
    if (!token || !confirm('Are you sure you want to delete this AREA?')) return;

    try {
      await areaAPI.deleteArea(token, areaId);
      loadAreas();
    } catch (err: any) {
      setError(err.message || 'Failed to delete AREA');
    }
  };

  // Get available actions for selected service
  const getAvailableActions = (): Action[] => {
    const service = availableServices.find(s => s.name === formData.action_service);
    return service?.actions || [];
  };

  // Get available reactions for selected service
  const getAvailableReactions = (): Reaction[] => {
    const service = availableServices.find(s => s.name === formData.reaction_service);
    return service?.reactions || [];
  };

  const hasConnectedServices = connectedServices.length > 0;
  const isGoogleConnected = connectedServices.some(s => s.service_name === 'google');

  if (!hasConnectedServices) {
    return (
      <main className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
        <div className="bg-yellow-50 dark:bg-yellow-900 border border-yellow-200 dark:border-yellow-700 rounded-lg p-6">
          <h2 className="text-xl font-semibold text-yellow-800 dark:text-yellow-200 mb-2">
            No Services Connected
          </h2>
          <p className="text-yellow-700 dark:text-yellow-300 mb-4">
            You need to connect at least one service before creating AREAs.
          </p>
          <Link
            to="/services"
            className="inline-block px-4 py-2 bg-yellow-600 text-white rounded-md hover:bg-yellow-700 transition"
          >
            Go to Services
          </Link>
        </div>
      </main>
    );
  }

  return (
    <main className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
      <div className="flex items-center justify-between mb-8">
        <div>
          <h1 className="text-3xl font-bold text-gray-900 dark:text-white mb-2">My AREAs</h1>
          <p className="text-gray-600 dark:text-gray-400">
            Create automations by connecting Actions and REActions
          </p>
        </div>
        <button
          onClick={() => setShowCreateForm(!showCreateForm)}
          className="px-4 py-2 bg-indigo-600 text-white rounded-md hover:bg-indigo-700 transition"
        >
          {showCreateForm ? 'Cancel' : '+ Create AREA'}
        </button>
      </div>

      {error && (
        <div className="mb-4 p-4 bg-red-100 border border-red-400 text-red-700 rounded dark:bg-red-900 dark:border-red-700 dark:text-red-200">
          {error}
        </div>
      )}

      {/* Create Form */}
      {showCreateForm && (
        <div className="bg-white dark:bg-gray-800 rounded-lg shadow-lg p-6 mb-8">
          <h2 className="text-xl font-semibold text-gray-900 dark:text-white mb-4">Create New AREA</h2>
          <form onSubmit={handleCreateArea} className="space-y-6">
            <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
              <div>
                <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                  AREA Name
                </label>
                <input
                  type="text"
                  value={formData.name}
                  onChange={(e) => setFormData({ ...formData, name: e.target.value })}
                  required
                  className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                  placeholder="My Gmail Automation"
                />
              </div>

              <div>
                <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                  Description
                </label>
                <input
                  type="text"
                  value={formData.description}
                  onChange={(e) => setFormData({ ...formData, description: e.target.value })}
                  className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                  placeholder="Describe what this AREA does"
                />
              </div>
            </div>

            {/* ACTION Section */}
            <div className="border-t border-gray-200 dark:border-gray-700 pt-6">
              <h3 className="text-lg font-semibold text-gray-900 dark:text-white mb-4">
                🔔 ACTION (Trigger)
              </h3>
              <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                <div>
                  <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                    Service
                  </label>
                  <select
                    value={formData.action_service}
                    onChange={(e) => setFormData({ ...formData, action_service: e.target.value })}
                    className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                  >
                    <option value="google">Google (Gmail)</option>
                  </select>
                </div>

                <div>
                  <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                    Action Type
                  </label>
                  <select
                    value={formData.action_type}
                    onChange={(e) => setFormData({ ...formData, action_type: e.target.value })}
                    className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                  >
                    {getAvailableActions().map((action) => (
                      <option key={action.name} value={action.name}>
                        {action.description}
                      </option>
                    ))}
                  </select>
                </div>
              </div>
            </div>

            {/* REACTION Section */}
            <div className="border-t border-gray-200 dark:border-gray-700 pt-6">
              <h3 className="text-lg font-semibold text-gray-900 dark:text-white mb-4">
                ⚡ REACTION (Response)
              </h3>
              <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                <div>
                  <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                    Service
                  </label>
                  <select
                    value={formData.reaction_service}
                    onChange={(e) => setFormData({ ...formData, reaction_service: e.target.value })}
                    className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                  >
                    <option value="google">Google (Gmail)</option>
                  </select>
                </div>

                <div>
                  <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                    REAction Type
                  </label>
                  <select
                    value={formData.reaction_type}
                    onChange={(e) => setFormData({ ...formData, reaction_type: e.target.value })}
                    className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                  >
                    {getAvailableReactions().map((reaction) => (
                      <option key={reaction.name} value={reaction.name}>
                        {reaction.description}
                      </option>
                    ))}
                  </select>
                </div>
              </div>
            </div>

            <div className="flex justify-end gap-3">
              <button
                type="button"
                onClick={() => setShowCreateForm(false)}
                className="px-4 py-2 border border-gray-300 text-gray-700 dark:text-gray-300 dark:border-gray-600 rounded-md hover:bg-gray-50 dark:hover:bg-gray-700 transition"
              >
                Cancel
              </button>
              <button
                type="submit"
                disabled={loading}
                className="px-4 py-2 bg-indigo-600 text-white rounded-md hover:bg-indigo-700 transition disabled:opacity-50"
              >
                {loading ? 'Creating...' : 'Create AREA'}
              </button>
            </div>
          </form>
        </div>
      )}

      {/* AREAs List */}
      {areas.length === 0 ? (
        <div className="text-center py-12 bg-white dark:bg-gray-800 rounded-lg">
          <p className="text-gray-500 dark:text-gray-400 mb-4">No AREAs created yet</p>
          <button
            onClick={() => setShowCreateForm(true)}
            className="px-4 py-2 bg-indigo-600 text-white rounded-md hover:bg-indigo-700 transition"
          >
            Create Your First AREA
          </button>
        </div>
      ) : (
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
          {areas.map((area) => (
            <div key={area.id} className="bg-white dark:bg-gray-800 rounded-lg shadow-lg p-6 border-l-4 border-indigo-600">
              <div className="flex items-start justify-between mb-4">
                <h3 className="text-lg font-semibold text-gray-900 dark:text-white">{area.name}</h3>
                <button
                  onClick={() => handleToggleArea(area.id, area.is_active)}
                  className={`px-3 py-1 text-xs rounded-full ${
                    area.is_active
                      ? 'bg-green-100 text-green-700 dark:bg-green-900 dark:text-green-200'
                      : 'bg-gray-100 text-gray-700 dark:bg-gray-700 dark:text-gray-300'
                  }`}
                >
                  {area.is_active ? 'Active' : 'Inactive'}
                </button>
              </div>

              <p className="text-sm text-gray-600 dark:text-gray-400 mb-4">{area.description}</p>

              <div className="space-y-3 mb-4">
                <div className="flex items-center gap-2 text-sm">
                  <span className="font-medium text-indigo-600 dark:text-indigo-400">🔔 ACTION:</span>
                  <span className="text-gray-700 dark:text-gray-300 capitalize">
                    {area.action_service} - {area.action_type.replace(/_/g, ' ')}
                  </span>
                </div>
                <div className="flex items-center gap-2 text-sm">
                  <span className="font-medium text-indigo-600 dark:text-indigo-400">⚡ REACTION:</span>
                  <span className="text-gray-700 dark:text-gray-300 capitalize">
                    {area.reaction_service} - {area.reaction_type.replace(/_/g, ' ')}
                  </span>
                </div>
              </div>

              <button
                onClick={() => handleDeleteArea(area.id)}
                className="w-full px-3 py-2 text-sm text-red-600 border border-red-600 rounded-md hover:bg-red-50 dark:hover:bg-red-900 transition"
              >
                Delete AREA
              </button>
            </div>
          ))}
        </div>
      )}
    </main>
  );
};

export default AREABuilderPage;
