import time
import dht
from machine import Pin

# Define the DHT22 sensor connected to GPIO 15
sensor = dht.DHT22(Pin(15))

while True:
    try:
        sensor.measure()  # Trigger a reading
        temperature = sensor.temperature()  # In °C
        humidity = sensor.humidity()        # In %

        print('Temperature: {:.1f}°C'.format(temperature))
        print('Humidity: {:.1f}%'.format(humidity))
    except OSError as e:
        print('Failed to read sensor.')

    time.sleep(2)  # Wait 2 seconds between readings
