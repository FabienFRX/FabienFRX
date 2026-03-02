from rest_framework import status
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from rest_framework.views import APIView
from django.shortcuts import get_object_or_404
from .models import Service, Area, Action, Reaction, UserService
from .serializers import ServiceSerializer, AreaSerializer


class ServicesListView(APIView):
    permission_classes = [IsAuthenticated]

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
            action_id = data.get('action_id')
            reaction_id = data.get('reaction_id')

            if not name or not action_id or not reaction_id:
                return Response({
                    'error': 'name, action_id, and reaction_id are required'
                }, status=status.HTTP_400_BAD_REQUEST)

            # Verify action and reaction exist
            try:
                action = Action.objects.get(id=action_id)
                reaction = Reaction.objects.get(id=reaction_id)
            except (Action.DoesNotExist, Reaction.DoesNotExist):
                return Response({
                    'error': 'Invalid action_id or reaction_id'
                }, status=status.HTTP_400_BAD_REQUEST)

            # Verify user has connected services for both action and reaction
            user_services = UserService.objects.filter(
                user=request.user,
                is_active=True
            ).values_list('service_id', flat=True)

            if action.service.id not in user_services:
                return Response({
                    'error': f'You must connect {action.service.name} service first'
                }, status=status.HTTP_400_BAD_REQUEST)

            if reaction.service.id not in user_services:
                return Response({
                    'error': f'You must connect {reaction.service.name} service first'
                }, status=status.HTTP_400_BAD_REQUEST)

            # Create AREA
            area = Area.objects.create(
                user=request.user,
                name=name,
                action=action,
                reaction=reaction,
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