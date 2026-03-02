import React, { useState } from 'react';
import { useAuth } from '../contexts/AuthContext';
import { useNavigate, Link } from 'react-router-dom';

const Login: React.FC = () => {
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');
  const [error, setError] = useState('');
  const [loading, setLoading] = useState(false);
  const { login } = useAuth();
  const navigate = useNavigate();

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setLoading(true);
    setError('');

    try {
      const success = await login(username, password);
      if (success) {
        navigate('/');
      } else {
        setError('Invalid credentials');
      }
    } catch (err) {
      setError('Login failed. Please try again.');
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="login-container">
      <div className="login-card">
        <div className="login-header">
          <div className="login-logo">
            <img 
              src="/logo.png" 
              alt="JEB Incubator Logo" 
              style={{ 
                height: '80px',
                borderRadius: '12px',
                marginBottom: '1rem'
              }}
            />
          </div>
          <h2 className="login-title">
            Welcome Back
          </h2>
          <p className="login-subtitle">Sign in to your account</p>
        </div>
        <form className="login-form" onSubmit={handleSubmit}>
          <div className="form-group">
            <label htmlFor="username" className="form-label">
              Username
            </label>
            <input
              id="username"
              name="username"
              type="text"
              required
              className="form-input"
              placeholder="Enter your username"
              value={username}
              onChange={(e) => setUsername(e.target.value)}
            />
          </div>
          
          <div className="form-group">
            <label htmlFor="password" className="form-label">
              Password
            </label>
            <input
              id="password"
              name="password"
              type="password"
              required
              className="form-input"
              placeholder="Enter your password"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
            />
          </div>

          {error && (
            <div className="error-message">{error}</div>
          )}

          <button
            type="submit"
            disabled={loading}
            className="login-button"
          >
            {loading ? (
              <span className="loading-text">
                <span className="loading-spinner"></span>
                Signing in...
              </span>
            ) : (
              'Sign in'
            )}
          </button>

          <div className="register-link">
            Don't have an account?{' '}
            <Link to="/register" className="link">
              Sign up here
            </Link>
          </div>
        </form>
      </div>
    </div>
  );
};

export default Login;
