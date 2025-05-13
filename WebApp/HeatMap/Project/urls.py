from django.urls import path
from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('api/sensors/', views.get_sensor_data, name='get_sensor_data'),
]
