from django.shortcuts import render
from django.http import JsonResponse
from django.views.decorators.cache import never_cache
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
