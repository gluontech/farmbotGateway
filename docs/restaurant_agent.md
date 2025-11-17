# Restaurant Scout Agent Plan

Task 7 designs the Restaurant Scout Agent to supply diverse dining recommendations aligned with
food preferences and itinerary geography.

## 1. Requirements

- Provide sufficient restaurants for trip duration with variety of cuisine type and price.
- Output format: **Name – Address – Cuisine – Price ($/$$/$$$) – Rating – Specialties – Why recommended**.
- Cluster suggestions near daily activities and accommodations.

## 2. Data Inputs

- Food preferences (vegan, halal, fine dining, street food, etc.).
- Activity/transportation outputs with coordinates/time windows.
- Budget context from traveler profile.

## 3. Data Sources & Tools

| Source | Usage |
| --- | --- |
| Yelp Fusion API | Ratings, price level, cuisine tags |
| Google Places | Backup data + hours of operation |
| OpenTable API | Availability and reservation links |
| Local blogs/feeds | Trendy/seasonal spots (scraped via RSS, curated) |

## 4. Selection Logic

1. For each day/region, query Yelp within 2 km of primary activities.
2. Filter by cuisine tags matching food preferences; ensure each day includes at least lunch + dinner option.
3. Rank by composite score = rating * review_count weight + preference match + proximity.
4. Include at least one splurge and one budget-friendly option per trip when preferences allow.

## 5. Output Schema

```json
{
  "day": 2,
  "region": "Shinjuku",
  "restaurants": [
    {
      "name": "Ichiran Ramen Shinjuku",
      "address": "1-22-7 Kabukicho, Shinjuku City, Tokyo",
      "cuisine": "Tonkotsu Ramen",
      "price": "$$",
      "rating": 4.5,
      "specialties": ["Customizable broth", "Private booths"],
      "why": "Great for solo foodie traveler seeking authentic ramen near evening activities.",
      "hours": "24/7",
      "booking": null,
      "dietary_notes": ["Pork-based"],
      "references": ["yelp:abc123"]
    }
  ]
}
```

## 6. Guardrails & Quality

- Ensure dietary flags accurate; require at least one reference confirming suitability (e.g., vegan-friendly).
- Avoid hallucinating Michelin stars—only include if verified from authoritative source.
- Sanitize descriptions to avoid marketing fluff; keep to factual info.

## 7. Token Efficiency

- Cache cuisine preference embeddings per user session.
- Use truncated review summaries (<40 words) generated via deterministic template.
- Deduplicate restaurants already stored in Agno memory store for given destination.

## 8. Implementation Checklist

- [ ] Build Yelp/Google/OpenTable connectors with rate-limit handling.
- [ ] Implement ranking logic + coverage validator ensuring meals per day.
- [ ] Add schema tests verifying fields + dietary notes.
- [ ] Register agent + guardrails within Agno orchestrator.
