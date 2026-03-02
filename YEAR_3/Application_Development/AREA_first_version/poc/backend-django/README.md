# Django POC for AREA Project

## Overview
This POC demonstrates Django's capabilities for the AREA project with Django REST Framework, CORS support, and the required `/about.json` endpoint.

## Features Demonstrated
- ✅ REST API with Django REST Framework
- ✅ CORS configuration for web client integration
- ✅ Database models for AREA core entities
- ✅ Required `/about.json` endpoint
- ✅ Clean project structure and rapid development

## Models
- **Service**: External services (Gmail, Twitter, etc.)
- **Action**: Triggers for automations
- **Reaction**: Responses to triggers
- **Area**: User-created automation rules

## API Endpoints
- `GET /hello/` - Test endpoint showing Django features
- `GET /about.json` - Required project endpoint with client/server info

## Quick Start
```bash
cd poc/backend-django
source venv/bin/activate
python manage.py runserver 8080
```

## Why Django for AREA
- Built-in security (CSRF, XSS protection)
- Rapid development with batteries-included approach
- Django REST Framework for robust APIs
- Proven scalability (Instagram, Pinterest)
- Strong community and documentation