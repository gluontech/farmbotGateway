# Deployment & Monitoring Plan

Task 10 defines how to ship the Travel Assistant to production with observability and cost control.

## 1. Environments

| Env | Purpose | Notes |
| --- | --- | --- |
| Dev | Individual developer testing | Docker Compose, mocked external APIs |
| Staging | Pre-prod with real APIs (sandbox keys) | Nightly integration tests, load testing |
| Production | Customer-facing | Auto-scaling, blue/green deployments |

## 2. Packaging & CI/CD

- Backend (FastAPI + Agno) packaged as Docker image via GitHub Actions.
- Frontend (Next.js) built to static + serverless bundle.
- Use Terraform to provision infrastructure (EKS/GKE). State stored in remote backend.
- CI pipeline stages: lint → unit tests → component tests → build → security scan → deploy to staging → automated smoke tests → manual approval → prod deploy.

## 3. Runtime Architecture

```
[Cloud Load Balancer]
      │
      ▼
[API Gateway / WAF] ──> [FastAPI + Agno pods]
      │                    │
      │                    ├─ connects to Redis (caching/memory)
      │                    ├─ connects to PostgreSQL (itinerary storage)
      │                    ├─ connects to Vault (secrets)
      ▼                    └─ emits telemetry to OpenTelemetry Collector
[Next.js Frontend (Vercel or CloudFront+Lambda@Edge)]
```

- Background workers (Celery or Agno async executors) handle long itineraries; triggered via message queue (RabbitMQ/SQS).
- CDN caches static assets + UI bundles.

## 4. Observability

- **Logging**: Structured JSON logs to CloudWatch/Stackdriver; include request ID, agent, token counts.
- **Metrics**: Prometheus scraping (tokens per request, job duration, guardrail block rate, queue depth).
- **Tracing**: OpenTelemetry instrumentation on FastAPI endpoints, Agno agent executions, and external API calls.
- **Dashboards**: Grafana panels for KPIs (itineraries/hour, avg latency, errors by agent).

## 5. Alerting

| Signal | Threshold | Action |
| --- | --- | --- |
| API error rate | >2% over 5 min | PagerDuty alert to on-call |
| Token cost/day | >$500 | Notify FinOps + throttle heavy destinations |
| Queue latency | >2 min avg | Auto-scale workers + alert |
| Vault failures | >3/min | Fail open? No—switch to secondary vault, alert SecOps |

## 6. Cost Controls

- Autoscale worker pool based on queue depth; scale to zero overnight if no jobs.
- Prefer cached itineraries for repeat requests; charge credits to discourage spam.
- Monitor API quota usage; rotate keys across regions to avoid throttling.

## 7. Disaster Recovery

- Daily PostgreSQL backups with PITR; replicate to secondary region.
- Redis snapshot every 30 minutes for memory store.
- Run chaos tests monthly (kill pods, simulate API outage) to ensure graceful degradation.

## 8. Implementation Checklist

- [ ] Write Terraform modules for networking, compute, databases, queues, Vault.
- [ ] Configure GitHub Actions workflows for CI/CD with environment protections.
- [ ] Set up monitoring stack (Prometheus, Grafana, Loki, Alertmanager).
- [ ] Document on-call runbooks and escalation paths.
- [ ] Perform load + failover drills before launch.
