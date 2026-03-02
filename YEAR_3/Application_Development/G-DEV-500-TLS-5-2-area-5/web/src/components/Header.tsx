import React from 'react';
import { Link, useLocation } from 'react-router-dom';
import {
  HomeIcon,
  CogIcon,
  LinkIcon,
  ArrowRightOnRectangleIcon
} from '@heroicons/react/24/outline';

interface HeaderProps {
  title: string;
  onLogout: () => void;
}

const Header: React.FC<HeaderProps> = ({ title, onLogout }) => {
  const location = useLocation();

  const navItems = [
    { path: '/dashboard', label: 'Dashboard', Icon: HomeIcon },
    { path: '/areas', label: 'Automations', Icon: CogIcon },
    { path: '/services', label: 'Services', Icon: LinkIcon },
  ];

  return (
    <header className="bg-white dark:bg-gray-800 shadow">
      <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
        <div className="flex justify-between items-center py-4">
          <div className="flex items-center space-x-8">
            <div className="flex items-center space-x-3">
              <div className="w-8 h-8 bg-indigo-600 rounded-lg flex items-center justify-center">
                <CogIcon className="w-5 h-5 text-white" />
              </div>
              <h1 className="text-xl font-bold text-gray-900 dark:text-white">AREA</h1>
            </div>

            {/* Navigation */}
            <nav className="hidden md:flex space-x-1">
              {navItems.map((item) => {
                const isActive = location.pathname === item.path;
                return (
                  <Link
                    key={item.path}
                    to={item.path}
                    className={`inline-flex items-center px-3 py-2 rounded-lg text-sm font-medium transition-colors ${
                      isActive
                        ? 'bg-indigo-100 text-indigo-700 dark:bg-indigo-900 dark:text-indigo-200'
                        : 'text-gray-600 hover:text-gray-900 dark:text-gray-400 dark:hover:text-gray-200 hover:bg-gray-100 dark:hover:bg-gray-700'
                    }`}
                  >
                    <item.Icon className="w-4 h-4 mr-2" />
                    {item.label}
                  </Link>
                );
              })}
            </nav>
          </div>

          {/* Mobile Navigation & Logout */}
          <div className="flex items-center space-x-3">
            {/* Mobile menu button */}
            <div className="md:hidden">
              <select
                value={location.pathname}
                onChange={(e) => window.location.href = e.target.value}
                className="px-3 py-2 border border-gray-300 rounded-lg text-sm bg-white dark:bg-gray-700 dark:border-gray-600 dark:text-white focus:ring-2 focus:ring-indigo-500 focus:border-indigo-500"
              >
                {navItems.map((item) => (
                  <option key={item.path} value={item.path}>
                    {item.label}
                  </option>
                ))}
              </select>
            </div>

            <button
              onClick={onLogout}
              className="inline-flex items-center px-4 py-2 border border-transparent rounded-lg text-sm font-medium text-white bg-indigo-600 hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-indigo-500 transition-colors"
            >
              <ArrowRightOnRectangleIcon className="w-4 h-4 mr-2" />
              <span className="hidden sm:inline">Logout</span>
            </button>
          </div>
        </div>
      </div>
    </header>
  );
};

export default Header;