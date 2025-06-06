from django.urls import path
from .views import index, receive_sensor_data, get_sensor_data, ping_view, view_data_file, dashboard

urlpatterns = [
    path('', index, name='index'),
    path('api/receive_sensor_data/', receive_sensor_data, name='receive_sensor_data'),
    path('api/sensors/', get_sensor_data, name='get_sensor_data'),
    path('api/ping/', ping_view, name='ping_view'),
    path('api/view_data_file/', view_data_file, name='view_data_file'),
    path('dashboard/', dashboard, name='dashboard'),
    
]
