# views.py

from django.shortcuts import render

def heatmap_dashboard(request):
    """
    View to render the Heatmap Dashboard page.
    """
    return render(request, 'index.html')
