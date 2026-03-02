import React, { useState, useEffect } from 'react';
import { startupService, Startup as StartupType } from '../../services/api';

const AdminProjects: React.FC = () => {
  const [startups, setStartups] = useState<StartupType[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [editingStartup, setEditingStartup] = useState<StartupType | null>(null);
  const [showForm, setShowForm] = useState(false);

  useEffect(() => {
    fetchStartups();
  }, []);

  const fetchStartups = async () => {
    try {
      setLoading(true);
      const data = await startupService.getAll();
      setStartups(data);
    } catch (err) {
      setError('Error loading startups');
      console.error('Error fetching startups:', err);
    } finally {
      setLoading(false);
    }
  };

  const handleEdit = (startup: StartupType) => {
    setEditingStartup(startup);
    setShowForm(true);
  };

  const handleDelete = async (id: number) => {
    if (window.confirm('Are you sure you want to delete this startup?')) {
      try {

        console.log('Deleting startup:', id);
        await fetchStartups(); // Reload the list
      } catch (err) {
        console.error('Error during deletion:', err);
      }
    }
  };

  const handleCreate = () => {
    setEditingStartup(null);
    setShowForm(true);
  };

  const handleSave = async (startupData: Partial<StartupType>) => {
    try {
      if (editingStartup) {

        console.log('Updating startup:', startupData);
      } else {

        console.log('Creating startup:', startupData);
      }
      setShowForm(false);
      setEditingStartup(null);
      await fetchStartups();
    } catch (err) {
      console.error('Error during save:', err);
    }
  };

  if (loading) {
    return (
      <div className="container" style={{ paddingTop: '2rem', textAlign: 'center' }}>
        <h1>Loading projects...</h1>
        <p>Retrieving data from backend...</p>
      </div>
    );
  }

  if (error) {
    return (
      <div className="container" style={{ paddingTop: '2rem', textAlign: 'center' }}>
        <h1>Error</h1>
        <p style={{ color: 'red' }}>{error}</p>
        <button onClick={fetchStartups} style={{
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
      <h1>Project Management</h1>
      <p>Startup CRUD - Real data from backend ({startups.length} startups)</p>

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
          + Add Startup
        </button>

        <div className="card">
          <h3>Startup List ({startups.length})</h3>
          <div style={{ overflowX: 'auto' }}>
            <table style={{ width: '100%', borderCollapse: 'collapse' }}>
              <thead>
                <tr style={{ backgroundColor: '#f5f5f5' }}>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>ID</th>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Name</th>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Sector</th>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Maturity</th>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Status</th>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Email</th>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Actions</th>
                </tr>
              </thead>
              <tbody>
                {startups.map(startup => (
                  <tr key={startup.id}>
                    <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>{startup.id}</td>
                    <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>{startup.name}</td>
                    <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>{startup.sector}</td>
                    <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>{startup.maturity}</td>
                    <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>{startup.project_status}</td>
                    <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>{startup.email}</td>
                    <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>
                      <button
                        onClick={() => handleEdit(startup)}
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
                        onClick={() => handleDelete(startup.id)}
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
              <h3>{editingStartup ? 'Edit Startup' : 'Create New Startup'}</h3>

              <form onSubmit={(e) => {
                e.preventDefault();
                const formData = new FormData(e.currentTarget);
                const data = Object.fromEntries(formData.entries());
                handleSave(data);
              }}>
                <div style={{ marginBottom: '1rem' }}>
                  <label style={{ display: 'block', marginBottom: '0.5rem' }}>
                    Startup Name
                  </label>
                  <input
                    name="name"
                    defaultValue={editingStartup?.name || ''}
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
                    Sector
                  </label>
                  <select
                    name="sector"
                    defaultValue={editingStartup?.sector || ''}
                    required
                    style={{
                      width: '100%',
                      padding: '0.75rem',
                      border: '1px solid #ddd',
                      borderRadius: '4px'
                    }}
                  >
                    <option value="">Select a sector</option>
                    <option value="CleanTech">CleanTech</option>
                    <option value="HealthTech">HealthTech</option>
                    <option value="FinTech">FinTech</option>
                    <option value="EdTech">EdTech</option>
                    <option value="AgriTech">AgriTech</option>
                  </select>
                </div>

                <div style={{ marginBottom: '1rem' }}>
                  <label style={{ display: 'block', marginBottom: '0.5rem' }}>
                    Maturity
                  </label>
                  <select
                    name="maturity"
                    defaultValue={editingStartup?.maturity || ''}
                    required
                    style={{
                      width: '100%',
                      padding: '0.75rem',
                      border: '1px solid #ddd',
                      borderRadius: '4px'
                    }}
                  >
                    <option value="">Select maturity level</option>
                    <option value="Idea">Idea</option>
                    <option value="MVP">MVP</option>
                    <option value="Growth">Growth</option>
                    <option value="Scale">Scale</option>
                  </select>
                </div>

                <div style={{ marginBottom: '1rem' }}>
                  <label style={{ display: 'block', marginBottom: '0.5rem' }}>
                    Email
                  </label>
                  <input
                    name="email"
                    type="email"
                    defaultValue={editingStartup?.email || ''}
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
                    Description
                  </label>
                  <textarea
                    name="description"
                    rows={3}
                    defaultValue={editingStartup?.description || ''}
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
                    {editingStartup ? 'Update' : 'Create'}
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

export default AdminProjects;
