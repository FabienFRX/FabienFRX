import React from 'react';
import { BrowserRouter, Routes, Route, Navigate } from 'react-router-dom';
import { AuthProvider } from '../contexts/AuthContext';
import ProtectedRoute from '../components/ProtectedRoute';
import LoginPage from '../pages/LoginPage';
import RegisterPage from '../pages/RegisterPage';
import Dashboard from '../pages/Dashboard';
import ServicesPage from '../pages/ServicesPage';
import AREABuilderPage from '../pages/AREABuilderPage';
import GitHubCallback from '../pages/GitHubCallback';
import GitLabCallback from '../pages/GitLabCallback';
import LinkedInCallback from '../pages/LinkedInCallback';
import Header from '../components/Header';
import { useAuth } from '../contexts/AuthContext';

const DashboardLayout: React.FC = () => {
  const { logout } = useAuth();

  return (
    <div className="min-h-screen bg-gray-50 dark:bg-gray-900">
      <Header title="AREA Dashboard" onLogout={logout} />
      <Dashboard />
    </div>
  );
};

const DashboardWithNav: React.FC<{ children: React.ReactNode }> = ({ children }) => {
  const { logout } = useAuth();

  return (
    <div className="min-h-screen bg-gray-50 dark:bg-gray-900">
      <Header title="AREA Dashboard" onLogout={logout} />
      {children}
    </div>
  );
};

const App: React.FC = () => {
  return (
    <BrowserRouter>
      <AuthProvider>
        <Routes>
          <Route path="/login" element={<LoginPage />} />
          <Route path="/register" element={<RegisterPage />} />
          <Route path="/github/callback" element={<GitHubCallback />} />
          <Route path="/gitlab/callback" element={<GitLabCallback />} />
          <Route path="/linkedin/callback" element={<LinkedInCallback />} />
            <Route
              path="/dashboard"
              element={
                <ProtectedRoute>
                  <DashboardLayout />
                </ProtectedRoute>
              }
            />
            <Route
              path="/services"
              element={
                <ProtectedRoute>
                  <DashboardWithNav><ServicesPage /></DashboardWithNav>
                </ProtectedRoute>
              }
            />
            <Route
              path="/areas"
              element={
                <ProtectedRoute>
                  <DashboardWithNav><AREABuilderPage /></DashboardWithNav>
                </ProtectedRoute>
              }
            />
            <Route path="/" element={<Navigate to="/dashboard" replace />} />
            <Route path="*" element={<Navigate to="/dashboard" replace />} />
        </Routes>
      </AuthProvider>
    </BrowserRouter>
  );
};

export default App;