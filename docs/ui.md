# Secure Traveler Input UI Plan

Task 2 focuses on designing and implementing a secure, user-friendly interface that captures all
required traveler inputs before orchestrating Agno agents. The UI will be implemented in
**Next.js 14 (App Router) + TypeScript** with TailwindCSS for styling and React Hook Form for
state management.

## 1. Key Screens & Flows

1. **Landing / Intake Screen**
   - Hero section describing capabilities and privacy assurances.
   - Primary CTA button to start itinerary request.
2. **Traveler Intake Wizard**
   - Multi-step form that captures destination, interests, duration, food preferences,
     accommodation budget, transportation preferences, and optional context (season, travelers, etc.).
   - Progress indicator and inline validation feedback.
3. **Submission & Status Screen**
   - Displays summary of inputs, allows edits, and submits securely to FastAPI backend.
   - Shows real-time status updates via WebSocket/EventSource until itinerary ready.
4. **Results Screen**
   - Renders markdown itinerary, highlights, Google Maps link, and download/share options.

## 2. Component Architecture

```
/app
  ├─ layout.tsx
  ├─ page.tsx (Landing)
  ├─ request/
  │    ├─ page.tsx (Wizard container)
  │    ├─ components/
  │    │    ├─ TravelerFormWizard.tsx
  │    │    ├─ StepDestination.tsx
  │    │    ├─ StepPreferences.tsx
  │    │    ├─ StepLogistics.tsx
  │    │    ├─ StepReview.tsx
  │    │    └─ SecureSubmitButton.tsx
  │    └─ hooks/useSubmitRequest.ts
  └─ results/[requestId]/page.tsx
```

Shared utilities live in `src/ui/lib/validation.ts` (Zod schemas), `src/ui/lib/api.ts`
(typed API client), and `src/ui/components/Input` for common fields.

## 3. Data Contract & Validation

- **Client-side**: Zod schema mirrors backend Pydantic model. Example:

```ts
const TravelerSchema = z.object({
  destination: z.string().min(2).max(120),
  interests: z.array(z.string()).min(1),
  tripDuration: z.number().int().min(1).max(60),
  foodPreferences: z.array(z.string()).min(1),
  budget: z.enum(["budget", "mid-range", "luxury"]),
  transportationPreferences: z.array(z.enum([
    "public_transport", "private_car", "flight", "boat", "land_only"
  ])).min(1)
});
```

- **Server-side**: FastAPI re-validates input; mismatches return structured errors rendered
  inline by wizard.
- Inputs sanitized (escape HTML, whitelist characters) before submission.

## 4. Security & Guardrails

| Risk | Mitigation |
| --- | --- |
| CSRF | Next.js server actions protected with anti-CSRF token derived from session cookie. |
| XSS | All rendered markdown sanitized via `@astrojs/markdown-remark` Safe Mode or `dompurify`. |
| Prompt injection | Frontend strips obvious prompt-injection strings and informs backend guardrail middleware. |
| Secrets leakage | Frontend never stores API keys; uses backend-issued short-lived `request_token` for submission. |
| Transport security | Enforce HTTPS, HSTS headers via Next.js middleware, secure cookies. |
| Rate limiting | Backend issues signed nonce per submission; UI enforces cooldown timers. |

## 5. Accessibility & UX Requirements

- WCAG 2.1 AA compliance: semantic HTML, labeled inputs, focus management between steps.
- Keyboard navigation for wizard controls; ARIA live regions for validation errors.
- Light/dark mode toggle stored in local preference.
- Responsive layout optimized for mobile travel planning.

## 6. API Integration Flow

1. User completes wizard; `TravelerSchema` validated locally.
2. UI calls `/api/travel/request` with payload + `request_token`.
3. Backend enqueues Agno workflow job and returns `requestId`.
4. UI subscribes to `/api/travel/updates/{requestId}` via SSE/WebSocket for status events.
5. When itinerary ready, UI navigates to `/results/{requestId}` and fetches formatted markdown.

## 7. Telemetry & Token Efficiency Hooks

- UI attaches lightweight analytics payload (destination, duration bucket, anonymized) to help backend
  tune caching/token strategies.
- Cached options for recent destinations stored in IndexedDB; reused to pre-fill suggestions to reduce
  exploratory LLM calls.
- UI displays estimated cost/token usage to set expectations and reduce repeated submissions.

## 8. Implementation Checklist

- [ ] Scaffold Next.js app with Tailwind, ESLint, Prettier, Husky pre-commit.
- [ ] Implement wizard components with React Hook Form + Zod resolver.
- [ ] Add session-aware CSRF + secure cookie configuration.
- [ ] Build API utilities with fetch wrappers handling retries and exponential backoff.
- [ ] Create results page that renders markdown itinerary using a sanitized renderer.
- [ ] Add Cypress end-to-end tests covering form completion, validation errors, and submission flow.

This plan ensures the traveler input UI is secure, accessible, and tightly integrated with the
Agno-based backend orchestrator for subsequent tasks.
