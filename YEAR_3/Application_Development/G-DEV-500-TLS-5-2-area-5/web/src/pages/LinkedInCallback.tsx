import React, { useEffect, useRef } from 'react';
import { useNavigate, useLocation } from 'react-router-dom';
import { useAuth } from '../contexts/AuthContext';
import { servicesAPI } from '../services/api';
import { LINKEDIN_REDIRECT_URI } from '../config/constants';

const LinkedInCallback: React.FC = () => {
    const { loginWithLinkedIn, isAuthenticated, token } = useAuth();
    const navigate = useNavigate();
    const location = useLocation();
    const processedRef = useRef(false);

    useEffect(() => {
        const handleCallback = async () => {
            if (processedRef.current) return;
            processedRef.current = true;

            const params = new URLSearchParams(location.search);
            const code = params.get('code');
            const error = params.get('error');
            const errorDescription = params.get('error_description');

            if (error) {
                console.error('LinkedIn Auth Error:', error, errorDescription);
                if (isAuthenticated) {
                    navigate('/services', { state: { error: `LinkedIn Connection Error: ${errorDescription || error}` } });
                } else {
                    navigate('/login', { state: { error: `LinkedIn Login Error: ${errorDescription || error}` } });
                }
                return;
            }

            if (code) {
                try {
                    if (isAuthenticated && token) {
                        // User is connecting the service
                        await servicesAPI.connectLinkedInService(token, code, LINKEDIN_REDIRECT_URI);
                        navigate('/services', { state: { success: 'LinkedIn service connected successfully!' } });
                    } else {
                        // User is logging in
                        await loginWithLinkedIn(code, LINKEDIN_REDIRECT_URI);
                        navigate('/dashboard');
                    }
                } catch (err: any) {
                    console.error('LinkedIn Error:', err);
                    if (isAuthenticated) {
                        navigate('/services', { state: { error: err.message || 'Failed to connect LinkedIn service.' } });
                    } else {
                        navigate('/login', { state: { error: err.message || 'Failed to authenticate with LinkedIn.' } });
                    }
                }
            } else {
                navigate(isAuthenticated ? '/services' : '/login');
            }
        };

        handleCallback();
    }, [location, loginWithLinkedIn, navigate, isAuthenticated, token]);

    return (
        <div className="flex items-center justify-center min-h-screen bg-gray-50 dark:bg-gray-900">
            <div className="text-center">
                <div className="animate-spin rounded-full h-12 w-12 border-b-2 border-indigo-600 mx-auto mb-4"></div>
                <h2 className="text-xl font-semibold text-gray-700 dark:text-gray-200">Authenticating with LinkedIn...</h2>
                <p className="mt-2 text-gray-500">Please wait, we log you in.</p>
            </div>
        </div>
    );
};

export default LinkedInCallback;
