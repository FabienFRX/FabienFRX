import React, { Suspense } from 'react';
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import { AuthProvider, useAuth } from './contexts/AuthContext';
import Navigation from './components/Navigation';
import ProtectedRoute from './components/ProtectedRoute';
import ErrorBoundary from './components/ErrorBoundary';
import LoadingSpinner from './components/LoadingSpinner';
import Dashboard from './pages/Dashboard';

// Lazy load pages for better performance
const Home = React.lazy(() => import('./pages/Home'));
const About = React.lazy(() => import('./pages/About'));
const StartupCatalog = React.lazy(() => import('./pages/StartupCatalog'));
const Events = React.lazy(() => import('./pages/Events'));
const News = React.lazy(() => import('./pages/News'));
const Investors = React.lazy(() => import('./pages/Investors'));
const Partners = React.lazy(() => import('./pages/Partners'));
const Users = React.lazy(() => import('./pages/Users'));
const TestAPI = React.lazy(() => import('./pages/TestAPI'));
const Login = React.lazy(() => import('./pages/Login'));
const Register = React.lazy(() => import('./pages/Register'));

// Admin pages
const AdminDashboard = React.lazy(() => import('./pages/admin/Dashboard'));
const AdminProjects = React.lazy(() => import('./pages/admin/Projects'));
const AdminEvents = React.lazy(() => import('./pages/admin/Events'));
const AdminNews = React.lazy(() => import('./pages/admin/News'));
const AdminUsers = React.lazy(() => import('./pages/admin/Users'));

// Startup pages
const StartupDashboard = React.lazy(() => import('./pages/startup/StartupDashboard'));
const StartupProfile = React.lazy(() => import('./pages/startup/Profile'));
const StartupOpportunities = React.lazy(() => import('./pages/startup/Opportunities'));

const AppContent: React.FC = () => {
  const { isAuthenticated, user, logout } = useAuth();

  return (
    <Router>
      <Navigation 
        isAuthenticated={isAuthenticated}
        userRole={user?.role || null}
        onLogout={logout}
      />
      
      <main>
        <Suspense fallback={<LoadingSpinner size="large" text="Loading page..." />}>
          <Routes>
            {/* Public routes */}
            <Route path="/" element={<Home />} />
            <Route path="/about" element={<About />} />
            <Route path="/projects" element={<StartupCatalog />} />
            <Route path="/events" element={<Events />} />
            <Route path="/news" element={<News />} />
          <Route path="/investors" element={<Investors />} />
          <Route path="/partners" element={<Partners />} />
          <Route path="/users" element={<Users />} />
          <Route path="/test-api" element={<TestAPI />} />
          <Route path="/login" element={<Login />} />
          <Route path="/register" element={<Register />} />
          <Route path="/dashboard" element={<Dashboard />} />

          {/* Startup routes */}
          <Route path="/startup/dashboard" element={
            <ProtectedRoute requiredRole="startup">
              <StartupDashboard />
            </ProtectedRoute>
          } />
          <Route path="/startup/profile" element={
            <ProtectedRoute requiredRole="startup">
              <StartupProfile />
            </ProtectedRoute>
          } />
          <Route path="/startup/opportunities" element={
            <ProtectedRoute requiredRole="startup">
              <StartupOpportunities />
            </ProtectedRoute>
          } />

          {/* Admin routes */}
          <Route path="/admin/dashboard" element={
            <ProtectedRoute requiredRole="admin">
              <AdminDashboard />
            </ProtectedRoute>
          } />
          <Route path="/admin/projects" element={
            <ProtectedRoute requiredRole="admin">
              <AdminProjects />
            </ProtectedRoute>
          } />
          <Route path="/admin/events" element={
            <ProtectedRoute requiredRole="admin">
              <AdminEvents />
            </ProtectedRoute>
          } />
          <Route path="/admin/news" element={
            <ProtectedRoute requiredRole="admin">
              <AdminNews />
            </ProtectedRoute>
          } />
          <Route path="/admin/users" element={
            <ProtectedRoute requiredRole="admin">
              <AdminUsers />
            </ProtectedRoute>
          } />
        </Routes>
        </Suspense>
      </main>
    </Router>
  );
};

const App: React.FC = () => {
  return (
    <ErrorBoundary>
      <AuthProvider>
        <AppContent />
      </AuthProvider>
    </ErrorBoundary>
  );
};

export default App;
