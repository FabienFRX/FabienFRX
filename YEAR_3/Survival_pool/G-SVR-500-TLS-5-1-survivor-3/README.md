# JEB Incubator Platform

```ascii
     ____._____________________     .___                   ___.           __                
    |    |\_   _____/\______   \    |   | ____   ____  __ _\_ |__ _____ _/  |_  ___________ 
    |    | |    __)_  |    |  _/    |   |/    \_/ ___\|  |  \ __ \\__  \\   __\/  _ \_  __ \
/\__|    | |        \ |    |   \    |   |   |  \  \___|  |  / \_\ \/ __ \|  | (  <_> )  | \/
\________|/_______  / |______  /    |___|___|  /\___  >____/|___  (____  /__|  \____/|__|   
                  \/         \/              \/     \/          \/     \/                   
```

A comprehensive web platform for managing the JEB incubation ecosystem. This Django-based project enables tracking of startups, events, and news within the incubator environment.

## 🚀 Features

- **Startup Management**: Track incubated companies with their information, status, and requirements
- **Event Organization**: Manage and organize incubator events
- **News & Updates**: Publish and track industry news and updates
- **Internal Messaging**: Built-in communication system
- **JEB API Integration**: Automatic synchronization with the official JEB API

## 🔧 Quick Start

Get up and running in minutes using our comprehensive Makefile:

```bash
# Clone the project
git clone <repo-url>
cd G-SVR-500-TLS-5-1-survivor-3

# Start everything at once
make quick-start
```

That's it! The application will be available at http://localhost:8000

## 📋 Available Commands

We've included a Makefile with all frequently used commands:

```bash
make help          # Show all available commands
make dev           # Start development environment
make logs          # View container logs
make shell         # Open shell in backend container
make migrate       # Apply database migrations
make superuser     # Create admin user
make clean         # Clean up containers and volumes
```

## 📁 Project Structure

```
├── api/                           # Main Django application
│   ├── models.py                  # Database models (Startup, Event, News, Message)
│   ├── views.py                   # REST API views
│   ├── serializers.py             # DRF serializers
│   ├── urls.py                    # API URL routing
│   ├── admin.py                   # Django admin configuration
│   ├── auth_views.py              # Authentication views
│   ├── services.py                # Business logic services
│   ├── pdf_service.py             # PDF generation service
│   ├── management/                # Custom management commands
│   ├── migrations/                # Database migrations
│   ├── templates/                 # HTML templates
│   └── static/                    # Static files
├── survival/                      # Django project configuration
│   ├── settings.py                # Django settings
│   ├── urls.py                    # Main URL configuration
│   ├── wsgi.py                    # WSGI configuration
│   └── asgi.py                    # ASGI configuration
├── frontend/                      # React TypeScript application
│   ├── src/                       # Source code
│   │   ├── components/            # React components
│   │   ├── pages/                 # Page components
│   │   ├── services/              # API services
│   │   ├── types/                 # TypeScript type definitions
│   │   ├── hooks/                 # Custom React hooks
│   │   └── utils/                 # Utility functions
│   ├── public/                    # Public assets
│   ├── package.json               # Node.js dependencies
│   ├── tsconfig.json              # TypeScript configuration
│   └── vite.config.ts             # Vite build configuration
├── Documents/                     # Project documentation
├── docker-compose.yml             # Docker services configuration
├── Dockerfile                     # Backend Docker image
├── Dockerfile.nginx               # Nginx Docker image
├── nginx.conf                     # Nginx configuration
├── requirements.txt               # Python dependencies
├── Makefile                       # Development commands
├── manage.py                      # Django management script
└── db.sqlite3                     # SQLite database
```

## 💾 Database

We use SQLite for simplified development. The database is located at `survival/db.sqlite3`.

To create a backup:
```bash
make backup
```

## 🔗 API Endpoints

The REST API is available at `/api/` using Django REST Framework:

- `/api/startups/` - Startup listings
- `/api/events/` - Event listings
- `/api/news/` - News articles
- `/api/messages/` - Internal messaging

## 🔄 JEB API Synchronization

Synchronize data with the official JEB API:

```bash
make sync-data
```

This automatically fetches startups, events, and news from the official API.

## 🛠️ Development

To add new features:

1. Modify models in `api/models.py`
2. Create migrations: `make makemigrations`
3. Apply migrations: `make migrate`
4. Add views in `api/views.py`
5. Test your changes: `make test`

### Development Commands

```bash
make dev           # Start complete development environment
make dev-backend   # Start backend only
make dev-frontend  # Start frontend in local development mode
make build         # Build Docker images
make up            # Start containers
make down          # Stop containers
make logs          # View logs
make shell         # Open shell in backend container
```

## 🐳 Docker

Everything runs in Docker to avoid dependency issues:

```bash
make build    # Build Docker images
make up       # Start services
make down     # Stop services
```

## ⚙️ Environment Variables

Create a `.env` file with:

```env
JEB_API_TOKEN=your_token_here
JEB_API_BASE_URL=https://api.jeb-incubator.com
DEBUG=1
```

## 🧪 Testing

Run the test suite:

```bash
make test         # Run all tests
make lint         # Check code quality with flake8
```

## 📝 Known Issues

- JEB API synchronization can be slow depending on connection speed
- SQLite may become slow with large datasets (adequate for development)

## 🚧 TODO

- [ ] Enhanced frontend interface
- [ ] Complete unit test coverage
- [ ] Notification system
- [ ] Messaging

## 📜 License

This project is part of the EPITECH curriculum.

## 👥 Authors

- **Fabien Fraixanet** - *Frontend & Documentation* - [Github](https://github.com/FabienFRX)
- **Andi Berard** - *Frontend* - [Github](https://github.com/Andiberard)
- **Gobijan Nathakrishnan** - *CI/CD & Backend* - [Github](https://github.com/GobyNathan)
- **Marceau Kersuzan** - *Backend & Database* - [Github](https://github.com/Andiberard)



---
