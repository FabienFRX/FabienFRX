import React from 'react';
import { partnerService, Partner as PartnerType } from '../services/api';
import { useState, useEffect } from 'react';

const Partners: React.FC = () => {
  const [partners, setPartners] = useState<PartnerType[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const fetchPartners = async () => {
      try {
        setLoading(true);
        const data = await partnerService.getAll();
        setPartners(data);
      } catch (err) {
        setError('Error loading des partners');
        console.error('Error fetching partners:', err);
      } finally {
        setLoading(false);
      }
    };

    fetchPartners();
  }, []);

  if (loading) {
    return (
      <div className="container" style={{ paddingTop: '2rem', textAlign: 'center' }}>
        <h1>Loading partners...</h1>
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
      <h1 style={{ textAlign: 'center', marginBottom: '3rem' }}>Partenaires & Institutions</h1>

      {partners.length === 0 ? (
        <div style={{ textAlign: 'center', padding: '2rem' }}>
          <p>Aucun partenaire trouvé.</p>
          <p>Assurez-vous que les données JEB sont synchronisées.</p>
        </div>
      ) : (
        <div style={{
          display: 'grid',
          gridTemplateColumns: 'repeat(auto-fit, minmax(350px, 1fr))',
          gap: '2rem'
        }}>
          {partners.map(partner => (
            <div key={partner.id} className="card">
              <h3>{partner.name}</h3>
              <p><strong>Type:</strong> {partner.partner_type || 'N/A'}</p>
              <p><strong>Focus:</strong> {partner.focus_area || 'N/A'}</p>
              <p><strong>Statut légal:</strong> {partner.legal_status}</p>
              {partner.address && <p><strong>Adresse:</strong> {partner.address}</p>}
              <p><strong>Email:</strong> {partner.email}</p>
              {partner.phone && <p><strong>Téléphone:</strong> {partner.phone}</p>}

              {partner.description && (
                <div style={{ marginTop: '1rem' }}>
                  <p><strong>Description:</strong></p>
                  <p>{partner.description}</p>
                </div>
              )}
            </div>
          ))}
        </div>
      )}
    </div>
  );
};

export default Partners;
