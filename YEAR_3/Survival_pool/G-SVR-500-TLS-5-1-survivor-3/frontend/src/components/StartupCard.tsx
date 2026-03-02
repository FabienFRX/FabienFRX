import React from 'react';
import { Startup as StartupType } from '../services/api';
import MarkdownContent from './MarkdownContent';
import PDFExportButton from './PDFExportButton';

interface StartupCardProps {
  startup: StartupType;
  onClick: () => void;
  showFullContent?: boolean;
  onPitchDeckOpen?: () => void;
  canAccessPitchDeck?: boolean;
  canEditPitchDeck?: boolean;
}

const StartupCard: React.FC<StartupCardProps> = ({
  startup,
  onClick,
  showFullContent = false,
  onPitchDeckOpen,
  canAccessPitchDeck = false,
  canEditPitchDeck = false
}) => {
  return (
    <div
      className="startup-card"
      onClick={onClick}
      style={{
        cursor: 'pointer',
        transition: 'all 0.3s ease',
        height: '100%',
        display: 'flex',
        flexDirection: 'column'
      }}
      onMouseEnter={(e) => {
        e.currentTarget.style.transform = 'translateY(-5px)';
        e.currentTarget.style.boxShadow = '0 15px 35px rgba(0,0,0,0.15)';
      }}
      onMouseLeave={(e) => {
        e.currentTarget.style.transform = 'translateY(0)';
        e.currentTarget.style.boxShadow = '0 5px 20px rgba(0,0,0,0.1)';
      }}
    >
      <div className="card" style={{ height: '100%', display: 'flex', flexDirection: 'column' }}>
        <h3 style={{ color: 'var(--purple-dark)', marginBottom: '1rem' }}>
          {startup.name}
        </h3>

        <div style={{
          display: 'flex',
          gap: '0.5rem',
          marginBottom: '1rem',
          flexWrap: 'wrap'
        }}>
          <span style={{
            background: 'var(--coral-medium)',
            color: 'white',
            padding: '0.25rem 0.75rem',
            borderRadius: '20px',
            fontSize: '0.8rem'
          }}>
            {startup.sector}
          </span>
          <span style={{
            background: 'var(--purple-medium)',
            color: 'white',
            padding: '0.25rem 0.75rem',
            borderRadius: '20px',
            fontSize: '0.8rem'
          }}>
            {startup.maturity}
          </span>
          <span style={{
            background: startup.project_status === 'Active' ? '#4caf50' : '#F49C9C',
            color: 'white',
            padding: '0.25rem 0.75rem',
            borderRadius: '20px',
            fontSize: '0.8rem'
          }}>
            {startup.project_status}
          </span>
        </div>

        <div style={{ flex: 1 }}>
          <p><strong>Legal status:</strong> {startup.legal_status}</p>
          {startup.address && <p><strong>Adress:</strong> {startup.address}</p>}
          <p><strong>Email:</strong> {startup.email}</p>
          {startup.phone && <p><strong>Phone:</strong> {startup.phone}</p>}

          {/* Founders Section */}
          {startup.founders && startup.founders.length > 0 && (
            <div style={{ marginTop: '1rem' }}>
              <p><strong>Founders:</strong></p>
              <div style={{ 
                display: 'flex', 
                flexWrap: 'wrap', 
                gap: '0.5rem', 
                marginTop: '0.5rem' 
              }}>
                {startup.founders.map((founder) => (
                  <div 
                    key={founder.id} 
                    style={{ 
                      display: 'flex', 
                      alignItems: 'center', 
                      gap: '0.5rem',
                      background: '#f8f9fa',
                      padding: '0.5rem',
                      borderRadius: '8px',
                      border: '1px solid #e9ecef'
                    }}
                  >
                    {(founder.image || founder.image_url) && (
                      <div style={{
                        width: '30px',
                        height: '30px',
                        borderRadius: '50%',
                        overflow: 'hidden',
                        backgroundColor: '#e9ecef',
                        display: 'flex',
                        alignItems: 'center',
                        justifyContent: 'center'
                      }}>
                        <img
                          src={founder.image || founder.image_url}
                          alt={founder.name}
                          style={{
                            width: '100%',
                            height: '100%',
                            objectFit: 'cover'
                          }}
                          onError={(e) => {
                            // Fallback to initials if image fails to load
                            const target = e.target as HTMLImageElement;
                            target.style.display = 'none';
                            const parent = target.parentElement;
                            if (parent) {
                              parent.textContent = founder.name.charAt(0).toUpperCase();
                              parent.style.fontSize = '12px';
                              parent.style.fontWeight = 'bold';
                              parent.style.color = 'var(--purple-dark)';
                            }
                          }}
                        />
                      </div>
                    )}
                    {!(founder.image || founder.image_url) && (
                      <div style={{
                        width: '30px',
                        height: '30px',
                        borderRadius: '50%',
                        backgroundColor: 'var(--coral-light)',
                        display: 'flex',
                        alignItems: 'center',
                        justifyContent: 'center',
                        fontSize: '12px',
                        fontWeight: 'bold',
                        color: 'var(--purple-dark)'
                      }}>
                        {founder.name.charAt(0).toUpperCase()}
                      </div>
                    )}
                    <span style={{ 
                      fontSize: '0.85rem', 
                      color: 'var(--purple-dark)',
                      fontWeight: '500'
                    }}>
                      {founder.name}
                    </span>
                  </div>
                ))}
              </div>
            </div>
          )}

          {startup.description && (
            <div style={{ marginTop: '1rem' }}>
              <p><strong>Description:</strong></p>
              <MarkdownContent
                content={startup.description}
                maxLength={showFullContent ? undefined : 100}
              />
            </div>
          )}

          {startup.needs && showFullContent && (
            <div style={{ marginTop: '1rem' }}>
              <p><strong>Needs:</strong></p>
              <MarkdownContent
                content={startup.needs}
                maxLength={150}
              />
            </div>
          )}
        </div>

        <div style={{
          marginTop: '1.5rem',
          display: 'flex',
          gap: '0.5rem',
          flexWrap: 'wrap'
        }}>
          {startup.website_url && (
            <a
              href={startup.website_url}
              target="_blank"
              rel="noopener noreferrer"
              onClick={(e) => e.stopPropagation()}
              style={{
                background: 'var(--coral-medium)',
                color: 'white',
                padding: '0.5rem 1rem',
                borderRadius: '25px',
                textDecoration: 'none',
                fontSize: '0.9rem'
              }}
            >
              Website
            </a>
          )}
          {startup.social_media_url && (
            <a
              href={startup.social_media_url}
              target="_blank"
              rel="noopener noreferrer"
              onClick={(e) => e.stopPropagation()}
              style={{
                background: 'var(--purple-medium)',
                color: 'white',
                padding: '0.5rem 1rem',
                borderRadius: '25px',
                textDecoration: 'none',
                fontSize: '0.9rem'
              }}
            >
              Social Media
            </a>
          )}
        </div>

        {/* Action buttons */}
        <div style={{
          marginTop: 'auto',
          paddingTop: '1rem',
          display: 'flex',
          gap: '0.5rem',
          flexWrap: 'wrap',
          justifyContent: 'center'
        }}
        onClick={(e) => e.stopPropagation()}
        >
          <PDFExportButton 
            startup={startup} 
            type="single" 
            variant="outline"
            className="startup-card-pdf-btn"
          >
            📄 PDF
          </PDFExportButton>
          
          {onPitchDeckOpen && canAccessPitchDeck && (
            <button
              onClick={(e) => {
                e.stopPropagation();
                onPitchDeckOpen();
              }}
              style={{
                background: 'var(--purple-medium)',
                color: 'white',
                border: 'none',
                padding: '0.5rem 1rem',
                borderRadius: '4px',
                fontSize: '0.85rem',
                cursor: 'pointer',
                fontWeight: '600',
                transition: 'all 0.3s ease'
              }}
              onMouseEnter={(e) => {
                e.currentTarget.style.background = 'var(--purple-dark)';
              }}
              onMouseLeave={(e) => {
                e.currentTarget.style.background = 'var(--purple-medium)';
              }}
            >
              {canEditPitchDeck ? 'Pitch Builder' : 'Pitch Deck'}
            </button>
          )}
        </div>

        <div style={{
          marginTop: '1rem',
          color: 'var(--coral-medium)',
          fontSize: '0.9rem',
          fontWeight: '500',
          textAlign: 'center'
        }}>
          {showFullContent ? 'Show complete details →' : 'Show detail →'}
        </div>
      </div>
    </div>
  );
};

export default StartupCard;
