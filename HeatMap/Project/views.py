from django.shortcuts import render, get_object_or_404, redirect
from .models import Order, Robotino, Station
from django_interface.functions import read_log
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Heatmap Dashboard</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
    <style>
        #heatmap {
            width: 100%;
            height: 500px;
            background-color: #f1f1f1;
            border: 2px dashed #ccc;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 1.5rem;
            color: #999;
        }
    </style>
</head>
<body class="bg-light">

    <div class="container mt-5">
        <!-- Header -->
        <h1 class="mb-4 text-center">Heatmap Dashboard</h1>

        <!-- Filter Dropdown -->
        <div class="mb-3 text-center">
            <label for="filterDropdown" class="form-label fw-bold">Filter by Category:</label>
            <select class="form-select w-50 mx-auto" id="filterDropdown">
                <option selected>Choose a filter</option>
                <option value="temperature">Temperature</option>
                <option value="humidity">Humidity</option>
            </select>
        </div>

        <!-- Heatmap Placeholder -->
        <div id="heatmap">
            [ Heatmap will load here ]
        </div>
    </div>

    <!-- Scripts -->
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js"></script>
    <script>
        const dropdown = document.getElementById('filterDropdown');
        dropdown.addEventListener('change', () => {
            const selected = dropdown.value;
            document.getElementById('heatmap').textContent = `[ Loading heatmap for: ${selected} ]`;
            // Optional: Add logic here to load actual data or map visualization
        });
    </script>

</body>
</html>
