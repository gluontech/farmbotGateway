# Itinerary Compilation Agent Guide

Task 8 requires synthesizing outputs from all specialized agents into a coherent, realistic itinerary.

## 1. Inputs

- Activity list with geo clusters and day assignments.
- Transportation legs with timings/modes.
- Accommodation options per region/day.
- Restaurant recommendations with meal slots.
- Guardrail metadata (citations, references) for traceability.

## 2. Responsibilities

- Assemble markdown itinerary matching exact trip duration.
- Provide Trip Highlights, Google Maps route link, daily breakdown, and travel tips.
- Ensure logistics align (no overlapping times, travel durations accounted for).
- Use only verified data references from upstream agents.

## 3. Agno Implementation

```python
class ItineraryCompilationAgent(Agent):
    schema = ItinerarySchema
    tools = [MarkdownFormatterTool(), MapLinkBuilderTool()]
    memory = SharedRedisMemory()
```

Prompt enforces referencing upstream entry IDs and requires final sanity-check reasoning.

## 4. Assembly Process

1. **Normalize Inputs**: convert all times to local timezone, align currencies.
2. **Daily Scheduler**:
   - Start from earliest activity per day; insert transportation legs before/after.
   - Add meal slots at logical intervals; choose restaurant near preceding activity.
   - Assign accommodation check-in/out with buffer times.
3. **Validation Pass**:
   - Ensure daily duration <= 16 hours; add rest blocks otherwise.
   - Confirm accommodations consistent for consecutive days unless travel requires move.
   - Validate transport preference adherence.
4. **Formatting**:
   - `TRIP HIGHLIGHTS` bullet list summarizing top experiences.
   - `TRIP ROUTE MAP` linking to Google My Maps; built via MapLinkBuilder using region coordinates.
   - `Daily Itinerary` tables with sections: Morning, Afternoon, Evening, Logistics tips.

## 5. Guardrails

- Must cite source IDs for each activity/hotel/restaurant; store mapping for audit.
- If data missing, agent requests re-run of specific upstream agent instead of fabricating content.
- Check for sensitive content; sanitize before returning to UI.

## 6. Token Efficiency

- Use references (IDs) in working memory; only expand to prose in final output.
- Summarize repeated logistics tips and reference them once with anchors.
- Paginate long itineraries (>14 days) with collapsible sections to reduce output size.

## 7. Implementation Checklist

- [ ] Define `ItinerarySchema` capturing highlights, map link, and per-day arrays.
- [ ] Build scheduler module with constraint solving (e.g., OR-Tools) for time conflicts.
- [ ] Implement validation + fallback triggers for missing data.
- [ ] Add markdown renderer with sanitization + unit tests for formatting.
