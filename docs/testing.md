# Testing & Validation Strategy

Task 9 establishes confidence in the multi-agent system via automated and manual testing.

## 1. Testing Pyramid

| Layer | Scope | Tooling |
| --- | --- | --- |
| Unit | Connectors, scoring functions, schema validators | Pytest, responses/httpx-mock |
| Component | Individual agents interacting with mock tools | Agno test harness, pytest fixtures |
| Integration | Full workflow from input to itinerary | FastAPI TestClient + Agno orchestrator |
| Security | Prompt-injection, XSS, secrets leakage | Custom attack scripts, ZAP, Semgrep |
| Performance | Load, latency, token usage | Locust, k6, Agno telemetry replay |

## 2. Unit Tests

- Mock external APIs with recorded fixtures; cover edge cases (no results, missing fields).
- Validate scoring logic for activities/hotels/restaurants.
- Check guardrail middleware rejects malicious payloads.

## 3. Component Tests

- Spin up Agno agent with fake tool outputs to ensure prompt templates produce valid schema.
- Verify Transportation agent respects preferences by asserting output modes.
- Ensure Itinerary agent fails gracefully when data missing.

## 4. Integration Tests

1. Submit canonical traveler request (e.g., 5-day Tokyo foodie trip) via FastAPI TestClient.
2. Stub external connectors to deterministic fixtures; run full workflow.
3. Assert itinerary length equals trip duration, contains highlights/map link, and references all upstream IDs.
4. Run multi-request concurrency test (10 simultaneous) to verify job queue + caching.

## 5. Security Testing

- Automated prompt-injection suite containing ~50 payloads; ensure guardrails block or sanitize.
- Run OWASP ZAP against UI/API for XSS, CSRF, SSRF.
- Use Semgrep rules for secrets-in-code and insecure random usage.
- Conduct manual red-team sessions quarterly.

## 6. Performance & Cost Testing

- Locust scenarios simulating 100 concurrent itinerary generations; monitor latency and token counts.
- Replay stored Agno traces to measure token usage per agent; adjust context windows accordingly.
- Stress-test Vault + secrets helper under rotation events.

## 7. QA Automation

- GitHub Actions pipeline running unit + component tests on every PR.
- Nightly integration tests using sandbox API keys.
- Manual QA checklist for UI release (accessibility, responsive layout, localization).

## 8. Implementation Checklist

- [ ] Create pytest suites per module with coverage reports.
- [ ] Add mocked fixtures for external APIs and keep them updated.
- [ ] Wire up CI workflows (lint, unit, component, integration).
- [ ] Automate security scans + artifact uploads.
- [ ] Document manual QA procedures and release gates.
