from django.urls import path
from .views import heatmap_dashboard, get_sensor_data

urlpatterns = [
    # Route to render the heatmap dashboard
    path('', heatmap_dashboard, name='heatmap-dashboard'),

    # API endpoint to serve the sensor data
    path('api/sensors/', get_sensor_data, name='sensor-data'),
]
