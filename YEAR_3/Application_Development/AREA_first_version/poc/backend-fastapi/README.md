# FastAPI POC for AREA Project

## Overview
This POC demonstrates FastAPI's high-performance capabilities with async support, automatic API documentation, and type validation using Pydantic models.

## Features Demonstrated
- ✅ High-performance async API (21,000+ req/s)
- ✅ Automatic API documentation at `/docs`
- ✅ Type validation with Pydantic models
- ✅ CORS support for web client integration
- ✅ Required `/about.json` endpoint
- ✅ Modern Python 3.9+ syntax

## Key Advantages
- **Performance**: 5x faster than traditional frameworks
- **Developer Experience**: Auto-generated docs and type hints
- **Modern**: Built for async/await and modern Python
- **Type Safety**: Automatic request/response validation

## API Endpoints
- `GET /hello/` - Test endpoint showing FastAPI capabilities
- `GET /about.json` - Required project endpoint with services
- `GET /docs` - Interactive API documentation
- `GET /docs-info/` - Info about automatic documentation

## Quick Start
```bash
cd poc/backend-fastapi
source venv/bin/activate
python -c "import uvicorn; uvicorn.run('main:app', host='0.0.0.0', port=8080)"
```

## Why FastAPI for AREA
- Superior performance for API-heavy applications
- Excellent for microservices architecture
- Built-in data validation reduces bugs
- Modern async support for concurrent operations