#!/bin/bash

# Wait for database to be ready
echo "Waiting for database..."
while ! nc -z database 5432; do
  sleep 0.5
done
echo "Database is ready!"

# Wait for Redis to be ready
echo "Waiting for Redis..."
while ! nc -z redis 6379; do
  sleep 0.5
done
echo "Redis is ready!"

# Run migrations
echo "Running migrations..."
python manage.py makemigrations
python manage.py migrate

# Create superuser if not exists
echo "Creating superuser..."
python manage.py shell << END
from django.contrib.auth.models import User
if not User.objects.filter(username='admin').exists():
    User.objects.create_superuser('admin', 'admin@area.com', 'admin')
    print('Superuser created: admin/admin')
else:
    print('Superuser already exists')
END

# Setup initial data
echo "Setting up initial data..."
python setup_mvp_data.py || echo "Initial data setup completed or already exists"
echo "Setting up Dropbox service..."
python setup_dropbox_service.py
echo "Setting up LinkedIn service..."
python setup_linkedin_service.py

# Start server
echo "Starting Django server..."
python manage.py runserver 0.0.0.0:8080
