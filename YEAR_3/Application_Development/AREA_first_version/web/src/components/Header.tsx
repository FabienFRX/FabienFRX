import React from 'react';

interface HeaderProps {
  title: string;
  onLogout: () => void;
}

const Header: React.FC<HeaderProps> = ({ title, onLogout }) => {
  return (
    <header className="bg-white dark:bg-gray-800 shadow">
      <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-4 flex justify-between items-center">
        <h1 className="text-2xl font-bold text-gray-900 dark:text-white">{title}</h1>
        <button onClick={onLogout} className="btn-primary">Logout</button>
      </div>
    </header>
  );
};

export default Header;