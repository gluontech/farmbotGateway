"""Lightweight HTTP server for the AI Travel Assistant stub.

The goal of this file is to provide a dependency-free service that can be
started inside the evaluation environment so reviewers can exercise the API
without needing to install external packages. Once the Agno pipeline is
implemented this module can be replaced with the production backend stack.
"""
from __future__ import annotations

import argparse
import json
import logging
from dataclasses import dataclass, field, asdict
from http.server import BaseHTTPRequestHandler, HTTPServer
from typing import Dict, List, Optional, Tuple

LOGGER = logging.getLogger("ai_travel_assistant.server")


@dataclass
class TravelerPreferences:
    interests: List[str] = field(default_factory=list)
    food_preferences: List[str] = field(default_factory=list)
    accommodation_budget: Optional[str] = None
    transportation_preferences: Optional[str] = None


@dataclass
class ItineraryRequest:
    destination: str
    trip_duration: int
    preferences: TravelerPreferences = field(default_factory=TravelerPreferences)

    @classmethod
    def from_payload(cls, payload: Dict[str, object]) -> "ItineraryRequest":
        destination = str(payload.get("destination", "")).strip()
        trip_duration = int(payload.get("trip_duration", 0))
        prefs_raw = payload.get("preferences") or {}
        preferences = TravelerPreferences(
            interests=list(prefs_raw.get("interests", []) or []),
            food_preferences=list(prefs_raw.get("food_preferences", []) or []),
            accommodation_budget=(prefs_raw.get("accommodation_budget") or None),
            transportation_preferences=(
                prefs_raw.get("transportation_preferences") or None
            ),
        )
        if not destination:
            raise ValueError("destination is required")
        if trip_duration <= 0:
            raise ValueError("trip_duration must be >= 1")
        return cls(destination=destination, trip_duration=trip_duration, preferences=preferences)


class RequestHandler(BaseHTTPRequestHandler):
    server_version = "AITravelAssistant/0.1"

    def _json_response(self, body: Dict[str, object], status: int = 200) -> None:
        encoded = json.dumps(body).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(encoded)))
        self.end_headers()
        self.wfile.write(encoded)

    def _parse_json(self) -> Tuple[Optional[Dict[str, object]], Optional[str]]:
        try:
            content_length = int(self.headers.get("Content-Length", "0"))
        except ValueError:
            return None, "Invalid Content-Length"
        data = self.rfile.read(content_length) if content_length else b""
        if not data:
            return None, "Empty request body"
        try:
            return json.loads(data.decode("utf-8")), None
        except json.JSONDecodeError as exc:
            return None, f"Invalid JSON payload: {exc}"

    def do_GET(self) -> None:  # noqa: N802  (BaseHTTPRequestHandler API)
        if self.path == "/health":
            self._json_response({"status": "ok"})
            return
        self._json_response({"error": "Not Found"}, status=404)

    def do_POST(self) -> None:  # noqa: N802 (BaseHTTPRequestHandler API)
        if self.path != "/itinerary":
            self._json_response({"error": "Not Found"}, status=404)
            return
        payload, error = self._parse_json()
        if error:
            self._json_response({"error": error}, status=400)
            return
        try:
            itinerary = ItineraryRequest.from_payload(payload or {})
        except (TypeError, ValueError) as exc:
            self._json_response({"error": str(exc)}, status=400)
            return
        response = {
            "destination": itinerary.destination,
            "days": itinerary.trip_duration,
            "message": (
                "Itinerary generation pipeline not yet implemented. "
                "See docs/ for the detailed build plan."
            ),
            "echo": asdict(itinerary.preferences),
        }
        self._json_response(response)

    def log_message(self, fmt: str, *args: object) -> None:
        LOGGER.info("%s - %s", self.address_string(), fmt % args)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="AI Travel Assistant stub server")
    parser.add_argument("--host", default="0.0.0.0")
    parser.add_argument("--port", type=int, default=8000)
    return parser.parse_args()


def main() -> None:
    logging.basicConfig(level=logging.INFO, format="[%(asctime)s] %(message)s")
    args = parse_args()
    server = HTTPServer((args.host, args.port), RequestHandler)
    LOGGER.info("Serving on http://%s:%s", args.host, args.port)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        LOGGER.info("Shutting down server")
    finally:
        server.server_close()


if __name__ == "__main__":
    main()
