from django.shortcuts import render
from django.http import JsonResponse
from django.views.decorators.cache import never_cache
from django.views.decorators.csrf import csrf_exempt
import json
import os
import logging

logger = logging.getLogger(__name__)

def load_sensor_data():
    current_dir = os.path.dirname(os.path.abspath(__file__))
    file_path = os.path.join(current_dir, 'Data', 'Data.json')
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            data = json.load(f)
            if isinstance(data, list):
                return data
            else:
                logger.warning("Expected a list of sensor data in Data.json.")
    except (FileNotFoundError, json.JSONDecodeError) as e:
        logger.error(f"Failed to load sensor data from JSON: {e}")
    return []

# View to render index.html (your heatmap dashboard)
def index(request):
    return render(request, 'index.html')  # ✅ Points to index.html

@csrf_exempt
def receive_sensor_data(request):
    if request.method == "POST":
        try:
            data = json.loads(request.body)
            node_id = data.get("node_id")
            temperature = data.get("temperature")
            humidity = data.get("humidity")

            # Optionally: Add coordinates or map node ID to location
            sensor_entry = {
                "node_id": node_id,
                "x": node_id * 10,  # Example mapping
                "y": 0,
                "temperature": temperature,
                "humidity": humidity,
                "timestamp": now().isoformat()
            }

            file_path = os.path.join(os.path.dirname(__file__), 'Data', 'Data.json')
            if os.path.exists(file_path):
                with open(file_path, 'r', encoding='utf-8') as f:
                    existing_data = json.load(f)
            else:
                existing_data = []

            existing_data.append(sensor_entry)

            with open(file_path, 'w', encoding='utf-8') as f:
                json.dump(existing_data, f, indent=2)

            return JsonResponse({"status": "success"}, status=200)
        except Exception as e:
            return JsonResponse({"status": "error", "message": str(e)}, status=400)

    return JsonResponse({"status": "only POST allowed"}, status=405)

# API endpoint to return sensor data as JSON
@never_cache
def get_sensor_data(request):
    sensor_data = load_sensor_data()
    category = request.GET.get('category', '').lower()

    def extract(sensor, cat):
        return {
            "x": sensor.get("x", 0),
            "y": sensor.get("y", 0),
            "category": cat,
            "value": sensor.get(cat)
        }

    if category in ["temperature", "humidity"]:
        filtered = [extract(s, category) for s in sensor_data if category in s]
        return JsonResponse(filtered, safe=False)

    # No filter: return both temperature and humidity
    all_data = []
    for s in sensor_data:
        if "temperature" in s:
            all_data.append(extract(s, "temperature"))
        if "humidity" in s:
            all_data.append(extract(s, "humidity"))

    return JsonResponse(all_data, safe=False)
