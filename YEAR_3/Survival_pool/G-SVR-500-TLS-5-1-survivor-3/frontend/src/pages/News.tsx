import React, { useState, useEffect } from 'react';
import { newsService, News as NewsType } from '../services/api';
import LoadingSpinner from '../components/LoadingSpinner';
import FadeInOnScroll from '../components/FadeInOnScroll';
import NewsCard from '../components/NewsCard';
import Modal from '../components/Modal';
import MarkdownContent from '../components/MarkdownContent';

const NewsPage: React.FC = () => {
  const [news, setNews] = useState<NewsType[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [selectedNews, setSelectedNews] = useState<NewsType | null>(null);
  const [isModalOpen, setIsModalOpen] = useState(false);

  useEffect(() => {
    const fetchNews = async () => {
      try {
        setLoading(true);
        const data = await newsService.getAll();
        setNews(data);
      } catch (err) {
        setError('Error loading des newss');
        console.error('Error fetching news:', err);
      } finally {
        setLoading(false);
      }
    };

    fetchNews();
  }, []);

  const handleNewsClick = (newsItem: NewsType) => {
    setSelectedNews(newsItem);
    setIsModalOpen(true);
  };

  const closeModal = () => {
    setIsModalOpen(false);
    setSelectedNews(null);
  };

  if (loading) {
    return <LoadingSpinner size="large" text="Loading newss..." />;
  }

  if (error) {
    return (
      <div className="container" style={{ paddingTop: '2rem', textAlign: 'center' }}>
        <h1>Error</h1>
        <p style={{ color: 'red' }}>{error}</p>
        <p>Vérifiez que le backend Django est démarré sur http://localhost:8000</p>
      </div>
    );
  }

  return (
    <div className="container" style={{ paddingTop: '2rem' }}>
      <h1 style={{ textAlign: 'center', marginBottom: '3rem' }}>Lastest News</h1>

      {news.length === 0 ? (
        <div style={{ textAlign: 'center', padding: '2rem' }}>
          <p>Aucune news trouvée.</p>
          <p>Assurez-vous que les données JEB sont synchronisées.</p>
        </div>
      ) : (
        <div style={{
          display: 'grid',
          gridTemplateColumns: 'repeat(auto-fit, minmax(350px, 1fr))',
          gap: '2rem'
        }}>
          {news.map((article, index) => (
            <FadeInOnScroll
              key={article.id}
              delay={index * 100}
              direction="up"
            >
              <NewsCard
                news={article}
                onClick={() => handleNewsClick(article)}
                showFullContent={false}
              />
            </FadeInOnScroll>
          ))}
        </div>
      )}

      {}
      <Modal
        isOpen={isModalOpen}
        onClose={closeModal}
        title={selectedNews?.title || ''}
        size="large"
      >
        {selectedNews && (
          <div>
            <div style={{
              display: 'flex',
              gap: '1rem',
              marginBottom: '2rem',
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
                {selectedNews.category}
              </span>
              <span style={{
                background: 'var(--purple-medium)',
                color: 'white',
                padding: '0.25rem 0.75rem',
                borderRadius: '20px',
                fontSize: '0.9rem'
              }}>
                {new Date(selectedNews.news_date).toLocaleDateString('fr-FR')}
              </span>
              {selectedNews.location && (
                <span style={{
                  background: '#F49C9C',
                  color: 'white',
                  padding: '0.25rem 0.75rem',
                  borderRadius: '20px',
                  fontSize: '0.9rem'
                }}>
                  📍 {selectedNews.location}
                </span>
              )}
            </div>

            {selectedNews.description && (
              <div style={{ marginBottom: '2rem' }}>
                <h3 style={{ color: 'var(--coral-medium)', marginBottom: '1rem' }}>
                  Summary
                </h3>
                <MarkdownContent content={selectedNews.description} />
              </div>
            )}

            {selectedNews.content && (
              <div style={{ marginBottom: '2rem' }}>
                <h3 style={{ color: 'var(--coral-medium)', marginBottom: '1rem' }}>
                  All Content
                </h3>
                <MarkdownContent content={selectedNews.content} />
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
                  Published on {new Date(selectedNews.created_at).toLocaleDateString('fr-FR')}
                </small>
                {selectedNews.updated_at !== selectedNews.created_at && (
                  <small style={{ color: '#666', marginLeft: '1rem' }}>
                    Updated on {new Date(selectedNews.updated_at).toLocaleDateString('fr-FR')}
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
        )}
      </Modal>
    </div>
  );
};

export default NewsPage;
