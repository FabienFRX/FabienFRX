# AREA API Documentation

## Overview
This document describes the REST API endpoints for the AREA project application server. All endpoints return JSON responses and support CORS for web client integration.

## Base URL
```
http://localhost:8080
```

## Authentication
The API will support OAuth2 authentication for external services and JWT tokens for user session management. Currently, POC endpoints are open for testing.

---

## Core Endpoints

### About Endpoint (Required)
Provides server information and available services.

```http
GET /about.json
```

**Response:**
```json
{
  "client": {
    "host": "127.0.0.1"
  },
  "server": {
    "current_time": 1764153580,
    "services": [
      {
        "name": "gmail",
        "actions": [
          {
            "name": "new_email",
            "description": "A new email is received"
          }
        ],
        "reactions": [
          {
            "name": "send_email",
            "description": "Send an email to a recipient"
          }
        ]
      }
    ]
  }
}
```

---

## User Management

### User Registration
```http
POST /api/auth/register
```

**Request Body:**
```json
{
  "email": "user@example.com",
  "password": "securePassword123",
  "first_name": "John",
  "last_name": "Doe"
}
```

### User Authentication
```http
POST /api/auth/login
```

**Request Body:**
```json
{
  "email": "user@example.com",
  "password": "securePassword123"
}
```

**Response:**
```json
{
  "access_token": "jwt_token_here",
  "refresh_token": "refresh_token_here",
  "user": {
    "id": 1,
    "email": "user@example.com",
    "first_name": "John",
    "last_name": "Doe"
  }
}
```

---

## Services Management

### List Available Services
```http
GET /api/services
```

**Response:**
```json
[
  {
    "id": 1,
    "name": "gmail",
    "description": "Google Gmail service",
    "oauth_required": true,
    "actions": [...],
    "reactions": [...]
  }
]
```

### Subscribe to Service
```http
POST /api/services/{service_id}/subscribe
```

**Headers:**
```
Authorization: Bearer {jwt_token}
```

**Request Body:**
```json
{
  "oauth_token": "oauth_access_token",
  "oauth_refresh_token": "oauth_refresh_token"
}
```

---

## AREA Management

### Create AREA
```http
POST /api/areas
```

**Headers:**
```
Authorization: Bearer {jwt_token}
```

**Request Body:**
```json
{
  "name": "Email to Drive",
  "action": {
    "service_id": 1,
    "action_name": "new_email_with_attachment",
    "parameters": {
      "sender": "important@company.com"
    }
  },
  "reaction": {
    "service_id": 2,
    "reaction_name": "upload_file",
    "parameters": {
      "folder": "Email Attachments"
    }
  }
}
```

### List User AREAs
```http
GET /api/areas
```

**Headers:**
```
Authorization: Bearer {jwt_token}
```

**Response:**
```json
[
  {
    "id": 1,
    "name": "Email to Drive",
    "enabled": true,
    "created_at": "2024-01-15T10:30:00Z",
    "action": {
      "service": "gmail",
      "action": "new_email_with_attachment",
      "parameters": {...}
    },
    "reaction": {
      "service": "google_drive",
      "reaction": "upload_file",
      "parameters": {...}
    }
  }
]
```

### Update AREA
```http
PUT /api/areas/{area_id}
```

### Delete AREA
```http
DELETE /api/areas/{area_id}
```

---

## Webhook Endpoints

### Service Webhook Handler
```http
POST /api/webhooks/{service_name}
```

Receives webhook notifications from external services to trigger AREAs.

---

## Error Responses

### 400 Bad Request
```json
{
  "error": "Bad Request",
  "message": "Invalid input parameters",
  "details": {
    "field": "email",
    "error": "Invalid email format"
  }
}
```

### 401 Unauthorized
```json
{
  "error": "Unauthorized",
  "message": "Invalid or expired token"
}
```

### 404 Not Found
```json
{
  "error": "Not Found",
  "message": "Resource not found"
}
```

### 500 Internal Server Error
```json
{
  "error": "Internal Server Error",
  "message": "An unexpected error occurred"
}
```

---

## Rate Limiting
- 100 requests per minute per user for authenticated endpoints
- 10 requests per minute for unauthenticated endpoints
- Webhook endpoints have higher limits (1000 req/min)

## CORS Policy
- Allows all origins in development
- Production will restrict to specific domains
- Supports all standard HTTP methods
- Allows standard headers plus Authorization