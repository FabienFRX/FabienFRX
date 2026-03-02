import React from 'react';

const StartupOpportunities: React.FC = () => {
  return (
    <div className="container" style={{ paddingTop: '2rem' }}>
      <h1>Opportunités</h1>
      <p>Suivez vos opportunités de financement et de partenariat</p>

      <div style={{
        display: 'grid',
        gridTemplateColumns: 'repeat(auto-fit, minmax(350px, 1fr))',
        gap: '2rem',
        marginTop: '2rem'
      }}>
        {}
        <div className="card">
          <h3>Pitch Session - Janvier 2024</h3>
          <p style={{ color: '#666', marginBottom: '1rem' }}>
            <strong>Type:</strong> Événement de pitch<br/>
            <strong>Date:</strong> 15 janvier 2024<br/>
            <strong>Statut:</strong> <span style={{
              color: 'green',
              fontWeight: 'bold'
            }}>Ouvert</span>
          </p>
          <p style={{ fontSize: '0.9rem', marginBottom: '1rem' }}>
            Présentez votre startup devant un panel d'investors.
            Durée: 5 minutes de présentation + 3 minutes de questions.
          </p>
          <button style={{
            padding: '0.5rem 1rem',
            backgroundColor: 'var(--coral-medium)',
            color: 'white',
            border: 'none',
            borderRadius: '4px',
            cursor: 'pointer',
            marginRight: '0.5rem'
          }}>
            S'inscrire
          </button>
          <button style={{
            padding: '0.5rem 1rem',
            backgroundColor: 'transparent',
            color: 'var(--coral-medium)',
            border: '1px solid var(--coral-medium)',
            borderRadius: '4px',
            cursor: 'pointer'
          }}>
            Détails
          </button>
        </div>

        {}
        <div className="card">
          <h3>Rencontre avec Investisseur ABC</h3>
          <p style={{ color: '#666', marginBottom: '1rem' }}>
            <strong>Type:</strong> Rencontre individuelle<br/>
            <strong>Date:</strong> 20 janvier 2024<br/>
            <strong>Statut:</strong> <span style={{
              color: 'orange',
              fontWeight: 'bold'
            }}>Programmée</span>
          </p>
          <p style={{ fontSize: '0.9rem', marginBottom: '1rem' }}>
            Rencontre privée avec un investisseur intéressé par le secteur CleanTech.
            Préparez votre pitch deck et vos projections financières.
          </p>
          <button style={{
            padding: '0.5rem 1rem',
            backgroundColor: 'var(--purple-medium)',
            color: 'white',
            border: 'none',
            borderRadius: '4px',
            cursor: 'pointer',
            marginRight: '0.5rem'
          }}>
            Préparer
          </button>
          <button style={{
            padding: '0.5rem 1rem',
            backgroundColor: 'transparent',
            color: 'var(--purple-medium)',
            border: '1px solid var(--purple-medium)',
            borderRadius: '4px',
            cursor: 'pointer'
          }}>
            Annuler
          </button>
        </div>

        {}
        <div className="card">
          <h3>Formation Marketing Digital</h3>
          <p style={{ color: '#666', marginBottom: '1rem' }}>
            <strong>Type:</strong> Formation<br/>
            <strong>Date:</strong> 25 janvier 2024<br/>
            <strong>Statut:</strong> <span style={{
              color: 'blue',
              fontWeight: 'bold'
            }}>Disponible</span>
          </p>
          <p style={{ fontSize: '0.9rem', marginBottom: '1rem' }}>
            Formation de 2 jours sur les stratégies de marketing digital pour startups.
            Inclut: SEO, réseaux sociaux, content marketing.
          </p>
          <button style={{
            padding: '0.5rem 1rem',
            backgroundColor: 'var(--coral-light)',
            color: 'white',
            border: 'none',
            borderRadius: '4px',
            cursor: 'pointer',
            marginRight: '0.5rem'
          }}>
            S'inscrire
          </button>
          <button style={{
            padding: '0.5rem 1rem',
            backgroundColor: 'transparent',
            color: 'var(--coral-light)',
            border: '1px solid var(--coral-light)',
            borderRadius: '4px',
            cursor: 'pointer'
          }}>
            Programme
          </button>
        </div>
      </div>
    </div>
  );
};

export default StartupOpportunities;
