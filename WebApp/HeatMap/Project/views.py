from django.shortcuts import render
from django.http import JsonResponse
from django.conf import settings
from django.views.decorators.cache import never_cache  # ✅ Needed to disable browser caching
from .models import SensorData
import json
import os
import logging

print("Project views loaded!")  # Should appear in console
def index(request):
    return render(request, 'index.html')  # Ensure index.html is in templates

def get_sensor_data(request):
    data = SensorData.objects.all().values()
    return JsonResponse(list(data), safe=False)

# Configure logger
logger = logging.getLogger(__name__)

# Fallback: Example sensor data for testing/demo
FALLBACK_SENSOR_DATA = [
    {"x": 150, "y": 100, "temperature": 22.5, "humidity": 50},
    {"x": 300, "y": 200, "temperature": 25.1, "humidity": 65},
    {"x": 450, "y": 300, "temperature": 20.8, "humidity": 65},
    {"x": 300, "y": 200, "temperature": 23.0, "humidity": 88}
]

# Load sensor data from JSON file
def load_sensor_data():
    file_path = os.path.join(settings.BASE_DIR, 'Project', 'Data', 'Data.json')
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            data = json.load(f)
            if isinstance(data, list):  # Extra check
                return data
            else:
                logger.warning("JSON data structure invalid. Expected a list.")
    except (FileNotFoundError, json.JSONDecodeError) as e:
        logger.warning(f"Sensor data fallback used due to error: {e}")
    
    return FALLBACK_SENSOR_DATA

# View to render the heatmap dashboard page
def heatmap_dashboard(request):
    return render(request, 'index.html')

# View to serve sensor data as JSON (polling target)
@never_cache  # ✅ Prevent browser from caching the response
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
        filtered_data = [extract(sensor, category) for sensor in sensor_data if category in sensor]
        return JsonResponse(filtered_data, safe=False)

    # Return both categories if none specified
    all_data = []
    for sensor in sensor_data:
        if "temperature" in sensor:
            all_data.append(extract(sensor, "temperature"))
        if "humidity" in sensor:
            all_data.append(extract(sensor, "humidity"))

    return JsonResponse(all_data, safe=False)
