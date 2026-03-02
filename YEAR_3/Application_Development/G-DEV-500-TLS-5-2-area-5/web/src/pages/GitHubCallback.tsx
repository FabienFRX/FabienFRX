import React, { useEffect, useRef } from 'react';
import { useNavigate, useLocation } from 'react-router-dom';
import { useAuth } from '../contexts/AuthContext';
import { GITHUB_REDIRECT_URI } from '../config/constants';
import { servicesAPI } from '../services/api';

const GitHubCallback: React.FC = () => {
    const { loginWithGitHub, token, isLoading } = useAuth();
    const navigate = useNavigate();
    const location = useLocation();
    const processedRef = useRef(false);

    useEffect(() => {
        const handleCallback = async () => {
            if (isLoading) return;

            if (processedRef.current) return;
            processedRef.current = true;

            const params = new URLSearchParams(location.search);
            const code = params.get('code');
            const error = params.get('error');
            const errorDescription = params.get('error_description');

            if (error) {
                console.error('GitHub Auth Error:', error, errorDescription);
                if (token) {
                    navigate('/services', { state: { error: `GitHub Connection Error: ${errorDescription || error}` } });
                } else {
                    navigate('/login', { state: { error: `GitHub Login Error: ${errorDescription || error}` } });
                }
                return;
            }

            if (code) {
                try {
                    if (token) {
                        // User is already logged in, connect the service
                        await servicesAPI.connectGitHubService(token, code, GITHUB_REDIRECT_URI);
                        navigate('/services', { state: { success: 'GitHub service connected successfully!' } });
                    } else {
                        // User is not logged in, perform login
                        await loginWithGitHub(code, GITHUB_REDIRECT_URI);
                        navigate('/dashboard');
                    }
                } catch (err: any) {
                    console.error('GitHub Auth Error:', err);
                    const errorMessage = err.message || 'Failed to authenticate with GitHub.';
                    if (token) {
                        navigate('/services', { state: { error: errorMessage } });
                    } else {
                        navigate('/login', { state: { error: errorMessage } });
                    }
                }
            } else {
                if (token) {
                    navigate('/services');
                } else {
                    navigate('/login');
                }
            }
        };

        handleCallback();
    }, [location, loginWithGitHub, navigate, token, isLoading]);

    return (
        <div className="flex items-center justify-center min-h-screen bg-gray-50 dark:bg-gray-900">
            <div className="text-center">
                <div className="animate-spin rounded-full h-12 w-12 border-b-2 border-gray-800 dark:border-gray-200 mx-auto mb-4"></div>
                <h2 className="text-xl font-semibold text-gray-700 dark:text-gray-200">Authenticating with GitHub...</h2>
                <p className="mt-2 text-gray-500 dark:text-gray-400">Please wait while we log you in.</p>
            </div>
        </div>
    );
};

export default GitHubCallback;
