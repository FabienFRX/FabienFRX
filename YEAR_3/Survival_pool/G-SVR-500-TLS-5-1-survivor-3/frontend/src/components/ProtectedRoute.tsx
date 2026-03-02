import React from 'react';
import { Navigate } from 'react-router-dom';
import { useAuth } from '../contexts/AuthContext';

interface ProtectedRouteProps {
  children: React.ReactNode;
  requiredRole?: 'admin' | 'startup' | 'visitor';
  redirectTo?: string;
}

const ProtectedRoute: React.FC<ProtectedRouteProps> = ({
  children,
  requiredRole,
  redirectTo = '/login'
}) => {
  const { isAuthenticated, user, loading } = useAuth();

  // Debug: Afficher les informations de protection
  console.log('ProtectedRoute - isAuthenticated:', isAuthenticated);
  console.log('ProtectedRoute - user:', user);
  console.log('ProtectedRoute - user role:', user?.role);
  console.log('ProtectedRoute - required role:', requiredRole);
  console.log('ProtectedRoute - role match:', user?.role === requiredRole);

  if (loading) {
    return <div>Loading...</div>;
  }

  if (!isAuthenticated) {
    console.log('ProtectedRoute - Not authenticated, redirecting to login');
    return <Navigate to={redirectTo} replace />;
  }

  if (requiredRole && user?.role !== requiredRole) {
    console.log('ProtectedRoute - Role mismatch, redirecting to home');
    return <Navigate to="/" replace />;
  }

  console.log('ProtectedRoute - Access granted');
  return <>{children}</>;
};

export default ProtectedRoute;
