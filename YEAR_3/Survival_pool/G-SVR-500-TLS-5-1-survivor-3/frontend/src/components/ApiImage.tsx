import React, { useState, useEffect } from 'react';
import { getImageWithFallback } from '../services/imageService';

interface ApiImageProps {
  endpoint: 'news' | 'events' | 'users' | 'startups' | 'founders';
  id: number;
  subId?: number; // Pour les fondateurs (startup_id)
  alt: string;
  className?: string;
  style?: React.CSSProperties;
  fallbackSrc?: string;
  loading?: React.ReactNode;
}

const ApiImage: React.FC<ApiImageProps> = ({
  endpoint,
  id,
  subId,
  alt,
  className,
  style,
  fallbackSrc = '/logo.png',
  loading = <div className="animate-pulse bg-gray-200 rounded" style={style} />
}) => {
  const [imageSrc, setImageSrc] = useState<string>('');
  const [isLoading, setIsLoading] = useState(true);
  const [hasError, setHasError] = useState(false);

  useEffect(() => {
    const loadImage = async () => {
      try {
        setIsLoading(true);
        setHasError(false);
        
        const url = await getImageWithFallback(endpoint, id, subId, fallbackSrc);
        setImageSrc(url);
      } catch (error) {
        console.error('Error loading image:', error);
        setImageSrc(fallbackSrc);
        setHasError(true);
      } finally {
        setIsLoading(false);
      }
    };

    loadImage();
  }, [endpoint, id, subId, fallbackSrc]);

  if (isLoading) {
    return <>{loading}</>;
  }

  return (
    <img
      src={imageSrc}
      alt={alt}
      className={className}
      style={style}
      onError={() => {
        if (!hasError) {
          setHasError(true);
          setImageSrc(fallbackSrc);
        }
      }}
    />
  );
};

export default ApiImage;
