from django.urls import path
from . import views
from .views import ping_view
from .views import view_data_file

urlpatterns = [
    path('', views.index, name='index'),
    path('api/sensor_data/', views.get_sensor_data, name='get_sensor_data'),
    path('api/sensors/', views.get_sensor_data),
    path('api/receive_sensor_data/', views.receive_sensor_data, name='receive_sensor_data'),
    path('api/ping/', ping_view, name='ping'),  
    path('view-data/', view_data_file),
    path('api/data/', views.receive_sensor_data, name='receive_sensor_data'),
]
