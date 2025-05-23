from django.urls import path
from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('api/sensor_data/', views.get_sensor_data, name='get_sensor_data'),
    path('api/sensors/', views.get_sensor_data), 
    path('api/receive_sensor_data/', views.receive_sensor_data, name='receive_sensor_data'),
]
