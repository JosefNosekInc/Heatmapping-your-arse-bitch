from django.db import models

class SensorData(models.Model):
    timestamp = models.DateTimeField(auto_now_add=True)
    x = models.FloatField()
    y = models.FloatField()
    temperature = models.FloatField()

    def __str__(self):
        return f"({self.x}, {self.y}) = {self.temperature}°C at {self.timestamp}"
