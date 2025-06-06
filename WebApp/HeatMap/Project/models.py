from django.db import models

class SensorData(models.Model):
    timestamp = models.DateTimeField(auto_now_add=True)
    node_id = models.IntegerField()
    x = models.FloatField()
    y = models.FloatField()
    temperature = models.FloatField()
    humidity = models.FloatField()

    def __str__(self):
        return f"Node {self.node_id} - Temp: {self.temperature}°C, Humidity: {self.humidity}% at {self.timestamp}"
