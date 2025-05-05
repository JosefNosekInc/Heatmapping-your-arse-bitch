from django.urls import path
from .views import heatmap_dashboard, get_sensor_data
from django.contrib import admin
from django.urls import path, include
from django.urls import path
from . import views


urlpatterns = [
    # Route to render the heatmap dashboard
    path('', heatmap_dashboard, name='heatmap-dashboard'),

    # API endpoint to serve the sensor data
    path('api/sensors/', get_sensor_data, name='sensor-data'),
]