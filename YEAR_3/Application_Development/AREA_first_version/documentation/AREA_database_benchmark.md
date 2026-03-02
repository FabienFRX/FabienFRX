# AREA benchmark db

## Critères de l’AREA

- Cohérence forte des données (users, tokens, services OAuth, actions, réactions, logs)
- Gestion de nombreux états et relations (AREA = relations complexes)
- Beaucoup d’écritures et lectures simultanées (hooks, polling, triggers)
- Architecture modulaire et évolutive (ajout de services)
- Déploiement simple via Docker

---

## Qu’est-ce que PostgreSQL ?

PostgreSQL est un système de gestion de base de données relationnel open-source réputé pour sa robustesse, sa conformité ACID stricte et ses nombreuses fonctionnalités avancées.  
Il est conçu pour les applications nécessitant une forte cohérence, un modèle de données structuré et des transactions fiables.

---

## Pourquoi utiliser PostgreSQL ?

- Intégrité des données excellente grâce à une conformité ACID complète.
- Modélisation claire pour des relations complexes (idéal pour les AREA, services, utilisateurs…).
- Gestion robuste de la concurrence (lectures/écritures simultanées).
- Performances élevées sur les requêtes structurées.
- Écosystème : extensions, outils, ORM, supervision, clustering.
- 100 % open-source, aucune fonctionnalité payante.

---

## Qui utilise PostgreSQL ?

1. Instagram
2. Spotify
3. Reddit
4. Ubisoft
5. TripAdvisor

---

## Qu’est-ce que MongoDB ?

MongoDB est une base NoSQL orientée documents, permettant de stocker des données sous forme de documents JSON (BSON).  
Elle est conçue pour les données non structurées ou évolutives et pour les systèmes nécessitant une scalabilité horizontale.

---

## Pourquoi utiliser MongoDB ?

- Schéma flexible, idéal pour les données changeantes.
- Scalabilité horizontale native.
- Format JSON naturel pour les stacks JavaScript.
- Très performant dans des cas d’usage orientés logs ou grosses volumétries.

---

## Limites de MongoDB

- Consommation mémoire importante.
- Moins adapté aux modèles fortement relationnels (comme AREA).

---

## Qui utilise MongoDB ?

1. Netflix
2. Uber
3. Coinbase
4. eBay
5. Adobe


---

## Qu’est-ce que MariaDB ?

MariaDB est un système de gestion de base de données relationnel issu d’un fork de MySQL.  
Il conserve la simplicité de MySQL tout en apportant de meilleures performances et une licence totalement open-source.

---

## Pourquoi utiliser MariaDB ?

- Compatible MySQL mais avec plus de fonctionnalités libres.
- Très bon niveau de performance.
- Facile à déployer et administrer.
- Open-source sans limitation.

---

## Limites de MariaDB

- Moins riche que PostgreSQL en fonctionnalités avancées.
- ACID correct mais moins robuste pour les transactions complexes.
- Moins adapté aux structures de données très relationnelles.


---

## Qui utilise MariaDB ?

1. Wikipedia
2. Google (certains services internes)
3. Mozilla
4. Samsung

---

## Popularité

Allons sur Google trends pour voir la popularité en terme de recherches :

[google trend](assets/google-trend_postgres_vs_mongo_vs_maria.png)

---

# Comparaison : PostgreSQL vs MongoDB vs MariaDB

| Critère                        | PostgreSQL           | MongoDB                  | MariaDB             | Avantage   |
| ------------------------------ | -------------------- | ------------------------ | ------------------- | ---------- |
| Cohérence forte (ACID)         | Excellent            | Moyen                    | Bon                 | PostgreSQL |
| Relations complexes            | Très facile          | Non adapté               | Correct mais limité | PostgreSQL |
| Concurrence (lecture/écriture) | Très bonne           | Bonne                    | Bonne               | PostgreSQL |
| Flexibilité du schéma          | Rigidité SQL         | Très flexible            | Rigidité SQL        | MongoDB    |
| Performance globale            | Stable et prévisible | Très haute (si sharding) | Très rapide         | =          |
| Mise en production Docker      | Simple               | Simple                   | Simple              | =          |
| Robustesse long terme          | Excellent            | Moyen                    | Bon                 | PostgreSQL |


---

## Conclusion : quelle base choisir pour l'AREA ?

Objectivement, au regard des besoins du projet AREA :

- modèle fortement relationnel
- cohérence stricte
- nombreux états liés
- gestion de triggers / hooks / actions

**PostgreSQL est le meilleur choix parmi les trois.**

MongoDB est trop flexible et risque de créer de l’incohérence dans un système événementiel comme AREA. 
MariaDB est performant mais manque de certaines capacités avancées utiles dans un projet structuré.

**PostgreSQL réunit : robustesse, cohérence, performances, stabilité et puissance de modélisation.**  
C’est le choix recommandé, professionnel, et le plus aligné avec les attentes du projet.