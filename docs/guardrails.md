# Secrets & Guardrails Implementation Plan

Task 3 covers secure storage of API keys/secrets plus multilayer guardrails against prompt
injection, malicious inputs, and runaway token usage.

## 1. Secret Management Strategy

| Component | Approach |
| --- | --- |
| Vault | HashiCorp Vault (or cloud alternative) with AppRole auth from backend pods. |
| Key lifecycle | Keys stored under `secret/data/travel-assistant/*`; rotate via Vault leases and CI
jobs. |
| Access path | FastAPI backend uses short-lived Vault tokens fetched via init container. Tokens held
in memory only, never written to disk or logs. |
| UI access | UI never sees secrets; it receives signed request tokens derived from backend public key. |

Implementation steps:
1. Provision Vault namespace/policies allowing read-only access per service (activities, booking, etc.).
2. Build `services/secrets.py` helper exposing `get_secret(key: str)` with in-memory TTL cache.
3. Configure Agno agent connectors to request credentials on demand via helper.
4. Add CI job to rotate API keys monthly and update Vault entries; send Slack alert on failure.

## 2. Guardrail Pipeline

1. **Input sanitation layer** (FastAPI middleware)
   - Enforce schema validation, HTML escaping, profanity filter, length limits.
   - Detect prompt-injection patterns ("ignore previous", "system override") via regex + ML classifier.
2. **Policy Engine** (OPA or Cedar)
   - Policies define allowed domains/tools per agent.
   - Blocks outbound requests to non-whitelisted hosts and prevents PII leakage via response scanning.
3. **Agno Guarded Agents**
   - Wrap each agent with Agno's `GuardedAgent` to run context filters before sending to LLM.
   - Use structured prompts with strict JSON outputs validated against Pydantic schemas.
4. **Post-processing filters**
   - Sanitize markdown, ensure no secrets/logs leaked, run toxicity detection for user-facing text.

## 3. Security Components

- **Rate limiting**: Redis-backed sliding window per IP + per-account; UI obtains signed nonce to prevent replay.
- **Audit logging**: All guardrail decisions logged to ELK stack with request ID for forensic review.
- **Threat detection**: Security agent monitors anomaly signals (spike in blocked prompts, high token usage).
- **Compliance**: Enable GDPR/CCPA data deletion endpoints; encrypt PII at rest using PostgreSQL TDE or application-level AES-GCM.

## 4. Token Efficiency Measures

- Shared Redis cache for repeated context (popular destinations) with TTL 24h.
- Embedding-based similarity search to reuse prior itineraries when confidence > 0.8.
- Prompt templates trimmed via Agno's context manager—limit to essential facts + bullet outputs.
- Streaming responses to UI to avoid storing long intermediate outputs in memory.

## 5. Monitoring & Alerts

| Metric | Source | Alert |
| --- | --- | --- |
| Token usage per request | Agno telemetry hook | Alert if > 50k tokens/request |
| Guardrail block rate | Middleware counters | Alert if > 5% to detect targeted attack |
| Vault failures | secrets helper exceptions | PagerDuty if > 3 failures in 5 min |

## 6. Implementation Checklist

- [ ] Provision Vault + AppRole, distribute role IDs/secret IDs securely.
- [ ] Implement FastAPI guardrail middleware with sanitizers + policy checks.
- [ ] Wrap Agno agents with schema enforcement and context trimming.
- [ ] Add telemetry + alerting dashboards (Grafana, PagerDuty).
- [ ] Document runbooks for key rotation, guardrail tuning, incident response.
