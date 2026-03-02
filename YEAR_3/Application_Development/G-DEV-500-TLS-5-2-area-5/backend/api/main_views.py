from rest_framework import status
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from rest_framework.views import APIView
from django.shortcuts import get_object_or_404
from .models import Service, Area, Action, Reaction, UserService, AreaExecution
from .serializers import ServiceSerializer, AreaSerializer
from .workflow_executor import start_workflow_executor, stop_workflow_executor, executor
import logging


class ServicesListView(APIView):
    permission_classes = []

    def get(self, request):
        """Get list of all available services"""
        try:
            services = Service.objects.all()
            services_data = ServiceSerializer(services, many=True).data

            return Response({
                'services': services_data
            }, status=status.HTTP_200_OK)

        except Exception as e:
            return Response({
                'error': str(e)
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


class UserAreasView(APIView):
    permission_classes = [IsAuthenticated]

    def get(self, request):
        """Get user's AREAs"""
        try:
            areas = Area.objects.filter(user=request.user).order_by('-created_at')
            areas_data = AreaSerializer(areas, many=True).data

            return Response({
                'areas': areas_data
            }, status=status.HTTP_200_OK)

        except Exception as e:
            return Response({
                'error': str(e)
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

    def post(self, request):
        """Create a new AREA"""
        try:
            data = request.data
            name = data.get('name')

            # Support both old format (action_id/reaction_id) and new format (service/type)
            action_id = data.get('action_id')
            reaction_id = data.get('reaction_id')
            action_service = data.get('action_service')
            action_type = data.get('action_type')
            reaction_service = data.get('reaction_service')
            reaction_type = data.get('reaction_type')

            if not name:
                return Response({
                    'error': 'name is required'
                }, status=status.HTTP_400_BAD_REQUEST)

            # If using new format (service/type), convert to IDs
            if action_service and action_type and reaction_service and reaction_type:
                try:
                    action = Action.objects.get(
                        service__name__iexact=action_service,
                        name=action_type
                    )
                    reaction = Reaction.objects.get(
                        service__name__iexact=reaction_service,
                        name=reaction_type
                    )
                except Action.DoesNotExist:
                    return Response({
                        'error': f'Action "{action_type}" not found for service "{action_service}"'
                    }, status=status.HTTP_400_BAD_REQUEST)
                except Reaction.DoesNotExist:
                    return Response({
                        'error': f'Reaction "{reaction_type}" not found for service "{reaction_service}"'
                    }, status=status.HTTP_400_BAD_REQUEST)

            # If using old format (IDs), get objects directly
            elif action_id and reaction_id:
                try:
                    action = Action.objects.get(id=action_id)
                    reaction = Reaction.objects.get(id=reaction_id)
                except (Action.DoesNotExist, Reaction.DoesNotExist):
                    return Response({
                        'error': 'Invalid action_id or reaction_id'
                    }, status=status.HTTP_400_BAD_REQUEST)
            else:
                return Response({
                    'error': 'Either action_id/reaction_id or action_service/action_type/reaction_service/reaction_type are required'
                }, status=status.HTTP_400_BAD_REQUEST)

            # Verify user has connected services for both action and reaction
            # Timer service doesn't require connection (no OAuth needed)
            user_services = UserService.objects.filter(
                user=request.user,
                is_active=True
            ).values_list('service_id', flat=True)

            # Check action service connection (skip timer service)
            if action.service.name != 'timer' and action.service.id not in user_services:
                return Response({
                    'error': f'You must connect {action.service.name} service first'
                }, status=status.HTTP_400_BAD_REQUEST)

            # Check reaction service connection (skip timer service)
            if reaction.service.name != 'timer' and reaction.service.id not in user_services:
                return Response({
                    'error': f'You must connect {reaction.service.name} service first'
                }, status=status.HTTP_400_BAD_REQUEST)

            # Get configurations
            description = data.get('description', '')
            action_config = data.get('action_config', {})
            reaction_config = data.get('reaction_config', {})

            # Create AREA
            area = Area.objects.create(
                user=request.user,
                name=name,
                description=description,
                action=action,
                reaction=reaction,
                action_config=action_config,
                reaction_config=reaction_config,
                enabled=True
            )

            return Response({
                'message': 'AREA created successfully',
                'area': AreaSerializer(area).data
            }, status=status.HTTP_201_CREATED)

        except Exception as e:
            return Response({
                'error': str(e)
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


class UserAreaDetailView(APIView):
    permission_classes = [IsAuthenticated]

    def patch(self, request, area_id):
        """Toggle AREA active/inactive or update"""
        try:
            area = get_object_or_404(Area, id=area_id, user=request.user)

            data = request.data

            # Toggle enabled status if provided
            if 'enabled' in data:
                area.enabled = data['enabled']
                area.save()

            # Update name if provided
            if 'name' in data:
                area.name = data['name']
                area.save()

            return Response({
                'message': 'AREA updated successfully',
                'area': AreaSerializer(area).data
            }, status=status.HTTP_200_OK)

        except Exception as e:
            return Response({
                'error': str(e)
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

    def delete(self, request, area_id):
        """Delete an AREA"""
        try:
            area = get_object_or_404(Area, id=area_id, user=request.user)
            area_name = area.name
            area.delete()

            return Response({
                'message': f'AREA "{area_name}" deleted successfully'
            }, status=status.HTTP_200_OK)

        except Exception as e:
            return Response({
                'error': str(e)
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


class UserServiceDetailView(APIView):
    permission_classes = [IsAuthenticated]

    def delete(self, request, service_id):
        """Disconnect a specific service"""
        try:
            user_service = get_object_or_404(
                UserService,
                id=service_id,
                user=request.user,
                is_active=True
            )

            service_name = user_service.service.name
            user_service.is_active = False
            user_service.save()

            return Response({
                'message': f'{service_name} service disconnected successfully'
            }, status=status.HTTP_200_OK)

        except Exception as e:
            return Response({
                'error': str(e)
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


class WorkflowExecutorView(APIView):
    """Control the workflow executor for MVP demonstration"""
    permission_classes = [IsAuthenticated]

    def get(self, request):
        """Get executor status"""
        active_areas = Area.objects.filter(enabled=True).count()
        return Response({
            'status': 'running via Celery',
            'active_areas': active_areas,
            'message': 'Workflow executor is managed by Celery Beat (automatic)'
        })

    def post(self, request):
        """Manually trigger workflow check"""
        from api.tasks import check_all_workflows
        
        # Trigger immediate workflow check
        result = check_all_workflows.delay()
        
        return Response({
            'message': 'Manual workflow check triggered',
            'task_id': result.id,
            'status': 'processing'
        })

    def delete(self, request):
        """Stop the workflow executor"""
        if executor.running:
            stop_workflow_executor()
            return Response({
                'message': 'Workflow executor stopped',
                'status': 'stopped'
            })
        else:
            return Response({
                'message': 'Workflow executor is already stopped',
                'status': 'stopped'
            })