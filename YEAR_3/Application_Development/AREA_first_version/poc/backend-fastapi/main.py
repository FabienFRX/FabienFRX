from fastapi import FastAPI, Request
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from typing import List, Dict, Any
import time


app = FastAPI(title="AREA FastAPI POC", version="1.0.0")

# CORS configuration
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


# Pydantic models
class ActionModel(BaseModel):
    name: str
    description: str


class ReactionModel(BaseModel):
    name: str
    description: str


class ServiceModel(BaseModel):
    name: str
    actions: List[ActionModel]
    reactions: List[ReactionModel]


class AboutResponse(BaseModel):
    client: Dict[str, str]
    server: Dict[str, Any]


# Sample data
SAMPLE_SERVICES = [
    ServiceModel(
        name="gmail",
        actions=[
            ActionModel(name="new_email", description="A new email is received"),
            ActionModel(name="new_email_with_attachment", description="A new email with attachment is received")
        ],
        reactions=[
            ReactionModel(name="send_email", description="Send an email to a recipient"),
            ReactionModel(name="mark_as_read", description="Mark an email as read")
        ]
    )
]


@app.get("/hello/")
async def hello_world():
    return {
        "message": "Hello AREA! FastAPI is blazing fast!",
        "framework": "FastAPI + Pydantic",
        "features": [
            "High performance async",
            "Auto API documentation",
            "Type validation with Pydantic",
            "Modern Python 3.9+ syntax"
        ],
        "performance": "21,000+ requests/second"
    }


@app.get("/about.json", response_model=AboutResponse)
async def about(request: Request):
    client_ip = request.client.host

    return AboutResponse(
        client={"host": client_ip},
        server={
            "current_time": int(time.time()),
            "services": [service.dict() for service in SAMPLE_SERVICES]
        }
    )


@app.get("/docs-info/")
async def docs_info():
    return {
        "message": "FastAPI auto-generates API docs!",
        "docs_url": "/docs",
        "redoc_url": "/redoc",
        "openapi_url": "/openapi.json"
    }


if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8080)