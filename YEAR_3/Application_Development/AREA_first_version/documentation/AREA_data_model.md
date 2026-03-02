# AREA Data Model Documentation

## Overview
This document describes the database schema and data models for the AREA project. The design supports user management, service integration, automation rules (AREAs), and audit logging.

## Database Choice
**Recommended**: PostgreSQL for production (reliability, ACID compliance, JSON support)
**Alternative**: SQLite for development (Django POC uses SQLite by default)

---

## Core Entities

### User
Stores user account information and authentication data.

```sql
CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    email VARCHAR(255) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    first_name VARCHAR(100) NOT NULL,
    last_name VARCHAR(100) NOT NULL,
    is_active BOOLEAN DEFAULT true,
    is_verified BOOLEAN DEFAULT false,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

**Relationships:**
- One-to-Many with Areas
- One-to-Many with UserServiceSubscriptions

---

### Service
Represents external services that can be integrated (Gmail, Twitter, etc.).

```sql
CREATE TABLE services (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) UNIQUE NOT NULL,
    display_name VARCHAR(100) NOT NULL,
    description TEXT,
    oauth_required BOOLEAN DEFAULT true,
    webhook_url VARCHAR(255),
    icon_url VARCHAR(255),
    is_active BOOLEAN DEFAULT true,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

**Example Data:**
```sql
INSERT INTO services (name, display_name, description, oauth_required) VALUES
('gmail', 'Gmail', 'Google Gmail email service', true),
('twitter', 'Twitter', 'Twitter social media platform', true),
('github', 'GitHub', 'GitHub code repository platform', true),
('timer', 'Timer', 'Internal timer service for scheduled actions', false);
```

---

### Action
Defines triggers that can start an automation.

```sql
CREATE TABLE actions (
    id SERIAL PRIMARY KEY,
    service_id INTEGER REFERENCES services(id) ON DELETE CASCADE,
    name VARCHAR(100) NOT NULL,
    display_name VARCHAR(100) NOT NULL,
    description TEXT NOT NULL,
    parameters_schema JSONB,
    webhook_event VARCHAR(100),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(service_id, name)
);
```

**Example Data:**
```sql
INSERT INTO actions (service_id, name, display_name, description, parameters_schema) VALUES
(1, 'new_email', 'New Email Received', 'Triggers when a new email is received',
 '{"properties": {"from": {"type": "string"}, "subject_contains": {"type": "string"}}}'),
(2, 'new_mention', 'New Mention', 'Triggers when user is mentioned in a tweet',
 '{"properties": {"hashtag": {"type": "string"}}}');
```

---

### Reaction
Defines actions that can be executed in response to triggers.

```sql
CREATE TABLE reactions (
    id SERIAL PRIMARY KEY,
    service_id INTEGER REFERENCES services(id) ON DELETE CASCADE,
    name VARCHAR(100) NOT NULL,
    display_name VARCHAR(100) NOT NULL,
    description TEXT NOT NULL,
    parameters_schema JSONB,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(service_id, name)
);
```

**Example Data:**
```sql
INSERT INTO reactions (service_id, name, display_name, description, parameters_schema) VALUES
(1, 'send_email', 'Send Email', 'Send an email to specified recipient',
 '{"required": ["to", "subject"], "properties": {"to": {"type": "string"}, "subject": {"type": "string"}, "body": {"type": "string"}}}'),
(3, 'create_issue', 'Create GitHub Issue', 'Create a new issue in a repository',
 '{"required": ["repository", "title"], "properties": {"repository": {"type": "string"}, "title": {"type": "string"}, "body": {"type": "string"}}}');
```

---

### UserServiceSubscription
Tracks which services a user has connected and their OAuth tokens.

