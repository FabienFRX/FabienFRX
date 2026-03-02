# 📘 JEB Documentation

## 1. Introduction
- **Site Name:** JEB Incubator  
- **Main Objective:** Showcase and manage startups, events, and news for the JEB Incubator community.  
- **Target Audience:** Entrepreneurs, investors, partners, incubator staff, and the general public interested in innovation.  
- **Technologies Used:** React (frontend), Django (backend), SQLite (database), Docker, CSS Modules, TypeScript, REST API.  

---

## 2. General Architecture
- **Architecture Diagram:** (insert a diagram or image)  
- **Front-end:** React app in `frontend/`, using TypeScript and CSS for UI.  
- **Back-end:** Django REST API in `api/` and `survival/`.  
- **Database:** SQLite by default, can be changed in Django settings.  
- **API / Third-party Services:** REST endpoints for startups, events, news; optional social media links.  
- **Main Dependencies:** React, react-router-dom, Django, djangorestframework, Docker, Vite or CRA.  

---

## 3. User Guide
### 3.1 Navigation
- Main menu links: Home, Projects, Events, News, About, Login.  
- Footer (if present): Contact info, quick links.  

### 3.2 Key Features
- Browse and filter startups by sector, maturity, and status.  
- View upcoming News
- Can filter project by type 
- Responsive design for mobile and desktop.   

### 3.3 Step-by-step Procedures
- Create an account: Use the registration form.  
- Other common actions: Search, filter, view details, register for events.  

---

## 4. Administrator Guide
- **Access to the admin panel:** Go to `/admin/` in the backend, login with admin credentials.  
- **User management:** Add, edit, or remove users via the Django admin.  
- **Content management:** Manage startups, events, news, and categories in the admin panel.    

---

## 5. Developer Guide
### 5.1 Installation
- Clone the repository.  
- Run `make dev` to start frontend and backend.  
- Configure environment variables in `.env` files as needed.  

### 5.2 Code Structure
- `frontend/src/pages/`: Main pages (Home, Events, StartupCatalog, News, etc.)  
- `frontend/src/components/`: Reusable UI components (Card, Modal, Badge, etc.)  
- `api/`: Django app for API endpoints.  
- `survival/`: Django project settings and URLs.  

### 5.3 API
- `/api/startups/`: List and detail of startups.  
- `/api/events/`: List and detail of events.  
- `/api/news/`: List and detail of news articles.  
- Example request: `GET /api/startups/` returns a list of startups in JSON.  

### 5.4 Standards
- Code conventions: Use Lint for JS/TS, flake for Python.  
- Commit conventions: Use clear, descriptive commit messages.  
- Tests and QA tools: CI/CD pipelines 

---

## 6. Security and Compliance
- Role and permission management: Use Django's built-in user roles and permissions.  
- Security best practices: Use HTTP, validate user input, keep dependencies updated.  
- GDPR / legal compliance: Store user data securely, provide privacy policy, allow data deletion on request.  

---

## 7. Deployment and Maintenance
- Deployment procedure: Use Docker for staging/production.  
- Site and dependency updates: Use `pip` and `npm` to update dependencies.  
- Monitoring and log management: Use Django logging, frontend error boundaries, and external monitoring tools.  

---

## 8. Appendices
- **Glossary:** Startup, incubator, API, REST, CRUD, etc.  
- **Useful links:** [Django Docs](https://docs.djangoproject.com/), [React Docs](https://react.dev/), [Docker Docs](https://docs.docker.com/), [Vite Docs](https://vitejs.dev/).  
- **Version history (changelog):** (Add release notes and version changes here)  

---
