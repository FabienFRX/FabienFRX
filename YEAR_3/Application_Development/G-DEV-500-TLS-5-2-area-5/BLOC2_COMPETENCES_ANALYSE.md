# Bloc 2 - Concevoir une architecture logicielle

## Analyse des compétences validables avec le code actuel

---

## Compétences FORTEMENT validables par le code actuel

### C9 - Architecture

> Sélectionner une hypothèse d'architecture et l'urbanisme de la solution logicielle ou web, afin de garantir l'intégration et la pérennité d'une solution, en prenant en compte le reste de l'écosystème technique présent au sein de l'entreprise ainsi que l'accessibilité numérique de la solution.

| Observable | Statut | Preuve dans le code |
|---|---|---|
| B2-C09.1 Architecture | OK | Architecture microservices claire : 7 services Docker Compose (backend Django, frontend React/Nginx, mobile Flutter, Celery Worker, Celery Beat, Redis, PostgreSQL). Communication REST API. |
| B2-C09.2 Intégration technique | OK | `docker-compose.yml` orchestre tout, health checks, volumes partagés (APK), depends_on avec conditions. |

---

### C10 - Implémentation de l'architecture

> Traduire les spécifications techniques et fonctionnelles en un système cohérent de composants logiciels en mobilisant son expertise et en s'appuyant sur l'état de l'art en termes d'architecture logicielle afin de produire une solution technique adaptée au besoin du client.

| Observable | Statut | Preuve dans le code |
|---|---|---|
| B2-C10.1 Implémentation de l'architecture | OK | Le code respecte l'architecture : Django REST API, React SPA, Flutter mobile, Celery async. Chaque service a son Dockerfile et sa logique isolée. |
| B2-C10.2 Bonnes pratiques de développement | OK | Design patterns : **Factory** (`google_services.py`), **Strategy** (switch action/reaction dans `workflow_executor.py`), **Observer** (Celery Beat), **MVC** (Django models/views/serializers), **Provider** (Flutter), **Context API** (React). Clean separation of concerns. |

---

### C11 - Segmentation

> Segmenter chaque problème complexe en un ensemble de sous-problèmes afin d'obtenir des tâches atomiques dans un objectif de performance, d'adaptabilité et de maintenabilité en fonction des besoins du client.

| Observable | Statut | Preuve dans le code |
|---|---|---|
| B2-C11.1 Organisation du code | OK | Structure claire : `backend/api/` (models, views, services, tasks), `web/src/` (pages, components, contexts, services), `mobile/lib/` (screens, providers, models, widgets). |
| B2-C11.2 Segmentation du code | OK | Vues séparées par responsabilité (`auth_views.py`, `main_views.py`, `execution_views.py`), un service par intégration (gmail_service, github_service, gitlab_service, linkedin_service, dropbox_service). Tâches async isolées dans `tasks.py`. |

---

### C13 - Persistance des données

> Sélectionner les solutions de persistance de données (fichier texte ou binaire, format de fichier structuré, base de donnée...) en s'appuyant sur son expertise et celle de l'équipe projet et en mobilisant l'état de l'art afin de s'adapter aux contraintes techniques, fonctionnelles et de sécurité de l'application en terme de stockage de données.

| Observable | Statut | Preuve dans le code |
|---|---|---|
| B2-C13.1 Persistance des données | OK | **PostgreSQL** pour les données relationnelles (User, Service, Area, AreaExecution), **Redis** pour le broker de messages Celery + cache, **JSONField** pour les configs flexibles action/reaction. Visible dans `models.py` et `settings.py`. |
| B2-C13.2 Persistance des données - comparatif | ~Dossier | Le code montre les choix, mais il faut **rédiger la justification** (pourquoi PostgreSQL vs MongoDB, pourquoi Redis vs RabbitMQ). La doc `AREA_data_model.md` aide. |

---

### C14 - Structures de données

> Sélectionner les structures de données répondant aux contraintes de l'application en tenant compte de leur complexité algorithmique et spatiale (tableaux, listes, sets, tables de hashage...) dans un objectif de performance, de maintenabilité et d'évolutivité de l'application.

| Observable | Statut | Preuve dans le code |
|---|---|---|
| B2-C14.1 Structures de données | OK | Modèles ORM bien conçus : relations FK (Service -> Action/Reaction, User -> Area -> AreaExecution), JSONField pour la config flexible, index auto via Django. `models.py` |
| B2-C14.2 Structures de données - justifications | ~Oral | Il faut pouvoir **argumenter les choix** : pourquoi JSONField plutôt qu'un modèle séparé pour les configs, pourquoi les FK et pas du NoSQL, etc. Le code fournit la matière. |

---

## Compétences validables AVEC travail de dossier modéré

### C8 - Prototypage

> Présenter une solution technique créative, en collaboration avec l'équipe projet et ses différentes expertises, en prenant en considération les différentes contraintes apportées par le client (économique, RSE, ...) ou imposées par l'environnement technique dans le but de résoudre la problématique exposée.

