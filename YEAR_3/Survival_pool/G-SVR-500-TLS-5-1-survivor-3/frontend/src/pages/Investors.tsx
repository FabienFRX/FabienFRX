import React from 'react';
import { investorService, Investor as InvestorType } from '../services/api';
import { useState, useEffect } from 'react';

const Investors: React.FC = () => {
  const [investors, setInvestors] = useState<InvestorType[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const fetchInvestors = async () => {
      try {
        setLoading(true);
        const data = await investorService.getAll();
        setInvestors(data);
      } catch (err) {
        setError('Error loading des investors');
        console.error('Error fetching investors:', err);
      } finally {
        setLoading(false);
      }
    };

    fetchInvestors();
  }, []);

  if (loading) {
    return (
      <div className="container" style={{ paddingTop: '2rem', textAlign: 'center' }}>
        <h1>Loading investors...</h1>
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
      <h1 style={{ textAlign: 'center', marginBottom: '3rem' }}>Investisseurs & Fonds</h1>

      {investors.length === 0 ? (
        <div style={{ textAlign: 'center', padding: '2rem' }}>
          <p>Aucun investisseur trouvé.</p>
          <p>Assurez-vous que les données JEB sont synchronisées.</p>
        </div>
      ) : (
        <div style={{
          display: 'grid',
          gridTemplateColumns: 'repeat(auto-fit, minmax(350px, 1fr))',
          gap: '2rem'
        }}>
          {investors.map(investor => (
            <div key={investor.id} className="card">
              <h3>{investor.name}</h3>
              <p><strong>Type:</strong> {investor.investor_type}</p>
              <p><strong>Focus:</strong> {investor.investment_focus}</p>
              <p><strong>Statut légal:</strong> {investor.legal_status}</p>
              {investor.address && <p><strong>Adresse:</strong> {investor.address}</p>}
              <p><strong>Email:</strong> {investor.email}</p>
              {investor.phone && <p><strong>Téléphone:</strong> {investor.phone}</p>}

              {investor.description && (
                <div style={{ marginTop: '1rem' }}>
                  <p><strong>Description:</strong></p>
                  <p>{investor.description}</p>
                </div>
              )}
            </div>
          ))}
        </div>
      )}
    </div>
  );
};

export default Investors;
