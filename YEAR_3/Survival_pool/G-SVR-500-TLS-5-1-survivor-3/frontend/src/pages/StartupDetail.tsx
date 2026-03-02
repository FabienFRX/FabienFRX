import React, { useState, useEffect } from 'react';
import { useParams, useNavigate } from 'react-router-dom';
import { startupService, Startup as StartupType } from '../services/api';
import LoadingSpinner from '../components/LoadingSpinner';
import MarkdownContent from '../components/MarkdownContent';

const StartupDetail: React.FC = () => {
  const { id } = useParams<{ id: string }>();
  const navigate = useNavigate();
  const [startup, setStartup] = useState<StartupType | null>(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    if (id) {
      fetchStartup(parseInt(id));
    }
  }, [id]);

  const fetchStartup = async (startupId: number) => {
    try {
      setLoading(true);
      const data = await startupService.getById(startupId);
      setStartup(data);
    } catch (err) {
      setError('Error loading de la startup');
      console.error('Error fetching startup:', err);
    } finally {
      setLoading(false);
    }
  };

  if (loading) {
    return <LoadingSpinner size="large" text="Chargement de la startup..." />;
  }

  if (error || !startup) {
    return (
      <div className="container" style={{ paddingTop: '2rem', textAlign: 'center' }}>
        <h1>Error</h1>
        <p style={{ color: 'red' }}>{error || 'Startup non trouvée'}</p>
        <button
          onClick={() => navigate('/projects')}
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
          Retour au catalogue
        </button>
      </div>
    );
  }

  return (
    <div className="container" style={{ paddingTop: '2rem' }}>
      <button
        onClick={() => navigate('/projects')}
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
        ← Retour au catalogue
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
              {startup.name}
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
                {startup.sector}
              </span>
              <span style={{
                background: 'var(--purple-medium)',
                color: 'white',
                padding: '0.25rem 0.75rem',
                borderRadius: '20px',
                fontSize: '0.9rem'
              }}>
                {startup.maturity}
              </span>
              <span style={{
                background: startup.project_status === 'Active' ? '#28a745' : '#6c757d',
                color: 'white',
                padding: '0.25rem 0.75rem',
                borderRadius: '20px',
                fontSize: '0.9rem'
              }}>
                {startup.project_status}
              </span>
            </div>

            {startup.description && (
              <div style={{ marginBottom: '2rem' }}>
                <h3 style={{ color: 'var(--coral-medium)', marginBottom: '1rem' }}>
                  Description
                </h3>
                <MarkdownContent content={startup.description} />
              </div>
            )}

            {startup.needs && (
              <div style={{ marginBottom: '2rem' }}>
                <h3 style={{ color: 'var(--coral-medium)', marginBottom: '1rem' }}>
                  Besoins
                </h3>
                <MarkdownContent content={startup.needs} />
              </div>
            )}

            {startup.progress && (
              <div style={{ marginBottom: '2rem' }}>
                <h3 style={{ color: 'var(--coral-medium)', marginBottom: '1rem' }}>
                  Progression
                </h3>
                <MarkdownContent content={startup.progress} />
              </div>
            )}
          </div>

          <div>
            <div className="card" style={{ padding: '1.5rem' }}>
              <h3 style={{ marginBottom: '1rem' }}>Informations</h3>

              <div style={{ marginBottom: '1rem' }}>
                <strong>Statut légal:</strong><br />
                <span style={{ color: '#666' }}>{startup.legal_status}</span>
              </div>

              {startup.address && (
                <div style={{ marginBottom: '1rem' }}>
                  <strong>Adresse:</strong><br />
                  <span style={{ color: '#666' }}>{startup.address}</span>
                </div>
              )}

              <div style={{ marginBottom: '1rem' }}>
                <strong>Email:</strong><br />
                <a href={`mailto:${startup.email}`} style={{ color: 'var(--coral-medium)' }}>
                  {startup.email}
                </a>
              </div>

              {startup.phone && (
                <div style={{ marginBottom: '1rem' }}>
                  <strong>Téléphone:</strong><br />
                  <a href={`tel:${startup.phone}`} style={{ color: 'var(--coral-medium)' }}>
                    {startup.phone}
                  </a>
                </div>
              )}

              <div style={{ marginBottom: '1rem' }}>
                <strong>Créé le:</strong><br />
                <span style={{ color: '#666' }}>
                  {new Date(startup.created_at).toLocaleDateString('fr-FR')}
                </span>
              </div>

              <div style={{ marginBottom: '1.5rem' }}>
                <strong>Dernière mise à jour:</strong><br />
                <span style={{ color: '#666' }}>
                  {new Date(startup.updated_at).toLocaleDateString('fr-FR')}
                </span>
              </div>

              <div style={{ display: 'flex', gap: '0.5rem', flexDirection: 'column' }}>
                {startup.website_url && (
                  <a
                    href={startup.website_url}
                    target="_blank"
                    rel="noopener noreferrer"
                    style={{
                      background: 'var(--coral-medium)',
                      color: 'white',
                      padding: '0.75rem',
                      borderRadius: '4px',
                      textDecoration: 'none',
                      textAlign: 'center',
                      fontSize: '0.9rem'
                    }}
                  >
                    🌐 Site Web
                  </a>
                )}
                {startup.social_media_url && (
                  <a
                    href={startup.social_media_url}
                    target="_blank"
                    rel="noopener noreferrer"
                    style={{
                      background: 'var(--purple-medium)',
                      color: 'white',
                      padding: '0.75rem',
                      borderRadius: '4px',
                      textDecoration: 'none',
                      textAlign: 'center',
                      fontSize: '0.9rem'
                    }}
                  >
                    📱 Réseaux Sociaux
                  </a>
                )}
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};

export default StartupDetail;
