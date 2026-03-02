import React, { useState, useEffect } from 'react';
import { useParams, useNavigate } from 'react-router-dom';
import { newsService, News as NewsType, getImageUrl } from '../services/api';
import LoadingSpinner from '../components/LoadingSpinner';
import MarkdownContent from '../components/MarkdownContent';
import ImageWithFallback from '../components/ImageWithFallback';

const NewsDetail: React.FC = () => {
  const { id } = useParams<{ id: string }>();
  const navigate = useNavigate();
  const [news, setNews] = useState<NewsType | null>(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    if (id) {
      fetchNews(parseInt(id));
    }
  }, [id]);

  const fetchNews = async (newsId: number) => {
    try {
      setLoading(true);
      const data = await newsService.getById(newsId);
      setNews(data);
    } catch (err) {
      setError('Error loading de l\'news');
      console.error('Error fetching news:', err);
    } finally {
      setLoading(false);
    }
  };

  if (loading) {
    return <LoadingSpinner size="large" text="Chargement de l'news..." />;
  }

  if (error || !news) {
    return (
      <div className="container" style={{ paddingTop: '2rem', textAlign: 'center' }}>
        <h1>Error</h1>
        <p style={{ color: 'red' }}>{error || 'Actualité non trouvée'}</p>
        <button
          onClick={() => navigate('/news')}
          style={{
            padding: '0.75rem 1.5rem',
            backgroundColor: 'var(--coral-medium)',
            color: 'white',
            border: 'none',
            borderRadius: '4px',
            cursor: 'pointer',
            marginTop: '1rem'
          }}
        >
          Retour aux newss
        </button>
      </div>
    );
  }

  return (
    <div className="container" style={{ paddingTop: '2rem' }}>
      <button
        onClick={() => navigate('/news')}
        style={{
          padding: '0.5rem 1rem',
          backgroundColor: '#f5f5f5',
          color: '#333',
          border: '1px solid #ddd',
          borderRadius: '4px',
          cursor: 'pointer',
          marginBottom: '2rem'
        }}
      >
        ← Retour aux newss
      </button>

      <div className="card">
        <div style={{ marginBottom: '2rem' }}>
          <h1 style={{ color: 'var(--purple-dark)', marginBottom: '1rem' }}>
            {news.title}
          </h1>

          <div style={{
            display: 'flex',
            gap: '1rem',
            marginBottom: '1.5rem',
            flexWrap: 'wrap',
            alignItems: 'center'
          }}>
            <span style={{
              background: 'var(--coral-medium)',
              color: 'white',
              padding: '0.25rem 0.75rem',
              borderRadius: '20px',
              fontSize: '0.9rem'
            }}>
              {news.category}
            </span>
            <span style={{
              background: 'var(--purple-medium)',
              color: 'white',
              padding: '0.25rem 0.75rem',
              borderRadius: '20px',
              fontSize: '0.9rem'
            }}>
              {new Date(news.news_date).toLocaleDateString('fr-FR')}
            </span>
            {news.location && (
              <span style={{
                background: '#6c757d',
                color: 'white',
                padding: '0.25rem 0.75rem',
                borderRadius: '20px',
                fontSize: '0.9rem'
              }}>
                📍 {news.location}
              </span>
            )}
          </div>

          {}
          <div style={{ marginBottom: '2rem', textAlign: 'center' }}>
            <ImageWithFallback
              src={getImageUrl.news(news.jeb_id)}
              alt={`Image de ${news.title}`}
              style={{
                width: '100%',
                maxWidth: '600px',
                height: '300px',
                objectFit: 'cover',
                borderRadius: '8px',
                margin: '0 auto'
              }}
            />
          </div>

          {news.description && (
            <div style={{ marginBottom: '2rem' }}>
              <h3 style={{ color: 'var(--coral-medium)', marginBottom: '1rem' }}>
                Summary
              </h3>
              <MarkdownContent content={news.description} />
            </div>
          )}

          {news.content && (
            <div style={{ marginBottom: '2rem' }}>
              <h3 style={{ color: 'var(--coral-medium)', marginBottom: '1rem' }}>
                All Content
              </h3>
              <MarkdownContent content={news.content} />
            </div>
          )}

          <div style={{
            display: 'flex',
            justifyContent: 'space-between',
            alignItems: 'center',
            padding: '1rem',
            backgroundColor: '#f8f9fa',
            borderRadius: '8px',
            marginTop: '2rem'
          }}>
            <div>
              <small style={{ color: '#666' }}>
                Published on {new Date(news.created_at).toLocaleDateString('fr-FR')}
              </small>
              {news.updated_at !== news.created_at && (
                <small style={{ color: '#666', marginLeft: '1rem' }}>
                  Updated on {new Date(news.updated_at).toLocaleDateString('fr-FR')}
                </small>
              )}
            </div>
            <div>
              <button
                style={{
                  background: 'var(--coral-medium)',
                  color: 'white',
                  padding: '0.5rem 1rem',
                  borderRadius: '4px',
                  border: 'none',
                  cursor: 'pointer',
                  fontSize: '0.9rem'
                }}
              >
                Share
              </button>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};

export default NewsDetail;
