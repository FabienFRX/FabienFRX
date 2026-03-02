# Build stage
FROM ghcr.io/cirruslabs/flutter:3.16.0 AS build

WORKDIR /app

# Copy files
COPY . .

# Get dependencies
RUN flutter pub get

# Build APK
RUN flutter build apk --release

# Serve stage
FROM nginx:alpine

# Copy the built APK to the nginx html directory
COPY --from=build /app/build/app/outputs/flutter-apk/app-release.apk /usr/share/nginx/html/client.apk

# Expose port 80
EXPOSE 80

# Start Nginx
CMD ["nginx", "-g", "daemon off;"]
