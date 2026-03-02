import React from 'react';
import { News as NewsType, getImageUrl } from '../services/api';
import ImageWithFallback from './ImageWithFallback';

interface NewsCardProps {
  news: NewsType;
  onClick: () => void;
  showFullContent?: boolean;
}

const NewsCard: React.FC<NewsCardProps> = ({
  news,
  onClick,
  showFullContent = false
}) => {
  return (
    <div
      className="news-card"
      onClick={onClick}
      style={{
        cursor: 'pointer',
        transition: 'all 0.3s ease',
        height: '100%',
        display: 'flex',
        flexDirection: 'column'
      }}
      onMouseEnter={(e) => {
        e.currentTarget.style.transform = 'translateY(-5px)';
        e.currentTarget.style.boxShadow = '0 15px 35px rgba(0,0,0,0.15)';
      }}
      onMouseLeave={(e) => {
        e.currentTarget.style.transform = 'translateY(0)';
        e.currentTarget.style.boxShadow = '0 5px 20px rgba(0,0,0,0.1)';
      }}
    >
      <div className="card" style={{ height: '100%', display: 'flex', flexDirection: 'column' }}>
        {}
        <div style={{ marginBottom: '1rem', textAlign: 'center' }}>
          <ImageWithFallback
            src={getImageUrl.news(news.jeb_id)}
            alt={`Image de ${news.title}`}
            style={{
              width: '100%',
              height: '150px',
              objectFit: 'cover',
              borderRadius: '8px'
            }}
          />
        </div>

        <h3 style={{ color: 'var(--purple-dark)', marginBottom: '1rem' }}>
          {news.title}
        </h3>

        <div style={{
          display: 'flex',
          gap: '0.5rem',
          marginBottom: '1rem',
          flexWrap: 'wrap'
        }}>
          <span style={{
            background: 'var(--coral-medium)',
            color: 'white',
            padding: '0.25rem 0.75rem',
            borderRadius: '20px',
            fontSize: '0.8rem'
          }}>
            {news.category}
          </span>
          <span style={{
            background: 'var(--purple-medium)',
            color: 'white',
            padding: '0.25rem 0.75rem',
            borderRadius: '20px',
            fontSize: '0.8rem'
          }}>
            {new Date(news.news_date).toLocaleDateString('fr-FR')}
          </span>
          {news.location && (
            <span style={{
              background: '#F49C9C',
              color: 'white',
              padding: '0.25rem 0.75rem',
              borderRadius: '20px',
              fontSize: '0.8rem'
            }}>
              📍 {news.location}
            </span>
          )}
        </div>

        {/*<div style={{ flex: 1 }}>
          {news.description && (
            <div style={{ marginBottom: '1rem' }}>
              {/*<p><strong>Résumé:</strong></p>
              <MarkdownContent
                content={news.description}
                maxLength={showFullContent ? undefined : 100}
              />
            </div>
          )}

          {news.content && showFullContent && (
            <div style={{ marginBottom: '1rem' }}>
              <p><strong>Contenu:</strong></p>
              <MarkdownContent
                content={news.content}
                maxLength={200}
              />
            </div>
          )}
        </div>*/}

        <div style={{
          marginTop: '1rem',
          color: 'var(--coral-medium)',
          fontSize: '0.9rem',
          fontWeight: '500',
          textAlign: 'center'
        }}>
          {showFullContent ? 'Read the complete article →' : 'Read the article →'}
        </div>
      </div>
    </div>
  );
};

export default NewsCard;
