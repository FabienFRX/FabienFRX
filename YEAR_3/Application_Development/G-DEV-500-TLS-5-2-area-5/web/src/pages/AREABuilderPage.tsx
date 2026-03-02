import React, { useState, useEffect } from 'react';
import { useAuth } from '../contexts/AuthContext';
import { areaAPI, servicesAPI, ServiceWithDetails, Action, Reaction } from '../services/api';
import { Link } from 'react-router-dom';
import {
  PlayIcon,
  StopIcon,
  EyeIcon,
  TrashIcon,
  CheckCircleIcon,
  XCircleIcon,
  ClockIcon,
  ArrowRightIcon,
  PlusIcon,
  QuestionMarkCircleIcon,
  AcademicCapIcon
} from '@heroicons/react/24/outline';
import AREAWelcomeGuide from '../components/AREAWelcomeGuide';

interface AREA {
  id: number;
  name: string;
  description: string;
  action_service: string;
  action_name: string;
  action_description: string;
  reaction_service: string;
  reaction_name: string;
  reaction_description: string;
  is_active: boolean;
  created_at: string;
  updated_at?: string;
  action_config?: any;
  reaction_config?: any;
}

const AREABuilderPage: React.FC = () => {
  const { token, handleAuthError } = useAuth();
  const [areas, setAreas] = useState<AREA[]>([]);
  const [connectedServices, setConnectedServices] = useState<any[]>([]);
  const [availableServices, setAvailableServices] = useState<ServiceWithDetails[]>([]);
  const [showCreateForm, setShowCreateForm] = useState(false);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [executorStatus, setExecutorStatus] = useState<'running' | 'stopped' | 'unknown'>('unknown');
  const [executorLoading, setExecutorLoading] = useState(false);
  const [selectedArea, setSelectedArea] = useState<AREA | null>(null);
  const [showDetailsModal, setShowDetailsModal] = useState(false);
  const [showWelcomeGuide, setShowWelcomeGuide] = useState(false);

  // Form state
  const [formData, setFormData] = useState({
    name: '',
    description: '',
    action_service: 'google',
    action_type: '',
    action_config: {} as any,
    reaction_service: 'google',
    reaction_type: '',
    reaction_config: {} as any,
    enabled: true,
  });

  useEffect(() => {
    loadAreas();
    loadConnectedServices();
    loadAvailableServices();
    loadExecutorStatus();
    
    // Show welcome guide on first visit
    const hasSeenGuide = localStorage.getItem('area_guide_seen');
    if (!hasSeenGuide) {
      setShowWelcomeGuide(true);
      localStorage.setItem('area_guide_seen', 'true');
    }
  }, []);

  // Initialize form defaults when services are loaded
  useEffect(() => {
    if (availableServices.length > 0 && !formData.action_type) {
      // Find a service with actions for the action_service
      const actionService = availableServices.find(s => s.actions.length > 0);
      // Find a service with reactions for the reaction_service
      const reactionService = availableServices.find(s => s.reactions.length > 0);

      if (actionService && reactionService) {
        const firstAction = actionService.actions[0];
        const firstReaction = reactionService.reactions[0];

        setFormData(prev => ({
          ...prev,
          action_service: actionService.name,
          action_type: firstAction?.name || '',
          reaction_service: reactionService.name,
          reaction_type: firstReaction?.name || '',
        }));
      }
    }
  }, [availableServices]);

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
      // If it's an auth error, clear the invalid token
      if (err.message?.includes('Failed to fetch user services')) {
        handleAuthError();
      }
    }
  };

  const loadAvailableServices = async () => {
    console.log('Loading services, token:', token);

    try {
      const data = await servicesAPI.getAvailableServices(token || '');
      console.log('Services loaded:', data);
      setAvailableServices(data.services || []);
    } catch (err: any) {
      console.error('Failed to load available services:', err);
    }
  };

  const loadExecutorStatus = async () => {
    if (!token) return;
    try {
      const data = await areaAPI.getExecutorStatus(token);
      setExecutorStatus(data.status);
    } catch (err: any) {
      console.error('Failed to load executor status:', err);
      setExecutorStatus('unknown');
    }
  };

  const handleStartExecutor = async () => {
    if (!token) return;
    setExecutorLoading(true);
    try {
      await areaAPI.startExecutor(token);
      setExecutorStatus('running');
      setError(null);
    } catch (err: any) {
      setError('Failed to start workflow executor');
      console.error('Failed to start executor:', err);
    }
    setExecutorLoading(false);
  };

  const handleStopExecutor = async () => {
    if (!token) return;
    setExecutorLoading(true);
    try {
      await areaAPI.stopExecutor(token);
      setExecutorStatus('stopped');
      setError(null);
    } catch (err: any) {
      setError('Failed to stop workflow executor');
      console.error('Failed to stop executor:', err);
    }
    setExecutorLoading(false);
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
        action_type: 'new_email_received',
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

  const handleViewDetails = (area: AREA) => {
    setSelectedArea(area);
    setShowDetailsModal(true);
  };

  const handleCloseDetailsModal = () => {
    setSelectedArea(null);
    setShowDetailsModal(false);
  };

  // Handle using an example template
  const handleUseExample = (example: any) => {
    setFormData({
      name: example.title,
      description: example.useCase,
      action_service: example.action.service,
      action_type: example.action.type,
      action_config: example.action.config,
      reaction_service: example.reaction.service,
      reaction_type: example.reaction.type,
      reaction_config: example.reaction.config,
      enabled: true,
    });
    setShowCreateForm(true);
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

  // Render action configuration based on action type
  const renderActionConfig = () => {
    if (!formData.action_type) return null;

    // Debug logging
    console.log('Current action_type:', formData.action_type);

    switch (formData.action_type) {
      case 'specific_time':
        return (
          <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Date
              </label>
              <input
                type="date"
                value={formData.action_config.date || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  action_config: { ...formData.action_config, date: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
              />
            </div>
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Time (HH:MM)
              </label>
              <input
                type="time"
                value={formData.action_config.time || '12:00'}
                onChange={(e) => setFormData({
                  ...formData,
                  action_config: { ...formData.action_config, time: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
              />
              <p className="text-xs text-gray-500 mt-1">Trigger once at specific date and time</p>
            </div>
          </div>
        );
      case 'daily_at_time':
        return (
          <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Time (HH:MM)
              </label>
              <input
                type="time"
                value={formData.action_config.time || '09:00'}
                onChange={(e) => setFormData({
                  ...formData,
                  action_config: { ...formData.action_config, time: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="09:00"
              />
            </div>
          </div>
        );
      case 'weekly_on_day':
        return (
          <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Day of week
              </label>
              <select
                value={formData.action_config.day || 'monday'}
                onChange={(e) => setFormData({
                  ...formData,
                  action_config: { ...formData.action_config, day: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
              >
                <option value="monday">Monday</option>
                <option value="tuesday">Tuesday</option>
                <option value="wednesday">Wednesday</option>
                <option value="thursday">Thursday</option>
                <option value="friday">Friday</option>
                <option value="saturday">Saturday</option>
                <option value="sunday">Sunday</option>
              </select>
            </div>
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Time (HH:MM)
              </label>
              <input
                type="time"
                value={formData.action_config.time || '09:00'}
                onChange={(e) => setFormData({
                  ...formData,
                  action_config: { ...formData.action_config, time: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="09:00"
              />
            </div>
          </div>
        );

      case 'every_x_seconds':
        return (
          <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Interval (seconds)
              </label>
              <input
                type="number"
                min="1"
                max="3600"
                value={formData.action_config.seconds || 30}
                onChange={(e) => setFormData({
                  ...formData,
                  action_config: { ...formData.action_config, seconds: parseInt(e.target.value) || 30 }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="30"
              />
              <p className="text-xs text-gray-500 mt-1">Trigger every X seconds (1-3600)</p>
            </div>
          </div>
        );

      case 'every_x_minutes':
        return (
          <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Interval (minutes)
              </label>
              <input
                type="number"
                min="1"
                max="1440"
                value={formData.action_config.minutes || 5}
                onChange={(e) => setFormData({
                  ...formData,
                  action_config: { ...formData.action_config, minutes: parseInt(e.target.value) || 5 }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="5"
              />
              <p className="text-xs text-gray-500 mt-1">Trigger every X minutes (1-1440)</p>
            </div>
          </div>
        );

      case 'every_x_hours':
        return (
          <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Interval (hours)
              </label>
              <input
                type="number"
                min="1"
                max="168"
                value={formData.action_config.hours || 1}
                onChange={(e) => setFormData({
                  ...formData,
                  action_config: { ...formData.action_config, hours: parseInt(e.target.value) || 1 }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="1"
              />
              <p className="text-xs text-gray-500 mt-1">Trigger every X hours (1-168 = 1 week)</p>
            </div>
          </div>
        );

      case 'new_email_from_sender':
      case 'new_email_received':
        return (
          <div className="grid grid-cols-1 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Check for emails from (optional)
              </label>
              <input
                type="email"
                value={formData.action_config.from_email || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  action_config: { ...formData.action_config, from_email: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="example@gmail.com (leave empty for any sender)"
              />
            </div>
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Subject contains (optional)
              </label>
              <input
                type="text"
                value={formData.action_config.subject_contains || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  action_config: { ...formData.action_config, subject_contains: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="Optional: keyword to search in subject"
              />
            </div>
            <p className="text-xs text-gray-500 mt-1">
              Leave both fields empty to trigger on any new unread email
            </p>
          </div>
        );
      
      case 'calendar_event_created':
        return (
          <div className="p-4 bg-blue-50 border border-blue-200 rounded-md dark:bg-blue-900 dark:border-blue-700">
            <p className="text-blue-800 dark:text-blue-200">
              ✓ No configuration needed - triggers when a new calendar event is created in your calendar
            </p>
          </div>
        );
      
      case 'calendar_event_upcoming':
        return (
          <div className="grid grid-cols-1 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Alert me (minutes before event starts) *
              </label>
              <input
                type="number"
                required
                min="5"
                max="1440"
                value={formData.action_config.minutes_before || 15}
                onChange={(e) => setFormData({
                  ...formData,
                  action_config: { ...formData.action_config, minutes_before: parseInt(e.target.value) || 15 }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="15"
              />
              <p className="text-xs text-gray-500 mt-1">
                Trigger when an event is starting soon (5-1440 minutes = 24 hours)
              </p>
            </div>
          </div>
        );
      
      case 'drive_file_uploaded':
        return (
          <div className="p-4 bg-blue-50 border border-blue-200 rounded-md dark:bg-blue-900 dark:border-blue-700">
            <p className="text-blue-800 dark:text-blue-200">
              ✓ No configuration needed - triggers when a new file is uploaded to your Google Drive
            </p>
          </div>
        );
      
      // Dropbox actions
      case 'new_file_in_folder':
        return (
          <div className="grid grid-cols-1 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Folder Path
              </label>
              <input
                type="text"
                value={formData.action_config.folder_path || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  action_config: { ...formData.action_config, folder_path: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="/Documents (leave empty for root folder)"
              />
              <p className="text-xs text-gray-500 mt-1">Monitor this folder for new files</p>
            </div>
          </div>
        );
      
      case 'file_modified':
        return (
          <div className="grid grid-cols-1 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                File Path *
              </label>
              <input
                type="text"
                required
                value={formData.action_config.file_path || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  action_config: { ...formData.action_config, file_path: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="/Documents/myfile.txt"
              />
              <p className="text-xs text-gray-500 mt-1">Full path to the file to monitor</p>
            </div>
          </div>
        );

      // LinkedIn actions
      case 'new_post_by_user':
        return (
          <div className="p-4 bg-blue-50 border border-blue-200 rounded-md dark:bg-blue-900 dark:border-blue-700">
            <p className="text-blue-800 dark:text-blue-200">
              ✓ No configuration needed - triggers when you share a new post on LinkedIn
            </p>
          </div>
        );

      // GitLab actions
      case 'new_issue':
      case 'new_merge_request':
      case 'new_commit':
      case 'new_star':
      case 'new_release':
        return (
          <div className="grid grid-cols-1 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Project ID *
              </label>
              <input
                type="text"
                required
                value={formData.action_config.project_id || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  action_config: { ...formData.action_config, project_id: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="12345678 or username/project-name"
              />
              <p className="text-xs text-gray-500 mt-1">
                GitLab project ID (numeric) or path (username/project-name)
              </p>
            </div>
          </div>
        );

      default:
        console.log('Unhandled action_type:', formData.action_type);
        return (
          <div className="p-4 bg-yellow-50 border border-yellow-200 rounded-md dark:bg-yellow-900 dark:border-yellow-700">
            <p className="text-yellow-800 dark:text-yellow-200">No configuration available for action: <code className="font-mono">{formData.action_type}</code></p>
            <p className="text-sm text-yellow-600 dark:text-yellow-300 mt-2">Available cases: daily_at_time, weekly_on_day, every_x_seconds, every_x_minutes, every_x_hours</p>
          </div>
        );
    }
  };

  // Render reaction configuration based on reaction type
  const renderReactionConfig = () => {
    if (!formData.reaction_type) return null;

    switch (formData.reaction_type) {
      case 'send_email':
        return (
          <div className="grid grid-cols-1 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                To Email *
              </label>
              <input
                type="email"
                required
                value={formData.reaction_config.to_email || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, to_email: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="recipient@example.com"
              />
            </div>
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Subject *
              </label>
              <input
                type="text"
                required
                value={formData.reaction_config.subject || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, subject: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="Email Subject"
              />
            </div>
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Body *
              </label>
              <textarea
                required
                rows={4}
                value={formData.reaction_config.body || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, body: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="Email content..."
              />
            </div>
          </div>
        );
      case 'create_calendar_event':
        return (
          <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Event Title *
              </label>
              <input
                type="text"
                required
                value={formData.reaction_config.title || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, title: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="Meeting Title"
              />
            </div>
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Duration (minutes) *
              </label>
              <input
                type="number"
                required
                min="15"
                value={formData.reaction_config.duration || 60}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, duration: parseInt(e.target.value) || 60 }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="60"
              />
            </div>
            <div className="md:col-span-2">
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Start Date & Time <span className="text-gray-500 text-xs">(optional - leave empty to create event immediately when triggered)</span>
              </label>
              <input
                type="datetime-local"
                value={formData.reaction_config.start_datetime || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, start_datetime: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
              />
            </div>
            <div className="md:col-span-2">
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Description
              </label>
              <textarea
                rows={3}
                value={formData.reaction_config.description || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, description: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="Event description..."
              />
            </div>
            <div className="md:col-span-2">
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Attendees <span className="text-gray-500 text-xs">(optional - comma-separated email addresses)</span>
              </label>
              <input
                type="text"
                value={formData.reaction_config.attendees || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, attendees: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="user1@example.com, user2@example.com"
              />
            </div>
            <div className="md:col-span-2">
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Send Notifications
              </label>
              <div className="flex items-center">
                <input
                  type="checkbox"
                  checked={formData.reaction_config.send_notifications !== false}
                  onChange={(e) => setFormData({
                    ...formData,
                    reaction_config: { ...formData.reaction_config, send_notifications: e.target.checked }
                  })}
                  className="w-4 h-4 text-blue-600 border-gray-300 rounded focus:ring-blue-500 cursor-pointer"
                />
                <span className="ml-2 text-sm font-medium text-gray-700 dark:text-gray-300">
                  Send email notifications to attendees {formData.reaction_config.send_notifications !== false ? '✓' : '✗'}
                </span>
              </div>
              <p className="text-xs text-gray-500 mt-1">
                {formData.reaction_config.send_notifications !== false 
                  ? 'Attendees will receive an email invitation with calendar event details' 
                  : 'No email notifications will be sent (attendees must be manually notified)'}
              </p>
            </div>
          </div>
        );

      // Dropbox reactions
      case 'create_folder':
        return (
          <div className="grid grid-cols-1 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Folder Path *
              </label>
              <input
                type="text"
                required
                value={formData.reaction_config.folder_path || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, folder_path: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="/NewFolder"
              />
              <p className="text-xs text-gray-500 mt-1">Full path for the new folder</p>
            </div>
          </div>
        );
      
      case 'move_file':
      case 'copy_file':
        return (
          <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                From Path *
              </label>
              <input
                type="text"
                required
                value={formData.reaction_config.from_path || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, from_path: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="/Documents/file.txt"
              />
            </div>
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                To Path *
              </label>
              <input
                type="text"
                required
                value={formData.reaction_config.to_path || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, to_path: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="/Archive/file.txt"
              />
            </div>
          </div>
        );
      
      case 'delete_file':
        return (
          <div className="grid grid-cols-1 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                File Path *
              </label>
              <input
                type="text"
                required
                value={formData.reaction_config.file_path || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, file_path: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="/Documents/file.txt"
              />
              <p className="text-xs text-gray-500 mt-1">File to delete</p>
            </div>
          </div>
        );

      case 'share_text_post':
        return (
          <div className="space-y-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Post Text *
              </label>
              <textarea
                required
                rows={4}
                value={formData.reaction_config.text || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, text: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="What do you want to share on LinkedIn?"
              />
            </div>
          </div>
        );

      // GitLab reactions
      case 'add_comment':
        return (
          <div className="grid grid-cols-1 gap-4">
            <div className="p-4 bg-blue-50 border border-blue-200 rounded-md dark:bg-blue-900 dark:border-blue-700">
              <p className="text-blue-800 dark:text-blue-200 text-sm mb-2">
                💡 <strong>Smart Comment:</strong> This will automatically comment on the issue or merge request that triggered the action.
              </p>
              <p className="text-blue-700 dark:text-blue-300 text-xs">
                For example: "New issue created" → "Add comment" will comment on that new issue automatically.
              </p>
            </div>
            
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Project ID (Optional if triggered by GitLab)
              </label>
              <input
                type="text"
                value={formData.reaction_config.project_id || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, project_id: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="12345678 or username/project-name"
              />
              <p className="text-xs text-gray-500 mt-1">
                Required if the trigger is NOT a GitLab event (e.g. Timer).
              </p>
            </div>

            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Comment Text *
              </label>
              <textarea
                required
                rows={4}
                value={formData.reaction_config.comment || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, comment: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="Thank you for reporting this issue! We'll look into it."
              />
              <p className="text-xs text-gray-500 mt-1">
                This comment will be posted automatically on the triggered issue/MR
              </p>
            </div>
          </div>
        );

      case 'create_issue':
        return (
          <div className="grid grid-cols-1 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Project ID *
              </label>
              <input
                type="text"
                required
                value={formData.reaction_config.project_id || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, project_id: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="12345678 or username/project-name"
              />
            </div>
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Issue Title *
              </label>
              <input
                type="text"
                required
                value={formData.reaction_config.title || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, title: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="Bug: Something is broken"
              />
            </div>
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Description
              </label>
              <textarea
                rows={4}
                value={formData.reaction_config.description || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, description: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="Detailed description of the issue..."
              />
            </div>
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Labels <span className="text-gray-500 text-xs">(comma-separated)</span>
              </label>
              <input
                type="text"
                value={formData.reaction_config.labels || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, labels: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="bug, urgent, backend"
              />
            </div>
          </div>
        );

      case 'create_project':
        return (
          <div className="grid grid-cols-1 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Project Name *
              </label>
              <input
                type="text"
                required
                value={formData.reaction_config.name || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, name: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="my-awesome-project"
              />
            </div>
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Description
              </label>
              <textarea
                rows={3}
                value={formData.reaction_config.description || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, description: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="Project description..."
              />
            </div>
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Visibility
              </label>
              <select
                value={formData.reaction_config.visibility || 'private'}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, visibility: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
              >
                <option value="private">Private</option>
                <option value="internal">Internal</option>
                <option value="public">Public</option>
              </select>
            </div>
          </div>
        );

      case 'star_project':
        return (
          <div className="grid grid-cols-1 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Project ID *
              </label>
              <input
                type="text"
                required
                value={formData.reaction_config.project_id || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, project_id: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="12345678 or username/project-name"
              />
              <p className="text-xs text-gray-500 mt-1">Project to star</p>
            </div>
          </div>
        );

      case 'create_merge_request':
        return (
          <div className="grid grid-cols-1 gap-4">
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Project ID *
              </label>
              <input
                type="text"
                required
                value={formData.reaction_config.project_id || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, project_id: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="12345678 or username/project-name"
              />
            </div>
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Title *
              </label>
              <input
                type="text"
                required
                value={formData.reaction_config.title || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, title: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="Feature: Add new functionality"
              />
            </div>
            <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
              <div>
                <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                  Source Branch *
                </label>
                <input
                  type="text"
                  required
                  value={formData.reaction_config.source_branch || ''}
                  onChange={(e) => setFormData({
                    ...formData,
                    reaction_config: { ...formData.reaction_config, source_branch: e.target.value }
                  })}
                  className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                  placeholder="feature-branch"
                />
              </div>
              <div>
                <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                  Target Branch *
                </label>
                <input
                  type="text"
                  required
                  value={formData.reaction_config.target_branch || 'main'}
                  onChange={(e) => setFormData({
                    ...formData,
                    reaction_config: { ...formData.reaction_config, target_branch: e.target.value }
                  })}
                  className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                  placeholder="main"
                />
              </div>
            </div>
            <div>
              <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                Description
              </label>
              <textarea
                rows={4}
                value={formData.reaction_config.description || ''}
                onChange={(e) => setFormData({
                  ...formData,
                  reaction_config: { ...formData.reaction_config, description: e.target.value }
                })}
                className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                placeholder="Merge request description..."
              />
            </div>
          </div>
        );

      default:
        return (
          <div className="text-sm text-gray-600 dark:text-gray-400">
            No configuration required for this reaction type.
          </div>
        );
    }
  };

  const hasConnectedServices = connectedServices.length > 0;

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
      {/* Welcome Guide Modal */}
      {showWelcomeGuide && (
        <AREAWelcomeGuide
          onClose={() => setShowWelcomeGuide(false)}
          onUseExample={handleUseExample}
        />
      )}

      <div className="flex items-center justify-between mb-8">
        <div>
          <h1 className="text-3xl font-bold text-gray-900 dark:text-white mb-2">My AREAs</h1>
          <p className="text-gray-600 dark:text-gray-400">
            Create automations by connecting Actions and REActions
          </p>
        </div>
        <div className="flex gap-3">
          <button
            onClick={() => setShowWelcomeGuide(true)}
            className="px-4 py-2 bg-purple-600 text-white rounded-md hover:bg-purple-700 transition flex items-center gap-2"
            title="View examples and guide"
          >
            <AcademicCapIcon className="w-5 h-5" />
            Examples & Guide
          </button>
          <button
            onClick={() => setShowCreateForm(!showCreateForm)}
            className="px-4 py-2 bg-indigo-600 text-white rounded-md hover:bg-indigo-700 transition"
          >
            {showCreateForm ? 'Cancel' : '+ Create AREA'}
          </button>
        </div>
      </div>

      {/* Empty State with Quick Start */}
      {!showCreateForm && areas.length === 0 && (
        <div className="bg-gradient-to-br from-blue-50 to-purple-50 dark:from-gray-800 dark:to-gray-700 rounded-lg shadow-lg p-8 mb-8 text-center">
          <QuestionMarkCircleIcon className="w-16 h-16 mx-auto text-purple-600 dark:text-purple-400 mb-4" />
          <h3 className="text-2xl font-bold text-gray-900 dark:text-white mb-2">
            Welcome to AREA Automation!
          </h3>
          <p className="text-gray-600 dark:text-gray-300 mb-6 max-w-2xl mx-auto">
            You don't have any automation yet. Start by creating your first AREA to connect services and automate your tasks!
          </p>
          <div className="flex flex-col sm:flex-row gap-4 justify-center">
            <button
              onClick={() => setShowWelcomeGuide(true)}
              className="px-6 py-3 bg-gradient-to-r from-purple-600 to-blue-600 text-white rounded-lg hover:from-purple-700 hover:to-blue-700 transition-all transform hover:scale-105 flex items-center justify-center gap-2 shadow-lg"
            >
              <AcademicCapIcon className="w-5 h-5" />
              View Examples & Learn How
            </button>
            <button
              onClick={() => setShowCreateForm(true)}
              className="px-6 py-3 bg-white dark:bg-gray-700 text-gray-900 dark:text-white border-2 border-purple-600 rounded-lg hover:bg-purple-50 dark:hover:bg-gray-600 transition flex items-center justify-center gap-2"
            >
              <PlusIcon className="w-5 h-5" />
              Create My First AREA
            </button>
          </div>
        </div>
      )}

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
                  placeholder="Your automatisation name"
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
                ACTION (Trigger)
              </h3>
              <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                <div>
                  <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                    Service
                  </label>
                  <select
                    value={formData.action_service}
                    onChange={(e) => {
                      const newService = e.target.value;
                      const service = availableServices.find(s => s.name === newService);
                      const firstAction = service?.actions[0];
                      setFormData({
                        ...formData,
                        action_service: newService,
                        action_type: firstAction?.name || ''
                      });
                    }}
                    className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                  >
                    {availableServices
                      .filter(service => {
                        // Show Timer (no auth needed) or connected services
                        if (service.name === 'timer') return service.actions.length > 0;
                        return connectedServices.some(cs => cs.service_name === service.name) && service.actions.length > 0;
                      })
                      .map((service) => (
                        <option key={service.name} value={service.name}>
                          {service.name.charAt(0).toUpperCase() + service.name.slice(1)} ({service.description})
                        </option>
                      ))}
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
                REACTION (Response)
              </h3>
              <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                <div>
                  <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-2">
                    Service
                  </label>
                  <select
                    value={formData.reaction_service}
                    onChange={(e) => {
                      const newService = e.target.value;
                      const service = availableServices.find(s => s.name === newService);
                      const firstReaction = service?.reactions[0];
                      setFormData({
                        ...formData,
                        reaction_service: newService,
                        reaction_type: firstReaction?.name || ''
                      });
                    }}
                    className="w-full px-3 py-2 border border-gray-300 rounded-md dark:bg-gray-700 dark:border-gray-600 dark:text-white"
                  >
                    {availableServices
                      .filter(service => {
                        // Show Timer (no auth needed) or connected services
                        if (service.name === 'timer') return service.reactions.length > 0;
                        return connectedServices.some(cs => cs.service_name === service.name) && service.reactions.length > 0;
                      })
                      .map((service) => (
                        <option key={service.name} value={service.name}>
                          {service.name.charAt(0).toUpperCase() + service.name.slice(1)} ({service.description})
                        </option>
                      ))}
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

              {/* Action Configuration */}
              {formData.action_type && (
                <div className="mt-6 p-4 bg-blue-50 dark:bg-blue-900/20 rounded-lg">
                  <h4 className="text-md font-medium text-blue-900 dark:text-blue-100 mb-3">
                    Configure Action: {formData.action_type}
                  </h4>
                  {renderActionConfig()}
                </div>
              )}
            </div>

            {/* REACTION Configuration */}
            {formData.reaction_type && (
              <div className="border-t border-gray-200 dark:border-gray-700 pt-6">
                <div className="p-4 bg-green-50 dark:bg-green-900/20 rounded-lg">
                  <h4 className="text-md font-medium text-green-900 dark:text-green-100 mb-3">
                    Configure Reaction: {formData.reaction_type}
                  </h4>
                  {renderReactionConfig()}
                </div>
              </div>
            )}

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
        <div className="grid grid-cols-1 md:grid-cols-2 xl:grid-cols-3 gap-6">
          {areas.map((area) => (
            <div key={area.id} className="bg-white dark:bg-gray-800 rounded-xl shadow-lg border border-gray-200 dark:border-gray-700 overflow-hidden hover:shadow-xl transition-shadow">
              {/* Header */}
              <div className="px-6 py-4 border-b border-gray-100 dark:border-gray-700">
                <div className="flex items-center justify-between">
                  <div className="flex items-center space-x-3">
                    <div className={`w-3 h-3 rounded-full ${area.is_active ? 'bg-green-400' : 'bg-gray-400'}`} />
                    <h3 className="text-lg font-semibold text-gray-900 dark:text-white truncate">{area.name}</h3>
                  </div>
                  <button
                    onClick={() => handleToggleArea(area.id, area.is_active)}
                    className={`inline-flex items-center px-2.5 py-1 rounded-full text-xs font-medium transition-colors ${
                      area.is_active
                        ? 'bg-green-100 text-green-700 hover:bg-green-200 dark:bg-green-900/30 dark:text-green-400'
                        : 'bg-gray-100 text-gray-700 hover:bg-gray-200 dark:bg-gray-700 dark:text-gray-300'
                    }`}
                  >
                    {area.is_active ? (
                      <>
                        <CheckCircleIcon className="w-3 h-3 mr-1" />
                        Active
                      </>
                    ) : (
                      <>
                        <XCircleIcon className="w-3 h-3 mr-1" />
                        Inactive
                      </>
                    )}
                  </button>
                </div>
              </div>

              {/* Content */}
              <div className="px-6 py-4">
                <p className="text-sm text-gray-600 dark:text-gray-400 mb-4 overflow-hidden" style={{
                  display: '-webkit-box',
                  WebkitLineClamp: 2,
                  WebkitBoxOrient: 'vertical'
                }}>{area.description || 'No description'}</p>

                {/* Workflow Flow */}
                <div className="space-y-3 mb-6">
                  <div className="flex items-center space-x-3 p-3 bg-blue-50 dark:bg-blue-900/20 rounded-lg">
                    <ClockIcon className="w-5 h-5 text-blue-600 dark:text-blue-400 flex-shrink-0" />
                    <div className="min-w-0">
                      <p className="text-xs font-medium text-blue-900 dark:text-blue-100 uppercase tracking-wide">Trigger</p>
                      <p className="text-sm text-blue-800 dark:text-blue-200 truncate">
                        {area.action_service} • {area.action_description || area.action_name?.replace(/_/g, ' ') || 'Unknown'}
                      </p>
                    </div>
                  </div>

                  <div className="flex justify-center">
                    <ArrowRightIcon className="w-5 h-5 text-gray-400" />
                  </div>

                  <div className="flex items-center space-x-3 p-3 bg-green-50 dark:bg-green-900/20 rounded-lg">
                    <PlayIcon className="w-5 h-5 text-green-600 dark:text-green-400 flex-shrink-0" />
                    <div className="min-w-0">
                      <p className="text-xs font-medium text-green-900 dark:text-green-100 uppercase tracking-wide">Action</p>
                      <p className="text-sm text-green-800 dark:text-green-200 truncate">
                        {area.reaction_service} • {area.reaction_description || area.reaction_name?.replace(/_/g, ' ') || 'Unknown'}
                      </p>
                    </div>
                  </div>
                </div>
              </div>

              {/* Actions */}
              <div className="px-6 py-4 bg-gray-50 dark:bg-gray-900/50 border-t border-gray-100 dark:border-gray-700">
                <div className="flex space-x-2">
                  <button
                    onClick={() => handleViewDetails(area)}
                    className="flex-1 inline-flex items-center justify-center px-3 py-2 text-sm font-medium text-gray-700 dark:text-gray-300 bg-white dark:bg-gray-800 border border-gray-300 dark:border-gray-600 rounded-lg hover:bg-gray-50 dark:hover:bg-gray-700 transition-colors"
                  >
                    <EyeIcon className="w-4 h-4 mr-1.5" />
                    Details
                  </button>
                  <button
                    onClick={() => handleDeleteArea(area.id)}
                    className="inline-flex items-center justify-center px-3 py-2 text-sm font-medium text-red-700 dark:text-red-400 bg-white dark:bg-gray-800 border border-red-300 dark:border-red-600 rounded-lg hover:bg-red-50 dark:hover:bg-red-900/20 transition-colors"
                  >
                    <TrashIcon className="w-4 h-4" />
                  </button>
                </div>
              </div>
            </div>
          ))}
        </div>
      )}

      {/* Details Modal */}
      {showDetailsModal && selectedArea && (
        <div className="fixed inset-0 bg-black bg-opacity-50 flex items-center justify-center z-50 p-4">
          <div className="bg-white dark:bg-gray-800 rounded-lg max-w-4xl w-full max-h-[90vh] overflow-y-auto">
            <div className="p-6">
              {/* Header */}
              <div className="flex items-start justify-between mb-6">
                <div>
                  <h2 className="text-2xl font-bold text-gray-900 dark:text-white">{selectedArea.name}</h2>
                  <div className="flex items-center gap-2 mt-2">
                    <span className={`px-3 py-1 text-xs rounded-full ${
                      selectedArea.is_active
                        ? 'bg-green-100 text-green-700 dark:bg-green-900 dark:text-green-200'
                        : 'bg-gray-100 text-gray-700 dark:bg-gray-700 dark:text-gray-300'
                    }`}>
                      {selectedArea.is_active ? 'Active' : 'Inactive'}
                    </span>
                    <span className="text-sm text-gray-500 dark:text-gray-400">
                      ID: {selectedArea.id}
                    </span>
                  </div>
                </div>
                <button
                  onClick={handleCloseDetailsModal}
                  className="text-gray-400 hover:text-gray-600 dark:hover:text-gray-200 text-2xl"
                >
                  ×
                </button>
              </div>

              {/* Description */}
              <div className="mb-6">
                <h3 className="text-lg font-semibold text-gray-900 dark:text-white mb-2">Description</h3>
                <p className="text-gray-600 dark:text-gray-400 bg-gray-50 dark:bg-gray-700 p-3 rounded-md">
                  {selectedArea.description || 'No description provided'}
                </p>
              </div>

              {/* Action & Reaction Overview */}
              <div className="grid grid-cols-1 md:grid-cols-2 gap-6 mb-6">
                {/* Action */}
                <div className="bg-blue-50 dark:bg-blue-900/20 p-4 rounded-lg border-l-4 border-blue-500">
                  <h3 className="text-lg font-semibold text-blue-900 dark:text-blue-100 mb-3">ACTION (Trigger)</h3>
                  <div className="space-y-2">
                    <div>
                      <span className="font-medium text-gray-700 dark:text-gray-300">Service:</span>
                      <span className="ml-2 text-blue-700 dark:text-blue-300 capitalize font-medium">
                        {selectedArea.action_service}
                      </span>
                    </div>
                    <div>
                      <span className="font-medium text-gray-700 dark:text-gray-300">Action:</span>
                      <span className="ml-2 text-gray-800 dark:text-gray-200">
                        {selectedArea.action_description || selectedArea.action_name?.replace(/_/g, ' ') || 'Unknown'}
                      </span>
                    </div>
                    {selectedArea.action_config && Object.keys(selectedArea.action_config).length > 0 && (
                      <div>
                        <span className="font-medium text-gray-700 dark:text-gray-300">Configuration:</span>
                        <pre className="mt-1 text-xs bg-white dark:bg-gray-800 p-2 rounded border overflow-x-auto">
{JSON.stringify(selectedArea.action_config, null, 2)}
                        </pre>
                      </div>
                    )}
                  </div>
                </div>

                {/* Reaction */}
                <div className="bg-green-50 dark:bg-green-900/20 p-4 rounded-lg border-l-4 border-green-500">
                  <h3 className="text-lg font-semibold text-green-900 dark:text-green-100 mb-3">REACTION (Response)</h3>
                  <div className="space-y-2">
                    <div>
                      <span className="font-medium text-gray-700 dark:text-gray-300">Service:</span>
                      <span className="ml-2 text-green-700 dark:text-green-300 capitalize font-medium">
                        {selectedArea.reaction_service}
                      </span>
                    </div>
                    <div>
                      <span className="font-medium text-gray-700 dark:text-gray-300">Reaction:</span>
                      <span className="ml-2 text-gray-800 dark:text-gray-200">
                        {selectedArea.reaction_description || selectedArea.reaction_name?.replace(/_/g, ' ') || 'Unknown'}
                      </span>
                    </div>
                    {selectedArea.reaction_config && Object.keys(selectedArea.reaction_config).length > 0 && (
                      <div>
                        <span className="font-medium text-gray-700 dark:text-gray-300">Configuration:</span>
                        <pre className="mt-1 text-xs bg-white dark:bg-gray-800 p-2 rounded border overflow-x-auto">
{JSON.stringify(selectedArea.reaction_config, null, 2)}
                        </pre>
                      </div>
                    )}
                  </div>
                </div>
              </div>

              {/* Additional Information */}
              <div className="border-t dark:border-gray-600 pt-4">
                <h3 className="text-lg font-semibold text-gray-900 dark:text-white mb-3">Additional Information</h3>
                <div className="grid grid-cols-1 md:grid-cols-3 gap-4 text-sm">
                  <div>
                    <span className="font-medium text-gray-700 dark:text-gray-300">Created:</span>
                    <span className="ml-2 text-gray-600 dark:text-gray-400">
                      {selectedArea.created_at ? new Date(selectedArea.created_at).toLocaleString() : 'Unknown'}
                    </span>
                  </div>
                  <div>
                    <span className="font-medium text-gray-700 dark:text-gray-300">Updated:</span>
                    <span className="ml-2 text-gray-600 dark:text-gray-400">
                      {selectedArea.updated_at ? new Date(selectedArea.updated_at).toLocaleString() : 'Unknown'}
                    </span>
                  </div>
                  <div>
                    <span className="font-medium text-gray-700 dark:text-gray-300">Status:</span>
                    <span className={`ml-2 font-medium ${
                      selectedArea.is_active
                        ? 'text-green-600 dark:text-green-400'
                        : 'text-gray-600 dark:text-gray-400'
                    }`}>
                      {selectedArea.is_active ? 'Monitoring' : 'Paused'}
                    </span>
                  </div>
                </div>
              </div>

              {/* Actions */}
              <div className="border-t dark:border-gray-600 pt-4 mt-6">
                <div className="flex flex-wrap gap-3">
                  <button
                    onClick={() => handleToggleArea(selectedArea.id, selectedArea.is_active)}
                    className={`px-4 py-2 rounded-md transition ${
                      selectedArea.is_active
                        ? 'bg-yellow-600 hover:bg-yellow-700 text-white'
                        : 'bg-green-600 hover:bg-green-700 text-white'
                    }`}
                  >
                    {selectedArea.is_active ? 'Pause AREA' : 'Activate AREA'}
                  </button>
                  <button
                    onClick={() => {
                      handleCloseDetailsModal();
                      handleDeleteArea(selectedArea.id);
                    }}
                    className="px-4 py-2 bg-red-600 hover:bg-red-700 text-white rounded-md transition"
                  >
                    Delete AREA
                  </button>
                  <button
                    onClick={handleCloseDetailsModal}
                    className="px-4 py-2 bg-gray-300 hover:bg-gray-400 dark:bg-gray-600 dark:hover:bg-gray-500 text-gray-700 dark:text-gray-200 rounded-md transition"
                  >
                    Close
                  </button>
                </div>
              </div>
            </div>
          </div>
        </div>
      )}
    </main>
  );
};

export default AREABuilderPage;
