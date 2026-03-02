import React, { useEffect, useRef } from 'react';
import { useNavigate, useLocation } from 'react-router-dom';
import { useAuth } from '../contexts/AuthContext';
import { GITLAB_REDIRECT_URI } from '../config/constants';
import { servicesAPI } from '../services/api';

const GitLabCallback: React.FC = () => {
    const { loginWithGitLab, token, isLoading } = useAuth();
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
                console.error('GitLab Auth Error:', error, errorDescription);
                if (token) {
                    navigate('/services', { state: { error: `GitLab Connection Error: ${errorDescription || error}` } });
                } else {
                    navigate('/login', { state: { error: `GitLab Login Error: ${errorDescription || error}` } });
                }
                return;
            }

            if (code) {
                try {
                    if (token) {
                        // User is already logged in, connect the service
                        await servicesAPI.connectGitLabService(token, code, GITLAB_REDIRECT_URI);
                        navigate('/services', { state: { success: 'GitLab service connected successfully!' } });
                    } else {
                        // User is not logged in, perform login
                        await loginWithGitLab(code, GITLAB_REDIRECT_URI);
                        navigate('/dashboard');
                    }
                } catch (err: any) {
                    console.error('GitLab Auth Error:', err);
                    const errorMessage = err.message || 'Failed to authenticate with GitLab.';
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
    }, [location, loginWithGitLab, navigate, token, isLoading]);

    return (
        <div className="flex items-center justify-center min-h-screen bg-gray-50 dark:bg-gray-900">
            <div className="text-center">
                <div className="animate-spin rounded-full h-12 w-12 border-b-2 border-orange-600 dark:border-orange-400 mx-auto mb-4"></div>
                <h2 className="text-xl font-semibold text-gray-700 dark:text-gray-200">Authenticating with GitLab...</h2>
                <p className="mt-2 text-gray-500 dark:text-gray-400">Please wait while we log you in.</p>
            </div>
        </div>
    );
};

export default GitLabCallback;
