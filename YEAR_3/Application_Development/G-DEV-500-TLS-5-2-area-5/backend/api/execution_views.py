"""
Additional views for AREA execution monitoring
"""
from rest_framework import status
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from rest_framework.views import APIView
from django.shortcuts import get_object_or_404
from .models import Area, AreaExecution


class AreaExecutionsView(APIView):
    """View execution history for AREAs"""
    permission_classes = [IsAuthenticated]

    def get(self, request, area_id=None):
        """Get execution history"""
        try:
            if area_id:
                # Get executions for specific AREA
                area = get_object_or_404(Area, id=area_id, user=request.user)
                executions = AreaExecution.objects.filter(area=area)[:20]  # Last 20
            else:
                # Get all user's AREA executions
                user_areas = Area.objects.filter(user=request.user)
                executions = AreaExecution.objects.filter(area__in=user_areas)[:50]  # Last 50

            executions_data = [{
                'id': ex.id,
                'area_id': ex.area.id,
                'area_name': ex.area.name,
                'executed_at': ex.executed_at.isoformat(),
                'status': ex.status,
                'action_data': ex.action_data,
                'reaction_result': ex.reaction_result,
                'error_message': ex.error_message
            } for ex in executions]

            return Response({
                'executions': executions_data,
                'count': len(executions_data)
            }, status=status.HTTP_200_OK)

        except Exception as e:
            return Response({
                'error': str(e)
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


class CeleryHealthView(APIView):
    """Check Celery worker and beat health"""
    permission_classes = []

    def get(self, request):
        """Check if Celery is running"""
        try:
            from celery_app import app as celery_app
            
            # Check if workers are active
            inspect = celery_app.control.inspect()
            active_workers = inspect.active()
            
            if active_workers:
                return Response({
                    'status': 'healthy',
                    'workers': list(active_workers.keys()),
                    'message': 'Celery workers are running'
                }, status=status.HTTP_200_OK)
            else:
                return Response({
                    'status': 'unhealthy',
                    'workers': [],
                    'message': 'No Celery workers detected'
                }, status=status.HTTP_503_SERVICE_UNAVAILABLE)
                
        except Exception as e:
            return Response({
                'status': 'error',
                'error': str(e),
                'message': 'Cannot connect to Celery'
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)
