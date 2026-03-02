import React, { useState, useEffect } from 'react';
import { useParams, useNavigate } from 'react-router-dom';
import { eventService, Event as EventType, getImageUrl } from '../services/api';
import LoadingSpinner from '../components/LoadingSpinner';
import MarkdownContent from '../components/MarkdownContent';
import ImageWithFallback from '../components/ImageWithFallback';

const EventDetail: React.FC = () => {
  const { id } = useParams<{ id: string }>();
  const navigate = useNavigate();
  const [event, setEvent] = useState<EventType | null>(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    if (id) {
      fetchEvent(parseInt(id));
    }
  }, [id]);

  const fetchEvent = async (eventId: number) => {
    try {
      setLoading(true);
      const data = await eventService.getById(eventId);
      setEvent(data);
    } catch (err) {
      setError('Error loading de l\'event');
      console.error('Error fetching event:', err);
    } finally {
      setLoading(false);
    }
  };

  if (loading) {
    return <LoadingSpinner size="large" text="Chargement de l'event..." />;
  }

  if (error || !event) {
    return (
      <div className="container" style={{ paddingTop: '2rem', textAlign: 'center' }}>
        <h1>Error</h1>
        <p style={{ color: 'red' }}>{error || 'Événement non trouvé'}</p>
        <button
          onClick={() => navigate('/events')}
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
          Back to Event
        </button>
      </div>
    );
  }

  return (
    <div className="container" style={{ paddingTop: '2rem' }}>
      <button
        onClick={() => navigate('/events')}
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
        ← Back to Event
      </button>

      <div className="card">
        <div style={{
          display: 'grid',
          gridTemplateColumns: '1fr 300px',
          gap: '2rem',
          marginBottom: '2rem'
        }}>
          <div>
            <h1 style={{ color: 'var(--purple-dark)', marginBottom: '1rem' }}>
              {event.name}
            </h1>

            <div style={{
              display: 'flex',
              gap: '1rem',
              marginBottom: '1.5rem',
              flexWrap: 'wrap'
            }}>
              <span style={{
                background: 'var(--coral-medium)',
                color: 'white',
                padding: '0.25rem 0.75rem',
                borderRadius: '20px',
                fontSize: '0.9rem'
              }}>
                {event.event_type}
              </span>
              <span style={{
                background: 'var(--purple-medium)',
                color: 'white',
                padding: '0.25rem 0.75rem',
                borderRadius: '20px',
                fontSize: '0.9rem'
              }}>
                {new Date(event.dates).toLocaleDateString('fr-FR')}
              </span>
            </div>

            {event.description && (
              <div style={{ marginBottom: '2rem' }}>
                <h3 style={{ color: 'var(--coral-medium)', marginBottom: '1rem' }}>
                  Description
                </h3>
                <MarkdownContent content={event.description} />
              </div>
            )}
          </div>

          <div>
            <div className="card" style={{ padding: '1.5rem' }}>
              <h3 style={{ marginBottom: '1rem' }}>Event's details</h3>

              <div style={{ marginBottom: '1rem' }}>
                <strong>Date:</strong><br />
                <span style={{ color: '#666' }}>
                  {new Date(event.dates).toLocaleDateString('fr-FR', {
                    weekday: 'long',
                    year: 'numeric',
                    month: 'long',
                    day: 'numeric'
                  })}
                </span>
              </div>

              <div style={{ marginBottom: '1rem' }}>
                <strong>Place:</strong><br />
                <span style={{ color: '#666' }}>{event.location}</span>
              </div>

              {event.target_audience && (
                <div style={{ marginBottom: '1rem' }}>
                  <strong>Target Public:</strong><br />
                  <span style={{ color: '#666' }}>{event.target_audience}</span>
                </div>
              )}

              {event.max_participants && (
                <div style={{ marginBottom: '1rem' }}>
                  <strong>Maximum Participants:</strong><br />
                  <span style={{ color: '#666' }}>{event.max_participants}</span>
                </div>
              )}

              <div style={{ marginBottom: '1.5rem' }}>
                <strong>Event type:</strong><br />
                <span style={{ color: '#666' }}>{event.event_type}</span>
              </div>

              <button
                style={{
                  width: '100%',
                  background: 'var(--coral-medium)',
                  color: 'white',
                  padding: '0.75rem',
                  borderRadius: '4px',
                  border: 'none',
                  cursor: 'pointer',
                  fontSize: '1rem'
                }}
              >
                Register Event
              </button>
            </div>

            {}
            <div className="card" style={{ padding: '1.5rem', marginTop: '1rem' }}>
              <h3 style={{ marginBottom: '1rem' }}>Image</h3>
              <ImageWithFallback
                src={getImageUrl.event(event.jeb_id)}
                alt={`Image de ${event.name}`}
                style={{
                  width: '100%',
                  height: '200px',
                  objectFit: 'cover',
                  borderRadius: '8px'
                }}
              />
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};

export default EventDetail;
