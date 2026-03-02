# AREA benchmark frontend web

## Critère de l'AREA

- Un web client léger, réactif, simple à maintenir
- Une architecture modulaire + beaucoup d’écran / états UI
- Un écosystème très riche (état / fast UI / drag-drop)
- Temps de développement limité
- Partie mobile

## Qu’est-ce que React ?

**React** est une bibliothèque JavaScript open source créée par Facebook. Elle sert à construire des **interfaces utilisateur** pour des applications web ou mobiles, en s’appuyant sur des **composants indépendants**.

React utilise une **liaison de données unidirectionnelle** (du parent vers l’enfant) et s’appuie sur un **Virtual DOM**. Cela évite de recharger toute la page : seules les parties modifiées sont mises à jour, ce qui rend les applications plus rapides et réactives.

## Pourquoi utiliser React ?

- **Facile à apprendre** : prise en main rapide, idéal pour débuter et avancer vite.
- **Performance** : rendu rapide grâce au Virtual DOM → temps de chargement réduits et SEO amélioré.
- **Code simplifié** : partage de logique entre client et serveur ; tests facilités.
- **Écosystème flexible** : on peut l’associer à d’autres bibliothèques selon les besoins.
- **Soutenu par Facebook** : forte communauté, maintenance continue, pérennité assurée.

## Qui utilise Angular ?

React est extrêmement populaire et largement adopté par l’industrie. Parmi ceux qui l'utilisent :

1. Facebook
2. Instagram
3. WhatsApp
4. Netflix
5. Yahoo
6. New York Times
7. Discord
8. Dropbox
9. Ubereats
10. Atlassian

________________________________________________________________________

## Qu’est-ce qu’Angular ?

**Angular** est un framework web open source développé par Google, basé sur **TypeScript**. Il sert principalement à créer des **applications web et mobiles** de type **SPA** (Single Page Application) en combinant HTML et TypeScript.

Il se distingue notamment par sa **liaison de données bidirectionnelle**, qui synchronise automatiquement le modèle et la vue, rendant l’interface réactive et cohérente sans effort supplémentaire.

## Pourquoi utiliser Angular ?

- **Écosystème complet** : templating, liaison bidirectionnelle, gestion des appels REST/Ajax, injection de dépendances, modules… tout est intégré.
- **Soutien de Google** : framework maintenu et utilisé en interne (grosse garantie de pérennité).
- **Documentation riche** : facile d’apprentissage et bien centralisée.
- **Moins de code, mieux structuré** : grâce à TypeScript, au typage, et à son organisation en modules.

Angular permet ainsi un développement plus propre, structuré, et efficace.

## Qui utilise Angular ?

Plus de **500 grandes entreprises** s’appuient sur Angular, dont :

1. Google
2. Microsoft
3. IBM
4. PayPal
5. Upwork
6. Deutsche Bank
7. Samsung
8. Forbes
9. The Guardian
10. Rockstar Games

________________________________________________________________________

## Qu’est-ce que Vue.js ?

**Vue.js** est un framework JavaScript progressif utilisé pour créer des interfaces utilisateur modernes. Il est léger, flexible, simple à prendre en main, et peut être intégré progressivement dans un projet sans tout réécrire. 

## Pourquoi utiliser Vue.js ?

- **Très facile à apprendre** : syntaxe claire, logique intuitive, idéale pour monter en compétence rapidement.
- **Flexible et progressif** : on peut l’utiliser pour une petite fonctionnalité ou une application complète.
- **Performance élevée** : grâce au **Virtual DOM** et à son système réactif très efficace.
- **Documentation excellente** : claire, complète, pédagogique (un des gros atouts de Vue).
- **Écosystème solide** : Vue Router, Pinia/Vuex, Nuxt… pour bâtir des applications avancées

## Qui utilise Vue.js ?

De grandes entreprises et plateformes ont adopté Vue pour sa légèreté et sa productivité :

1. GitLab
2. Adobe
3. Behance
4. Font Awesome
5. Grammarly
6. Laravel Spark
7. Habitica
8. Teleo

________________________________________________________________________

## Comparaison :

### **React vs Vue.js

| **Besoin AREA**              | **React**               | **Vue.js**              | **Avantage**                                                     |
| ---------------------------- | ----------------------- | ----------------------- | ---------------------------------------------------------------- |
| **Gestion état complexe**    | Context API + Redux     | Vuex/Pinia              | **=**                                                            |
| **Intégration API**          | Axios + hooks           | Axios + Composition API | **=**                                                            |
| **UI dynamique (drag-drop)** | ==React Beautiful Dnd== | Vue Draggable           | **React** plus optimisé car support +100 element                 |
| **Écosystème OAuth2**        | ==15+ librairies==      | 5+ librairies           | **React** s'adapte donc plus aux potentiels différents services  |
| **Mobile**                   | ==React Native==        | NativeScript            | **React** étant utilisable pour la partie mobile (gain de temps) |

### **React vs Abgular

| **Critère**                | **React**              | **Angular**             | **Impact**               |
| -------------------------- | ---------------------- | ----------------------- | ------------------------ |
| **Courbe d'apprentissage** | ==Rapide à apprendre== | Complexe pour commencer | Facile à prendre en main |
| **Bundle size**            | ==~40KB==              | ~500KB                  | meilleure perf           |
| **Hot reload**             | ==Instantané==         | 2-3 secondes            | dev plus rapide          |

________________________________________________________________________

## Popularité :

Allons sur Google trends pour voir la popularité en terme de recherches :

[google-trend](assets/google-trend_react_vs_angular_vs_vue.png)

Passons à Github qui est l’endroit où la plupart des projets open source mettent leur code et où les développeurs trouvent les informations sur les frameworks :

[github](assets/github_comparaison.png)

A noter qu'une forte popularité veut dire plus d'exemple en ligne et d'aide dispo.

________________________________________________________________________

## + vs -

### **Angular**

**Plus :**
- Très utilisé par les moyennes et grandes entreprises
- Haute productivité
- Idéal pour les projets complexes et les grosses équipes
- Performant
- Excellent support de Google et d’une grande communauté

**Moins :**
- Long et difficile à apprendre
- Peu flexible
- Pas adapté aux petits projets

### **React**

**Plus :**
- Facile à apprendre
- Le framework le plus populaire du marché
- Très performant
- Fort soutien de Facebook et de la communauté

**Moins :**
- Pas de router ni de gestion d’état officiels
- Demande une architecture solide

### **Vue.js**

**Plus :**
- Très simple à prendre en main
- Flexible et complet
- Idéal pour POC, petites et moyennes applications

**Moins :**
- Pas de soutien d’un grand groupe (pas de GAFAM)
- Un seul contributeur full-time sur le core


________________________________________________________________________

## Pour conclure

React est de loin le choix le plus efficace, performant, le plus flexible et le plus réaliste pour répondre aux attentes de l'AREA et de notre groupe.
