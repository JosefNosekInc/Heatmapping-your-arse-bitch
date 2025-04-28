from django.shortcuts import render
from django.http import JsonResponse

# Example sensor data with both temperature and humidity for each sensor
sensor_data = [
    {"x": 150, "y": 100, "temperature": 22.5, "humidity": 60},
    {"x": 300, "y": 200, "temperature": 25.1, "humidity": 65},
    {"x": 450, "y": 300, "temperature": 20.8, "humidity": 55},
    {"x": 500, "y": 400, "temperature": 23.0, "humidity": 70},
]

# View to render the heatmap dashboard page
def heatmap_dashboard(request):
    return render(request, 'index.html')

# View to serve sensor data as JSON
def get_sensor_data(request):
    # You can filter based on the type (temperature or humidity)
    category = request.GET.get('category', None)

    if category:
        # Filter data based on category (either 'temperature' or 'humidity')
        filtered_data = []
        for sensor in sensor_data:
            if category == "temperature":
                filtered_data.append({"x": sensor["x"], "y": sensor["y"], "category": "temperature", "value": sensor["temperature"]})
            elif category == "humidity":
                filtered_data.append({"x": sensor["x"], "y": sensor["y"], "category": "humidity", "value": sensor["humidity"]})

        return JsonResponse(filtered_data, safe=False)

    # If no category is provided, return all data (with both temperature and humidity)
    all_data = []
    for sensor in sensor_data:
        all_data.append({"x": sensor["x"], "y": sensor["y"], "category": "temperature", "value": sensor["temperature"]})
        all_data.append({"x": sensor["x"], "y": sensor["y"], "category": "humidity", "value": sensor["humidity"]})

    return JsonResponse(all_data, safe=False)
