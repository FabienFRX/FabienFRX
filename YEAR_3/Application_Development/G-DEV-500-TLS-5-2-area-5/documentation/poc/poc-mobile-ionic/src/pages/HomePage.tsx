import {
  IonContent,
  IonHeader,
  IonPage,
  IonTitle,
  IonToolbar,
  IonButton,
  IonIcon,
  IonCard,
  IonCardContent,
  IonCardHeader,
  IonCardTitle,
} from '@ionic/react';
import { addCircle, list } from 'ionicons/icons';
import { useHistory } from 'react-router-dom';

const HomePage: React.FC = () => {
  const history = useHistory();

  return (
    <IonPage>
      <IonHeader>
        <IonToolbar color="primary">
          <IonTitle>Automation Platform - PoC Ionic</IonTitle>
        </IonToolbar>
      </IonHeader>
      <IonContent className="ion-padding">
        <div style={{ 
          display: 'flex', 
          flexDirection: 'column', 
          alignItems: 'center', 
          justifyContent: 'center',
          minHeight: '80vh',
          textAlign: 'center'
        }}>
          <h1 style={{ fontSize: '2.5rem', marginBottom: '20px' }}>⚡ AREA Builder</h1>
          
          <IonCard style={{ maxWidth: '500px', marginBottom: '30px' }}>
            <IonCardHeader>
              <IonCardTitle>Automatisez vos tâches</IonCardTitle>
            </IonCardHeader>
            <IonCardContent>
              <p>
                Créez des automatisations (AREA) en connectant vos services préférés :
                <strong> SI</strong> une action se produit, <strong>ALORS</strong> déclenchez une réaction.
              </p>
            </IonCardContent>
          </IonCard>

          <IonButton 
            onClick={() => history.push('/builder')}
            size="large"
            expand="block"
            style={{ marginBottom: '15px', minWidth: '300px' }}
          >
            <IonIcon slot="start" icon={addCircle} />
            Créer une AREA
          </IonButton>

          <IonButton 
            onClick={() => history.push('/my-areas')}
            size="large"
            expand="block"
            fill="outline"
            style={{ minWidth: '300px' }}
          >
            <IonIcon slot="start" icon={list} />
            Mes AREAs
          </IonButton>
        </div>
      </IonContent>
    </IonPage>
  );
};

export default HomePage;
