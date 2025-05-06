from django.shortcuts import render
from django.http import JsonResponse
from .models import SensorData

def index(request):
    return render(request, 'index.html')

def get_sensor_data(request):
    data = SensorData.objects.all().values()
    return JsonResponse(list(data), safe=False)
