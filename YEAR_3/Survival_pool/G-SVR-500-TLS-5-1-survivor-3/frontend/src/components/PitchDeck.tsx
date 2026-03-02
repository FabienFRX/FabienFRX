import React, { useState, useEffect } from 'react';
import { Startup } from '../services/api';
import { useAuth } from '../contexts/AuthContext';
import { PitchDeckData, generatePitchDeckPDF } from '../utils/pdfExport';
import './PitchDeck.css';

interface PitchDeckProps {
  startup: Startup;
  onClose?: () => void;
}

const PitchDeck: React.FC<PitchDeckProps> = ({ startup }) => {
  const { user, isAuthenticated } = useAuth();
  const [isGenerating, setIsGenerating] = useState(false);
  const [isSaving, setIsSaving] = useState(false);
  const [isLoading, setIsLoading] = useState(true);
  const [lastSaved, setLastSaved] = useState<Date | null>(null);
  const [pitchData, setPitchData] = useState<PitchDeckData>({
    startup,
    founders: startup.founders?.map(f => ({ name: f.name, role: 'Founder' })) || [],
    financials: {
      currentFunding: '',
      targetFunding: '',
      revenue: '',
      projectedRevenue: ''
    },
    metrics: {
      users: 0,
      growth: '',
      marketSize: ''
    },
    competitive: {
      advantages: [],
      competitors: []
    }
  });

  // Check if current user can edit this pitch deck
  const canEdit = isAuthenticated && user?.role === 'startup' && user?.startup_id === startup.id;

  // Load saved data when component mounts
  useEffect(() => {
    const loadPitchData = async () => {
      setIsLoading(true);
      try {
        // Prepare headers - only include Authorization if token exists
        const token = localStorage.getItem('token');
        const headers: Record<string, string> = {};
        if (token) {
          headers['Authorization'] = `Token ${token}`;
        }

        // Load from backend API
        const response = await fetch(`http://localhost:8000/api/pitch-deck/${startup.id}/`, {
          headers,
        });
        
        if (response.ok) {
          const savedData = await response.json();
          setPitchData(prev => ({ 
            ...prev, 
            financials: savedData.financials || prev.financials,
            metrics: savedData.metrics || prev.metrics,
            competitive: savedData.competitive || prev.competitive
          }));
          setLastSaved(new Date(savedData.last_saved));
        } else if (response.status !== 404) {
          // 404 is expected if no pitch deck exists yet
          console.error('Error loading pitch deck data:', response.status, response.statusText);
        }
      } catch (error) {
        console.error('Error loading pitch deck data:', error);
        // Fallback to localStorage if API is not available
        const savedDataString = localStorage.getItem(`pitchDeck_${startup.id}`);
        if (savedDataString) {
          const savedData = JSON.parse(savedDataString);
          setPitchData(prev => ({ 
            ...prev, 
            financials: savedData.data.financials || prev.financials,
            metrics: savedData.data.metrics || prev.metrics,
            competitive: savedData.data.competitive || prev.competitive
          }));
          setLastSaved(new Date(savedData.lastModified));
        }
      } finally {
        setIsLoading(false);
      }
    };

    loadPitchData();
  }, [startup.id]);

  const handleSave = async () => {
    if (!canEdit) {
      alert('You can only edit your own startup\'s pitch deck.');
      return;
    }

    setIsSaving(true);
    try {
      const token = localStorage.getItem('token');
      if (!token) {
        throw new Error('No authentication token found');
      }

      const response = await fetch(`http://localhost:8000/api/pitch-deck/${startup.id}/`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          'Authorization': `Token ${token}`,
        },
        body: JSON.stringify({
          financials: pitchData.financials,
          metrics: pitchData.metrics,
          competitive: pitchData.competitive
        }),
      });

      if (response.ok) {
        setLastSaved(new Date());
      } else {
        const errorData = await response.json().catch(() => ({}));
        throw new Error(errorData.error || `Server error: ${response.status}`);
      }
    } catch (error) {
      console.error('Error saving pitch data:', error);
      
      // Provide specific error message based on the error type
      const errorMessage = error instanceof Error ? error.message : String(error);
      
      if (error instanceof TypeError && errorMessage.includes('fetch')) {
        alert('❌ Cannot connect to server. Please check if the backend is running.');
      } else if (errorMessage.includes('401')) {
        alert('❌ Authentication failed. Please log in again.');
      } else if (errorMessage.includes('403')) {
        alert('❌ Permission denied. You can only edit your own startup\'s pitch deck.');
      } else {
        alert(`❌ Failed to save to database: ${errorMessage}`);
      }
      
      // Don't fall back to localStorage - make it clear that data wasn't saved
      console.log('Data not saved. Backend connection required for persistence.');
    } finally {
      setIsSaving(false);
    }
  };

  const handleExportPDF = async () => {
    setIsGenerating(true);
    try {
      // Temporarily show the hidden preview element for PDF generation
      const previewElement = document.getElementById('pitch-deck-content');
      if (previewElement) {
        const originalDisplay = previewElement.style.display;
        previewElement.style.display = 'block';
        
        // Small delay to ensure element is rendered
        await new Promise(resolve => setTimeout(resolve, 100));
        
        await generatePitchDeckPDF(pitchData);
        
        // Hide the element again
        previewElement.style.display = originalDisplay;
      } else {
        throw new Error('PDF preview element not found');
      }
    } catch (error) {
      console.error('Failed to generate PDF:', error);
      alert('Failed to generate PDF. Please try again.');
    } finally {
      setIsGenerating(false);
    }
  };

  const addAdvantage = () => {
    setPitchData(prev => ({
      ...prev,
      competitive: {
        ...prev.competitive!,
        advantages: [...prev.competitive!.advantages, '']
      }
    }));
  };

  const updateAdvantage = (index: number, value: string) => {
    setPitchData(prev => ({
      ...prev,
      competitive: {
        ...prev.competitive!,
        advantages: prev.competitive!.advantages.map((adv, i) => i === index ? value : adv)
      }
    }));
  };

  const removeAdvantage = (index: number) => {
    setPitchData(prev => ({
      ...prev,
      competitive: {
        ...prev.competitive!,
        advantages: prev.competitive!.advantages.filter((_, i) => i !== index)
      }
    }));
  };

  return (
    <div className="pitch-deck-container">
      <div className="pitch-deck-header">
        <div>
          <h2>
            {canEdit ? 'Pitch Builder' : 'Pitch Deck'} - {startup.name}
          </h2>
          {!canEdit && (
            <p style={{ fontSize: '0.9rem', color: '#666', margin: '0.5rem 0' }}>
              View-only mode - Click "Download PDF" below to save this pitch deck
            </p>
          )}
          {lastSaved && (
            <p style={{ fontSize: '0.9rem', color: '#666', margin: '0.5rem 0' }}>
              Last saved: {lastSaved.toLocaleString()}
            </p>
          )}
          {isLoading && (
            <p style={{ fontSize: '0.9rem', color: '#666', margin: '0.5rem 0' }}>
              Loading saved data...
            </p>
          )}
        </div>
        <div className="pitch-deck-actions">
          {canEdit && (
            <button 
              onClick={handleSave}
              disabled={isSaving || isLoading}
              className="btn btn-success"
              style={{ marginRight: '0.5rem' }}
            >
              {isSaving ? 'Saving...' : 'Save'}
            </button>
          )}
          <button 
            onClick={handleExportPDF}
            disabled={isGenerating}
            className="btn btn-primary"
            style={{ marginRight: '0.5rem' }}
          >
            {isGenerating ? 'Generating PDF...' : 'Download PDF'}
          </button>
        </div>
      </div>

      {/* Hidden preview section for PDF generation - always available */}
      <div className="pitch-deck-preview" id="pitch-deck-content" style={{ display: 'none' }}>
        {/* Company Header */}
        <div className="preview-section company-header">
          <h1>{startup.name}</h1>
          <p className="company-description">{startup.description}</p>
          <div className="company-details">
            <span className="sector-tag">{startup.sector}</span>
            <span className="maturity-tag">{startup.maturity}</span>
          </div>
        </div>

        {/* Financial Information */}
        <div className="preview-section">
          <h2>Financial Overview</h2>
          <div className="financial-grid">
            <div className="financial-item">
              <h4>Current Funding</h4>
              <p className="financial-value">{pitchData.financials?.currentFunding || 'Not specified'}</p>
            </div>
            <div className="financial-item">
              <h4>Target Funding</h4>
              <p className="financial-value">{pitchData.financials?.targetFunding || 'Not specified'}</p>
            </div>
            <div className="financial-item">
              <h4>Current Revenue</h4>
              <p className="financial-value">{pitchData.financials?.revenue || 'Not specified'}</p>
            </div>
            <div className="financial-item">
              <h4>Projected Revenue</h4>
              <p className="financial-value">{pitchData.financials?.projectedRevenue || 'Not specified'}</p>
            </div>
          </div>
        </div>

        {/* Metrics Section */}
        <div className="preview-section">
          <h2>Key Metrics</h2>
          <div className="metrics-grid">
            <div className="metric-item">
              <h4>User Base</h4>
              <p className="metric-value">{pitchData.metrics?.users || 'Not specified'}</p>
            </div>
            <div className="metric-item">
              <h4>Growth Rate</h4>
              <p className="metric-value">{pitchData.metrics?.growth || 'Not specified'}</p>
            </div>
            <div className="metric-item">
              <h4>Market Size</h4>
              <p className="metric-value">{pitchData.metrics?.marketSize || 'Not specified'}</p>
            </div>
          </div>
        </div>

        {/* Competitive Advantages */}
        <div className="preview-section">
          <h2>Competitive Advantages</h2>
          <div className="advantages-list">
            {pitchData.competitive?.advantages && pitchData.competitive.advantages.length > 0 ? (
              pitchData.competitive.advantages.map((advantage, index) => (
                <div key={index} className="advantage-item">
                  <span className="advantage-text">{advantage || 'Not specified'}</span>
                </div>
              ))
            ) : (
              <p>No competitive advantages specified</p>
            )}
          </div>
        </div>

        {/* Team Section */}
        <div className="preview-section">
          <h2>Founding Team</h2>
          <div className="team-grid">
            {pitchData.founders && pitchData.founders.length > 0 ? (
              pitchData.founders.map((founder, index) => (
                <div key={index} className="team-member">
                  <h4>{founder.name}</h4>
                  <p>{founder.role || 'Founder'}</p>
                </div>
              ))
            ) : (
              <p>Team information not available</p>
            )}
          </div>
        </div>
      </div>

      {canEdit ? (
        // Editor view for owners
        <div className="pitch-deck-editor">
          {/* Financials Section */}
          <div className="pitch-section">
            <h3>Financial Information</h3>
            <div className="form-row">
              <div className="form-group">
                <label>Current Funding</label>
                <input
                  type="text"
                  value={pitchData.financials?.currentFunding || ''}
                  onChange={(e) => setPitchData(prev => ({
                    ...prev,
                    financials: { ...prev.financials!, currentFunding: e.target.value }
                  }))}
                  placeholder="e.g., €500K"
                  readOnly={!canEdit}
                  style={{ backgroundColor: !canEdit ? '#f5f5f5' : 'white' }}
                />
              </div>
              <div className="form-group">
                <label>Target Funding</label>
                <input
                  type="text"
                  value={pitchData.financials?.targetFunding || ''}
                  onChange={(e) => setPitchData(prev => ({
                    ...prev,
                    financials: { ...prev.financials!, targetFunding: e.target.value }
                  }))}
                  placeholder="e.g., €2M"
                  readOnly={!canEdit}
                  style={{ backgroundColor: !canEdit ? '#f5f5f5' : 'white' }}
                />
              </div>
            </div>
            <div className="form-row">
              <div className="form-group">
                <label>Current Revenue</label>
                <input
                  type="text"
                  value={pitchData.financials?.revenue || ''}
                  onChange={(e) => setPitchData(prev => ({
                    ...prev,
                    financials: { ...prev.financials!, revenue: e.target.value }
                  }))}
                  placeholder="e.g., €100K/month"
                  readOnly={!canEdit}
                  style={{ backgroundColor: !canEdit ? '#f5f5f5' : 'white' }}
                />
              </div>
              <div className="form-group">
                <label>Projected Revenue</label>
                <input
                  type="text"
                  value={pitchData.financials?.projectedRevenue || ''}
                  onChange={(e) => setPitchData(prev => ({
                    ...prev,
                    financials: { ...prev.financials!, projectedRevenue: e.target.value }
                  }))}
                  placeholder="e.g., €1M/month"
                  readOnly={!canEdit}
                  style={{ backgroundColor: !canEdit ? '#f5f5f5' : 'white' }}
                />
              </div>
            </div>
          </div>

          {/* Metrics Section */}
          <div className="pitch-section">
            <h3>Key Metrics</h3>
            <div className="form-row">
              <div className="form-group">
                <label>Number of Users</label>
                <input
                  type="number"
                  value={pitchData.metrics?.users || 0}
                  onChange={(e) => setPitchData(prev => ({
                    ...prev,
                    metrics: { ...prev.metrics!, users: parseInt(e.target.value) || 0 }
                  }))}
                  readOnly={!canEdit}
                  style={{ backgroundColor: !canEdit ? '#f5f5f5' : 'white' }}
                />
              </div>
              <div className="form-group">
                <label>Growth Rate</label>
                <input
                  type="text"
                  value={pitchData.metrics?.growth || ''}
                  onChange={(e) => setPitchData(prev => ({
                    ...prev,
                    metrics: { ...prev.metrics!, growth: e.target.value }
                  }))}
                  placeholder="e.g., 20% MoM"
                  readOnly={!canEdit}
                  style={{ backgroundColor: !canEdit ? '#f5f5f5' : 'white' }}
                />
              </div>
            </div>
            <div className="form-group">
              <label>Market Size</label>
              <input
                type="text"
                value={pitchData.metrics?.marketSize || ''}
                onChange={(e) => setPitchData(prev => ({
                  ...prev,
                  metrics: { ...prev.metrics!, marketSize: e.target.value }
                }))}
                placeholder="e.g., €50B TAM"
                readOnly={!canEdit}
                style={{ backgroundColor: !canEdit ? '#f5f5f5' : 'white' }}
              />
            </div>
          </div>

          {/* Competitive Advantages */}
          <div className="pitch-section">
            <h3>Competitive Advantages</h3>
            {pitchData.competitive?.advantages.map((advantage, index) => (
              <div key={index} className="advantage-item">
                <input
                  type="text"
                  value={advantage}
                  onChange={(e) => updateAdvantage(index, e.target.value)}
                  placeholder="Enter competitive advantage"
                  readOnly={!canEdit}
                  style={{ backgroundColor: !canEdit ? '#f5f5f5' : 'white' }}
                />
                <button 
                  onClick={() => removeAdvantage(index)}
                  className="btn btn-danger btn-sm"
                  disabled={!canEdit}
                  style={{ opacity: !canEdit ? 0.5 : 1 }}
                >
                  Remove
                </button>
              </div>
            ))}
            <button 
              onClick={addAdvantage} 
              className="btn btn-outline"
              disabled={!canEdit}
              style={{ opacity: !canEdit ? 0.5 : 1 }}
            >
              Add Advantage
            </button>
          </div>
        </div>
      ) : (
        // Preview view for non-owners
        <div className="pitch-deck-preview" id="pitch-deck-content">
          {/* Company Header */}
          <div className="preview-section company-header">
            <h1>{startup.name}</h1>
            <p className="company-description">{startup.description}</p>
            <div className="company-details">
              <span className="sector-tag">{startup.sector}</span>
              <span className="maturity-tag">{startup.maturity}</span>
            </div>
          </div>

          {/* Financial Information */}
          <div className="preview-section">
            <h2>Financial Overview</h2>
            <div className="financial-grid">
              <div className="financial-item">
                <h4>Current Funding</h4>
                <p className="financial-value">{pitchData.financials?.currentFunding || 'Not specified'}</p>
              </div>
              <div className="financial-item">
                <h4>Target Funding</h4>
                <p className="financial-value">{pitchData.financials?.targetFunding || 'Not specified'}</p>
              </div>
              <div className="financial-item">
                <h4>Current Revenue</h4>
                <p className="financial-value">{pitchData.financials?.revenue || 'Not specified'}</p>
              </div>
              <div className="financial-item">
                <h4>Projected Revenue</h4>
                <p className="financial-value">{pitchData.financials?.projectedRevenue || 'Not specified'}</p>
              </div>
            </div>
          </div>

          {/* Key Metrics */}
          <div className="preview-section">
            <h2>Key Metrics</h2>
            <div className="metrics-grid">
              <div className="metric-item">
                <h4>Users</h4>
                <p className="metric-value">{pitchData.metrics?.users?.toLocaleString() || '0'}</p>
              </div>
              <div className="metric-item">
                <h4>Growth Rate</h4>
                <p className="metric-value">{pitchData.metrics?.growth || 'Not specified'}</p>
              </div>
              <div className="metric-item">
                <h4>Market Size</h4>
                <p className="metric-value">{pitchData.metrics?.marketSize || 'Not specified'}</p>
              </div>
            </div>
          </div>

          {/* Competitive Advantages */}
          <div className="preview-section">
            <h2>Competitive Advantages</h2>
            {pitchData.competitive?.advantages && pitchData.competitive.advantages.length > 0 ? (
              <ul className="advantages-list">
                {pitchData.competitive.advantages.map((advantage, index) => (
                  advantage && <li key={index} className="advantage-item">{advantage}</li>
                ))}
              </ul>
            ) : (
              <p className="no-data">No competitive advantages specified</p>
            )}
          </div>

          {/* Founders */}
          {pitchData.founders && pitchData.founders.length > 0 && (
            <div className="preview-section">
              <h2>Team</h2>
              <div className="founders-grid">
                {pitchData.founders.map((founder, index) => (
                  <div key={index} className="founder-item">
                    <h4>{founder.name}</h4>
                    <p>{founder.role}</p>
                  </div>
                ))}
              </div>
            </div>
          )}
        </div>
      )}
    </div>
  );
};

export default PitchDeck;
