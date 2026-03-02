import React, { useState } from 'react';
import {
  XMarkIcon,
  LightBulbIcon,
  ClockIcon,
  EnvelopeIcon,
  CalendarIcon,
  FolderIcon,
  ArrowRightIcon,
  SparklesIcon
} from '@heroicons/react/24/outline';

interface AREAExample {
  id: string;
  title: string;
  description: string;
  icon: React.ComponentType<{ className?: string }>;
  action: {
    service: string;
    type: string;
    description: string;
    config: any;
  };
  reaction: {
    service: string;
    type: string;
    description: string;
    config: any;
  };
  useCase: string;
  category: 'productivity' | 'automation' | 'notification' | 'backup';
}

const areaExamples: AREAExample[] = [
  {
    id: 'daily-email-reminder',
    title: 'Daily Email Reminder',
    description: 'Receive an email every morning at 9 AM',
    icon: EnvelopeIcon,
    action: {
      service: 'timer',
      type: 'daily_at_time',
      description: 'Every day at 9:00 AM',
      config: { time: '09:00' }
    },
    reaction: {
      service: 'google',
      type: 'send_email',
      description: 'Send motivational email',
      config: {
        to_email: 'your-email@example.com',
        subject: 'Good Morning! Time to start your day ',
        body: 'Have a productive day! Remember to check your tasks.'
      }
    },
    useCase: 'Perfect for daily reminders, morning routines, or task notifications',
    category: 'productivity'
  },
  {
    id: 'weekly-calendar-event',
    title: 'Weekly Meeting Scheduler',
    description: 'Automatically create a calendar event every Monday',
    icon: CalendarIcon,
    action: {
      service: 'timer',
      type: 'weekly_on_day',
      description: 'Every Monday at 9:00 AM',
      config: { day: 'monday', time: '09:00' }
    },
    reaction: {
      service: 'google',
      type: 'create_calendar_event',
      description: 'Create weekly team meeting',
      config: {
        title: 'Weekly Team Sync',
        duration: 60,
        description: 'Weekly team synchronization meeting',
        attendees: '',
        send_notifications: true
      }
    },
    useCase: 'Ideal for recurring meetings, weekly check-ins, or scheduled events',
    category: 'productivity'
  },
  {
    id: 'periodic-backup',
    title: 'Periodic Backup Notification',
    description: 'Send reminder email every 2 hours',
    icon: ClockIcon,
    action: {
      service: 'timer',
      type: 'every_x_hours',
      description: 'Every 2 hours',
      config: { interval: 2 }
    },
    reaction: {
      service: 'google',
      type: 'send_email',
      description: 'Send backup reminder',
      config: {
        to: 'admin@example.com',
        subject: 'Backup Reminder - {{timestamp}}',
        body: 'Time to create a backup of your important files.'
      }
    },
    useCase: 'Essential for backup reminders, scheduled notifications, or system monitoring',
    category: 'backup'
  },
  {
    id: 'github-notification',
    title: 'GitHub Activity Alert',
    description: 'Get email notifications for new GitHub events',
    icon: SparklesIcon,
    action: {
      service: 'github',
      type: 'new_push',
      description: 'When new push to repository',
      config: { repo: 'username/repository' }
    },
    reaction: {
      service: 'google',
      type: 'send_email',
      description: 'Send notification email',
      config: {
        to_email: 'your-email@example.com',
        subject: 'New GitHub Push Detected!',
        body: 'A new push has been made to your repository.'
      }
    },
    useCase: 'Stay updated with code changes, team contributions, or project activity',
    category: 'notification'
  },
];

interface Props {
  onClose: () => void;
  onUseExample: (example: AREAExample) => void;
}

