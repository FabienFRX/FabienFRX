import React, { useState, useEffect } from 'react';
import { eventService, Event as EventType } from '../services/api';
import LoadingSpinner from '../components/LoadingSpinner';
import FadeInOnScroll from '../components/FadeInOnScroll';
import EventCard from '../components/EventCard';
import Modal from '../components/Modal';
import MarkdownContent from '../components/MarkdownContent';

const Events: React.FC = () => {
  const [events, setEvents] = useState<EventType[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [selectedEvent, setSelectedEvent] = useState<EventType | null>(null);
  const [isModalOpen, setIsModalOpen] = useState(false);

  useEffect(() => {
    const fetchEvents = async () => {
      try {
        setLoading(true);
        const data = await eventService.getAll();
        setEvents(data);
      } catch (err) {
        setError('Error loading des events');
        console.error('Error fetching events:', err);
      } finally {
        setLoading(false);
      }
    };

    fetchEvents();
  }, []);

  const handleEventClick = (event: EventType) => {
    setSelectedEvent(event);
    setIsModalOpen(true);
  };

  const closeModal = () => {
    setIsModalOpen(false);
    setSelectedEvent(null);
  };

  if (loading) {
    return <LoadingSpinner size="large" text="Loading events..." />;
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
      <h1 style={{ textAlign: 'center', marginBottom: '3rem' }}>Events & Workshops</h1>

      {events.length === 0 ? (
        <div style={{ textAlign: 'center', padding: '2rem' }}>
          <p>No events found.</p>
          <p>Make sure JEB data is synchronized.</p>
        </div>
      ) : (
        <div style={{
          display: 'grid',
          gridTemplateColumns: 'repeat(auto-fit, minmax(350px, 1fr))',
          gap: '2rem'
        }}>
          {events.map((event, index) => (
            <FadeInOnScroll
              key={event.id}
              delay={index * 100}
              direction="up"
            >
              <EventCard
                event={event}
                onClick={() => handleEventClick(event)}
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
        title={selectedEvent?.name || ''}
        size="large"
      >
        {selectedEvent && (
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
                {selectedEvent.event_type}
              </span>
              <span style={{
                background: 'var(--purple-medium)',
                color: 'white',
                padding: '0.25rem 0.75rem',
                borderRadius: '20px',
                fontSize: '0.9rem'
              }}>
                {new Date(selectedEvent.dates).toLocaleDateString('fr-FR')}
              </span>
            </div>

            <div style={{
              display: 'grid',
              gridTemplateColumns: '1fr 300px',
              gap: '2rem',
              marginBottom: '2rem'
            }}>
              <div>
                {selectedEvent.description && (
                  <div style={{ marginBottom: '2rem' }}>
                    <h3 style={{ color: 'var(--coral-medium)', marginBottom: '1rem' }}>
                      Description
                    </h3>
                    <MarkdownContent content={selectedEvent.description} />
                  </div>
                )}
              </div>

              <div>
                <div className="card" style={{ padding: '1.5rem' }}>
                  <h3 style={{ marginBottom: '1rem' }}>Event's Details</h3>

                  <div style={{ marginBottom: '1rem' }}>
                    <strong>Date:</strong><br />
                    <span style={{ color: '#666' }}>
                      {new Date(selectedEvent.dates).toLocaleDateString('fr-FR', {
                        weekday: 'long',
                        year: 'numeric',
                        month: 'long',
                        day: 'numeric'
                      })}
                    </span>
                  </div>

                  <div style={{ marginBottom: '1rem' }}>
                    <strong>Place:</strong><br />
                    <span style={{ color: '#666' }}>{selectedEvent.location}</span>
                  </div>

                  {selectedEvent.target_audience && (
                    <div style={{ marginBottom: '1rem' }}>
                      <strong>Target Public:</strong><br />
                      <span style={{ color: '#666' }}>{selectedEvent.target_audience}</span>
                    </div>
                  )}

                  {selectedEvent.max_participants && (
                    <div style={{ marginBottom: '1rem' }}>
                      <strong>Maximum Participants:</strong><br />
                      <span style={{ color: '#666' }}>{selectedEvent.max_participants}</span>
                    </div>
                  )}

                  <div style={{ marginBottom: '1.5rem' }}>
                    <strong>Event Type:</strong><br />
                    <span style={{ color: '#666' }}>{selectedEvent.event_type}</span>
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
              </div>
            </div>
          </div>
        )}
      </Modal>
    </div>
  );
};

export default Events;
