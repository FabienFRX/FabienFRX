import React, { useState, useEffect } from 'react';
import { useNavigate, Link, useSearchParams } from 'react-router-dom';
import { useAuth } from '../contexts/AuthContext';
import { OAUTH_ENDPOINTS } from '../config/constants';

const LoginPage: React.FC = () => {
  const [formData, setFormData] = useState({ email: '', password: '' });
  const [error, setError] = useState<string | null>(null);
  const [loading, setLoading] = useState(false);
  const { login, setAuthToken } = useAuth();
  const navigate = useNavigate();
  const [searchParams] = useSearchParams();

  // Handle OAuth callback (token in URL params)
  useEffect(() => {
    const token = searchParams.get('token');
    const userEmail = searchParams.get('user');
    const errorParam = searchParams.get('error');

    if (errorParam) {
      setError(`OAuth authentication failed: ${errorParam}`);
      // Clean URL
      window.history.replaceState({}, document.title, '/login');
    } else if (token && userEmail) {
      // OAuth login successful - create user object
      const user = {
        id: 0, // Will be updated by API calls
        username: userEmail,
        email: userEmail,
        first_name: '',
        last_name: ''
      };
      
      setAuthToken(token);
      localStorage.setItem('auth_token', token);
      localStorage.setItem('user', JSON.stringify(user));
      navigate('/dashboard');
    }
  }, [searchParams, setAuthToken, navigate]);

  const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    setFormData({ ...formData, [e.target.name]: e.target.value });
    setError(null);
  };

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setError(null);
    setLoading(true);

    try {
      await login(formData.email, formData.password);
      navigate('/dashboard');
    } catch (err: any) {
      setError(err.message || 'Login failed. Please check your credentials.');
    } finally {
      setLoading(false);
    }
  };

  const handleGoogleLogin = () => {
    // Redirect to backend which handles OAuth flow
    window.location.href = OAUTH_ENDPOINTS.google.init;
  };

  const handleLinkedInLogin = () => {
    // Redirect to backend which handles OAuth flow
    window.location.href = OAUTH_ENDPOINTS.linkedin.init;
  };

  const handleGitHubLogin = () => {
    // Redirect to backend which handles OAuth flow
    window.location.href = OAUTH_ENDPOINTS.github.init;
  };

  const handleGitLabLogin = () => {
    // Redirect to backend which handles OAuth flow
    window.location.href = OAUTH_ENDPOINTS.gitlab.init;
  };

  return (
    <div className="flex items-center min-h-screen bg-white dark:bg-gray-900">
      <div className="container mx-auto">
        <div className="max-w-md mx-auto my-10">
          <div className="text-center">
            <h1 className="my-3 text-3xl font-semibold text-gray-700 dark:text-gray-200">Login</h1>
            <p className="text-gray-500 dark:text-gray-400">Login to access your AREA account</p>
          </div>
          <div className="m-7">
            {error && (
              <div className="mb-4 p-3 bg-red-100 border border-red-400 text-red-700 rounded dark:bg-red-900 dark:border-red-700 dark:text-red-200">
                {error}
              </div>
            )}
            <form onSubmit={handleSubmit}>
              <div className="mb-6">
                <label htmlFor="email" className="block mb-2 text-sm text-gray-600 dark:text-gray-400">Email Address</label>
                <input
                  type="email"
                  name="email"
                  id="email"
                  placeholder="you@company.com"
                  value={formData.email}
                  onChange={handleChange}
                  required
                  disabled={loading}
                  className="w-full px-3 py-2 placeholder-gray-300 border border-gray-300 rounded-md focus:outline-none focus:ring focus:ring-indigo-100 focus:border-indigo-300 dark:bg-gray-700 dark:text-white dark:placeholder-gray-500 dark:border-gray-600 dark:focus:ring-gray-900 dark:focus:border-gray-500 disabled:opacity-50"
                />
              </div>
              <div className="mb-6">
                <div className="flex justify-between mb-2">
                  <label htmlFor="password" className="text-sm text-gray-600 dark:text-gray-400">Password</label>
                  <button
                    onClick={(e) => e.preventDefault()}
                    className="text-sm text-gray-400 focus:outline-none focus:text-indigo-500 hover:text-indigo-500 dark:hover:text-indigo-300"
                  >
                    Forgot password?
                  </button>
                </div>
                <input
                  type="password"
                  name="password"
                  id="password"
                  placeholder="Your Password"
                  value={formData.password}
                  onChange={handleChange}
                  required
                  disabled={loading}
                  className="w-full px-3 py-2 placeholder-gray-300 border border-gray-300 rounded-md focus:outline-none focus:ring focus:ring-indigo-100 focus:border-indigo-300 dark:bg-gray-700 dark:text-white dark:placeholder-gray-500 dark:border-gray-600 dark:focus:ring-gray-900 dark:focus:border-gray-500 disabled:opacity-50"
                />
              </div>
              <div className="mb-6">
                <button
                  type="submit"
                  disabled={loading}
                  className="w-full px-3 py-4 text-white bg-indigo-500 rounded-md hover:bg-indigo-600 focus:bg-indigo-600 focus:outline-none transition disabled:opacity-50 disabled:cursor-not-allowed"
                >
                  {loading ? 'Logging in...' : 'Login'}
                </button>
              </div>
            </form>

            {/* Divider */}
            <div className="flex items-center my-6">
              <div className="flex-grow border-t border-gray-300 dark:border-gray-600"></div>
              <span className="mx-4 text-sm text-gray-500 dark:text-gray-400">OR</span>
              <div className="flex-grow border-t border-gray-300 dark:border-gray-600"></div>
            </div>

            {/* Google Login Button */}
            <button
              onClick={handleGoogleLogin}
              disabled={loading}
              className="w-full px-3 py-3 mb-6 text-gray-700 bg-white border border-gray-300 rounded-md hover:bg-gray-50 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-indigo-500 transition disabled:opacity-50 disabled:cursor-not-allowed flex items-center justify-center gap-3"
            >
              <svg className="w-5 h-5" viewBox="0 0 24 24">
                <path fill="#4285F4" d="M22.56 12.25c0-.78-.07-1.53-.2-2.25H12v4.26h5.92c-.26 1.37-1.04 2.53-2.21 3.31v2.77h3.57c2.08-1.92 3.28-4.74 3.28-8.09z"/>
                <path fill="#34A853" d="M12 23c2.97 0 5.46-.98 7.28-2.66l-3.57-2.77c-.98.66-2.23 1.06-3.71 1.06-2.86 0-5.29-1.93-6.16-4.53H2.18v2.84C3.99 20.53 7.7 23 12 23z"/>
                <path fill="#FBBC05" d="M5.84 14.09c-.22-.66-.35-1.36-.35-2.09s.13-1.43.35-2.09V7.07H2.18C1.43 8.55 1 10.22 1 12s.43 3.45 1.18 4.93l2.85-2.22.81-.62z"/>
                <path fill="#EA4335" d="M12 5.38c1.62 0 3.06.56 4.21 1.64l3.15-3.15C17.45 2.09 14.97 1 12 1 7.7 1 3.99 3.47 2.18 7.07l3.66 2.84c.87-2.6 3.3-4.53 6.16-4.53z"/>
              </svg>
              <span className="font-medium">Continue with Google</span>
            </button>

            {/* LinkedIn Login Button */}
            <button
              onClick={handleLinkedInLogin}
              disabled={loading}
              className="w-full px-3 py-3 mb-3 text-white bg-[#0077b5] border border-transparent rounded-md hover:bg-[#006097] focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-[#0077b5] transition disabled:opacity-50 disabled:cursor-not-allowed flex items-center justify-center gap-3"
            >
              <svg className="w-5 h-5 fill-current" viewBox="0 0 24 24">
                <path d="M20.447 20.452h-3.554v-5.569c0-1.328-.027-3.037-1.852-3.037-1.853 0-2.136 1.445-2.136 2.939v5.667H9.351V9h3.414v1.561h.046c.477-.9 1.637-1.85 3.37-1.85 3.601 0 4.267 2.37 4.267 5.455v6.286zM5.337 7.433c-1.144 0-2.063-.926-2.063-2.065 0-1.138.92-2.063 2.063-2.063 1.14 0 2.064.925 2.064 2.063 0 1.139-.925 2.065-2.064 2.065zm1.782 13.019H3.555V9h3.564v11.452zM22.225 0H1.771C.792 0 0 .774 0 1.729v20.542C0 23.227.792 24 1.771 24h20.451C23.2 24 24 23.227 24 22.271V1.729C24 .774 23.2 0 22.222 0h.003z"/>
              </svg>
              <span className="font-medium">Continue with LinkedIn</span>
            </button>

            {/* GitHub Login Button */}
            <button
              onClick={handleGitHubLogin}
              disabled={loading}
              className="w-full px-3 py-3 mb-3 text-white bg-[#24292e] border border-transparent rounded-md hover:bg-[#1b1f23] focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-[#24292e] transition disabled:opacity-50 disabled:cursor-not-allowed flex items-center justify-center gap-3"
            >
              <svg className="w-5 h-5 fill-current" viewBox="0 0 24 24">
                <path d="M12 0C5.37 0 0 5.37 0 12c0 5.31 3.435 9.795 8.205 11.385.6.105.825-.255.825-.57 0-.285-.015-1.23-.015-2.235-3.015.555-3.795-.735-4.035-1.41-.135-.345-.72-1.41-1.23-1.695-.42-.225-1.02-.78-.015-.795.945-.015 1.62.87 1.845 1.23 1.08 1.815 2.805 1.305 3.495.99.105-.78.42-1.305.765-1.605-2.67-.3-5.46-1.335-5.46-5.925 0-1.305.465-2.385 1.23-3.225-.12-.3-.54-1.53.12-3.18 0 0 1.005-.315 3.3 1.23.96-.27 1.98-.405 3-.405s2.04.135 3 .405c2.295-1.56 3.3-1.23 3.3-1.23.66 1.65.24 2.88.12 3.18.765.84 1.23 1.905 1.23 3.225 0 4.605-2.805 5.625-5.475 5.925.435.375.81 1.095.81 2.22 0 1.605-.015 2.895-.015 3.3 0 .315.225.69.825.57A12.02 12.02 0 0024 12c0-6.63-5.37-12-12-12z"/>
              </svg>
              <span className="font-medium">Continue with GitHub</span>
            </button>

            {/* GitLab Login Button */}
            <button
              onClick={handleGitLabLogin}
              disabled={loading}
              className="w-full px-3 py-3 mb-6 text-white bg-[#fc6d26] border border-transparent rounded-md hover:bg-[#e24329] focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-[#fc6d26] transition disabled:opacity-50 disabled:cursor-not-allowed flex items-center justify-center gap-3"
            >
              <svg className="w-5 h-5 fill-current" viewBox="0 0 24 24">
                <path d="M23.955 13.587l-1.342-4.135-2.664-8.189a.455.455 0 0 0-.867 0L16.418 9.45H7.582L4.919 1.263a.455.455 0 0 0-.867 0L1.388 9.452.046 13.587a.924.924 0 0 0 .331 1.023L12 23.054l11.623-8.443a.92.92 0 0 0 .332-1.024"/>
              </svg>
              <span className="font-medium">Continue with GitLab</span>
            </button>

            <p className="text-sm text-center text-gray-400">
              Don't have an account yet?{' '}
              <Link to="/register" className="text-indigo-400 focus:outline-none focus:underline focus:text-indigo-500 dark:focus:border-indigo-800">
                Sign up
              </Link>.
            </p>
          </div>
        </div>
      </div>
    </div>
  );
};

export default LoginPage;
