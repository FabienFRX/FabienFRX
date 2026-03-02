FROM python:3.12-slim

ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1

WORKDIR /app

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        postgresql-client \
        build-essential \
        libpq-dev \
        libcairo2-dev \
        libpango1.0-dev \
        libgdk-pixbuf-xlib-2.0-dev \
        libffi-dev \
        shared-mime-info \
        libxml2-dev \
        libxslt1-dev \
        libjpeg-dev \
        libpng-dev \
        zlib1g-dev \
        fonts-liberation \
        fontconfig \
    && rm -rf /var/lib/apt/lists/*

COPY requirements.txt /app/
RUN pip install --no-cache-dir -r requirements.txt

COPY . /app/

RUN adduser --disabled-password --gecos '' appuser \
    && chown -R appuser:appuser /app
USER appuser

EXPOSE 8000

CMD ["python", "manage.py", "runserver", "0.0.0.0:8000"]
