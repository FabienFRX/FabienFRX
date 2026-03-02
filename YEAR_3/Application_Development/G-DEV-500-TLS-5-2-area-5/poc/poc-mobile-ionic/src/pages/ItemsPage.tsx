import { useState, useEffect } from 'react';
import {
  IonContent,
  IonHeader,
  IonPage,
  IonTitle,
  IonToolbar,
  IonButton,
  IonIcon,
  IonList,
  IonItem,
  IonLabel,
  IonSpinner,
  IonCard,
  IonCardHeader,
  IonCardTitle,
  IonCardContent,
  IonBackButton,
  IonButtons,
  IonRefresher,
  IonRefresherContent,
} from '@ionic/react';
import { refresh, alertCircle } from 'ionicons/icons';
import axios from 'axios';

interface Post {
  id: number;
  title: string;
  body: string;
}

const ItemsPage: React.FC = () => {
  const [items, setItems] = useState<Post[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  const fetchItems = async () => {
    try {
      setLoading(true);
      setError(null);
      const response = await axios.get(
        'https://jsonplaceholder.typicode.com/posts?_limit=10'
      );
      setItems(response.data);
    } catch (err) {
      setError('Erreur lors du chargement des données');
      console.error(err);
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    fetchItems();
  }, []);

  const handleRefresh = async (event: CustomEvent) => {
    await fetchItems();
    event.detail.complete();
  };

  return (
    <IonPage>
      <IonHeader>
        <IonToolbar color="primary">
          <IonButtons slot="start">
            <IonBackButton defaultHref="/" />
          </IonButtons>
          <IonTitle>Liste des items</IonTitle>
          <IonButtons slot="end">
            <IonButton onClick={fetchItems}>
              <IonIcon slot="icon-only" icon={refresh} />
            </IonButton>
          </IonButtons>
        </IonToolbar>
      </IonHeader>
      <IonContent>
        <IonRefresher slot="fixed" onIonRefresh={handleRefresh}>
          <IonRefresherContent />
        </IonRefresher>

        {loading ? (
          <div style={{ 
            display: 'flex', 
            justifyContent: 'center', 
            alignItems: 'center',
            height: '100%'
          }}>
            <IonSpinner name="crescent" />
          </div>
        ) : error ? (
          <div className="ion-padding" style={{ textAlign: 'center' }}>
            <IonIcon
              icon={alertCircle}
              style={{ fontSize: '60px', color: 'var(--ion-color-danger)' }}
            />
            <h2>{error}</h2>
            <IonButton onClick={fetchItems}>
              Réessayer
            </IonButton>
          </div>
        ) : (
          <IonList>
            {items.map((item) => (
              <IonCard key={item.id}>
                <IonCardHeader>
                  <IonCardTitle>#{item.id} - {item.title}</IonCardTitle>
                </IonCardHeader>
                <IonCardContent>
                  {item.body}
                </IonCardContent>
              </IonCard>
            ))}
          </IonList>
        )}
      </IonContent>
    </IonPage>
  );
};

export default ItemsPage;
