from django.urls import path
from . import views

urlpatterns = [
path('', views.index, name='index'),
path('api/data/', views.receive_sensor_data, name='receive_sensor_data'),
path('api/sensor-data/', views.get_sensor_data, name='get_sensor_data'),
path('api/ping/', views.ping_view, name='ping'),
path('api/view-data-file/', views.view_data_file, name='view_data_file'),
]
