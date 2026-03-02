FROM python:3.11-slim

ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1

WORKDIR /app
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        postgresql-client \
        build-essential \
        libpq-dev \
    && rm -rf /var/lib/apt/lists/*
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY . .
RUN echo '#!/bin/bash\n\
set -e\n\
\n\
echo "Waiting for postgres..."\n\
while ! pg_isready -h $DB_HOST -p $DB_PORT -U $DB_USER; do\n\
    sleep 1\n\
done\n\
echo "PostgreSQL started"\n\
\n\
echo "Running migrations..."\n\
python manage.py migrate\n\
\n\
echo "Creating superuser if not exists..."\n\
python manage.py shell -c "from django.contrib.auth import get_user_model; User = get_user_model(); User.objects.filter(email='\''admin@area.com'\'').exists() or User.objects.create_superuser('\''admin@area.com'\'', '\''admin123'\'')" || true\n\
\n\
echo "Loading sample data..."\n\
python manage.py load_sample_data\n\
\n\
echo "Starting server..."\n\
python manage.py runserver 0.0.0.0:8080\n\
' > /entrypoint.sh && chmod +x /entrypoint.sh

EXPOSE 8080

ENTRYPOINT ["/entrypoint.sh"]