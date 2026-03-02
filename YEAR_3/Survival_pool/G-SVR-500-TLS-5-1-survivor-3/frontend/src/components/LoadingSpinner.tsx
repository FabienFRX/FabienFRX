import React from 'react';

interface LoadingSpinnerProps {
  size?: 'small' | 'medium' | 'large';
  text?: string;
}

const LoadingSpinner: React.FC<LoadingSpinnerProps> = ({
  size = 'medium',
  text = 'Loading...'
}) => {
  const sizeClasses = {
    small: 'w-4 h-4',
    medium: 'w-8 h-8',
    large: 'w-12 h-12'
  };

  return (
    <div style={{
      display: 'flex',
      flexDirection: 'column',
      alignItems: 'center',
      justifyContent: 'center',
      padding: '2rem'
    }}>
      <div
        className={`${sizeClasses[size]} border-3 border-gray-300 border-t-coral-medium rounded-full animate-spin`}
        style={{
          border: '3px solid #f3f3f3',
          borderTop: '3px solid var(--coral-medium)',
          borderRadius: '50%',
          animation: 'spin 1s linear infinite'
        }}
      ></div>
      {text && (
        <p style={{
          marginTop: '1rem',
          color: 'var(--coral-medium)',
          fontSize: '0.9rem'
        }}>
          {text}
        </p>
      )}
    </div>
  );
};

export default LoadingSpinner;
