import { useState, useEffect } from 'react';
import {
  IonContent,
  IonHeader,
  IonPage,
  IonTitle,
  IonToolbar,
  IonButton,
  IonIcon,
  IonBackButton,
  IonButtons,
  IonItem,
  IonLabel,
  IonSelect,
  IonSelectOption,
  IonCard,
  IonCardContent,
  IonCardHeader,
  IonCardTitle,
} from '@ionic/react';
import { addCircle, checkmarkCircle } from 'ionicons/icons';
import { useHistory } from 'react-router-dom';
import { availableServices } from '../servicesData';

interface Action {
  id: string;
  label: string;
  description: string;
  service?: string;
}

interface Reaction {
  id: string;
  label: string;
  description: string;
}

const AREABuilderPage: React.FC = () => {
  const history = useHistory();
  const [selectedAction, setSelectedAction] = useState<Action | null>(null);
  const [selectedReaction, setSelectedReaction] = useState<Reaction | null>(null);
  const [availableReactions, setAvailableReactions] = useState<Reaction[]>([]);

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

    // Récupérer les AREAs existantes depuis localStorage
    const existingAreas = JSON.parse(localStorage.getItem('userAreas') || '[]');
    const updatedAreas = [...existingAreas, newArea];
    localStorage.setItem('userAreas', JSON.stringify(updatedAreas));

    // Rediriger vers la liste
    history.push('/my-areas');
  };

  const allActions = Object.entries(availableServices).flatMap(([serviceKey, service]) =>
    (service.actions || []).map(action => ({ ...action, service: serviceKey }))
  );

  return (
    <IonPage>
      <IonHeader>
        <IonToolbar color="primary">
          <IonButtons slot="start">
            <IonBackButton defaultHref="/" />
          </IonButtons>
          <IonTitle>Créer une AREA</IonTitle>
        </IonToolbar>
      </IonHeader>
      <IonContent className="ion-padding">
        <IonCard>
          <IonCardHeader>
            <IonCardTitle>Configuration de l'automation</IonCardTitle>
          </IonCardHeader>
          <IonCardContent>
            <IonItem>
              <IonLabel position="stacked">
                <strong>QUAND</strong> (Action) :
              </IonLabel>
              <IonSelect
                placeholder="Choisir une action..."
                onIonChange={(e) => {
                  const actionData = JSON.parse(e.detail.value || 'null');
                  setSelectedAction(actionData);
                }}
              >
                {allActions.map((action, index) => (
                  <IonSelectOption 
                    key={index} 
                    value={JSON.stringify(action)}
                  >
                    {availableServices[action.service!]?.icon} {availableServices[action.service!]?.name} - {action.label}
                  </IonSelectOption>
                ))}
              </IonSelect>
            </IonItem>

            <IonItem style={{ marginTop: '20px' }}>
              <IonLabel position="stacked">
                <strong>ALORS</strong> (REAction) :
              </IonLabel>
              <IonSelect
                placeholder="Choisir une réaction..."
                disabled={!selectedAction}
                onIonChange={(e) => {
                  const reactionData = JSON.parse(e.detail.value || 'null');
                  setSelectedReaction(reactionData);
                }}
              >
                {availableReactions.map((reaction, index) => (
                  <IonSelectOption 
                    key={index} 
                    value={JSON.stringify(reaction)}
                  >
                    {reaction.label}
                  </IonSelectOption>
                ))}
              </IonSelect>
            </IonItem>

            <IonButton
              expand="block"
              onClick={handleCreateArea}
              disabled={!selectedAction || !selectedReaction}
              style={{ marginTop: '30px' }}
            >
              <IonIcon slot="start" icon={checkmarkCircle} />
              Créer l'AREA
            </IonButton>
          </IonCardContent>
        </IonCard>

        {selectedAction && selectedReaction && (
          <IonCard color="light" style={{ marginTop: '20px' }}>
            <IonCardHeader>
              <IonCardTitle>Aperçu</IonCardTitle>
            </IonCardHeader>
            <IonCardContent>
              <p><strong>QUAND:</strong> {selectedAction.label}</p>
              <p><strong>ALORS:</strong> {selectedReaction.label}</p>
            </IonCardContent>
          </IonCard>
        )}
      </IonContent>
    </IonPage>
  );
};

export default AREABuilderPage;
