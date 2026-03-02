# AREA - Système de Hook et Exécution Automatique

## Architecture du Système

Le projet utilise **Celery** avec **Redis** comme broker pour gérer l'exécution automatique des AREAs (workflows).

### Composants

```
┌─────────────────────────────────────────────────────────────┐
│                     INFRASTRUCTURE                          │
├──────────────┬──────────────┬──────────────┬────────────────┤
│  PostgreSQL  │    Redis     │  Celery      │  Celery Beat   │
│  (Database)  │   (Broker)   │  (Worker)    │  (Scheduler)   │
└──────────────┴──────────────┴──────────────┴────────────────┘
                       ↕                ↕
┌─────────────────────────────────────────────────────────────┐
│                   DJANGO BACKEND (8080)                     │
│  - REST API                                                 │
│  - Workflow Executor Logic                                  │
│  - Service Integrations (Google, Timer, Spotify)            │
└─────────────────────────────────────────────────────────────┘
                       ↕
┌─────────────────────────────────────────────────────────────┐
│              CLIENTS (Web 8081 + Mobile)                    │
└─────────────────────────────────────────────────────────────┘
```

## Fonctionnement du Hook

### 1. Création d'une AREA

Quand un utilisateur crée une AREA via l'interface :

```typescript
POST /api/areas/
{
  "name": "Email to Calendar",
  "action_service": "google",
  "action_type": "new_email_received",
  "reaction_service": "google",
  "reaction_type": "create_calendar_event",
  "action_config": {},
  "reaction_config": {
    "title": "New Email Alert",
    "duration": 30
  }
}
```

L'AREA est créée en base avec `enabled=True` par défaut.

### 2. Vérification Automatique (Celery Beat)

**Celery Beat** est un scheduler qui exécute automatiquement la tâche `check_all_workflows` :

- **Toutes les 30 secondes** : Vérification rapide
- **Toutes les 60 secondes** : Vérification normale

Configuration dans [`celery_app.py`](celery_app.py) :

```python
app.conf.beat_schedule = {
    'check-workflows-frequent': {
        'task': 'api.tasks.check_all_workflows',
        'schedule': 30.0,  # 30 secondes
    },
}
```

### 3. Détection des Actions

La tâche `check_all_workflows` parcourt toutes les AREAs actives et vérifie si l'action est déclenchée :

```python
@shared_task
def check_all_workflows(self):
    active_areas = Area.objects.filter(enabled=True)
    
    for area in active_areas:
        if executor.check_action_triggered(area):
            # Déclencher la réaction
            execute_reaction_task.delay(area.id)
```

### 4. Exécution de la Réaction

Quand une action est détectée, une tâche asynchrone est créée pour exécuter la réaction :

```python
@shared_task
def execute_reaction_task(self, area_id):
    area = Area.objects.get(id=area_id)
    
    # Créer un log d'exécution
    execution = AreaExecution.objects.create(
        area=area,
        status='partial'
    )
    
    # Exécuter la réaction
    executor.execute_reaction(area)
    
    # Marquer comme réussie
    execution.status = 'success'
    execution.save()
```

## Services Implémentés

### 1. Timer Service

Actions disponibles :
- `daily_at_time` : Déclenche à une heure précise chaque jour
- `every_x_seconds` : Intervalle en secondes
- `every_x_minutes` : Intervalle en minutes
- `every_x_hours` : Intervalle en heures

Configuration :
```json
{
  "action_config": {
    "time": "14:30"  // Pour daily_at_time
    // OU
    "seconds": 60     // Pour every_x_seconds
    // OU
    "minutes": 5      // Pour every_x_minutes
  }
}
```

### 2. Google Gmail

**Actions** :
- `new_email_received` : Nouvel email non lu
- `new_email_from_sender` : Email d'un expéditeur spécifique
- `new_email_with_label` : Email avec un label

**Réactions** :
- `send_email` : Envoyer un email
- `forward_email` : Transférer un email
- `mark_email_as_read` : Marquer comme lu
- `add_email_label` : Ajouter un label

### 3. Google Calendar

**Actions** :
- `calendar_event_created` : Nouvel événement créé
- `calendar_event_upcoming` : Événement à venir (X minutes avant)

**Réactions** :
- `create_calendar_event` : Créer un événement
- `delete_calendar_event` : Supprimer un événement

### 4. Google Drive

**Actions** :
- `drive_file_uploaded` : Nouveau fichier uploadé
- `drive_file_shared` : Fichier partagé avec vous

**Réactions** :
- `save_to_drive` : Sauvegarder un fichier
- `create_drive_folder` : Créer un dossier
- `share_drive_file` : Partager un fichier

## Démarrage du Système

### 1. Lancer l'infrastructure complète

```bash
docker-compose up -d
```

Cela démarre :
- PostgreSQL (database)
- Redis (broker)
- Django Server (8080)
- Celery Worker (traitement des tâches)
- Celery Beat (scheduler)
- Web Client (8081)

### 2. Vérifier que Celery fonctionne

```bash
# Voir les logs du worker
docker-compose logs -f celery_worker

# Voir les logs du scheduler
docker-compose logs -f celery_beat
```

Vous devriez voir :
```
celery_beat    | Scheduler: Sending due task check-workflows-frequent
celery_worker  | Task api.tasks.check_all_workflows received
celery_worker  | Checking 5 active AREAs
```

