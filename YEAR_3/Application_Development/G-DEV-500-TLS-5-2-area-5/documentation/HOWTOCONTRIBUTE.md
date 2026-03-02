# Comment Contribuer au Projet AREA

## 📝 Table des Matières

- [Ajouter un Nouveau Service](#ajouter-un-nouveau-service)
- [Créer une Nouvelle Action](#créer-une-nouvelle-action)
- [Créer une Nouvelle Réaction](#créer-une-nouvelle-réaction)
- [Architecture du Code](#architecture-du-code)
- [Standards de Code](#standards-de-code)

## 🔧 Ajouter un Nouveau Service

### Exemple : Ajouter GitHub

#### 1. Créer le Service en Base de Données

```python
# backend/manage.py shell

from api.models import Service

github = Service.objects.create(
    name='github',
    description='GitHub version control and collaboration'
)
```

#### 2. Créer le Module de Service

Créer `backend/api/github_service.py` :

```python
"""
GitHub Service Integration
"""
import requests
from .models import UserService

class GitHubService:
    """GitHub API integration"""
    
    BASE_URL = 'https://api.github.com'
    
    def __init__(self, user):
        self.user = user
        self.user_service = UserService.objects.filter(
            user=user,
            service__name='github',
            is_active=True
        ).first()
        
        if not self.user_service:
            raise ValueError("User has not connected GitHub")
        
        self.token = self.user_service.access_token
        
    @property
    def headers(self):
        return {
            'Authorization': f'token {self.token}',
            'Accept': 'application/vnd.github.v3+json'
        }
    
    def get_recent_repos(self, max_results=10):
        """Get user's recent repositories"""
        url = f'{self.BASE_URL}/user/repos'
        params = {
            'sort': 'updated',
            'per_page': max_results
        }
        
        response = requests.get(url, headers=self.headers, params=params)
        
        if response.status_code == 200:
            return response.json()
        return []
    
    def get_recent_issues(self, repo_owner, repo_name, max_results=10):
        """Get recent issues from a repository"""
        url = f'{self.BASE_URL}/repos/{repo_owner}/{repo_name}/issues'
        params = {
            'state': 'open',
            'sort': 'created',
            'per_page': max_results
        }
        
        response = requests.get(url, headers=self.headers, params=params)
        
        if response.status_code == 200:
            return response.json()
        return []
    
    def create_issue(self, repo_owner, repo_name, title, body):
        """Create a new issue"""
        url = f'{self.BASE_URL}/repos/{repo_owner}/{repo_name}/issues'
        data = {
            'title': title,
            'body': body
        }
        
        response = requests.post(url, headers=self.headers, json=data)
        
        if response.status_code == 201:
            return response.json()
        return None
    
    def get_recent_commits(self, repo_owner, repo_name, max_results=10):
        """Get recent commits"""
        url = f'{self.BASE_URL}/repos/{repo_owner}/{repo_name}/commits'
        params = {'per_page': max_results}
        
        response = requests.get(url, headers=self.headers, params=params)
        
        if response.status_code == 200:
            return response.json()
        return []


def get_github_service(user):
    """Factory function to get GitHubService for a user"""
    try:
        return GitHubService(user)
    except ValueError:
        return None
```

#### 3. Ajouter OAuth2 (Si Nécessaire)

Dans `backend/api/auth_views.py`, ajouter :

```python
class GitHubOAuth2LoginView(APIView):
    """Login or register with GitHub OAuth2"""
    permission_classes = []

    def post(self, request):
        access_token = request.data.get('access_token')
        
        if not access_token:
            return Response({
                'error': 'access_token is required'
            }, status=status.HTTP_400_BAD_REQUEST)

        # Get GitHub user info
        headers = {'Authorization': f'token {access_token}'}
        response = requests.get('https://api.github.com/user', headers=headers)
        
        if response.status_code != 200:
            return Response({
                'error': 'Invalid GitHub token'
            }, status=status.HTTP_401_UNAUTHORIZED)

        github_data = response.json()
        github_id = github_data['id']
        github_username = github_data['login']
        email = github_data.get('email', f'{github_username}@github.user')

        # Get or create user
        user, created = User.objects.get_or_create(
            email=email,
            defaults={'username': github_username}
        )

        # Store GitHub token
        github_service = Service.objects.get(name='github')
        UserService.objects.update_or_create(
            user=user,
            service=github_service,
            defaults={
                'access_token': access_token,
                'service_user_id': str(github_id),
                'service_username': github_username,
                'is_active': True
            }
        )

        # Create Django token
        token, _ = Token.objects.get_or_create(user=user)

        return Response({
            'token': token.key,
            'user': {
                'id': user.id,
                'email': user.email,
                'username': user.username
            }
        }, status=status.HTTP_200_OK)
```

## ➕ Créer une Nouvelle Action

### Exemple : Action "New GitHub Issue"

#### 1. Créer l'Action en Base

```python
# backend/manage.py shell

from api.models import Service, Action

github = Service.objects.get(name='github')

Action.objects.create(
    service=github,
    name='new_issue_created',
    description='Triggers when a new issue is created in a repository'
)
```

#### 2. Implémenter le Checker

Dans `backend/api/workflow_executor.py`, ajouter dans `_check_github_action` :

```python
def _check_github_action(self, area: Area, action_name: str) -> bool:
    """Check GitHub-based actions"""
    area_key = f"github_{area.id}_{action_name}"
    now = datetime.now()

    if area_key not in self.last_check:
        self.last_check[area_key] = now - timedelta(minutes=5)

    time_diff = now - self.last_check[area_key]

    # Only check every 2 minutes
    if time_diff.total_seconds() < 120:
        return False

    user = area.user
    action_config = area.action_config or {}

    if action_name == "new_issue_created":
        repo_owner = action_config.get('repo_owner')
        repo_name = action_config.get('repo_name')
        
        if not repo_owner or not repo_name:
            logger.warning(f"Missing repo config for AREA {area.id}")
            return False

        github_service = get_github_service(user)
        if github_service:
            issues = github_service.get_recent_issues(repo_owner, repo_name, max_results=1)
            if issues:
                issue_id = issues[0]['id']
                last_issue_key = f"{area_key}_last_issue"
                
                if not hasattr(self, last_issue_key) or getattr(self, last_issue_key) != issue_id:
                    logger.info(f"New GitHub issue detected for user {user.username}")
                    setattr(self, last_issue_key, issue_id)
                    self.last_check[area_key] = now
                    
                    # Store issue data for reaction
                    area.temp_action_data = {
                        'issue_title': issues[0]['title'],
                        'issue_body': issues[0]['body'],
                        'issue_url': issues[0]['html_url']
                    }
                    return True

    # Add more GitHub actions here...

    self.last_check[area_key] = now
    return False
```

#### 3. Configuration JSON

L'utilisateur devra fournir dans `action_config` :

```json
{
  "repo_owner": "octocat",
  "repo_name": "Hello-World"
}
```

## 🎬 Créer une Nouvelle Réaction

### Exemple : Réaction "Create GitHub Issue"

#### 1. Créer la Réaction en Base

```python
# backend/manage.py shell

from api.models import Service, Reaction

github = Service.objects.get(name='github')

Reaction.objects.create(
    service=github,
    name='create_issue',
    description='Creates a new issue in a GitHub repository'
)
```

#### 2. Implémenter l'Exécuteur

Dans `backend/api/workflow_executor.py`, ajouter dans `_execute_github_reaction` :

```python
def _execute_github_reaction(self, area: Area, reaction_name: str):
    """Execute GitHub reactions"""
    user = area.user
    config = area.reaction_config or {}

    if reaction_name == "create_issue":
        github_service = get_github_service(user)
        if github_service:
            repo_owner = config.get('repo_owner')
            repo_name = config.get('repo_name')
            title = config.get('title', 'AREA Automated Issue')
            body = config.get('body', 'This issue was created automatically by AREA.')

            if not repo_owner or not repo_name:
                logger.warning(f"Missing repo config for AREA {area.id}")
                return

            # Use data from action if available
            if hasattr(area, 'temp_action_data'):
                action_data = area.temp_action_data
                # Replace placeholders
                title = title.replace('{{issue_title}}', action_data.get('issue_title', ''))
                body = body.replace('{{issue_body}}', action_data.get('issue_body', ''))

            issue = github_service.create_issue(repo_owner, repo_name, title, body)
            if issue:
                logger.info(f"GitHub issue created: {issue['html_url']}")
            else:
                logger.error(f"Failed to create GitHub issue")
        else:
            logger.warning(f"No GitHub service for user {user.username}")

    # Add more GitHub reactions here...
```

#### 3. Configuration JSON

```json
{
  "repo_owner": "myusername",
  "repo_name": "my-repo",
  "title": "Automated Issue",
  "body": "This was triggered by AREA"
}
```

## 🏗️ Architecture du Code

```
backend/api/
├── models.py              # Modèles de données (Service, Action, Reaction, Area)
├── tasks.py               # Tâches Celery (check_all_workflows, execute_reaction_task)
├── workflow_executor.py   # Logique principale (checkers + executors)
├── *_service.py          # Intégrations services (gmail_service, github_service, etc.)
├── main_views.py         # Views API principales
├── execution_views.py    # Views monitoring
└── urls.py               # Routes API
```

### Flow d'Exécution

```
1. Celery Beat déclenche check_all_workflows() toutes les 30s
   ↓
2. tasks.py → check_all_workflows()
   ↓
3. Pour chaque AREA : workflow_executor.check_action_triggered()
   ↓
4. Si triggered : tasks.py → execute_reaction_task.delay()
   ↓
5. workflow_executor.execute_reaction()
   ↓
6. Appel au service (*_service.py)
   ↓
7. Enregistrement dans AreaExecution
```

## 📏 Standards de Code

### Python (Backend)

```python
# Docstrings obligatoires
def my_function(param1, param2):
    """
    Brief description of the function
    
    Args:
        param1 (str): Description
        param2 (int): Description
        
    Returns:
        bool: Description
    """
    pass

# Logs appropriés
import logging
logger = logging.getLogger(__name__)

logger.info("Action completed successfully")
logger.warning("Potential issue detected")
logger.error("Operation failed", exc_info=True)

# Gestion d'erreurs
try:
    risky_operation()
except SpecificException as e:
    logger.error(f"Failed: {e}")
    return None
```

### TypeScript (Frontend)

```typescript
// Types explicites
interface ServiceConfig {
  name: string;
  description: string;
  actions: Action[];
  reactions: Reaction[];
}

// Async/await pour API calls
const fetchServices = async (): Promise<ServiceConfig[]> => {
  try {
    const response = await api.get('/services/');
    return response.data.services;
  } catch (error) {
    console.error('Failed to fetch services:', error);
    throw error;
  }
};
```

### Commits Git

```bash
# Format : <type>: <description>

feat: Add GitHub service integration
fix: Correct Timer action trigger logic
docs: Update API documentation
refactor: Simplify workflow executor
test: Add unit tests for GitHub service
```

Types : `feat`, `fix`, `docs`, `refactor`, `test`, `chore`

## 🧪 Tests

### Test Unitaire

```python
# backend/api/tests.py

from django.test import TestCase
from .models import Service, Action, Reaction
from .github_service import GitHubService

class GitHubServiceTestCase(TestCase):
    def setUp(self):
        self.service = Service.objects.create(
            name='github',
            description='GitHub'
        )
    
    def test_create_service(self):
        self.assertEqual(self.service.name, 'github')
    
    def test_action_creation(self):
        action = Action.objects.create(
            service=self.service,
            name='new_issue',
            description='New issue created'
        )
        self.assertEqual(action.service.name, 'github')
```

### Test d'Intégration

```bash
# Créer une AREA et vérifier qu'elle s'exécute
./test-hook-system.sh
```

## 📋 Checklist pour Nouvelle Feature

- [ ] Service créé en base de données
- [ ] Module *_service.py créé avec méthodes API
- [ ] Actions créées en base
- [ ] Checkers implémentés dans workflow_executor.py
- [ ] Réactions créées en base
- [ ] Executors implémentés dans workflow_executor.py
- [ ] OAuth2 ajouté si nécessaire
- [ ] Documentation ajoutée
- [ ] Tests écrits
- [ ] Exemple d'AREA créé

## 🔄 Workflow de Contribution

```bash
# 1. Fork le projet

# 2. Créer une branche
git checkout -b feature/github-service

# 3. Développer
# ... code ...

# 4. Tester
make hook-test

# 5. Commit
git add .
git commit -m "feat: Add GitHub service with issue tracking"

# 6. Push
git push origin feature/github-service

# 7. Créer Pull Request
```

## 📚 Ressources

- [Django REST Framework](https://www.django-rest-framework.org/)
- [Celery Documentation](https://docs.celeryq.dev/)
- [Docker Compose](https://docs.docker.com/compose/)
- [OAuth2 Flow](https://oauth.net/2/)

## 💡 Exemples Complets

Voir les services existants pour des exemples complets :
- [`backend/api/gmail_service.py`](backend/api/gmail_service.py)
- [`backend/api/calendar_service.py`](backend/api/calendar_service.py)
- [`backend/api/drive_service.py`](backend/api/drive_service.py)

## ❓ Questions

Pour toute question sur le développement :
- Consulter la documentation : [`documentation/`](documentation/)
- Voir les exemples existants
- Tester localement avec `make hook-test`

---

**Bon développement ! 🚀**