const AREAWelcomeGuide: React.FC<Props> = ({ onClose, onUseExample }) => {
  const [selectedCategory, setSelectedCategory] = useState<string>('all');

  const filteredExamples = selectedCategory === 'all'
    ? areaExamples
    : areaExamples.filter(ex => ex.category === selectedCategory);

  return (
    <div className="fixed inset-0 bg-black bg-opacity-50 z-50 flex items-center justify-center p-4 overflow-y-auto">
      <div className="bg-white dark:bg-gray-800 rounded-lg shadow-2xl max-w-6xl w-full max-h-[90vh] overflow-y-auto">
        {/* Header */}
        <div className="sticky top-0 bg-gradient-to-r from-blue-600 to-purple-600 text-white p-6 rounded-t-lg">
          <div className="flex justify-between items-start">
            <div>
              <h2 className="text-3xl font-bold mb-2 flex items-center">
                <LightBulbIcon className="w-8 h-8 mr-3" />
                Welcome to AREA Automation!
              </h2>
              <p className="text-blue-100 text-lg">
                Create powerful workflows by connecting triggers (Actions) with automated responses (Reactions)
              </p>
            </div>
            <button
              onClick={onClose}
              className="text-white hover:text-gray-200 transition"
            >
              <XMarkIcon className="w-6 h-6" />
            </button>
          </div>
        </div>

        {/* How it works */}
        <div className="p-6 bg-gray-50 dark:bg-gray-700/50 border-b dark:border-gray-600">
          <h3 className="text-xl font-semibold text-gray-900 dark:text-white mb-4">
            How AREA Works
          </h3>
          <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
            <div className="bg-white dark:bg-gray-800 p-4 rounded-lg shadow">
              <div className="w-12 h-12 bg-blue-100 dark:bg-blue-900/30 rounded-full flex items-center justify-center mb-3">
                <span className="text-2xl font-bold text-blue-600 dark:text-blue-400">1</span>
              </div>
              <h4 className="font-semibold text-gray-900 dark:text-white mb-2">Choose a Trigger (Action)</h4>
              <p className="text-sm text-gray-600 dark:text-gray-400">
                Select what event should start your automation (timer, new email, GitHub push, etc.)
              </p>
            </div>
            <div className="bg-white dark:bg-gray-800 p-4 rounded-lg shadow">
              <div className="w-12 h-12 bg-green-100 dark:bg-green-900/30 rounded-full flex items-center justify-center mb-3">
                <span className="text-2xl font-bold text-green-600 dark:text-green-400">2</span>
              </div>
              <h4 className="font-semibold text-gray-900 dark:text-white mb-2">Choose a Response (Reaction)</h4>
              <p className="text-sm text-gray-600 dark:text-gray-400">
                Select what should happen automatically (send email, create calendar event, save file, etc.)
              </p>
            </div>
            <div className="bg-white dark:bg-gray-800 p-4 rounded-lg shadow">
              <div className="w-12 h-12 bg-purple-100 dark:bg-purple-900/30 rounded-full flex items-center justify-center mb-3">
                <span className="text-2xl font-bold text-purple-600 dark:text-purple-400">3</span>
              </div>
              <h4 className="font-semibold text-gray-900 dark:text-white mb-2">Activate & Automate</h4>
              <p className="text-sm text-gray-600 dark:text-gray-400">
                Your AREA runs automatically in the background, saving you time and effort!
              </p>
            </div>
          </div>
        </div>

        {/* Category Filter */}
        <div className="p-6 border-b dark:border-gray-600">
          <h3 className="text-xl font-semibold text-gray-900 dark:text-white mb-4">
            Example Templates
          </h3>
          <div className="flex flex-wrap gap-2 mb-4">
            <button
              onClick={() => setSelectedCategory('all')}
              className={`px-4 py-2 rounded-full text-sm font-medium transition ${
                selectedCategory === 'all'
                  ? 'bg-blue-600 text-white'
                  : 'bg-gray-200 dark:bg-gray-700 text-gray-700 dark:text-gray-300 hover:bg-gray-300 dark:hover:bg-gray-600'
              }`}
            >
              All Templates
            </button>
            <button
              onClick={() => setSelectedCategory('productivity')}
              className={`px-4 py-2 rounded-full text-sm font-medium transition ${
                selectedCategory === 'productivity'
                  ? 'bg-blue-600 text-white'
                  : 'bg-gray-200 dark:bg-gray-700 text-gray-700 dark:text-gray-300 hover:bg-gray-300 dark:hover:bg-gray-600'
              }`}
            >
              Productivity
            </button>
            <button
              onClick={() => setSelectedCategory('notification')}
              className={`px-4 py-2 rounded-full text-sm font-medium transition ${
                selectedCategory === 'notification'
                  ? 'bg-blue-600 text-white'
                  : 'bg-gray-200 dark:bg-gray-700 text-gray-700 dark:text-gray-300 hover:bg-gray-300 dark:hover:bg-gray-600'
              }`}
            >
              Notifications
            </button>
            <button
              onClick={() => setSelectedCategory('backup')}
              className={`px-4 py-2 rounded-full text-sm font-medium transition ${
                selectedCategory === 'backup'
                  ? 'bg-blue-600 text-white'
                  : 'bg-gray-200 dark:bg-gray-700 text-gray-700 dark:text-gray-300 hover:bg-gray-300 dark:hover:bg-gray-600'
              }`}
            >
              Backup
            </button>
          </div>

          {/* Examples Grid */}
          <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
            {filteredExamples.map((example) => (
              <div
                key={example.id}
                className="bg-white dark:bg-gray-800 border border-gray-200 dark:border-gray-700 rounded-lg p-5 hover:shadow-lg transition-all hover:border-blue-500"
              >
                <div className="flex items-start justify-between mb-3">
                  <div className="flex items-start">
                    <div className="w-10 h-10 bg-gradient-to-br from-blue-500 to-purple-500 rounded-lg flex items-center justify-center mr-3 flex-shrink-0">
                      <example.icon className="w-6 h-6 text-white" />
                    </div>
                    <div>
                      <h4 className="font-semibold text-gray-900 dark:text-white mb-1">
                        {example.title}
                      </h4>
                      <p className="text-sm text-gray-600 dark:text-gray-400">
                        {example.description}
                      </p>
                    </div>
                  </div>
                </div>

                {/* Workflow visualization */}
                <div className="bg-gray-50 dark:bg-gray-700/50 rounded-lg p-3 mb-3">
                  <div className="flex items-center justify-between text-xs">
                    <div className="flex-1">
                      <span className="font-medium text-blue-600 dark:text-blue-400 block mb-1">
                        TRIGGER
                      </span>
                      <span className="text-gray-700 dark:text-gray-300 block">
                        {example.action.description}
                      </span>
                    </div>
                    <ArrowRightIcon className="w-5 h-5 text-gray-400 mx-2 flex-shrink-0" />
                    <div className="flex-1">
                      <span className="font-medium text-green-600 dark:text-green-400 block mb-1">
                        ACTION
                      </span>
                      <span className="text-gray-700 dark:text-gray-300 block">
                        {example.reaction.description}
                      </span>
                    </div>
                  </div>
                </div>

                {/* Use case */}
                <p className="text-xs text-gray-500 dark:text-gray-400 mb-3 italic">
                  {example.useCase}
                </p>

                {/* Use Template button */}
                <button
                  onClick={() => {
                    onUseExample(example);
                    onClose();
                  }}
                  className="w-full bg-gradient-to-r from-blue-600 to-purple-600 hover:from-blue-700 hover:to-purple-700 text-white font-medium py-2 px-4 rounded-lg transition-all transform hover:scale-105"
                >
                  Use This Template
                </button>
              </div>
            ))}
          </div>
        </div>

        {/* Footer */}
        <div className="p-6 bg-gray-50 dark:bg-gray-700/50">
          <div className="flex flex-col md:flex-row justify-between items-center gap-4">
            <div className="text-sm text-gray-600 dark:text-gray-400">
              <p className="mb-1">
                <strong>Tip:</strong> Make sure to connect the required services before creating an AREA.
              </p>
              <p>
                Visit the <strong>Services</strong> page to connect Google, GitHub, LinkedIn, and more!
              </p>
            </div>
            <button
              onClick={onClose}
              className="px-6 py-2 bg-gray-600 hover:bg-gray-700 text-white rounded-lg transition whitespace-nowrap"
            >
              Start Creating
            </button>
          </div>
        </div>
      </div>
    </div>
  );
};

export default AREAWelcomeGuide;
