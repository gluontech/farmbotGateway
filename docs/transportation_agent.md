# Transportation Scout Agent Blueprint

Task 5 defines how the Transportation Scout Agent sequences travel legs that respect user
preferences and minimize backtracking.

## 1. Inputs & Outputs

- Inputs: ordered activity clusters, travel windows, transportation preferences, accommodation
anchors.
- Outputs: leg-by-leg plan with departure/arrival times, mode, provider, cost, booking links,
contingency tips.

## 2. Data & Tools

| Tool | Usage |
| --- | --- |
| Google Directions API | Base routing/time estimates for driving, public transit, walking. |
| Rome2Rio API | Multimodal routing (flight, ferry, long-distance bus). |
| Local transit GTFS | Timetables for metro/bus (ingested nightly). |
| Airline/train scrapers | Optional if API unavailable (respect ToS). |

## 3. Algorithm

1. Receive ordered list of regions/days from Activity Agent.
2. Build graph where nodes = activities/hotels, edges = travel legs with weights for duration + cost.
3. Apply constrained shortest path (multi-objective Dijkstra) honoring `transportation_preferences`.
4. Group consecutive legs with same mode/provider to reduce context size.
5. Insert buffer times based on reliability data (e.g., +15% for public transit, +90 min for airports).

## 4. Output Format

```json
{
  "day": 3,
  "legs": [
    {
      "from": "Hotel Granvia Kyoto",
      "to": "Arashiyama Bamboo Grove",
      "mode": "JR Sagano Line",
      "provider": "JR West",
      "departure_time": "08:10",
      "arrival_time": "08:32",
      "duration_minutes": 22,
      "cost": "¥240",
      "booking": "https://www.japanrailpass.net/",
      "contact": "+81-75-123-4567",
      "preference_match": ["public_transport"],
      "notes": "Board local train at Track 33; seats unreserved."
    }
  ]
}
```

## 5. Guardrails & Validation

- Validate coordinates/hours from Activity Agent to avoid impossible transfers.
- Ensure costs/currencies normalized.
- Limit generated text to factual logistics; avoid hallucinating providers—must cite API reference IDs.

## 6. Token & API Efficiency

- Cache distance matrix results by geo hash pair for 6 hours.
- Use Directions API `optimize:true` when traveler allows reordering to find best route.
- Summarize long timetables into bullet ranges instead of listing each trip.

## 7. Implementation Checklist

- [ ] Implement routing service abstraction with fallback order (cache → GTFS → Google → Rome2Rio).
- [ ] Build Agno agent prompt template referencing structured input.
- [ ] Add validations for mode preference compliance and time window overlaps.
- [ ] Create unit tests for leg-building and cost normalization.
