# Architecture & Orchestration Plan

This document outlines Task 1 for the multi-agent Travel Assistant: defining the architecture and orchestrating agents using the **Agno Agentic framework**.

## 1. System Overview

```
[Traveler UI]
    │
    ▼
[API Gateway / Backend (FastAPI)]
    │
    ▼
[Agno Orchestrator]
 ┌────────────┬───────────────┬──────────────┬──────────────┐
 │Activity    │Transportation │Accommodation │Restaurant    │
 │Planning    │Scout Agent    │Scout Agent   │Scout Agent   │
 └─────┬──────┴────┬──────────┴──────┬───────┴──────┬───────┘
       │           │                 │              │
       ▼           ▼                 ▼              ▼
[Shared Knowledge Graph & Memory Store]
       │
       ▼
[Itinerary Compilation Agent]
       │
       ▼
[Result Formatter + UI Delivery]
```

## 2. Technology Choices

| Layer | Technology |
| --- | --- |
| Frontend UI | React + TypeScript (Next.js for SSR) |
| API Layer | FastAPI with Pydantic schemas |
| Orchestration | **Agno Agentic framework** with custom planner |
| Data Connectors | RESTful connectors to Google Places, Rome2Rio, Booking.com, Yelp |
| Persistence | PostgreSQL for itineraries, Redis for caching |
| Secrets | HashiCorp Vault (or cloud equivalent) integrated via backend |
| Observability | OpenTelemetry, Prometheus/Grafana |

## 3. Backend Modules

```
/backend
  ├─ api/
  │   ├─ routers/travel.py          # Request validation, job submission
  │   ├─ schemas.py                 # Pydantic request/response models
  │   └─ middlewares/security.py    # Guardrails (prompt sanitation, rate limiting)
  ├─ services/
  │   ├─ orchestrator.py            # Interface with Agno workflow
  │   ├─ secrets.py                 # Vault integration + key rotation
  │   └─ telemetry.py               # Token + cost tracking hooks
  ├─ agents/
  │   ├─ activity_agent.py
  │   ├─ transportation_agent.py
  │   ├─ accommodation_agent.py
  │   ├─ restaurant_agent.py
  │   └─ itinerary_agent.py
  ├─ data/
  │   ├─ connectors/                # Wrappers for external APIs
  │   └─ repositories/              # Storage (Postgres)
  └─ main.py                        # FastAPI entry point
```

## 4. Agno Orchestration Flow

1. **Input Validation** – FastAPI validates traveler input with Pydantic.
2. **Context Builder** – Prepares normalized context (locations, interests, budgets) and stores in Redis for reuse.
3. **Agno Workflow**
   - Orchestrator configures Agno `AgentGraph` with nodes for each specialized agent plus shared memory edges.
   - Execution order: Activity → Transportation → Accommodation → Restaurant → Itinerary.
   - Guardrails enforce tool usage policies and sanitize prompts before passing to Agno.
4. **Shared Memory** – Results stored in Agno MemoryStore (Redis backend) and knowledge graph (Neo4j optional) for cross-agent reasoning.
5. **Itinerary Compilation** – Consumes verified data references only.
6. **Response Delivery** – Markdown itinerary returned to UI; UI renders highlights, Google Maps links.

## 5. Agent Contracts

| Agent | Inputs | Outputs |
| --- | --- | --- |
| Activity Planning | Destination, interests, trip duration, season data | Structured activities array (name, location, description, hours, cost, rating, suitability reason, geo tags) |
| Transportation Scout | Activity sequence, transport preferences, geo distances | Route legs with mode, provider, time, cost, contact |
| Accommodation Scout | Activity clusters, budget, preferences | Hotel options per region with attributes |
| Restaurant Scout | Food preferences, activity locations, budget | Restaurants per region with attributes |
| Itinerary Compilation | Aggregated data from above agents | Day-by-day markdown itinerary with highlights, Google map URL, logistics |

All agents share a **Schema Registry** (Pydantic) to ensure structural consistency and allow Agno to validate messages automatically.

## 6. Security & Guardrails Integration

- **Prompt Sanitizer** middleware strips malicious instructions before orchestrator invocation.
- **Policy Engine** (Open Policy Agent) enforces tool restrictions and response filtering.
- **Secrets Gateway** fetches API keys at runtime; never exposes to frontend.
- **Token Efficiency** achieved via:
  - Cached embeddings/context for repeated destinations.
  - Summarized memory between agents.
  - Adaptive context window trimming implemented in Agno config.

## 7. Deployment Considerations

- Backend containerized with Docker; orchestrator packaged as part of the FastAPI service.
- Use asynchronous workers (Celery or Agno's async runners) for long-running itineraries; status updates via WebSocket.
- Observability hooks emit events for each agent node execution for monitoring dashboards.

This architecture ensures the Travel Assistant meets Task 1 requirements while leveraging the Agno Agentic framework for reliable multi-agent orchestration.
