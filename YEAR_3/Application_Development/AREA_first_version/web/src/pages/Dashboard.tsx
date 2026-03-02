import React from 'react';
import { Link } from 'react-router-dom';

const Dashboard: React.FC = () => {
  return (
    <main className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
      <div className="bg-white dark:bg-gray-800 rounded-lg shadow p-6 mb-6">
        <h2 className="text-xl font-semibold text-gray-900 dark:text-white mb-4">Welcome to AREA</h2>
        <p className="text-gray-600 dark:text-gray-400 mb-4">
          Your automation platform is ready. Start connecting services and creating automations.
        </p>
      </div>

      {/* Quick Stats */}
      <div className="grid grid-cols-1 md:grid-cols-3 gap-6 mb-6">
        <Link to="/areas" className="bg-white dark:bg-gray-800 rounded-lg shadow p-6 hover:shadow-lg transition">
          <h3 className="text-sm font-medium text-gray-500 dark:text-gray-400">Active AREAs</h3>
          <p className="text-3xl font-bold text-gray-900 dark:text-white mt-2">0</p>
        </Link>
        <Link to="/services" className="bg-white dark:bg-gray-800 rounded-lg shadow p-6 hover:shadow-lg transition">
          <h3 className="text-sm font-medium text-gray-500 dark:text-gray-400">Connected Services</h3>
          <p className="text-3xl font-bold text-gray-900 dark:text-white mt-2">0</p>
        </Link>
        <div className="bg-white dark:bg-gray-800 rounded-lg shadow p-6">
          <h3 className="text-sm font-medium text-gray-500 dark:text-gray-400">Executions Today</h3>
          <p className="text-3xl font-bold text-gray-900 dark:text-white mt-2">0</p>
        </div>
      </div>

      {/* Call to Action */}
      <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
        <div className="bg-indigo-50 dark:bg-indigo-900 rounded-lg p-6">
          <h3 className="text-lg font-semibold text-indigo-900 dark:text-indigo-100 mb-2">
            Connect Services
          </h3>
          <p className="text-indigo-700 dark:text-indigo-300 mb-4">
            Connect your Google account to unlock email automation.
          </p>
          <Link
            to="/services"
            className="inline-block px-4 py-2 bg-indigo-600 text-white rounded-md hover:bg-indigo-700 transition"
          >
            Manage Services
          </Link>
        </div>

        <div className="bg-green-50 dark:bg-green-900 rounded-lg p-6">
          <h3 className="text-lg font-semibold text-green-900 dark:text-green-100 mb-2">
            Create AREA
          </h3>
          <p className="text-green-700 dark:text-green-300 mb-4">
            Build your first automation by connecting an Action and a REAction.
          </p>
          <Link
            to="/areas"
            className="inline-block px-4 py-2 bg-green-600 text-white rounded-md hover:bg-green-700 transition"
          >
            Create Automation
          </Link>
        </div>
      </div>
    </main>
  );
};

export default Dashboard;