FROM node:18-alpine AS build
WORKDIR /app

COPY package*.json ./
RUN npm ci || npm install

COPY . .
RUN npm run build

# Runtime: NGINX serving static files
FROM nginx:stable-alpine

# Copy custom NGINX config
COPY nginx.conf /etc/nginx/conf.d/default.conf

# Copy built assets
COPY --from=build /app/dist /usr/share/nginx/html

# Create directory for APK (will be mounted from volume)
RUN mkdir -p /usr/share/nginx/html/apk

EXPOSE 80

CMD ["nginx", "-g", "daemon off;"]