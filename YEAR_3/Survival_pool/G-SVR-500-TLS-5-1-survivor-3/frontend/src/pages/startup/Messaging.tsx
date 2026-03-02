import React from 'react';

const StartupMessaging: React.FC = () => {
  return (
    <div className="container" style={{ paddingTop: '2rem' }}>
      <h1>Messagerie Interne</h1>
      <p>Communiquez avec les investors et l'équipe JEB</p>

      <div style={{
        display: 'grid',
        gridTemplateColumns: '1fr 2fr',
        gap: '2rem',
        marginTop: '2rem',
        height: '60vh'
      }}>
        {}
        <div className="card">
          <h3>Conversations</h3>
          <div style={{
            border: '1px solid #ddd',
            borderRadius: '4px',
            padding: '1rem',
            marginTop: '1rem'
          }}>
            <div style={{
              padding: '0.5rem',
              borderBottom: '1px solid #eee',
              cursor: 'pointer'
            }}>
              <strong>Investisseur ABC</strong>
              <p style={{ margin: '0.25rem 0 0 0', fontSize: '0.9rem', color: '#666' }}>
                Dernier message: "Intéressé par votre pitch..."
              </p>
            </div>
            <div style={{
              padding: '0.5rem',
              borderBottom: '1px solid #eee',
              cursor: 'pointer'
            }}>
              <strong>Équipe JEB</strong>
              <p style={{ margin: '0.25rem 0 0 0', fontSize: '0.9rem', color: '#666' }}>
                Dernier message: "Rappel: Pitch session demain"
              </p>
            </div>
          </div>
        </div>

        {}
        <div className="card" style={{ display: 'flex', flexDirection: 'column' }}>
          <h3>Conversation avec Investisseur ABC</h3>

          {}
          <div style={{
            flex: 1,
            border: '1px solid #ddd',
            borderRadius: '4px',
            padding: '1rem',
            marginTop: '1rem',
            overflowY: 'auto'
          }}>
            <div style={{ marginBottom: '1rem' }}>
              <div style={{
                background: '#f0f0f0',
                padding: '0.5rem',
                borderRadius: '8px',
                marginBottom: '0.5rem'
              }}>
                <strong>Investisseur ABC:</strong> Bonjour, je suis intéressé par votre projet EcoTech.
              </div>
              <div style={{
                background: 'var(--coral-light)',
                color: 'white',
                padding: '0.5rem',
                borderRadius: '8px',
                marginBottom: '0.5rem'
              }}>
                <strong>Vous:</strong> Merci ! Voulez-vous que je vous envoie notre pitch deck ?
              </div>
            </div>
          </div>

          {}
          <div style={{ marginTop: '1rem' }}>
            <div style={{ display: 'flex', gap: '0.5rem' }}>
              <input
                type="text"
                placeholder="Tapez votre message..."
                style={{
                  flex: 1,
                  padding: '0.75rem',
                  border: '1px solid #ddd',
                  borderRadius: '4px'
                }}
              />
              <button style={{
                padding: '0.75rem 1.5rem',
                backgroundColor: 'var(--coral-medium)',
                color: 'white',
                border: 'none',
                borderRadius: '4px',
                cursor: 'pointer'
              }}>
                Envoyer
              </button>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};

export default StartupMessaging;
