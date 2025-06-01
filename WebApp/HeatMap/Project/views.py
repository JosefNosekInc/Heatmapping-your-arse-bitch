from django.shortcuts import render
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.views.decorators.cache import never_cache
from django.views.decorators.http import require_GET
from django.utils.timezone import now
import json
import logging
import os
from .models import SensorData

logger = logging.getLogger(__name__)

def index(request):
    return render(request, 'index.html')

@csrf_exempt
def receive_sensor_data(request):
    if request.method == 'POST':
        try:
            data = json.loads(request.body)
            node_id = data.get("node_id")
            temperature = data.get("temperature")
            humidity = data.get("humidity")

            x = node_id * 10 if node_id else 0
            y = 0  # You can adjust based on physical layout

            SensorData.objects.create(
                node_id=node_id,
                temperature=temperature,
                humidity=humidity,
                x=x,
                y=y,
            )

            logger.info(f"✅ Stored: Node {node_id}, Temp {temperature}, Humidity {humidity}")
            return JsonResponse({"status": "success"}, status=200)

        except Exception as e:
            logger.error(f"❌ Error saving data: {e}", exc_info=True)
            return JsonResponse({"status": "error", "message": str(e)}, status=400)

    return JsonResponse({"status": "only POST allowed"}, status=405)

@never_cache
def get_sensor_data(request):
    category = request.GET.get("category", "").lower()
    data = SensorData.objects.all().order_by("-timestamp")

    def extract(sensor, cat):
        return {
            "x": sensor.x,
            "y": sensor.y,
            "category": cat,
            "value": getattr(sensor, cat),
            "timestamp": sensor.timestamp.isoformat()
        }

    if category in ["temperature", "humidity"]:
        filtered = [extract(s, category) for s in data]
        return JsonResponse(filtered, safe=False)

    all_data = []
    for s in data:
        all_data.append(extract(s, "temperature"))
        all_data.append(extract(s, "humidity"))

    return JsonResponse(all_data, safe=False)

@require_GET
def ping_view(request):
    return JsonResponse({"status": "ok"})

def view_data_file(request):
    file_path = os.path.join(os.path.dirname(__file__), 'data.json')
    if os.path.exists(file_path):
        with open(file_path, 'r', encoding='utf-8') as f:
            data = json.load(f)
        return JsonResponse(data, safe=False)
    else:
        return JsonResponse({"status": "no data file found"}, status=404)
