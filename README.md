# AI-Travel-Assistant Repository

This repository now hosts the AI-Travel-Assistant project, which delivers a secure multi-agent travel-planning workflow powered by the Agno Agentic framework. The previous FarmBot-specific firmware/layout has been removed so the repo can focus entirely on the travel assistant stack.

## Repository layout

```
README.md
backend/              # Future Agno service entrypoints, API surface, and orchestrator glue
frontend/             # Traveler intake UI plus guardrail-aware client helpers
infra/                # IaC, deployment manifests, monitoring configuration
docs/                 # Architecture, guardrails, agent specs, testing & deployment plans
```

The repo root is now the canonical **AI-Travel-Assistant** workspace so GitHub and local checkouts reflect the correct project name instead of the legacy `farmbotGateway` label.

## Getting started

1. Review `docs/architecture.md` for the end-to-end system design and module breakdown.
2. Use the UI, guardrails, and agent specification docs (under `docs/`) to guide implementation task-by-task.
3. Commit new backend, frontend, and infrastructure code directly under their dedicated root folders to keep the repo scoped to the travel assistant environment.

## Running the backend stub

The `backend/` directory now includes a dependency-free HTTP server so contributors can verify their environment. The service exposes `/health` and `/itinerary` endpoints; the latter simply echoes the validated payload until the Agno agents are implemented.

```bash
cd backend
python server.py --host 0.0.0.0 --port 8000
```

Then, in a separate terminal you can hit the API:

```bash
curl http://localhost:8000/health
curl -X POST http://localhost:8000/itinerary \
  -H 'Content-Type: application/json' \
  -d '{"destination": "Kyoto", "trip_duration": 5, "preferences": {"interests": ["culture"]}}'
```
