import React, { useState, useEffect } from 'react';
import { eventService, Event as EventType } from '../../services/api';

const AdminEvents: React.FC = () => {
  const [events, setEvents] = useState<EventType[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [editingEvent, setEditingEvent] = useState<EventType | null>(null);
  const [showForm, setShowForm] = useState(false);

  useEffect(() => {
    fetchEvents();
  }, []);

  const fetchEvents = async () => {
    try {
      setLoading(true);
      const data = await eventService.getAll();
      setEvents(data);
    } catch (err) {
      setError('Error loading events');
      console.error('Error fetching events:', err);
    } finally {
      setLoading(false);
    }
  };

  const handleEdit = (event: EventType) => {
    setEditingEvent(event);
    setShowForm(true);
  };

  const handleDelete = async (id: number) => {
    if (window.confirm('Are you sure you want to delete this event?')) {
      try {

        console.log('Deleting event:', id);
        await fetchEvents(); // Reload the list
      } catch (err) {
        console.error('Error during deletion:', err);
      }
    }
  };

  const handleCreate = () => {
    setEditingEvent(null);
    setShowForm(true);
  };

  const handleSave = async (eventData: Partial<EventType>) => {
    try {
      if (editingEvent) {

        console.log('Updating event:', eventData);
      } else {

        console.log('Creating event:', eventData);
      }
      setShowForm(false);
      setEditingEvent(null);
      await fetchEvents();
    } catch (err) {
      console.error('Error during save:', err);
    }
  };

  if (loading) {
    return (
      <div className="container" style={{ paddingTop: '2rem', textAlign: 'center' }}>
        <h1>Loading events...</h1>
        <p>Retrieving data from backend...</p>
      </div>
    );
  }

  if (error) {
    return (
      <div className="container" style={{ paddingTop: '2rem', textAlign: 'center' }}>
        <h1>Error</h1>
        <p style={{ color: 'red' }}>{error}</p>
        <button onClick={fetchEvents} style={{
          padding: '0.75rem 1.5rem',
          backgroundColor: 'var(--coral-medium)',
          color: 'white',
          border: 'none',
          borderRadius: '4px',
          cursor: 'pointer'
        }}>
          Retry
        </button>
      </div>
    );
  }

  return (
    <div className="container" style={{ paddingTop: '2rem' }}>
      <h1>Event Management</h1>
      <p>Event CRUD - Real data from backend ({events.length} events)</p>

      <div style={{ marginTop: '2rem' }}>
        <button
          onClick={handleCreate}
          style={{
            padding: '0.75rem 1.5rem',
            backgroundColor: 'var(--coral-medium)',
            color: 'white',
            border: 'none',
            borderRadius: '4px',
            marginBottom: '1rem',
            cursor: 'pointer'
          }}
        >
          + Add Event
        </button>

        <div className="card">
          <h3>Event List ({events.length})</h3>
          <div style={{ overflowX: 'auto' }}>
            <table style={{ width: '100%', borderCollapse: 'collapse' }}>
              <thead>
                <tr style={{ backgroundColor: '#f5f5f5' }}>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>ID</th>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Name</th>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Type</th>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Date</th>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Location</th>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Actions</th>
                </tr>
              </thead>
              <tbody>
                {events.map(event => (
                  <tr key={event.id}>
                    <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>{event.id}</td>
                    <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>{event.name}</td>
                    <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>{event.event_type}</td>
                    <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>
                      {new Date(event.dates).toLocaleDateString('fr-FR')}
                    </td>
                    <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>{event.location}</td>
                    <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>
                      <button
                        onClick={() => handleEdit(event)}
                        style={{
                          marginRight: '0.5rem',
                          padding: '0.25rem 0.5rem',
                          backgroundColor: 'var(--coral-medium)',
                          color: 'white',
                          border: 'none',
                          borderRadius: '3px',
                          cursor: 'pointer'
                        }}
                      >
                        Edit
                      </button>
                      <button
                        onClick={() => handleDelete(event.id)}
                        style={{
                          padding: '0.25rem 0.5rem',
                          backgroundColor: 'red',
                          color: 'white',
                          border: 'none',
                          borderRadius: '3px',
                          cursor: 'pointer'
                        }}
                      >
                        Delete
                      </button>
                    </td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        </div>

        {}
        {showForm && (
          <div style={{
            position: 'fixed',
            top: 0,
            left: 0,
            right: 0,
            bottom: 0,
            backgroundColor: 'rgba(0,0,0,0.5)',
            display: 'flex',
            justifyContent: 'center',
            alignItems: 'center',
            zIndex: 1000
          }}>
            <div className="card" style={{ width: '500px', maxHeight: '80vh', overflowY: 'auto' }}>
              <h3>{editingEvent ? 'Edit Event' : 'Create New Event'}</h3>

              <form onSubmit={(e) => {
                e.preventDefault();
                const formData = new FormData(e.currentTarget);
                const data = Object.fromEntries(formData.entries());
                handleSave(data);
              }}>
                <div style={{ marginBottom: '1rem' }}>
                  <label style={{ display: 'block', marginBottom: '0.5rem' }}>
                    Event Name
                  </label>
                  <input
                    name="name"
                    defaultValue={editingEvent?.name || ''}
                    required
                    style={{
                      width: '100%',
                      padding: '0.75rem',
                      border: '1px solid #ddd',
                      borderRadius: '4px'
                    }}
                  />
                </div>

                <div style={{ marginBottom: '1rem' }}>
                  <label style={{ display: 'block', marginBottom: '0.5rem' }}>
                    Event Type
                  </label>
                  <select
                    name="event_type"
                    defaultValue={editingEvent?.event_type || ''}
                    required
                    style={{
                      width: '100%',
                      padding: '0.75rem',
                      border: '1px solid #ddd',
                      borderRadius: '4px'
                    }}
                  >
                    <option value="">Select a type</option>
                    <option value="Workshop">Workshop</option>
                    <option value="Conference">Conference</option>
                    <option value="Pitch Session">Pitch Session</option>
                    <option value="Networking">Networking</option>
                    <option value="Training">Training</option>
                  </select>
                </div>

                <div style={{ marginBottom: '1rem' }}>
                  <label style={{ display: 'block', marginBottom: '0.5rem' }}>
                    Date
                  </label>
                  <input
                    name="dates"
                    type="date"
                    defaultValue={editingEvent?.dates ? editingEvent.dates.split('T')[0] : ''}
                    required
                    style={{
                      width: '100%',
                      padding: '0.75rem',
                      border: '1px solid #ddd',
                      borderRadius: '4px'
                    }}
                  />
                </div>

                <div style={{ marginBottom: '1rem' }}>
                  <label style={{ display: 'block', marginBottom: '0.5rem' }}>
                    Location
                  </label>
                  <input
                    name="location"
                    defaultValue={editingEvent?.location || ''}
                    required
                    style={{
                      width: '100%',
                      padding: '0.75rem',
                      border: '1px solid #ddd',
                      borderRadius: '4px'
                    }}
                  />
                </div>

                <div style={{ marginBottom: '1rem' }}>
                  <label style={{ display: 'block', marginBottom: '0.5rem' }}>
                    Target Audience
                  </label>
                  <input
                    name="target_audience"
                    defaultValue={editingEvent?.target_audience || ''}
                    style={{
                      width: '100%',
                      padding: '0.75rem',
                      border: '1px solid #ddd',
                      borderRadius: '4px'
                    }}
                  />
                </div>

                <div style={{ marginBottom: '1rem' }}>
                  <label style={{ display: 'block', marginBottom: '0.5rem' }}>
                    Participants maximum
                  </label>
                  <input
                    name="max_participants"
                    type="number"
                    defaultValue={editingEvent?.max_participants || ''}
                    style={{
                      width: '100%',
                      padding: '0.75rem',
                      border: '1px solid #ddd',
                      borderRadius: '4px'
                    }}
                  />
                </div>

                <div style={{ marginBottom: '1rem' }}>
                  <label style={{ display: 'block', marginBottom: '0.5rem' }}>
                    Description
                  </label>
                  <textarea
                    name="description"
                    rows={3}
                    defaultValue={editingEvent?.description || ''}
                    style={{
                      width: '100%',
                      padding: '0.75rem',
                      border: '1px solid #ddd',
                      borderRadius: '4px'
                    }}
                  />
                </div>

                <div style={{ display: 'flex', gap: '1rem', justifyContent: 'flex-end' }}>
                  <button
                    type="button"
                    onClick={() => setShowForm(false)}
                    style={{
                      padding: '0.75rem 1.5rem',
                      backgroundColor: '#ccc',
                      color: 'black',
                      border: 'none',
                      borderRadius: '4px',
                      cursor: 'pointer'
                    }}
                  >
                    Cancel
                  </button>
                  <button
                    type="submit"
                    style={{
                      padding: '0.75rem 1.5rem',
                      backgroundColor: 'var(--coral-medium)',
                      color: 'white',
                      border: 'none',
                      borderRadius: '4px',
                      cursor: 'pointer'
                    }}
                  >
                    {editingEvent ? 'Update' : 'Create'}
                  </button>
                </div>
              </form>
            </div>
          </div>
        )}
      </div>
    </div>
  );
};

export default AdminEvents;