### 3. Créer une AREA de test

Via l'interface web (http://localhost:8081) ou API :

```bash
curl -X POST http://localhost:8080/api/areas/ \
  -H "Authorization: Token YOUR_TOKEN" \
  -H "Content-Type: application/json" \
  -d '{
    "name": "Test Timer",
    "action_service": "timer",
    "action_type": "every_x_seconds",
    "reaction_service": "google",
    "reaction_type": "send_email",
    "action_config": {"seconds": 60},
    "reaction_config": {
      "to_email": "test@example.com",
      "subject": "Test AREA",
      "body": "This is a test email from AREA!"
    }
  }'
```

### 4. Surveiller les exécutions

```bash
# Via API
curl http://localhost:8080/api/executions/ \
  -H "Authorization: Token YOUR_TOKEN"

# Logs en temps réel
docker-compose logs -f celery_worker | grep "triggered"
```

## API Endpoints

### Gestion des AREAs

- `GET /api/areas/` : Liste des AREAs de l'utilisateur
- `POST /api/areas/` : Créer une AREA
- `PATCH /api/areas/<id>/` : Modifier une AREA (enable/disable)
- `DELETE /api/areas/<id>/` : Supprimer une AREA

### Monitoring

- `GET /api/executions/` : Historique de toutes les exécutions
- `GET /api/executions/<area_id>/` : Exécutions d'une AREA spécifique
- `GET /api/executor/` : Statut du système
- `POST /api/executor/` : Déclencher une vérification manuelle
- `GET /api/health/celery/` : Vérifier la santé de Celery

## Debugging

### Problème : Les AREAs ne s'exécutent pas

1. **Vérifier que Celery Beat tourne** :
```bash
docker-compose ps celery_beat
# STATUS doit être "Up"
```

2. **Vérifier les logs** :
```bash
docker-compose logs celery_beat | grep "Scheduler"
```

3. **Vérifier Redis** :
```bash
docker-compose exec redis redis-cli ping
# Doit retourner: PONG
```

4. **Vérifier la connexion DB** :
```bash
docker-compose exec server python manage.py shell
>>> from api.models import Area
>>> Area.objects.filter(enabled=True).count()
```

### Problème : Celery Worker crashe

```bash
# Voir les logs détaillés
docker-compose logs celery_worker --tail=100

# Redémarrer le worker
docker-compose restart celery_worker
```

### Problème : OAuth2 ne fonctionne pas

Les services Google nécessitent des credentials OAuth2 valides dans `.env` :

```env
GOOGLE_OAUTH2_CLIENT_ID=your_client_id
GOOGLE_OAUTH2_CLIENT_SECRET=your_secret
```

## Performance

- **Intervalle de vérification** : 30-60 secondes (configurable dans `celery_app.py`)
- **Timeout des tâches** : 30 minutes par tâche
- **Retry automatique** : 3 tentatives avec délai de 30 secondes

Pour ajuster :

```python
# celery_app.py
app.conf.beat_schedule = {
    'check-workflows': {
        'task': 'api.tasks.check_all_workflows',
        'schedule': 10.0,  # Vérifier toutes les 10 secondes
    },
}
```

## Architecture Celery en Détail

```
┌──────────────────────────────────────────────────────────┐
│                    Celery Beat                           │
│                  (Scheduler)                             │
│  - Lance check_all_workflows toutes les 30s              │
└────────────────────┬─────────────────────────────────────┘
                     │ Envoie tâche
                     ↓
┌──────────────────────────────────────────────────────────┐
│                     Redis                                │
│                 (Message Broker)                         │
│  - File d'attente des tâches                             │
└────────────────────┬─────────────────────────────────────┘
                     │ Récupère tâche
                     ↓
┌──────────────────────────────────────────────────────────┐
│                  Celery Worker                           │
│               (Task Processor)                           │
│  - Exécute check_all_workflows()                         │
│  - Pour chaque AREA déclenchée:                          │
│    → Lance execute_reaction_task.delay(area_id)          │
└──────────────────────────────────────────────────────────┘
                     ↓
┌──────────────────────────────────────────────────────────┐
│              Workflow Executor                           │
│         (Business Logic)                                 │
│  - check_action_triggered(area)                          │
│  - execute_reaction(area)                                │
│  - Appels API externes (Google, etc.)                    │
└──────────────────────────────────────────────────────────┘
```

## Sécurité

- Les tokens OAuth2 sont stockés chiffrés en base
- Les tâches Celery tournent dans des conteneurs isolés
- Retry automatique avec backoff exponentiel
- Timeout pour éviter les tâches infinies

## Prochaines Améliorations

1. **Webhooks** : Au lieu de polling, utiliser webhooks pour Gmail/Calendar
2. **Caching** : Cache Redis pour réduire les appels API
3. **Rate Limiting** : Respect des quotas API Google
4. **Monitoring** : Grafana + Prometheus pour métriques
5. **Notifications** : Alertes en cas d'échec d'exécution

## Support

Pour toute question sur le système de hooks :
- Consultez les logs : `docker-compose logs -f celery_worker celery_beat`
- Vérifiez la santé : `curl http://localhost:8080/api/health/celery/`
- Testez manuellement : `curl -X POST http://localhost:8080/api/executor/`
