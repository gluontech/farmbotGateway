# Activity Planning Agent Design

Task 4 focuses on researching, ranking, and structuring activities per traveler inputs via the Agno
Activity Planning Agent.

## 1. Responsibilities

- Discover up-to-date activities within destination regions via external APIs/search tools.
- Filter and rank based on traveler interests, seasonality, logistics, and trip duration.
- Output structured entries with **name, location, description, hours, cost, rating, suitability reason,
  geo tags**, and references.

## 2. Data Sources & Connectors

| Source | Purpose | Notes |
| --- | --- | --- |
| Google Places / Maps | Core POI data | Requires Places API key; apply field masks for cost efficiency. |
| TripAdvisor / Viator | Tours & attractions | Use affiliate API where available. |
| Event APIs | Seasonal events/festivals | Query based on travel dates. |

Connector pattern: asynchronous HTTP clients with retry/backoff, caching, and Pydantic parsing.

## 3. Agno Agent Implementation

```python
class ActivityPlanningAgent(Agent):
    schema = ActivityListSchema
    tools = [GooglePlacesTool(), TripAdvisorTool(), WeatherLookupTool()]
    memory = DestinationMemoryStore()
```

Prompt template enforces JSON output; guardrail ensures only approved tools invoked.

## 4. Ranking & Clustering Algorithm

1. Generate candidate list (top 30) from APIs filtered by destination + interests.
2. Score each candidate using weighted factors:
   - Interest match score (semantic similarity using embeddings).
   - Logistics score (distance to base region, time needed).
   - Popularity (rating * review count).
   - Cost alignment with budget.
3. Cluster by region using HDBSCAN or K-means on lat/long.
4. Select activities per day ensuring coverage across clusters and trip duration.

## 5. Output Schema

```json
{
  "region": "Kyoto - Gion",
  "day": 2,
  "activities": [
    {
      "name": "Fushimi Inari Taisha Sunrise Hike",
      "location": "68 Fukakusa Yabunouchicho, Kyoto",
      "description": "Iconic torii gate hike before crowds arrive.",
      "hours": "24/7",
      "cost": "Free",
      "rating": 4.8,
      "suitability": "Aligns with traveler interest in photography and hiking.",
      "geo": {"lat": 34.967, "lng": 135.772},
      "references": ["google_places:ChIJ..."],
      "duration_hours": 2.5
    }
  ]
}
```

## 6. Token Efficiency

- Use search API filters to limit responses (e.g., `fields=name,rating,geometry`).
- Summarize long descriptions to <= 60 words before passing to other agents.
- Cache embedding comparisons per interest keyword.

## 7. Implementation Checklist

- [ ] Build connectors with typed responses + caching.
- [ ] Implement scoring/clustering pipeline with reproducible randomness seed.
- [ ] Add schema validation + unit tests covering ranking logic.
- [ ] Register agent within Agno orchestrator and verify tool permissions.
