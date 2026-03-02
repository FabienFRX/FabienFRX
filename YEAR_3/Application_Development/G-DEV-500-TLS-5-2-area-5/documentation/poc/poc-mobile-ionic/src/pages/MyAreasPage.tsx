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
  IonCard,
  IonCardContent,
  IonCardHeader,
  IonCardTitle,
  IonBadge,
  IonItem,
  IonLabel,
} from '@ionic/react';
import { trash, power, powerOutline, addCircle } from 'ionicons/icons';
import { useHistory } from 'react-router-dom';

interface Area {
  id: number;
  action: { label: string };
  reaction: { label: string };
  active: boolean;
  createdAt: string;
}

const MyAreasPage: React.FC = () => {
  const history = useHistory();
  const [userAreas, setUserAreas] = useState<Area[]>([]);

  useEffect(() => {
    loadAreas();
  }, []);

  const loadAreas = () => {
    const areas = JSON.parse(localStorage.getItem('userAreas') || '[]');
    setUserAreas(areas);
  };

  const toggleArea = (areaId: number) => {
    const updatedAreas = userAreas.map(area =>
      area.id === areaId ? { ...area, active: !area.active } : area
    );
    setUserAreas(updatedAreas);
    localStorage.setItem('userAreas', JSON.stringify(updatedAreas));
  };

  const deleteArea = (areaId: number) => {
    const updatedAreas = userAreas.filter(area => area.id !== areaId);
    setUserAreas(updatedAreas);
    localStorage.setItem('userAreas', JSON.stringify(updatedAreas));
  };

  return (
    <IonPage>
      <IonHeader>
        <IonToolbar color="primary">
          <IonButtons slot="start">
            <IonBackButton defaultHref="/" />
          </IonButtons>
          <IonTitle>Mes AREAs ({userAreas.length})</IonTitle>
          <IonButtons slot="end">
            <IonButton onClick={() => history.push('/builder')}>
              <IonIcon slot="icon-only" icon={addCircle} />
            </IonButton>
          </IonButtons>
        </IonToolbar>
      </IonHeader>
      <IonContent className="ion-padding">
        {userAreas.length === 0 ? (
          <IonCard>
            <IonCardContent style={{ textAlign: 'center', padding: '40px' }}>
              <p style={{ fontSize: '1.2rem', marginBottom: '20px' }}>
                Aucune AREA créée. Commencez par en créer une !
              </p>
              <IonButton onClick={() => history.push('/builder')}>
                <IonIcon slot="start" icon={addCircle} />
                Créer une AREA
              </IonButton>
            </IonCardContent>
          </IonCard>
        ) : (
          userAreas.map(area => (
            <IonCard 
              key={area.id}
              style={{
                border: area.active ? '2px solid var(--ion-color-success)' : '2px solid var(--ion-color-medium)',
                marginBottom: '15px'
              }}
            >
              <IonCardHeader>
                <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
                  <IonCardTitle>AREA #{area.id}</IonCardTitle>
                  <IonBadge color={area.active ? 'success' : 'medium'}>
                    {area.active ? 'Active' : 'Inactive'}
                  </IonBadge>
                </div>
              </IonCardHeader>
              <IonCardContent>
                <IonItem lines="none">
                  <IonLabel>
                    <p><strong>QUAND:</strong></p>
                    <p>{area.action.label}</p>
                  </IonLabel>
                </IonItem>
                <IonItem lines="none">
                  <IonLabel>
                    <p><strong>ALORS:</strong></p>
                    <p>{area.reaction.label}</p>
                  </IonLabel>
                </IonItem>
                
                <div style={{ display: 'flex', gap: '10px', marginTop: '15px' }}>
                  <IonButton
                    expand="block"
                    fill="outline"
                    onClick={() => toggleArea(area.id)}
                    color={area.active ? 'medium' : 'success'}
                  >
                    <IonIcon slot="start" icon={area.active ? powerOutline : power} />
                    {area.active ? 'Désactiver' : 'Activer'}
                  </IonButton>
                  <IonButton
                    expand="block"
                    fill="outline"
                    color="danger"
                    onClick={() => deleteArea(area.id)}
                  >
                    <IonIcon slot="start" icon={trash} />
                    Supprimer
                  </IonButton>
                </div>
              </IonCardContent>
            </IonCard>
          ))
        )}
      </IonContent>
    </IonPage>
  );
};

export default MyAreasPage;
