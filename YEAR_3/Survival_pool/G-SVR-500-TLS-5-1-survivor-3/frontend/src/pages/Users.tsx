import React from 'react';
import { jebUserService, JEBUser, getImageUrl } from '../services/api';
import { useState, useEffect } from 'react';
import ImageWithFallback from '../components/ImageWithFallback';

const Users: React.FC = () => {
  const [users, setUsers] = useState<JEBUser[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const fetchUsers = async () => {
      try {
        setLoading(true);
        const data = await jebUserService.getAll();
        setUsers(data);
      } catch (err) {
        setError('Error loading des utilisateurs');
        console.error('Error fetching users:', err);
      } finally {
        setLoading(false);
      }
    };

    fetchUsers();
  }, []);

  if (loading) {
    return (
      <div className="container" style={{ paddingTop: '2rem', textAlign: 'center' }}>
        <h1>Loading utilisateurs...</h1>
        <p>Veuillez patienter...</p>
      </div>
    );
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
      <h1 style={{ textAlign: 'center', marginBottom: '3rem' }}>Utilisateurs JEB</h1>

      {users.length === 0 ? (
        <div style={{ textAlign: 'center', padding: '2rem' }}>
          <p>Aucun utilisateur trouvé.</p>
          <p>Assurez-vous que les données JEB sont synchronisées.</p>
        </div>
      ) : (
        <div style={{
          display: 'grid',
          gridTemplateColumns: 'repeat(auto-fit, minmax(300px, 1fr))',
          gap: '2rem'
        }}>
          {users.map(user => (
            <div key={user.id} className="card" style={{ textAlign: 'center' }}>
              <div style={{ marginBottom: '1rem' }}>
                <ImageWithFallback
                  src={getImageUrl.user(user.jeb_id)}
                  alt={`Photo de ${user.name}`}
                  style={{
                    width: '80px',
                    height: '80px',
                    borderRadius: '50%',
                    objectFit: 'cover',
                    margin: '0 auto'
                  }}
                />
              </div>

              <h3>{user.name}</h3>
              <p><strong>Rôle:</strong> {user.role}</p>
              <p><strong>Email:</strong> {user.email}</p>
              {user.company && <p><strong>Entreprise:</strong> {user.company}</p>}
              {user.position && <p><strong>Poste:</strong> {user.position}</p>}

              {user.bio && (
                <div style={{ marginTop: '1rem' }}>
                  <p><strong>Bio:</strong></p>
                  <p style={{ fontSize: '0.9rem' }}>{user.bio}</p>
                </div>
              )}

              <div style={{ marginTop: '1rem', display: 'flex', gap: '0.5rem', justifyContent: 'center', flexWrap: 'wrap' }}>
                {user.linkedin_url && (
                  <a
                    href={user.linkedin_url}
                    target="_blank"
                    rel="noopener noreferrer"
                    style={{
                      background: '#0077b5',
                      color: 'white',
                      padding: '0.5rem 1rem',
                      borderRadius: '25px',
                      textDecoration: 'none',
                      fontSize: '0.9rem'
                    }}
                  >
                    LinkedIn
                  </a>
                )}
                {user.twitter_url && (
                  <a
                    href={user.twitter_url}
                    target="_blank"
                    rel="noopener noreferrer"
                    style={{
                      background: '#1da1f2',
                      color: 'white',
                      padding: '0.5rem 1rem',
                      borderRadius: '25px',
                      textDecoration: 'none',
                      fontSize: '0.9rem'
                    }}
                  >
                    Twitter
                  </a>
                )}
              </div>
            </div>
          ))}
        </div>
      )}
    </div>
  );
};

export default Users;
