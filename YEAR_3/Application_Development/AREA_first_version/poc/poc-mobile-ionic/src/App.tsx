import { IonApp, IonRouterOutlet, setupIonicReact } from '@ionic/react';
import { IonReactRouter } from '@ionic/react-router';
import { Route } from 'react-router-dom';
import HomePage from './pages/HomePage';
import AREABuilderPage from './pages/AREABuilderPage';
import MyAreasPage from './pages/MyAreasPage';

setupIonicReact();

function App() {
  return (
    <IonApp>
      <IonReactRouter>
        <IonRouterOutlet>
          <Route exact path="/" component={HomePage} />
          <Route exact path="/builder" component={AREABuilderPage} />
          <Route exact path="/my-areas" component={MyAreasPage} />
        </IonRouterOutlet>
      </IonReactRouter>
    </IonApp>
  );
}

export default App;
