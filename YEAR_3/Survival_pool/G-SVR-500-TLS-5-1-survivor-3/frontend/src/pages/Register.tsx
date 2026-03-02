import React, { useState, useEffect } from 'react';
import { useAuth } from '../contexts/AuthContext';
import { useNavigate, Link } from 'react-router-dom';
import { startupService, Startup } from '../services/api';

const Register: React.FC = () => {
  const [formData, setFormData] = useState({
    username: '',
    email: '',
    password: '',
    password_confirm: '',
    first_name: '',
    last_name: '',
    role: 'visitor' as 'startup' | 'investor' | 'visitor',
    startup_id: ''
  });
  const [startups, setStartups] = useState<Startup[]>([]);
  const [error, setError] = useState('');
  const [loading, setLoading] = useState(false);
  const { register } = useAuth();
  const navigate = useNavigate();

  // Fetch startups when component mounts
  useEffect(() => {
    const fetchStartups = async () => {
      try {
        const data = await startupService.getAll();
        setStartups(data);
      } catch (error) {
        console.error('Error fetching startups:', error);
      }
    };
    fetchStartups();
  }, []);

  const handleChange = (e: React.ChangeEvent<HTMLInputElement | HTMLSelectElement>) => {
    const { name, value } = e.target;
    setFormData(prev => ({
      ...prev,
      [name]: value
    }));
  };

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setLoading(true);
    setError('');

    // Client-side validation
    if (formData.password !== formData.password_confirm) {
      setError('Passwords do not match');
      setLoading(false);
      return;
    }

    if (formData.password.length < 6) {
      setError('Password must be at least 6 characters long');
      setLoading(false);
      return;
    }

    // Validate startup selection for startup founders
    if (formData.role === 'startup' && !formData.startup_id) {
      setError('Please select which startup you represent');
      setLoading(false);
      return;
    }

    try {
      const result = await register(formData);
      if (result.success) {
        navigate('/');
      } else {
        setError(result.error || 'Registration failed');
      }
    } catch (err) {
      setError('Registration failed. Please try again.');
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
            Create Account
          </h2>
          <p className="login-subtitle">Join JEB Incubator today</p>
        </div>
        <form className="login-form" onSubmit={handleSubmit}>
          <div className="form-group">
            <label htmlFor="username" className="form-label">
              Username *
            </label>
            <input
              id="username"
              name="username"
              type="text"
              required
              className="form-input"
              placeholder="Choose a username"
              value={formData.username}
              onChange={handleChange}
              minLength={3}
            />
          </div>

          <div className="form-group">
            <label htmlFor="email" className="form-label">
              Email *
            </label>
            <input
              id="email"
              name="email"
              type="email"
              required
              className="form-input"
              placeholder="Enter your email"
              value={formData.email}
              onChange={handleChange}
            />
          </div>

          <div className="form-row">
            <div className="form-group">
              <label htmlFor="first_name" className="form-label">
                First Name
              </label>
              <input
                id="first_name"
                name="first_name"
                type="text"
                className="form-input"
                placeholder="First name"
                value={formData.first_name}
                onChange={handleChange}
              />
            </div>
            
            <div className="form-group">
              <label htmlFor="last_name" className="form-label">
                Last Name
              </label>
              <input
                id="last_name"
                name="last_name"
                type="text"
                className="form-input"
                placeholder="Last name"
                value={formData.last_name}
                onChange={handleChange}
              />
            </div>
          </div>
          
          <div className="form-group">
            <label htmlFor="role" className="form-label">
              I am a...
            </label>
            <select
              id="role"
              name="role"
              className="form-input"
              value={formData.role}
              onChange={handleChange}
            >
              <option value="visitor">Visitor</option>
              <option value="startup">Startup Founder</option>
              <option value="investor">Investor</option>
            </select>
          </div>

          {/* Startup selection - only show for startup founders */}
          {formData.role === 'startup' && (
            <div className="form-group">
              <label htmlFor="startup_id" className="form-label">
                Which startup do you represent? *
              </label>
              <select
                id="startup_id"
                name="startup_id"
                className="form-input"
                value={formData.startup_id}
                onChange={handleChange}
                required={formData.role === 'startup'}
              >
                <option value="">-- Select your startup --</option>
                {startups.map((startup) => (
                  <option key={startup.id} value={startup.id}>
                    {startup.name} ({startup.sector})
                  </option>
                ))}
              </select>
              {startups.length === 0 && (
                <p style={{ fontSize: '0.9rem', color: '#666', marginTop: '0.5rem' }}>
                  Loading startups...
                </p>
              )}
            </div>
          )}

          <div className="form-group">
            <label htmlFor="password" className="form-label">
              Password *
            </label>
            <input
              id="password"
              name="password"
              type="password"
              required
              className="form-input"
              placeholder="Create a password"
              value={formData.password}
              onChange={handleChange}
              minLength={6}
            />
          </div>

          <div className="form-group">
            <label htmlFor="password_confirm" className="form-label">
              Confirm Password *
            </label>
            <input
              id="password_confirm"
              name="password_confirm"
              type="password"
              required
              className="form-input"
              placeholder="Confirm your password"
              value={formData.password_confirm}
              onChange={handleChange}
              minLength={6}
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
                Creating account...
              </span>
            ) : (
              'Create Account'
            )}
          </button>

          <div className="register-link">
            Already have an account?{' '}
            <Link to="/login" className="link">
              Sign in here
            </Link>
          </div>
        </form>
      </div>
    </div>
  );
};

export default Register;
