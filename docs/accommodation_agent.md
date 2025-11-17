# Accommodation Scout Agent Specification

Task 6 mandates curated lodging recommendations per region/day aligned with activities and budget.

## 1. Objectives

- Recommend 2–3 options per region (budget, mid-range, luxury) near planned activities.
- Include full address, nightly rate range, star rating, amenities, guest review score, and rationale.
- Ensure coverage for entire trip duration; minimize hotel changes by clustering days.

## 2. Data Sources

| Source | Details |
| --- | --- |
| Booking.com API | Real-time pricing + availability |
| Expedia Rapid API | Backup inventory |
| Google Hotels | Supplemental reviews/ratings |
| Internal DB | Past traveler feedback stored in PostgreSQL |

## 3. Processing Flow

1. Receive region clusters + day groupings from Activity Agent.
2. For each cluster, query Booking.com filtered by budget band + date range.
3. Enrich with reviews/amenities from Google Hotels API; dedupe by location hash.
4. Score hotels using weighted formula (proximity 40%, rating 30%, amenities 20%, budget fit 10%).
5. Select top 3 per cluster ensuring at least one option per budget tier.

## 4. Output Schema

```json
{
  "region": "Kyoto Central",
  "stay_dates": "Day 1-3",
  "options": [
    {
      "name": "Hotel Gracery Kyoto Sanjo",
      "address": "420 Sakuranocho, Nakagyo-ku, Kyoto",
      "nightly_rate": "$150-190",
      "category": "mid-range",
      "star_rating": 4,
      "amenities": ["Free Wi-Fi", "On-site laundry", "Tea lounge"],
      "review_score": 9.0,
      "why": "5-minute walk to Nishiki Market and Gion activities; aligns with foodie interests."
    }
  ]
}
```

## 5. Guardrails

- Rates pulled directly from API; include timestamp + currency.
- If availability unknown, mark as "Check availability" rather than inventing rooms.
- Enforce privacy: remove PII beyond hotel contact details.

## 6. Token Efficiency & Storage

- Store hotel metadata in Postgres `hotels` table; only fetch prices per request.
- Cache amenity descriptions to avoid repeating long text in prompts.
- Use Agno memory references (`hotel_id`) so downstream agents reference IDs instead of full text.

## 7. Implementation Checklist

- [ ] Build Booking.com + Expedia connectors with pagination + retry logic.
- [ ] Implement scoring/selection service with configurable weights.
- [ ] Add schema validation + unit tests covering tier coverage and clustering.
- [ ] Integrate with Agno orchestrator and share outputs via memory store.
