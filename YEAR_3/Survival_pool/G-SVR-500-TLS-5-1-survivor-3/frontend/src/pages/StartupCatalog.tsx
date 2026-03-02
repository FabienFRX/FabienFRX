import React, { useState, useEffect } from 'react';
import { useLocation } from 'react-router-dom';
import { startupService, Startup as StartupType } from '../services/api';
import { useAuth } from '../contexts/AuthContext';
import LoadingSpinner from '../components/LoadingSpinner';
import FadeInOnScroll from '../components/FadeInOnScroll';
import StartupCard from '../components/StartupCard';
import Modal from '../components/Modal';
import MarkdownContent from '../components/MarkdownContent';
import PitchDeck from '../components/PitchDeck';

const StartupCatalog: React.FC = () => {
  const { user, isAuthenticated } = useAuth();
  const [startups, setStartups] = useState<StartupType[]>([]);
  const [filteredStartups, setFilteredStartups] = useState<StartupType[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [selectedStartup, setSelectedStartup] = useState<StartupType | null>(null);
  const [isModalOpen, setIsModalOpen] = useState(false);
  const [isPitchDeckOpen, setIsPitchDeckOpen] = useState(false);
  
  // Filter states
  const [selectedSector, setSelectedSector] = useState<string>('');
  const [selectedMaturity, setSelectedMaturity] = useState<string>('');
  const [selectedStatus, setSelectedStatus] = useState<string>('');
  const [searchTerm, setSearchTerm] = useState<string>('');

  // Get search from URL on mount
  const location = useLocation();
  useEffect(() => {
    const params = new URLSearchParams(location.search);
    const searchFromUrl = params.get('search');
    if (searchFromUrl) {
      setSearchTerm(searchFromUrl);
    }
  }, [location.search]);

  useEffect(() => {
    const fetchStartups = async () => {
      try {
        setLoading(true);
        const data = await startupService.getAll();
        setStartups(data);
        setFilteredStartups(data);
      } catch (err) {
        setError('Error loading startups');
        console.error('Error fetching startups:', err);
      } finally {
        setLoading(false);
      }
    };

    fetchStartups();
  }, []);

  // Filter effect
  useEffect(() => {
    let filtered = startups;

    // Filter by search term
    if (searchTerm) {
      filtered = filtered.filter(startup =>
        startup.name.toLowerCase().includes(searchTerm.toLowerCase()) ||
        startup.description?.toLowerCase().includes(searchTerm.toLowerCase()) ||
        startup.sector.toLowerCase().includes(searchTerm.toLowerCase())
      );
    }

    // Filter by sector
    if (selectedSector) {
      filtered = filtered.filter(startup => startup.sector === selectedSector);
    }

    // Filter by maturity
    if (selectedMaturity) {
      filtered = filtered.filter(startup => startup.maturity === selectedMaturity);
    }

    // Filter by status
    if (selectedStatus) {
      filtered = filtered.filter(startup => startup.project_status === selectedStatus);
    }

    setFilteredStartups(filtered);
  }, [startups, searchTerm, selectedSector, selectedMaturity, selectedStatus]);

  // Get unique values for filter options
  const uniqueSectors = [...new Set(startups.map(startup => startup.sector))].sort();
  const uniqueMaturities = [...new Set(startups.map(startup => startup.maturity))].sort();
  const uniqueStatuses = [...new Set(startups.map(startup => startup.project_status))].sort();

  // Helper function to check if user can access pitch deck for a startup
  const canUserAccessPitchDeck = (): boolean => {
    // Anyone can view pitch decks (editing permissions are handled in PitchDeck component)
    return true;
  };

  // Helper function to check if user can edit pitch deck for a startup
  const canUserEditPitchDeck = (startup: StartupType): boolean => {
    return isAuthenticated && user?.role === 'startup' && user?.startup_id === startup.id;
  };

  const clearFilters = () => {
    setSearchTerm('');
    setSelectedSector('');
    setSelectedMaturity('');
    setSelectedStatus('');
  };

  const handleStartupClick = (startup: StartupType) => {
    setSelectedStartup(startup);
    setIsModalOpen(true);
  };

  const handlePitchDeckOpen = (startup: StartupType) => {
    // Check if user can edit (is the owner)
    if (canUserEditPitchDeck(startup)) {
      // Open the pitch deck builder for editing
      setSelectedStartup(startup);
      setIsPitchDeckOpen(true);
    } else {
      // For non-owners, still open the modal but in view-only mode
      // They will see a prominent download button
      setSelectedStartup(startup);
      setIsPitchDeckOpen(true);
    }
  };

  const closePitchDeck = () => {
    setIsPitchDeckOpen(false);
    setSelectedStartup(null);
  };

  const closeModal = () => {
    setIsModalOpen(false);
    setSelectedStartup(null);
  };

  if (loading) {
    return <LoadingSpinner size="large" text="Loading startup catalog..." />;
  }

  if (error) {
    return (
      <div className="container" style={{ paddingTop: '2rem', textAlign: 'center' }}>
        <h1>Error</h1>
        <p style={{ color: 'red' }}>{error}</p>
        <p>Make sure the Django backend is running on http://localhost:8000</p>
      </div>
    );
  }

  return (
    <div className="container" style={{ paddingTop: '2rem' }}>
      <div style={{ 
        display: 'flex', 
        justifyContent: 'space-between', 
        alignItems: 'center', 
        marginBottom: '2rem',
        flexWrap: 'wrap',
        gap: '1rem'
      }}>
        <h1 style={{ margin: 0 }}>Startup Catalog</h1>
      </div>

      {/* Filter Section */}
      <div style={{
        background: '#f8f9fa',
        padding: '1.5rem',
        borderRadius: '8px',
        marginBottom: '2rem',
        border: '1px solid #e9ecef'
      }}>
        <h3 style={{ marginBottom: '1rem', color: 'var(--coral-medium)' }}>Filter Startups</h3>
        
        <div style={{
          display: 'grid',
          gridTemplateColumns: 'repeat(auto-fit, minmax(200px, 1fr))',
          gap: '1rem',
          marginBottom: '1rem'
        }}>
          {/* Search */}
          <div>
            <label style={{ display: 'block', marginBottom: '0.5rem', fontWeight: 'bold' }}>
              Search
            </label>
            <input
              type="text"
              placeholder="Search by name, description, or sector..."
              value={searchTerm}
              onChange={(e) => setSearchTerm(e.target.value)}
              style={{
                width: '100%',
                padding: '0.5rem',
                border: '1px solid #ddd',
                borderRadius: '4px',
                fontSize: '0.9rem'
              }}
            />
          </div>

          {/* Sector Filter */}
          <div>
            <label style={{ display: 'block', marginBottom: '0.5rem', fontWeight: 'bold' }}>
              Sector
            </label>
            <select
              value={selectedSector}
              onChange={(e) => setSelectedSector(e.target.value)}
              style={{
                width: '100%',
                padding: '0.5rem',
                border: '1px solid #ddd',
                borderRadius: '4px',
                fontSize: '0.9rem',
                background: 'white'
              }}
            >
              <option value="">All Sectors</option>
              {uniqueSectors.map(sector => (
                <option key={sector} value={sector}>{sector}</option>
              ))}
            </select>
          </div>

          {/* Maturity Filter */}
          <div>
            <label style={{ display: 'block', marginBottom: '0.5rem', fontWeight: 'bold' }}>
              Maturity
            </label>
            <select
              value={selectedMaturity}
              onChange={(e) => setSelectedMaturity(e.target.value)}
              style={{
                width: '100%',
                padding: '0.5rem',
                border: '1px solid #ddd',
                borderRadius: '4px',
                fontSize: '0.9rem',
                background: 'white'
              }}
            >
              <option value="">All Maturities</option>
              {uniqueMaturities.map(maturity => (
                <option key={maturity} value={maturity}>{maturity}</option>
              ))}
            </select>
          </div>

          {/* Status Filter */}
          <div>
            <label style={{ display: 'block', marginBottom: '0.5rem', fontWeight: 'bold' }}>
              Status
            </label>
            <select
              value={selectedStatus}
              onChange={(e) => setSelectedStatus(e.target.value)}
              style={{
                width: '100%',
                padding: '0.5rem',
                border: '1px solid #ddd',
                borderRadius: '4px',
                fontSize: '0.9rem',
                background: 'white'
              }}
            >
              <option value="">All Statuses</option>
              {uniqueStatuses.map(status => (
                <option key={status} value={status}>{status}</option>
              ))}
            </select>
          </div>
        </div>

        <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
          <div style={{ fontSize: '0.9rem', color: '#666' }}>
            Showing {filteredStartups.length} of {startups.length} startups
          </div>
          
          <button
            onClick={clearFilters}
            style={{
              background: 'var(--coral-medium)',
              color: 'white',
              border: 'none',
              padding: '0.5rem 1rem',
              borderRadius: '4px',
              cursor: 'pointer',
              fontSize: '0.9rem'
            }}
          >
            Clear Filters
          </button>
        </div>
      </div>

      {filteredStartups.length === 0 ? (
        <div style={{ textAlign: 'center', padding: '2rem' }}>
          <p>No startups found matching your filters.</p>
          {(searchTerm || selectedSector || selectedMaturity || selectedStatus) && (
            <p>Try adjusting your filters or <button onClick={clearFilters} style={{ color: 'var(--coral-medium)', background: 'none', border: 'none', cursor: 'pointer', textDecoration: 'underline' }}>clear all filters</button>.</p>
          )}
        </div>
      ) : (
        <div style={{
          display: 'grid',
          gridTemplateColumns: 'repeat(auto-fit, minmax(350px, 1fr))',
          gap: '2rem'
        }}>
          {filteredStartups.map((startup, index) => (
            <FadeInOnScroll
              key={startup.id}
              delay={index * 100}
              direction="up"
            >
              <StartupCard
                startup={startup}
                onClick={() => handleStartupClick(startup)}
                onPitchDeckOpen={() => handlePitchDeckOpen(startup)}
                canAccessPitchDeck={canUserAccessPitchDeck()}
                canEditPitchDeck={canUserEditPitchDeck(startup)}
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
        title={selectedStartup?.name || ''}
        size="large"
      >
        {selectedStartup && (
          <div>
            <div style={{
              display: 'flex',
              gap: '1rem',
              marginBottom: '2rem',
              flexWrap: 'wrap'
            }}>
              <span style={{
                background: 'var(--coral-medium)',
                color: 'white',
                padding: '0.25rem 0.75rem',
                borderRadius: '20px',
                fontSize: '0.9rem'
              }}>
                {selectedStartup.sector}
              </span>
              <span style={{
                background: 'var(--purple-medium)',
                color: 'white',
                padding: '0.25rem 0.75rem',
                borderRadius: '20px',
                fontSize: '0.9rem'
              }}>
                {selectedStartup.maturity}
              </span>
              <span style={{
                background: selectedStartup.project_status === 'Active' ? '#4caf50' : '#F49C9C',
                color: 'white',
                padding: '0.25rem 0.75rem',
                borderRadius: '20px',
                fontSize: '0.9rem'
              }}>
                {selectedStartup.project_status}
              </span>
            </div>

            <div style={{
              display: 'grid',
              gridTemplateColumns: '1fr 300px',
              gap: '2rem',
              marginBottom: '2rem'
            }}>
              <div>
                {selectedStartup.description && (
                  <div style={{ marginBottom: '2rem' }}>
                    <h3 style={{ color: 'var(--coral-medium)', marginBottom: '1rem' }}>
                      Description
                    </h3>
                    <MarkdownContent content={selectedStartup.description} />
                  </div>
                )}

                {selectedStartup.needs && (
                  <div style={{ marginBottom: '2rem' }}>
                    <h3 style={{ color: 'var(--coral-medium)', marginBottom: '1rem' }}>
                      Needs
                    </h3>
                    <MarkdownContent content={selectedStartup.needs} />
                  </div>
                )}

                {selectedStartup.progress && (
                  <div style={{ marginBottom: '2rem' }}>
                    <h3 style={{ color: 'var(--coral-medium)', marginBottom: '1rem' }}>
                      Progress
                    </h3>
                    <MarkdownContent content={selectedStartup.progress} />
                  </div>
                )}
              </div>

              <div>
                <div className="card" style={{ padding: '1.5rem' }}>
                  <h3 style={{ marginBottom: '1rem' }}>Information</h3>

                  <div style={{ marginBottom: '1rem' }}>
                    <strong>Legal status:</strong><br />
                    <span style={{ color: '#CB90F1' }}>{selectedStartup.legal_status}</span>
                  </div>

                  {selectedStartup.address && (
                    <div style={{ marginBottom: '1rem' }}>
                      <strong>Address:</strong><br />
                      <span style={{ color: '#CB90F1' }}>{selectedStartup.address}</span>
                    </div>
                  )}

                  <div style={{ marginBottom: '1rem' }}>
                    <strong>Email:</strong><br />
                    <a href={`mailto:${selectedStartup.email}`} style={{ color: 'var(--coral-medium)' }}>
                      {selectedStartup.email}
                    </a>
                  </div>

                  {selectedStartup.phone && (
                    <div style={{ marginBottom: '1rem' }}>
                      <strong>Phone:</strong><br />
                      <a href={`tel:${selectedStartup.phone}`} style={{ color: 'var(--coral-medium)' }}>
                        {selectedStartup.phone}
                      </a>
                    </div>
                  )}

                  <div style={{ marginBottom: '1rem' }}>
                    <strong>Created on:</strong><br />
                    <span style={{ color: '#CB90F1' }}>
                      {new Date(selectedStartup.created_at).toLocaleDateString('en-US')}
                    </span>
                  </div>

                  <div style={{ marginBottom: '1.5rem' }}>
                    <strong>Last updated:</strong><br />
                    <span style={{ color: '#CB90F1' }}>
                      {new Date(selectedStartup.updated_at).toLocaleDateString('en-US')}
                    </span>
                  </div>

                  <div style={{ display: 'flex', gap: '0.5rem', flexDirection: 'column' }}>
                    {selectedStartup.website_url && (
                      <a
                        href={selectedStartup.website_url}
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
                        🌐 Website
                      </a>
                    )}
                    {selectedStartup.social_media_url && (
                      <a
                        href={selectedStartup.social_media_url}
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
                        📱 Social Media
                      </a>
                    )}
                  </div>
                </div>
              </div>
            </div>
          </div>
        )}
      </Modal>

      {/* Pitch Deck Modal */}
      {isPitchDeckOpen && selectedStartup && (
        <Modal
          isOpen={isPitchDeckOpen}
          onClose={closePitchDeck}
          title={`Pitch Deck - ${selectedStartup.name}`}
          size="full"
        >
          <PitchDeck 
            startup={selectedStartup} 
            onClose={closePitchDeck}
          />
        </Modal>
      )}
    </div>
  );
};

export default StartupCatalog;
