import React from 'react';
import { Link, useLocation } from 'react-router-dom';

interface NavigationProps {
  isAuthenticated: boolean;
  userRole: 'startup' | 'admin' | 'investor' | 'visitor' | null;
  onLogout: () => void;
}

const Navigation: React.FC<NavigationProps> = ({ isAuthenticated, userRole, onLogout }) => {
  const location = useLocation();

  const isActive = (path: string) => location.pathname === path;

  return (
    <header>
      <nav>
        <div className="logo">
          <Link to="/" style={{ color: 'inherit', textDecoration: 'none', display: 'flex', alignItems: 'center', gap: '10px' }}>
            <img 
              src="/logo.png" 
              alt="JEB Incubator Logo" 
              style={{ 
                height: '100px',
                borderRadius: '12px',
                boxShadow: '0 8px 16px rgba(0, 0, 0, 0.2), 0 4px 8px rgba(0, 0, 0, 0.1)',
                border: '2px solid rgba(255, 255, 255, 0.3)',
                transition: 'transform 0.3s ease, box-shadow 0.3s ease'
              }}
              onMouseEnter={(e) => {
                e.currentTarget.style.transform = 'translateY(-2px)';
                e.currentTarget.style.boxShadow = '0 12px 20px rgba(0, 0, 0, 0.25), 0 6px 12px rgba(0, 0, 0, 0.15)';
              }}
              onMouseLeave={(e) => {
                e.currentTarget.style.transform = 'translateY(0)';
                e.currentTarget.style.boxShadow = '0 8px 16px rgba(0, 0, 0, 0.2), 0 4px 8px rgba(0, 0, 0, 0.1)';
              }}
            />
          </Link>
        </div>

        <ul className="nav-links">
          {}
          <li><Link to="/" className={isActive('/') ? 'active' : ''}>Home</Link></li>
          <li><Link to="/projects" className={isActive('/projects') ? 'active' : ''}>Projects</Link></li>
          <li><Link to="/news" className={isActive('/news') ? 'active' : ''}>News</Link></li>
          <li><Link to="/events" className={isActive('/events') ? 'active' : ''}>Events</Link></li>
          <li><Link to="/about" className={isActive('/about') ? 'active' : ''}>About</Link></li>
          
          {}
          {isAuthenticated && userRole === 'admin' && (
            <li><Link to="/dashboard" className={isActive('/dashboard') ? 'active' : ''}>Dashboard</Link></li>
          )}

          {}
          {isAuthenticated && userRole === 'admin' && (
            <>
              <li><Link to="/admin/projects" className={isActive('/admin/projects') ? 'active' : ''}>Projects</Link></li>
              <li><Link to="/admin/news" className={isActive('/admin/news') ? 'active' : ''}>News</Link></li>
              <li><Link to="/admin/events" className={isActive('/admin/events') ? 'active' : ''}>Events</Link></li>
              <li><Link to="/admin/users" className={isActive('/admin/users') ? 'active' : ''}>Users</Link></li>
            </>
          )}

          {}
          {!isAuthenticated ? (
            <li><Link to="/login" className={isActive('/login') ? 'active' : ''}>Login</Link></li>
          ) : (
            <li>
              <button onClick={onLogout} style={{
                background: 'none',
                border: 'none',
                color: 'white',
                cursor: 'pointer',
                fontSize: 'inherit',
                fontFamily: 'inherit'
              }}>
                Logout
              </button>
            </li>
          )}
        </ul>
      </nav>
    </header>
  );
};

export default Navigation;
