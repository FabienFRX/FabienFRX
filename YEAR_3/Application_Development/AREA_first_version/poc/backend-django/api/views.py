from rest_framework.decorators import api_view
from rest_framework.response import Response
from rest_framework import status
from django.utils import timezone
from .models import Service
from .serializers import ServiceSerializer, AboutSerializer
import time


@api_view(['GET'])
def about(request):
    client_ip = request.META.get('HTTP_X_FORWARDED_FOR')
    if client_ip:
        client_ip = client_ip.split(',')[0]
    else:
        client_ip = request.META.get('REMOTE_ADDR', 'unknown')

    services = Service.objects.all()
    services_data = ServiceSerializer(services, many=True).data

    data = {
        "client": {
            "host": client_ip
        },
        "server": {
            "current_time": int(time.time()),
            "services": services_data
        }
    }

    return Response(data)


@api_view(['GET'])
def hello_world(request):
    return Response({
        "message": "Hello AREA! Django is working perfectly!",
        "framework": "Django + DRF",
        "features": [
            "REST API ready",
            "Database models created",
            "CORS enabled",
            "Authentication ready"
        ]
    })
