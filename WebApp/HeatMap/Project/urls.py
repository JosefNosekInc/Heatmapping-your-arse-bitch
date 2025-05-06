from django.urls import path
from . import views  # Safe here because views is NOT importing urls

urlpatterns = [
    path('', views.index, name='index'),
    path('get-sensor-data/', views.get_sensor_data, name='get_sensor_data'),
]