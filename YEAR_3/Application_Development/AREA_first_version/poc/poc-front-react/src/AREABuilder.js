import React, { useState, useEffect } from 'react';
import { availableServices } from './servicesData';
import './AREABuilder.css';

const AREABuilder = () => {
  const [selectedAction, setSelectedAction] = useState(null);
  const [selectedReaction, setSelectedReaction] = useState(null);
  const [userAreas, setUserAreas] = useState([]);
  const [availableReactions, setAvailableReactions] = useState([]);

  useEffect(() => {
    if (selectedAction) {
      const reactions = Object.values(availableServices)
        .flatMap(service => service.reactions || []);
      setAvailableReactions(reactions);
    }
  }, [selectedAction]);

  const handleCreateArea = () => {
    if (!selectedAction || !selectedReaction) return;

    const newArea = {
      id: Date.now(),
      action: selectedAction,
      reaction: selectedReaction,
      active: true,
      createdAt: new Date().toISOString()
    };

    setUserAreas(prev => [...prev, newArea]);
    setSelectedAction(null);
    setSelectedReaction(null);
  };

  const toggleArea = (areaId) => {
    setUserAreas(prev =>
      prev.map(area =>
        area.id === areaId ? { ...area, active: !area.active } : area
      )
    );
  };

  const deleteArea = (areaId) => {
    setUserAreas(prev => prev.filter(area => area.id !== areaId));
  };

  return (
    <div className="area-builder">
      <div className="creation-section">
        <h2>Créer une nouvelle AREA</h2>
        
        <div className="form-group">
          <label htmlFor="action-select">Quand (Action) :</label>
          <select 
            id="action-select"
            value={selectedAction ? JSON.stringify(selectedAction) : ''}
            onChange={(e) => setSelectedAction(e.target.value ? JSON.parse(e.target.value) : null)}
          >
            <option value="">Choisir une action...</option>
            {Object.entries(availableServices).map(([serviceKey, service]) =>
              service.actions?.map(action => (
                <option key={`${serviceKey}-${action.id}`} value={JSON.stringify({ service: serviceKey, ...action })}>
                  {service.icon} {service.name} - {action.label}
                </option>
              ))
            )}
          </select>
        </div>

        <div className="form-group">
          <label htmlFor="reaction-select">Alors (REAction) :</label>
          <select 
            id="reaction-select"
            value={selectedReaction ? JSON.stringify(selectedReaction) : ''}
            onChange={(e) => setSelectedReaction(e.target.value ? JSON.parse(e.target.value) : null)}
            disabled={!selectedAction}
          >
            <option value="">Choisir une réaction...</option>
            {availableReactions.map((reaction, index) => (
              <option key={index} value={JSON.stringify(reaction)}>
                {reaction.label}
              </option>
            ))}
          </select>
        </div>

        <button 
          onClick={handleCreateArea}
          disabled={!selectedAction || !selectedReaction}
          className="create-button"
        >
          Créer l'AREA
        </button>
      </div>

      <div className="areas-list">
        <h2>Mes AREAs ({userAreas.length})</h2>
        {userAreas.length === 0 ? (
          <p className="no-areas">Aucune AREA créée. Commencez par en créer une !</p>
        ) : (
          userAreas.map(area => (
            <div key={area.id} className={`area-card ${area.active ? 'active' : 'inactive'}`}>
              <div className="area-content">
                <div className="area-description">
                  <span className="area-when">
                    <strong>QUAND:</strong> {area.action.label}
                  </span>
                  <span className="area-then">
                    <strong>ALORS:</strong> {area.reaction.label}
                  </span>
                </div>
                <div className="area-actions">
                  <button 
                    onClick={() => toggleArea(area.id)}
                    className={`toggle-button ${area.active ? 'deactivate' : 'activate'}`}
                  >
                    {area.active ? 'Désactiver' : 'Activer'}
                  </button>
                  <button 
                    onClick={() => deleteArea(area.id)}
                    className="delete-button"
                  >
                    Supprimer
                  </button>
                </div>
              </div>
            </div>
          ))
        )}
      </div>
    </div>
  );
};

export default AREABuilder;