```sql
CREATE TABLE user_service_subscriptions (
    id SERIAL PRIMARY KEY,
    user_id INTEGER REFERENCES users(id) ON DELETE CASCADE,
    service_id INTEGER REFERENCES services(id) ON DELETE CASCADE,
    oauth_access_token TEXT,
    oauth_refresh_token TEXT,
    oauth_expires_at TIMESTAMP,
    service_user_id VARCHAR(255),
    service_username VARCHAR(255),
    is_active BOOLEAN DEFAULT true,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(user_id, service_id)
);
```

---

### Area
The core automation rule connecting an action to a reaction.

```sql
CREATE TABLE areas (
    id SERIAL PRIMARY KEY,
    user_id INTEGER REFERENCES users(id) ON DELETE CASCADE,
    name VARCHAR(255) NOT NULL,
    description TEXT,
    action_id INTEGER REFERENCES actions(id) ON DELETE CASCADE,
    reaction_id INTEGER REFERENCES reactions(id) ON DELETE CASCADE,
    action_parameters JSONB DEFAULT '{}',
    reaction_parameters JSONB DEFAULT '{}',
    is_enabled BOOLEAN DEFAULT true,
    last_triggered_at TIMESTAMP,
    trigger_count INTEGER DEFAULT 0,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

---

### AreaExecution
Logs each time an AREA is triggered and executed.

```sql
CREATE TABLE area_executions (
    id SERIAL PRIMARY KEY,
    area_id INTEGER REFERENCES areas(id) ON DELETE CASCADE,
    status VARCHAR(20) NOT NULL CHECK (status IN ('success', 'failed', 'pending')),
    trigger_data JSONB,
    response_data JSONB,
    error_message TEXT,
    execution_time_ms INTEGER,
    executed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

---

## Entity Relationships

### ER Diagram (Textual)
```
User ||--o{ Area : creates
User ||--o{ UserServiceSubscription : subscribes_to
Service ||--o{ Action : provides
Service ||--o{ Reaction : provides
Service ||--o{ UserServiceSubscription : subscribed_by
Action ||--o{ Area : triggers
Reaction ||--o{ Area : executes
Area ||--o{ AreaExecution : logs
```

### Key Relationships
1. **User → Area**: One user can create multiple automation rules
2. **Service → Actions/Reactions**: Each service provides multiple actions and reactions
3. **Area → Action/Reaction**: Each area connects exactly one action to one reaction
4. **User → Service Subscription**: Users can subscribe to multiple services with OAuth
5. **Area → Executions**: Each area execution is logged for audit and debugging

---

## Data Constraints

### Business Rules
- A user cannot create an area without subscribing to both required services
- Action and reaction must belong to services the user has subscribed to
- OAuth tokens are encrypted before storage
- User email must be unique and verified
- Area names must be unique per user

### Indexes for Performance
```sql
-- Frequently queried fields
CREATE INDEX idx_areas_user_id ON areas(user_id);
CREATE INDEX idx_areas_enabled ON areas(is_enabled);
CREATE INDEX idx_user_subscriptions_user_service ON user_service_subscriptions(user_id, service_id);
CREATE INDEX idx_area_executions_area_id ON area_executions(area_id);
CREATE INDEX idx_area_executions_status ON area_executions(status);

-- Composite indexes for complex queries
CREATE INDEX idx_areas_user_enabled ON areas(user_id, is_enabled);
CREATE INDEX idx_executions_area_status_date ON area_executions(area_id, status, executed_at);
```

---

## Sample Data Flow

### User Registration & Service Connection
1. User registers → `users` table
2. User connects Gmail → `user_service_subscriptions` with OAuth tokens
3. User connects GitHub → Another subscription record

### AREA Creation
1. User creates "Gmail to GitHub" automation
2. System validates user has both service subscriptions
3. Creates record in `areas` table with action/reaction IDs

### AREA Execution
1. Gmail webhook triggers action
2. System finds matching areas for user
3. Executes GitHub reaction
4. Logs result in `area_executions`

This data model provides the foundation for a scalable, auditable automation platform with proper user isolation and service integration capabilities.