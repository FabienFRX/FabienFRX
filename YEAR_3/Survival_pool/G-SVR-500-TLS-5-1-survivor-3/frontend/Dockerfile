# Utiliser Node.js 18 comme image de base
FROM node:18-alpine

# Définir le répertoire de travail
WORKDIR /app

# Copier les fichiers de dépendances
COPY package*.json ./

# Installer les dépendances
RUN npm install

# Copier le code source
COPY . .

# Exposer le port
EXPOSE 5173

# Commande par défaut
CMD ["npm", "run", "dev", "--", "--host", "0.0.0.0"] 