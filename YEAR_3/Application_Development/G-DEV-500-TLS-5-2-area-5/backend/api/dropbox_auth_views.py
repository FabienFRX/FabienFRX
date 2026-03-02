from django.http import JsonResponse, HttpResponseRedirect
from django.conf import settings
from rest_framework.decorators import api_view, permission_classes
from rest_framework.permissions import IsAuthenticated
import requests
import logging
from .models import UserService, Service

logger = logging.getLogger(__name__)

# Dropbox OAuth2 configuration
DROPBOX_CLIENT_ID = getattr(settings, 'DROPBOX_CLIENT_ID', '')
DROPBOX_CLIENT_SECRET = getattr(settings, 'DROPBOX_CLIENT_SECRET', '')
DROPBOX_REDIRECT_URI = 'http://localhost:8080/api/auth/dropbox/callback'
DROPBOX_AUTH_URL = 'https://www.dropbox.com/oauth2/authorize'
DROPBOX_TOKEN_URL = 'https://api.dropboxapi.com/oauth2/token'


@api_view(['GET'])
@permission_classes([IsAuthenticated])
def dropbox_auth_init(request):
    """Initiate Dropbox OAuth2 flow"""
    try:
        # Encode user ID in state parameter instead of session
        import base64
        user_id_encoded = base64.urlsafe_b64encode(str(request.user.id).encode()).decode()
        
        # Build authorization URL
        auth_url = (
            f"{DROPBOX_AUTH_URL}"
            f"?client_id={DROPBOX_CLIENT_ID}"
            f"&response_type=code"
            f"&redirect_uri={DROPBOX_REDIRECT_URI}"
            f"&token_access_type=offline"  # Request refresh token
            f"&state={user_id_encoded}"  # Pass user_id in state
        )
        
        logger.info(f"Redirecting user {request.user.username} to Dropbox OAuth")
        return JsonResponse({'auth_url': auth_url})
        
    except Exception as e:
        logger.error(f"Error initiating Dropbox OAuth: {e}")
        return JsonResponse({'error': str(e)}, status=500)


@api_view(['GET'])
def dropbox_auth_callback(request):
    """Handle Dropbox OAuth2 callback"""
    try:
        code = request.GET.get('code')
        state = request.GET.get('state')
        error = request.GET.get('error')
        
        if error:
            logger.error(f"Dropbox OAuth error: {error}")
            return HttpResponseRedirect(f'http://localhost:8081/services?dropbox_error={error}')
        
        if not code:
            return JsonResponse({'error': 'No authorization code received'}, status=400)
        
        if not state:
            return JsonResponse({'error': 'No state parameter received'}, status=400)
        
        # Decode user ID from state parameter
        import base64
        try:
            user_id = int(base64.urlsafe_b64decode(state.encode()).decode())
        except Exception as e:
            logger.error(f"Failed to decode state parameter: {e}")
            return JsonResponse({'error': 'Invalid state parameter'}, status=400)
        
        # Exchange code for tokens
        token_response = requests.post(DROPBOX_TOKEN_URL, data={
            'code': code,
            'grant_type': 'authorization_code',
            'client_id': DROPBOX_CLIENT_ID,
            'client_secret': DROPBOX_CLIENT_SECRET,
            'redirect_uri': DROPBOX_REDIRECT_URI
        })
        
        if token_response.status_code != 200:
            logger.error(f"Failed to exchange code for tokens: {token_response.text}")
            return JsonResponse({'error': 'Failed to get access token'}, status=400)
        
        token_data = token_response.json()
        access_token = token_data.get('access_token')
        refresh_token = token_data.get('refresh_token')
        
        if not access_token:
            return JsonResponse({'error': 'No access token in response'}, status=400)
        
        # Get Dropbox service
        from django.contrib.auth.models import User
        user = User.objects.get(id=user_id)
        dropbox_service = Service.objects.filter(name='dropbox').first()
        
        if not dropbox_service:
            # Create Dropbox service if it doesn't exist
            dropbox_service = Service.objects.create(
                name='dropbox',
                description='Dropbox file storage and sharing'
            )
            logger.info("Created Dropbox service in database")
        
        # Store or update tokens
        user_service, created = UserService.objects.update_or_create(
            user=user,
            service=dropbox_service,
            defaults={
                'access_token': access_token,
                'refresh_token': refresh_token,
                'is_active': True
            }
        )
        
        action = 'connected' if created else 'reconnected'
        logger.info(f"User {user.username} {action} Dropbox successfully")
        
        # Redirect to frontend
        return HttpResponseRedirect('http://localhost:8081/services?dropbox_connected=true')
        
    except Exception as e:
        logger.error(f"Error in Dropbox callback: {e}")
        return HttpResponseRedirect(f'http://localhost:8081/services?dropbox_error={str(e)}')


@api_view(['POST'])
@permission_classes([IsAuthenticated])
def dropbox_disconnect(request):
    """Disconnect Dropbox account"""
    try:
        dropbox_service = Service.objects.filter(name='dropbox').first()
        if not dropbox_service:
            return JsonResponse({'error': 'Dropbox service not found'}, status=404)
        
        # Delete user's Dropbox connection
        deleted_count = UserService.objects.filter(
            user=request.user,
            service=dropbox_service
        ).delete()[0]
        
        if deleted_count > 0:
            logger.info(f"User {request.user.username} disconnected Dropbox")
            return JsonResponse({'message': 'Dropbox disconnected successfully'})
        else:
            return JsonResponse({'error': 'Dropbox not connected'}, status=404)
        
    except Exception as e:
        logger.error(f"Error disconnecting Dropbox: {e}")
        return JsonResponse({'error': str(e)}, status=500)


@api_view(['GET'])
@permission_classes([IsAuthenticated])
def dropbox_status(request):
    """Check if user has connected Dropbox"""
    try:
        dropbox_service = Service.objects.filter(name='dropbox').first()
        if not dropbox_service:
            return JsonResponse({'connected': False})
        
        user_service = UserService.objects.filter(
            user=request.user,
            service=dropbox_service,
            is_active=True
        ).first()
        
        if user_service:
            return JsonResponse({
                'connected': True,
                'service': 'dropbox'
            })
        else:
            return JsonResponse({'connected': False})
        
    except Exception as e:
        logger.error(f"Error checking Dropbox status: {e}")
        return JsonResponse({'error': str(e)}, status=500)
