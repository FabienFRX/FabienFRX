# AREA Liste des Services

## Services disponibles

### 1. Gmail

**Description** : Service de messagerie électronique de Google.

#### Actions :

- `new_email` : Un nouvel email est reçu.
- `new_email_with_attachment` : Un nouvel email contenant une pièce jointe est reçu.

#### Réactions :

- `send_email` : Envoyer un email à un destinataire.
- `mark_as_read` : Marquer un email comme lu.

---

### 2. Google Drive

**Description** : Service de stockage et de partage de fichiers.

#### Actions :

- `new_file_in_folder` : Un nouveau fichier est ajouté dans un dossier.

#### Réactions :

- `upload_file` : Téléverser un fichier dans un dossier.
- `create_folder` : Créer un nouveau dossier.

---

### 3. Twitter

**Description** : Réseau social.

#### Actions :

- `new_mention` : L'utilisateur est mentionné dans un tweet.

#### Réactions :

- `tweet` : Poster un tweet.

---

### 4. GitHub

**Description** : Plateforme de gestion de code et de collaboration.

#### Actions :

- `new_issue` : Une nouvelle issue est créée sur un dépôt.
- `new_push` : Un nouveau push est effectué sur une branche.


---

### 5. Timer

**Description** : Service interne de déclenchement temporel.

#### Actions :

- `daily_at` : Déclenchement quotidien à une heure spécifique.
- `every_hour` : Déclenchement toutes les heures.

---

### 6. Reddit

**Description** : Plateforme de discussion et de partage de contenu organisée en communautés (subreddits).

#### Actions :

- `post_reaches_score` : Un post atteint un certain nombre de upvotes
- `new_post_in_subreddit` : Un nouveau post est publié dans un subreddit spécifique

#### Réactions :

- `create_post` : Créer un nouveau post dans un subreddit


---


## AREA 1 : Sauvegarde automatique de pièces jointes

**Action** : `new_email_with_attachment` (Gmail)  
**Réaction** : `upload_file` (Google Drive)

**Description** : Lorsqu'un email contenant une pièce jointe est reçu dans la boîte Gmail, la pièce jointe est automatiquement sauvegardée dans un dossier spécifique de Google Drive.

**Configuration** :

- Dossier source : Boîte de réception Gmail
- Dossier destination : "Sauvegardes/Emails" dans Google Drive
- Format des fichiers conservé

---

## AREA 2 : Rappel quotidien de tâches

**Action** : `daily_at` (Timer) - 09:00  
**Réaction** : `send_email` (Gmail)

**Description** : Chaque jour à 9h00, un email de rappel est automatiquement envoyé avec la liste des tâches de la journée.

**Configuration** :

- Heure de déclenchement : 09:00
- Destinataire : L'utilisateur lui-même
- Objet : "Rappel Tâches Quotidiennes"
- Corps du message prédéfini avec template de tâches


---

## AREA 3 : Tweet toute les heures

**Action** : `every_hour` (Timer)  
**Réaction** : `tweet` (Twitter)

**Description** : Toutes les heures, un tweet est automatiquement publié avec l'heure actuelle et un message personnalisé.

**Configuration** :

- Fréquence : Déclenchement toutes les heures
- Message : "Il est actuellement [Heure]"
- Format de l'heure : HH:00 (ex: "14:00", "15:00")
- Ajout automatique de l'heure courante dans le tweet

---

## Scénarios d'Utilisation Avancés

### Pour les Développeurs

- Surveillance continue des dépôts GitHub
- Automatisation des communications de projet
- Sauvegarde automatique des documents techniques

### Pour la Productivité

- Rappels horaires et quotidiens
- Organisation automatique des fichiers
- Gestion des priorités par email
