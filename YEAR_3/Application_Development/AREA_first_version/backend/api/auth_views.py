from rest_framework import status
from rest_framework.decorators import permission_classes
from rest_framework.permissions import IsAuthenticated, AllowAny
from rest_framework.response import Response
from rest_framework.views import APIView
from rest_framework.authtoken.models import Token
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.models import User
from django.db import IntegrityError
from .models import UserService, Service
from .serializers import UserSerializer, UserServiceSerializer
import json
import requests


class SignUpView(APIView):
    permission_classes = [AllowAny]

    def post(self, request):
        """User registration endpoint"""
        try:
            data = request.data
            email = data.get('email')
            password = data.get('password')
            first_name = data.get('first_name', '')
            last_name = data.get('last_name', '')

            if not email or not password:
                return Response({
                    'error': 'Email and password are required'
                }, status=status.HTTP_400_BAD_REQUEST)

            # Create user
            user = User.objects.create_user(
                username=email,
                email=email,
                password=password,
                first_name=first_name,
                last_name=last_name
            )

            # Create token
            token, created = Token.objects.get_or_create(user=user)

            return Response({
                'message': 'User created successfully',
                'user': UserSerializer(user).data,
                'token': token.key
            }, status=status.HTTP_201_CREATED)

        except IntegrityError:
            return Response({
                'error': 'User with this email already exists'
            }, status=status.HTTP_400_BAD_REQUEST)
        except Exception as e:
            return Response({
                'error': str(e)
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


class LoginView(APIView):
    permission_classes = [AllowAny]

    def post(self, request):
        """User login endpoint"""
        try:
            data = request.data
            email = data.get('email')
            password = data.get('password')

            if not email or not password:
                return Response({
                    'error': 'Email and password are required'
                }, status=status.HTTP_400_BAD_REQUEST)

            user = authenticate(request, username=email, password=password)

            if user:
                login(request, user)
                token, created = Token.objects.get_or_create(user=user)

                return Response({
                    'message': 'Login successful',
                    'user': UserSerializer(user).data,
                    'token': token.key
                }, status=status.HTTP_200_OK)
            else:
                return Response({
                    'error': 'Invalid credentials'
                }, status=status.HTTP_401_UNAUTHORIZED)

        except Exception as e:
            return Response({
                'error': str(e)
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


class LogoutView(APIView):
    permission_classes = [IsAuthenticated]

    def post(self, request):
        """User logout endpoint"""
        try:
            # Delete the user's token
            request.user.auth_token.delete()
            logout(request)

            return Response({
                'message': 'Logout successful'
            }, status=status.HTTP_200_OK)

        except Exception as e:
            return Response({
                'error': str(e)
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


class ProfileView(APIView):
    permission_classes = [IsAuthenticated]

    def get(self, request):
        """Get user profile"""
        try:
            user_services = UserService.objects.filter(user=request.user, is_active=True)

            return Response({
                'user': UserSerializer(request.user).data,
                'connected_services': UserServiceSerializer(user_services, many=True).data
            }, status=status.HTTP_200_OK)

        except Exception as e:
            return Response({
                'error': str(e)
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


class GoogleLoginView(APIView):
    permission_classes = [AllowAny]

    def post(self, request):
        """Google OAuth2 login/signup endpoint"""
        try:
            data = request.data
            access_token = data.get('access_token')

            if not access_token:
                return Response({
                    'error': 'Access token is required'
                }, status=status.HTTP_400_BAD_REQUEST)

            # Verify token with Google and get user info
            google_user_info_url = f'https://www.googleapis.com/oauth2/v2/userinfo?access_token={access_token}'
            response = requests.get(google_user_info_url)

            if response.status_code != 200:
                return Response({
                    'error': 'Invalid Google access token'
                }, status=status.HTTP_400_BAD_REQUEST)

            google_user_data = response.json()
            email = google_user_data.get('email')
            first_name = google_user_data.get('given_name', '')
            last_name = google_user_data.get('family_name', '')
            google_id = google_user_data.get('id')

            if not email:
                return Response({
                    'error': 'Unable to retrieve email from Google'
                }, status=status.HTTP_400_BAD_REQUEST)

            # Check if user exists by email or username, create if not
            user = None
            try:
                # Try to find user by email first
                user = User.objects.get(email=email)
            except User.DoesNotExist:
                try:
                    # Try by username (in case username is the email)
                    user = User.objects.get(username=email)
                except User.DoesNotExist:
                    # Create new user
                    user = User.objects.create_user(
                        username=email,
                        email=email,
                        first_name=first_name,
                        last_name=last_name,
                    )
            except User.MultipleObjectsReturned:
                # If multiple users have the same email, get the first one
                user = User.objects.filter(email=email).first()

            # Get or create token
            token, created = Token.objects.get_or_create(user=user)

            # Auto-connect Google service
            google_service, created = Service.objects.get_or_create(
                name='google',
                defaults={'description': 'Google OAuth2 Service'}
            )

            # Calculate token expiration
            token_expires_at = None
            expires_in = data.get('expires_in')
            if expires_in:
                from django.utils import timezone
                from datetime import timedelta
                token_expires_at = timezone.now() + timedelta(seconds=int(expires_in))

            # Create or update user service connection
            UserService.objects.update_or_create(
                user=user,
                service=google_service,
                defaults={
                    'access_token': access_token,
                    'refresh_token': data.get('refresh_token', ''),
                    'token_expires_at': token_expires_at,
                    'service_user_id': google_id,
                    'service_username': email,
                    'is_active': True
                }
            )

            return Response({
                'message': 'Google login successful',
                'user': UserSerializer(user).data,
                'token': token.key,
                'google_service_connected': True
            }, status=status.HTTP_200_OK)

        except Exception as e:
            return Response({
                'error': str(e)
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


class GoogleConnectView(APIView):
    permission_classes = [IsAuthenticated]

    def post(self, request):
        """Connect Google OAuth2 service"""
        try:
            data = request.data
            access_token = data.get('access_token')
            refresh_token = data.get('refresh_token', '')
            expires_in = data.get('expires_in')
            user_info = data.get('user_info', {})

            if not access_token:
                return Response({
                    'error': 'Access token is required'
                }, status=status.HTTP_400_BAD_REQUEST)

            # Get or create Google service
            google_service, created = Service.objects.get_or_create(
                name='google',
                defaults={'description': 'Google OAuth2 Service'}
            )

            # Calculate token expiration
            token_expires_at = None
            if expires_in:
                from django.utils import timezone
                from datetime import timedelta
                token_expires_at = timezone.now() + timedelta(seconds=int(expires_in))

            # Create or update user service connection
            user_service, created = UserService.objects.update_or_create(
                user=request.user,
                service=google_service,
                defaults={
                    'access_token': access_token,
                    'refresh_token': refresh_token,
                    'token_expires_at': token_expires_at,
                    'service_user_id': user_info.get('id', ''),
                    'service_username': user_info.get('email', ''),
                    'is_active': True
                }
            )

            return Response({
                'message': 'Google service connected successfully',
                'service': UserServiceSerializer(user_service).data
            }, status=status.HTTP_200_OK)

        except Exception as e:
            return Response({
                'error': str(e)
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


class UserServicesView(APIView):
    permission_classes = [IsAuthenticated]

    def get(self, request):
        """Get user's connected services"""
        try:
            user_services = UserService.objects.filter(user=request.user, is_active=True)

            return Response({
                'services': UserServiceSerializer(user_services, many=True).data
            }, status=status.HTTP_200_OK)

        except Exception as e:
            return Response({
                'error': str(e)
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

    def delete(self, request):
        """Disconnect a service"""
        try:
            service_id = request.data.get('service_id')

            if not service_id:
                return Response({
                    'error': 'Service ID is required'
                }, status=status.HTTP_400_BAD_REQUEST)

            user_service = UserService.objects.get(
                user=request.user,
                service_id=service_id,
                is_active=True
            )

            user_service.is_active = False
            user_service.save()

            return Response({
                'message': 'Service disconnected successfully'
            }, status=status.HTTP_200_OK)

        except UserService.DoesNotExist:
            return Response({
                'error': 'Service connection not found'
            }, status=status.HTTP_404_NOT_FOUND)
        except Exception as e:
            return Response({
                'error': str(e)
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)