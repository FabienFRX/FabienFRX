import React from 'react';
import { Event as EventType, getImageUrl } from '../services/api';
import MarkdownContent from './MarkdownContent';
import ImageWithFallback from './ImageWithFallback';

interface EventCardProps {
  event: EventType;
  onClick: () => void;
  showFullContent?: boolean;
}

const EventCard: React.FC<EventCardProps> = ({
  event,
  onClick,
  showFullContent = false
}) => {
  return (
    <div
      className="event-card"
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
            src={getImageUrl.event(event.jeb_id)}
            alt={`Image de ${event.name}`}
            style={{
              width: '100%',
              height: '150px',
              objectFit: 'cover',
              borderRadius: '8px'
            }}
          />
        </div>

        <h3 style={{ color: 'var(--purple-dark)', marginBottom: '1rem' }}>
          {event.name}
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
            {event.event_type}
          </span>
          <span style={{
            background: 'var(--purple-medium)',
            color: 'white',
            padding: '0.25rem 0.75rem',
            borderRadius: '20px',
            fontSize: '0.8rem'
          }}>
            {new Date(event.dates).toLocaleDateString('fr-FR')}
          </span>
        </div>

        <div style={{ flex: 1 }}>
          <p><strong>Place :</strong> {event.location}</p>
          {event.target_audience && <p><strong>Target Public:</strong> {event.target_audience}</p>}
          {event.max_participants && <p><strong>Max participants:</strong> {event.max_participants}</p>}

          {event.description && (
            <div style={{ marginTop: '1rem' }}>
              <p><strong>Description:</strong></p>
              <MarkdownContent
                content={event.description}
                maxLength={showFullContent ? undefined : 100}
              />
            </div>
          )}
        </div>

        <div style={{
          marginTop: '1rem',
          color: 'var(--coral-medium)',
          fontSize: '0.9rem',
          fontWeight: '500',
          textAlign: 'center'
        }}>
          {showFullContent ? 'Show complete details →' : 'Show details →'}
        </div>
      </div>
    </div>
  );
};

export default EventCard;