| Observable | Statut | Preuve dans le code |
|---|---|---|
| B2-C08.1 Prototypes | ~OK | Le projet est littéralement un POC (`area_poc` dans le nom Django). L'existence du Makefile, du Docker Compose, de l'entrypoint automatisé montre une démarche itérative. |
| B2-C08.2 Prototypes - comparatif | ~Dossier | Il faut rédiger les avantages/inconvénients des choix de prototypage (polling vs webhooks, Celery vs cron jobs, etc.). |

---

### C12 - Solutions algorithmiques

> Identifier des solutions existantes ou originales afin de répondre à chaque problème posé en tenant compte des contraintes de performance et de scalabilité de la solution et de son environnement d'exécution.

| Observable | Statut | Preuve dans le code |
|---|---|---|
| B2-C12.1 Implémentation algorithmique | OK | `workflow_executor.py` (1474 lignes) : algorithme de polling périodique, détection d'actions par comparaison d'état, gestion d'expiration de tokens, scheduling Celery Beat. |
| B2-C12.2 Implémentation d'algorithmes originaux | ~OK | Le workflow executor implémente une logique originale de chainage action->reaction multi-services (Gmail, Calendar, GitHub, GitLab, Dropbox, LinkedIn). Pas d'algorithme ad-hoc existant pour ce cas précis. Il faut bien **le mettre en valeur** dans le dossier. |

---

## Compétences DIFFICILES à valider avec le code seul

### C6 - Veille technologique

> Mettre en place une veille légale et réglementaire prenant en compte les besoins des PSH en menant des recherches fréquentes sur les usages liés aux technologies, en recensant les aspects légaux et réglementaires parus et en participant régulièrement aux rencontres de la communauté professionnelle afin de minimiser la dette technique pour les aspects couverts par le projet.

| Observable | Statut | Ce qui manque |
|---|---|---|
| B2-C06.1 Etude comparative | ~Partiel | Le doc `BACKEND_BENCHMARK.md` existe mais il faut une **vraie étude comparative formelle** (Django vs Express vs Spring, PostgreSQL vs MongoDB, React vs Vue, etc.). |
| B2-C06.2 Identification des technologies | ~Partiel | Les technologies sont identifiables dans le code (`requirements.txt`, `package.json`, `pubspec.yaml`) mais il faut les **formaliser dans un document**. |

---

### C7 - Sécurité

> Réviser régulièrement les protocoles existants, notamment au regard des nouvelles failles de sécurité identifiées afin de contribuer à l'utilisation de standards technologiques élevés au sein de l'entreprise, en impulsant une application régulière des nouveaux usages et outils à l'entreprise.

| Observable | Statut | Ce qui manque |
|---|---|---|
| B2-C07.1 Etude sécurité informatique | Absent | Besoin d'une **étude des CVE récentes** des technologies utilisées (Django, React, PostgreSQL, Redis). Rien dans le code actuel. |
| B2-C07.2 Veille sécurité informatique | Absent | Besoin de démontrer une **connaissance de l'actualité sécu** (OWASP, failles récentes). Le code a OAuth2 et token auth, mais pas d'audit formel. Les tokens OAuth sont stockés en clair dans la DB, ce qui est un point faible identifiable. |

---

## Tableau récapitulatif

| Compétence | Validable par le code ? | Effort dossier restant |
|---|---|---|
| **C9** - Architecture | **OUI** | Faible (schématiser l'existant) |
| **C10** - Implémentation | **OUI** | Faible (lister les patterns utilisés) |
| **C11** - Segmentation | **OUI** | Faible (documenter l'organisation) |
| **C13** - Persistance | **OUI** | Modéré (rédiger le comparatif) |
| **C14** - Structures de données | **OUI** | Modéré (justifier les choix) |
| **C12** - Algorithmes | **OUI** | Modéré (mettre en valeur le workflow executor) |
| **C8** - Prototypage | **OUI** | Modéré (documenter la démarche POC) |
| **C6** - Veille techno | **PARTIEL** | Important (rédiger l'étude comparative) |
| **C7** - Sécurité | **NON** | Très important (étude CVE + veille sécu) |

---

## Conclusion

**Maximum validable avec le code actuel : 7 compétences sur 9** (C8, C9, C10, C11, C12, C13, C14), en les accompagnant d'un dossier qui met en valeur ce qui est déjà dans le code.

- **C6** est récupérable avec un effort modéré de rédaction (le code montre les choix technologiques, il suffit de formaliser le comparatif).
- **C7 est la compétence la plus difficile** à valider : le code a des éléments de sécurité (OAuth2, token auth) mais il n'y a aucune étude de sécurité, et les tokens sont stockés en clair. Il faudrait un vrai travail de veille sécurité externe au code.

En "maxant" avec un bon dossier qui exploite le code existant, **8/9 est atteignable**. C7 nécessite un vrai travail de recherche supplémentaire.
