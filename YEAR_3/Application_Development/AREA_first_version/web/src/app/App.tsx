import React from 'react';
import { BrowserRouter, Routes, Route, Navigate } from 'react-router-dom';
import { GoogleOAuthProvider } from '@react-oauth/google';
import { AuthProvider } from '../contexts/AuthContext';
import ProtectedRoute from '../components/ProtectedRoute';
import LoginPage from '../pages/LoginPage';
import RegisterPage from '../pages/RegisterPage';
import Dashboard from '../pages/Dashboard';
import ServicesPage from '../pages/ServicesPage';
import AREABuilderPage from '../pages/AREABuilderPage';
import Header from '../components/Header';
import { useAuth } from '../contexts/AuthContext';
import { GOOGLE_CLIENT_ID } from '../config/constants';

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
    <GoogleOAuthProvider clientId={GOOGLE_CLIENT_ID}>
      <BrowserRouter>
        <AuthProvider>
          <Routes>
            <Route path="/login" element={<LoginPage />} />
            <Route path="/register" element={<RegisterPage />} />
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
    </GoogleOAuthProvider>
  );
};

export default App;