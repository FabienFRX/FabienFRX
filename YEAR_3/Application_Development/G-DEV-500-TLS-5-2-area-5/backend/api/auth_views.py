from rest_framework import status
from rest_framework.decorators import permission_classes
from rest_framework.permissions import IsAuthenticated, AllowAny
from rest_framework.response import Response
from rest_framework.views import APIView
from rest_framework.authtoken.models import Token
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.models import User
from django.db import IntegrityError
from django.shortcuts import redirect
from django.http import HttpResponse
from .models import UserService, Service
from .serializers import UserSerializer, UserServiceSerializer
from allauth.socialaccount.models import SocialAccount, SocialToken, SocialApp
import json
import requests
import os
import logging
from urllib.parse import urlencode

logger = logging.getLogger(__name__)


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


class GoogleOAuthInitView(APIView):
    """Initiate Google OAuth flow - redirects user to Google"""
    permission_classes = [AllowAny]

    def get(self, request):
        client_id = os.getenv('GOOGLE_OAUTH2_CLIENT_ID')
        redirect_uri = f"{os.getenv('BACKEND_URL', 'http://localhost:8080')}/api/auth/google/callback/"
        
        params = {
            'client_id': client_id,
            'redirect_uri': redirect_uri,
            'response_type': 'code',
            'scope': 'openid email profile https://www.googleapis.com/auth/gmail.readonly https://www.googleapis.com/auth/gmail.send https://www.googleapis.com/auth/calendar https://www.googleapis.com/auth/drive',
            'access_type': 'offline',
            'prompt': 'consent',
        }
        
        auth_url = f"https://accounts.google.com/o/oauth2/v2/auth?{urlencode(params)}"
        return redirect(auth_url)


class GoogleOAuthCallbackView(APIView):
    """Handle Google OAuth callback - exchanges code for token and redirects to frontend"""
    permission_classes = [AllowAny]

    def get(self, request):
        code = request.GET.get('code')
        error = request.GET.get('error')
        
        frontend_url = os.getenv('FRONTEND_URL', 'http://localhost:8081')
        
        if error:
            return redirect(f"{frontend_url}/login?error={error}")
        
        if not code:
            return redirect(f"{frontend_url}/login?error=no_code")
        
        try:
            # Exchange code for tokens
            token_url = 'https://oauth2.googleapis.com/token'
            redirect_uri = f"{os.getenv('BACKEND_URL', 'http://localhost:8080')}/api/auth/google/callback/"
            
            token_data = {
                'code': code,
                'client_id': os.getenv('GOOGLE_OAUTH2_CLIENT_ID'),
                'client_secret': os.getenv('GOOGLE_OAUTH2_CLIENT_SECRET'),
                'redirect_uri': redirect_uri,
                'grant_type': 'authorization_code',
            }
            
            token_response = requests.post(token_url, data=token_data)
            if token_response.status_code != 200:
                return redirect(f"{frontend_url}/login?error=token_exchange_failed")
            
            tokens = token_response.json()
            access_token = tokens.get('access_token')
            refresh_token = tokens.get('refresh_token')
            expires_in = tokens.get('expires_in')
            
            # Get user info
            user_info_url = f'https://www.googleapis.com/oauth2/v2/userinfo?access_token={access_token}'
            user_response = requests.get(user_info_url)
            
            if user_response.status_code != 200:
                return redirect(f"{frontend_url}/login?error=user_info_failed")
            
            user_data = user_response.json()
            email = user_data.get('email')
            first_name = user_data.get('given_name', '')
            last_name = user_data.get('family_name', '')
            google_id = user_data.get('id')
            
            # Get or create user
            user = None
            try:
                user = User.objects.get(email=email)
            except User.DoesNotExist:
                try:
                    user = User.objects.get(username=email)
                except User.DoesNotExist:
                    user = User.objects.create_user(
                        username=email,
                        email=email,
                        first_name=first_name,
                        last_name=last_name,
                    )
            except User.MultipleObjectsReturned:
                user = User.objects.filter(email=email).first()
            
            # Create token
            token, created = Token.objects.get_or_create(user=user)
            
            # Connect Google service
            google_service, created = Service.objects.get_or_create(
                name='google',
                defaults={'description': 'Google OAuth2 Service'}
            )
            
            token_expires_at = None
            if expires_in:
                from django.utils import timezone
                from datetime import timedelta
                token_expires_at = timezone.now() + timedelta(seconds=int(expires_in))
            
            UserService.objects.update_or_create(
                user=user,
                service=google_service,
                defaults={
                    'access_token': access_token,
                    'refresh_token': refresh_token,
                    'token_expires_at': token_expires_at,
                    'service_user_id': google_id,
                    'service_username': email,
                    'is_active': True
                }
            )
            
            # Redirect to frontend with token
            return redirect(f"{frontend_url}/login?token={token.key}&user={email}")
            
        except Exception as e:
            logger.error(f"Google OAuth callback error: {str(e)}")
            return redirect(f"{frontend_url}/login?error=server_error")


class GoogleLoginView(APIView):
    permission_classes = [AllowAny]

    def post(self, request):
        """Google OAuth2 login/signup endpoint"""
        try:
            data = request.data
            access_token = data.get('access_token')
            authorization_code = data.get('authorization_code')
            redirect_uri = data.get('redirect_uri', 'postmessage')  # Accept redirect_uri from frontend

            # Handle authorization code flow (preferred for refresh token)
            if authorization_code:
                token_data = self._exchange_code_for_tokens(authorization_code, redirect_uri)
                if not token_data:
                    return Response({
                        'error': 'Failed to exchange authorization code for tokens'
                    }, status=status.HTTP_400_BAD_REQUEST)

                access_token = token_data.get('access_token')
                refresh_token = token_data.get('refresh_token')
                expires_in = token_data.get('expires_in')

            # Handle direct access token (fallback)
            elif access_token:
                refresh_token = data.get('refresh_token', '')
                expires_in = data.get('expires_in')
            else:
                return Response({
                    'error': 'Either access_token or authorization_code is required'
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
                    'refresh_token': refresh_token,
                    'token_expires_at': token_expires_at,
                    'service_user_id': google_id,
                    'service_username': email,
                    'is_active': True
                }
            )

            # Also create SocialAccount and SocialToken for django-allauth compatibility
            from allauth.socialaccount.models import SocialAccount, SocialToken, SocialApp
            try:
                social_app = SocialApp.objects.get(provider='google')

                # Create or update SocialAccount
                social_account, _ = SocialAccount.objects.update_or_create(
                    user=user,
                    provider='google',
                    defaults={
                        'uid': google_id,
                        'extra_data': google_user_data
                    }
                )

                # Create or update SocialToken
                SocialToken.objects.update_or_create(
                    account=social_account,
                    app=social_app,
                    defaults={
                        'token': access_token,
                        'token_secret': refresh_token or '',
                        'expires_at': token_expires_at
                    }
                )
            except SocialApp.DoesNotExist:
                logger.warning("SocialApp for Google not found, skipping SocialAccount creation")

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

    def _exchange_code_for_tokens(self, authorization_code, redirect_uri=None):
        """Exchange authorization code for access and refresh tokens"""
        try:
            import os

            client_id = os.getenv('GOOGLE_OAUTH2_CLIENT_ID')
            client_secret = os.getenv('GOOGLE_OAUTH2_CLIENT_SECRET')
            
            # Try potential redirect_uris
            backend_url = os.getenv('BACKEND_URL', 'http://localhost:8080')
            candidate_uris = [
                '',  # Mobile often sends empty string (Try FIRST as requested)
                redirect_uri, # Try passed URI (often 'postmessage' default)
                'postmessage',  # Web often uses this
                f"{backend_url}/api/auth/google/callback/", # Explicit callback
                os.getenv('GOOGLE_OAUTH2_REDIRECT_URI') # Configured env var
            ]
            
            # Remove duplicates and None
            candidate_uris = list(dict.fromkeys([u for u in candidate_uris if u is not None]))

            if not client_id or not client_secret:
                logger.error("Missing Google OAuth2 client credentials")
                return None

            token_url = 'https://oauth2.googleapis.com/token'
            token_data = {
                'client_id': client_id,
                'client_secret': client_secret,
                'code': authorization_code,
                'grant_type': 'authorization_code',
            }

            for uri in candidate_uris:
                token_data['redirect_uri'] = uri
                logger.debug(f"Exchanging code with redirect_uri: '{uri}'")
                
                response = requests.post(token_url, data=token_data)

                if response.status_code == 200:
                    logger.info(f"Successfully exchanged code with redirect_uri: '{uri}'")
                    return response.json()
                else:
                    logger.warning(f"Failed exchange with redirect_uri '{uri}': {response.text}")
            
            # If all fail, log the last error
            logger.error(f"Failed to exchange code for tokens. All candidates failed. Last response: {response.text}")
            return None

        except Exception as e:
            logger.error(f"Error exchanging code for tokens: {e}")
            return None


class GoogleConnectView(APIView):
    permission_classes = [IsAuthenticated]

    def post(self, request):
        """Connect Google OAuth2 service"""
        try:
            data = request.data
            access_token = data.get('access_token')
            authorization_code = data.get('code') or data.get('authorization_code')  # Support both field names
            refresh_token = data.get('refresh_token', '')
            expires_in = data.get('expires_in')
            user_info = data.get('user_info', {})

            # Handle authorization code flow (preferred for refresh token)
            if authorization_code:
                token_data = self._exchange_code_for_tokens(authorization_code)
                if not token_data:
                    return Response({
                        'error': 'Failed to exchange authorization code for tokens'
                    }, status=status.HTTP_400_BAD_REQUEST)

                access_token = token_data.get('access_token')
                refresh_token = token_data.get('refresh_token')
                expires_in = token_data.get('expires_in')

                # Get user info with the access token
                google_user_info_url = f'https://www.googleapis.com/oauth2/v2/userinfo?access_token={access_token}'
                response = requests.get(google_user_info_url)
                if response.status_code == 200:
                    user_info = response.json()

            elif not access_token:
                return Response({
                    'error': 'Either access token or authorization code is required'
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

            # ALSO create django-allauth SocialAccount and SocialToken
            try:
                # Get the Google OAuth app
                google_app = SocialApp.objects.get(provider='google')

                # Create or get social account
                social_account, created = SocialAccount.objects.get_or_create(
                    user=request.user,
                    provider='google',
                    uid=user_info.get('id', ''),
                    defaults={
                        'extra_data': user_info
                    }
                )

                # Create or update social token
                social_token, created = SocialToken.objects.update_or_create(
                    account=social_account,
                    app=google_app,
                    defaults={
                        'token': access_token,
                        'token_secret': refresh_token  # refresh_token goes in token_secret field
                    }
                )

                logger.info(f"Created SocialAccount and SocialToken for user {request.user.email}")

            except Exception as e:
                logger.error(f"Failed to create SocialAccount/SocialToken: {e}")
                # Continue anyway since UserService is created

            return Response({
                'message': 'Google service connected successfully',
                'service': UserServiceSerializer(user_service).data
            }, status=status.HTTP_200_OK)

        except Exception as e:
            return Response({
                'error': str(e)
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

    def _exchange_code_for_tokens(self, authorization_code):
        """Exchange authorization code for access and refresh tokens"""
        try:
            import os

            client_id = os.getenv('GOOGLE_OAUTH2_CLIENT_ID')
            client_secret = os.getenv('GOOGLE_OAUTH2_CLIENT_SECRET')
            # Try potential redirect_uris: empty string (mobile), postmessage (web), and explicit backend URL
            backend_url = os.getenv('BACKEND_URL', 'http://localhost:8080')
            candidate_uris = [
                '',  # Mobile often sends empty string
                'postmessage',  # Web often uses this
                f"{backend_url}/api/auth/google/callback/", # Explicit callback
                os.getenv('GOOGLE_OAUTH2_REDIRECT_URI') # Configured env var
            ]
            
            # Remove duplicates and None
            candidate_uris = list(dict.fromkeys([u for u in candidate_uris if u is not None]))

            for uri in candidate_uris:
                token_data['redirect_uri'] = uri
                logger.debug(f"Exchanging code with redirect_uri: '{uri}'")
                
                response = requests.post(token_url, data=token_data)

                if response.status_code == 200:
                    logger.info(f"Successfully exchanged code with redirect_uri: '{uri}'")
                    return response.json()
                else:
                    logger.warning(f"Failed exchange with redirect_uri '{uri}': {response.text}")
            
            # If all fail, log the last error
            logger.error(f"Failed to exchange code for tokens. All candidates failed. Last response: {response.text}")
            return None
            
            # If all fail, log the last error
            logger.error(f"Failed to exchange code for tokens. Last response: {response.text}")
            return None

        except Exception as e:
            logger.error(f"Error exchanging code for tokens: {e}")
            return None


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


class LinkedInOAuthInitView(APIView):
    """Initiate LinkedIn OAuth flow - redirects user to LinkedIn"""
    permission_classes = [AllowAny]

    def get(self, request):
        client_id = os.getenv('LINKEDIN_CLIENT_ID')
        redirect_uri = f"{os.getenv('BACKEND_URL', 'http://localhost:8080')}/api/auth/linkedin/callback/"
        
        params = {
            'response_type': 'code',
            'client_id': client_id,
            'redirect_uri': redirect_uri,
            'scope': 'openid profile email w_member_social',
            'prompt': 'login',
        }
        
        auth_url = f"https://www.linkedin.com/oauth/v2/authorization?{urlencode(params)}"
        return redirect(auth_url)


class LinkedInOAuthCallbackView(APIView):
    """Handle LinkedIn OAuth callback"""
    permission_classes = [AllowAny]

    def get(self, request):
        code = request.GET.get('code')
        error = request.GET.get('error')
        
        frontend_url = os.getenv('FRONTEND_URL', 'http://localhost:8081')
        
        if error:
            return redirect(f"{frontend_url}/login?error={error}")
        
        if not code:
            return redirect(f"{frontend_url}/login?error=no_code")
        
        try:
            # Exchange code for token
            token_url = 'https://www.linkedin.com/oauth/v2/accessToken'
            redirect_uri = f"{os.getenv('BACKEND_URL', 'http://localhost:8080')}/api/auth/linkedin/callback/"
            
            token_data = {
                'grant_type': 'authorization_code',
                'code': code,
                'redirect_uri': redirect_uri,
                'client_id': os.getenv('LINKEDIN_CLIENT_ID'),
                'client_secret': os.getenv('LINKEDIN_CLIENT_SECRET'),
            }
            
            token_response = requests.post(token_url, data=token_data)
            if token_response.status_code != 200:
                return redirect(f"{frontend_url}/login?error=token_exchange_failed")
            
            tokens = token_response.json()
            access_token = tokens.get('access_token')
            expires_in = tokens.get('expires_in')
            
            # Get user info
            user_info_url = 'https://api.linkedin.com/v2/userinfo'
            user_response = requests.get(user_info_url, headers={
                'Authorization': f'Bearer {access_token}'
            })
            
            if user_response.status_code != 200:
                return redirect(f"{frontend_url}/login?error=user_info_failed")
            
            user_data = user_response.json()
            linkedin_id = user_data.get('sub')
            email = user_data.get('email')
            given_name = user_data.get('given_name', '')
            family_name = user_data.get('family_name', '')
            
            if not email:
                return redirect(f"{frontend_url}/login?error=no_email")
            
            # Get or create user
            user = None
            try:
                user = User.objects.get(email=email)
            except User.DoesNotExist:
                try:
                    user = User.objects.get(username=email)
                except User.DoesNotExist:
                    user = User.objects.create_user(
                        username=email,
                        email=email,
                        first_name=given_name,
                        last_name=family_name,
                    )
            except User.MultipleObjectsReturned:
                user = User.objects.filter(email=email).first()
            
            # Create token
            token, created = Token.objects.get_or_create(user=user)
            
            # Connect LinkedIn service
            linkedin_service, created = Service.objects.get_or_create(
                name='linkedin',
                defaults={'description': 'LinkedIn OAuth2 Service'}
            )
            
            token_expires_at = None
            if expires_in:
                from django.utils import timezone
                from datetime import timedelta
                token_expires_at = timezone.now() + timedelta(seconds=int(expires_in))
            
            UserService.objects.update_or_create(
                user=user,
                service=linkedin_service,
                defaults={
                    'access_token': access_token,
                    'token_expires_at': token_expires_at,
                    'service_user_id': linkedin_id,
                    'service_username': email,
                    'is_active': True
                }
            )
            
            # Redirect to frontend with token
            return redirect(f"{frontend_url}/login?token={token.key}&user={email}")
            
        except Exception as e:
            logger.error(f"LinkedIn OAuth callback error: {str(e)}")
            return redirect(f"{frontend_url}/login?error=server_error")


class LinkedInLoginView(APIView):
    permission_classes = [AllowAny]

    def post(self, request):
        """LinkedIn OAuth2 login/signup endpoint"""
        try:
            data = request.data
            code = data.get('code')
            redirect_uri = data.get('redirect_uri')

            if not code:
                return Response({
                    'error': 'Authorization code is required'
                }, status=status.HTTP_400_BAD_REQUEST)

             # Exchange code for access token
            token_url = 'https://www.linkedin.com/oauth/v2/accessToken'
            token_data = {
                'grant_type': 'authorization_code',
                'code': code,
                'redirect_uri': redirect_uri,
                'client_id': os.getenv('LINKEDIN_CLIENT_ID'),
                'client_secret': os.getenv('LINKEDIN_CLIENT_SECRET'),
            }
            
            token_response = requests.post(token_url, data=token_data)
            
            if token_response.status_code != 200:
                print(f"LinkedIn Token Error: {token_response.text}")
                return Response({
                    'error': 'Failed to exchange LinkedIn code',
                    'details': token_response.json()
                }, status=status.HTTP_400_BAD_REQUEST)

            token_json = token_response.json()
            access_token = token_json.get('access_token')
            expires_in = token_json.get('expires_in')

            # Get user info (OIDC)
            user_info_url = 'https://api.linkedin.com/v2/userinfo'
            user_response = requests.get(user_info_url, headers={
                'Authorization': f'Bearer {access_token}'
            })

            if user_response.status_code != 200:
                print(f"LinkedIn User Info Error: {user_response.text}")
                return Response({
                    'error': 'Failed to fetch LinkedIn user info'
                }, status=status.HTTP_400_BAD_REQUEST)

            user_data = user_response.json()
            linkedin_id = user_data.get('sub')
            email = user_data.get('email')
            if not email:
                 # Fallback if email field is named differently
                 email = user_data.get('email_verified')

            given_name = user_data.get('given_name', '')
            family_name = user_data.get('family_name', '')

            if not email:
                return Response({
                    'error': 'Email not provided by LinkedIn'
                }, status=status.HTTP_400_BAD_REQUEST)

            # Check if user exists
            user = None
            try:
                user = User.objects.get(email=email)
            except User.DoesNotExist:
                try:
                     user = User.objects.get(username=email)
                except User.DoesNotExist:
                    user = User.objects.create_user(
                        username=email,
                        email=email,
                        first_name=given_name,
                        last_name=family_name,
                    )
            except User.MultipleObjectsReturned:
                user = User.objects.filter(email=email).first()

            token, created = Token.objects.get_or_create(user=user)

            linkedin_service, created = Service.objects.get_or_create(
                name='linkedin',
                defaults={'description': 'LinkedIn OAuth2 Service'}
            )

            token_expires_at = None
            if expires_in:
                from django.utils import timezone
                from datetime import timedelta
                token_expires_at = timezone.now() + timedelta(seconds=int(expires_in))

            UserService.objects.update_or_create(
                user=user,
                service=linkedin_service,
                defaults={
                    'access_token': access_token,
                    'token_expires_at': token_expires_at,
                    'service_user_id': linkedin_id,
                    'service_username': email,
                    'is_active': True
                }
            )

            return Response({
                'message': 'LinkedIn login successful',
                'user': UserSerializer(user).data,
                'token': token.key,
                'linkedin_service_connected': True
            }, status=status.HTTP_200_OK)

        except Exception as e:
            return Response({
                'error': str(e)
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


class LinkedInConnectView(APIView):
    permission_classes = [IsAuthenticated]

    def post(self, request):
        """Connect LinkedIn OAuth2 service"""
        try:
            data = request.data
            code = data.get('code')
            redirect_uri = data.get('redirect_uri')

            if not code:
                return Response({'error': 'Code required'}, status=400)

             # Exchange code for access token
            token_url = 'https://www.linkedin.com/oauth/v2/accessToken'
            token_data = {
                'grant_type': 'authorization_code',
                'code': code,
                'redirect_uri': redirect_uri,
                'client_id': os.getenv('LINKEDIN_CLIENT_ID'),
                'client_secret': os.getenv('LINKEDIN_CLIENT_SECRET'),
            }
            
            token_response = requests.post(token_url, data=token_data)
            if token_response.status_code != 200:
                print(f"Token Exchange Error: {token_response.text}")
                return Response({'error': 'Failed exchange'}, status=400)
                
            token_json = token_response.json()
            access_token = token_json.get('access_token')
            expires_in = token_json.get('expires_in')

             # Get user info
            user_info_url = 'https://api.linkedin.com/v2/userinfo'
            user_response = requests.get(user_info_url, headers={
                'Authorization': f'Bearer {access_token}'
            })

            if user_response.status_code != 200:
                return Response({'error': 'Failed user info'}, status=400)

            user_data = user_response.json()
            linkedin_id = user_data.get('sub')
            email = user_data.get('email')

            linkedin_service, created = Service.objects.get_or_create(
                name='linkedin',
                defaults={'description': 'LinkedIn OAuth2 Service'}
            )

            token_expires_at = None
            if expires_in:
                from django.utils import timezone
                from datetime import timedelta
                token_expires_at = timezone.now() + timedelta(seconds=int(expires_in))

            UserService.objects.update_or_create(
                user=request.user,
                service=linkedin_service,
                defaults={
                    'access_token': access_token,
                    'token_expires_at': token_expires_at,
                    'service_user_id': linkedin_id,
                    'service_username': email,
                    'is_active': True
                }
            )

            return Response({
                'message': 'LinkedIn connected',
                'service': 'linkedin'
            }, status=status.HTTP_200_OK)

        except Exception as e:
            return Response({'error': str(e)}, status=500)


class GitHubOAuthInitView(APIView):
    """Initiate GitHub OAuth flow - redirects user to GitHub"""
    permission_classes = [AllowAny]

    def get(self, request):
        client_id = os.getenv('GITHUB_CLIENT_ID')
        redirect_uri = f"{os.getenv('BACKEND_URL', 'http://localhost:8080')}/api/auth/github/callback/"
        
        params = {
            'client_id': client_id,
            'redirect_uri': redirect_uri,
            'scope': 'repo user read:org',
        }
        
        auth_url = f"https://github.com/login/oauth/authorize?{urlencode(params)}"
        return redirect(auth_url)


class GitHubOAuthCallbackView(APIView):
    """Handle GitHub OAuth callback"""
    permission_classes = [AllowAny]

    def get(self, request):
        code = request.GET.get('code')
        error = request.GET.get('error')
        
        frontend_url = os.getenv('FRONTEND_URL', 'http://localhost:8081')
        
        if error:
            return redirect(f"{frontend_url}/login?error={error}")
        
        if not code:
            return redirect(f"{frontend_url}/login?error=no_code")
        
        try:
            # Exchange code for token
            token_url = 'https://github.com/login/oauth/access_token'
            redirect_uri = f"{os.getenv('BACKEND_URL', 'http://localhost:8080')}/api/auth/github/callback/"
            
            token_data = {
                'client_id': os.getenv('GITHUB_CLIENT_ID'),
                'client_secret': os.getenv('GITHUB_CLIENT_SECRET'),
                'code': code,
                'redirect_uri': redirect_uri
            }
            
            token_response = requests.post(
                token_url,
                data=token_data,
                headers={'Accept': 'application/json'}
            )
            
            if token_response.status_code != 200:
                return redirect(f"{frontend_url}/login?error=token_exchange_failed")
            
            tokens = token_response.json()
            access_token = tokens.get('access_token')
            
            if not access_token:
                return redirect(f"{frontend_url}/login?error=no_access_token")
            
            # Get user info
            user_info_url = 'https://api.github.com/user'
            user_response = requests.get(user_info_url, headers={
                'Authorization': f'Bearer {access_token}',
                'Accept': 'application/json'
            })
            
            if user_response.status_code != 200:
                return redirect(f"{frontend_url}/login?error=user_info_failed")
            
            user_data = user_response.json()
            github_id = str(user_data.get('id'))
            github_login = user_data.get('login')
            email = user_data.get('email')
            
            # Try to get email if not public
            if not email:
                emails_response = requests.get(
                    'https://api.github.com/user/emails',
                    headers={'Authorization': f'Bearer {access_token}', 'Accept': 'application/json'}
                )
                if emails_response.status_code == 200:
                    emails = emails_response.json()
                    primary_email = next((e for e in emails if e.get('primary')), None)
                    if primary_email:
                        email = primary_email.get('email')
            
            if not email:
                return redirect(f"{frontend_url}/login?error=no_email")
            
            # Get or create user
            user = None
            try:
                user = User.objects.get(email=email)
            except User.DoesNotExist:
                try:
                    user = User.objects.get(username=email)
                except User.DoesNotExist:
                    name = user_data.get('name', '')
                    user = User.objects.create_user(
                        username=email,
                        email=email,
                        first_name=name.split()[0] if name else '',
                        last_name=' '.join(name.split()[1:]) if name and len(name.split()) > 1 else '',
                    )
            except User.MultipleObjectsReturned:
                user = User.objects.filter(email=email).first()
            
            # Create token
            token, created = Token.objects.get_or_create(user=user)
            
            # Connect GitHub service
            github_service, created = Service.objects.get_or_create(
                name='github',
                defaults={'description': 'GitHub code hosting platform'}
            )
            
            UserService.objects.update_or_create(
                user=user,
                service=github_service,
                defaults={
                    'access_token': access_token,
                    'service_user_id': github_id,
                    'service_username': github_login,
                    'is_active': True
                }
            )
            
            # Redirect to frontend with token
            return redirect(f"{frontend_url}/login?token={token.key}&user={email}")
            
        except Exception as e:
            logger.error(f"GitHub OAuth callback error: {str(e)}")
            return redirect(f"{frontend_url}/login?error=server_error")


class GitHubLoginView(APIView):
    permission_classes = [AllowAny]

    def post(self, request):
        """GitHub OAuth2 login/signup endpoint"""
        try:
            data = request.data
            code = data.get('code')
            redirect_uri = data.get('redirect_uri')

            if not code:
                return Response({
                    'error': 'Authorization code is required'
                }, status=status.HTTP_400_BAD_REQUEST)

            token_url = 'https://github.com/login/oauth/access_token'
            token_data = {
                'client_id': os.getenv('GITHUB_CLIENT_ID'),
                'client_secret': os.getenv('GITHUB_CLIENT_SECRET'),
                'code': code,
                'redirect_uri': redirect_uri
            }

            token_response = requests.post(
                token_url,
                data=token_data,
                headers={'Accept': 'application/json'}
            )

            if token_response.status_code != 200:
                logger.error(f"GitHub Token Error: {token_response.text}")
                return Response({
                    'error': 'Failed to exchange GitHub code',
                    'details': token_response.json() if token_response.content else {}
                }, status=status.HTTP_400_BAD_REQUEST)

            token_json = token_response.json()
            access_token = token_json.get('access_token')

            if not access_token:
                return Response({
                    'error': 'No access token received from GitHub'
                }, status=status.HTTP_400_BAD_REQUEST)

            user_info_url = 'https://api.github.com/user'
            user_response = requests.get(user_info_url, headers={
                'Authorization': f'Bearer {access_token}',
                'Accept': 'application/json'
            })

            if user_response.status_code != 200:
                logger.error(f"GitHub User Info Error: {user_response.text}")
                return Response({
                    'error': 'Failed to fetch GitHub user info'
                }, status=status.HTTP_400_BAD_REQUEST)

            user_data = user_response.json()
            github_id = str(user_data.get('id'))
            github_login = user_data.get('login')
            email = user_data.get('email')

            if not email:
                emails_response = requests.get(
                    'https://api.github.com/user/emails',
                    headers={'Authorization': f'Bearer {access_token}', 'Accept': 'application/json'}
                )
                if emails_response.status_code == 200:
                    emails = emails_response.json()
                    primary_email = next((e for e in emails if e.get('primary')), None)
                    if primary_email:
                        email = primary_email.get('email')

            if not email:
                return Response({
                    'error': 'Email not provided by GitHub'
                }, status=status.HTTP_400_BAD_REQUEST)

            user = None
            try:
                user = User.objects.get(email=email)
            except User.DoesNotExist:
                try:
                    user = User.objects.get(username=email)
                except User.DoesNotExist:
                    user = User.objects.create_user(
                        username=email,
                        email=email,
                        first_name=user_data.get('name', '').split()[0] if user_data.get('name') else '',
                        last_name=' '.join(user_data.get('name', '').split()[1:]) if user_data.get('name') and len(user_data.get('name', '').split()) > 1 else '',
                    )
            except User.MultipleObjectsReturned:
                user = User.objects.filter(email=email).first()

            token, created = Token.objects.get_or_create(user=user)

            github_service, created = Service.objects.get_or_create(
                name='github',
                defaults={'description': 'GitHub code hosting platform'}
            )

            UserService.objects.update_or_create(
                user=user,
                service=github_service,
                defaults={
                    'access_token': access_token,
                    'service_user_id': github_id,
                    'service_username': github_login,
                    'is_active': True
                }
            )

            return Response({
                'message': 'GitHub login successful',
                'user': UserSerializer(user).data,
                'token': token.key,
                'github_service_connected': True
            }, status=status.HTTP_200_OK)

        except Exception as e:
            logger.error(f"GitHub login error: {str(e)}")
            return Response({
                'error': str(e)
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


class GitHubConnectView(APIView):
    permission_classes = [IsAuthenticated]

    def post(self, request):
        """Connect GitHub OAuth2 service to existing user"""
        try:
            data = request.data
            code = data.get('code')
            redirect_uri = data.get('redirect_uri')

            if not code:
                return Response({'error': 'Authorization code required'}, status=400)

            token_url = 'https://github.com/login/oauth/access_token'
            token_data = {
                'client_id': os.getenv('GITHUB_CLIENT_ID'),
                'client_secret': os.getenv('GITHUB_CLIENT_SECRET'),
                'code': code,
                'redirect_uri': redirect_uri
            }

            token_response = requests.post(
                token_url,
                data=token_data,
                headers={'Accept': 'application/json'}
            )

            if token_response.status_code != 200:
                logger.error(f"GitHub Token Exchange Error: {token_response.text}")
                return Response({'error': 'Failed to exchange code'}, status=400)

            token_json = token_response.json()
            access_token = token_json.get('access_token')

            if not access_token:
                return Response({'error': 'No access token received'}, status=400)

            user_info_url = 'https://api.github.com/user'
            user_response = requests.get(user_info_url, headers={
                'Authorization': f'Bearer {access_token}',
                'Accept': 'application/json'
            })

            if user_response.status_code != 200:
                logger.error(f"GitHub User Info Error: {user_response.text}")
                return Response({'error': 'Failed to fetch user info'}, status=400)

            user_data = user_response.json()
            github_id = str(user_data.get('id'))
            github_login = user_data.get('login')

            github_service, created = Service.objects.get_or_create(
                name='github',
                defaults={'description': 'GitHub code hosting platform'}
            )

            UserService.objects.update_or_create(
                user=request.user,
                service=github_service,
                defaults={
                    'access_token': access_token,
                    'service_user_id': github_id,
                    'service_username': github_login,
                    'is_active': True
                }
            )

            return Response({
                'message': 'GitHub connected successfully',
                'service': 'github'
            }, status=status.HTTP_200_OK)

        except Exception as e:
            logger.error(f"GitHub connect error: {str(e)}")
            return Response({'error': str(e)}, status=500)


class GitLabOAuthInitView(APIView):
    """Initiate GitLab OAuth flow - redirects user to GitLab"""
    permission_classes = [AllowAny]

    def get(self, request):
        client_id = os.getenv('GITLAB_CLIENT_ID')
        redirect_uri = f"{os.getenv('BACKEND_URL', 'http://localhost:8080')}/api/auth/gitlab/callback/"

        params = {
            'client_id': client_id,
            'redirect_uri': redirect_uri,
            'response_type': 'code',
            'scope': 'read_user api write_repository',
        }

        auth_url = f"https://gitlab.com/oauth/authorize?{urlencode(params)}"
        return redirect(auth_url)


class GitLabOAuthCallbackView(APIView):
    """Handle GitLab OAuth callback"""
    permission_classes = [AllowAny]

    def get(self, request):
        code = request.GET.get('code')
        error = request.GET.get('error')

        frontend_url = os.getenv('FRONTEND_URL', 'http://localhost:8081')

        if error:
            return redirect(f"{frontend_url}/login?error={error}")

        if not code:
            return redirect(f"{frontend_url}/login?error=no_code")

        try:
            # Exchange code for token
            token_url = 'https://gitlab.com/oauth/token'
            redirect_uri = f"{os.getenv('BACKEND_URL', 'http://localhost:8080')}/api/auth/gitlab/callback/"

            token_data = {
                'client_id': os.getenv('GITLAB_CLIENT_ID'),
                'client_secret': os.getenv('GITLAB_CLIENT_SECRET'),
                'code': code,
                'grant_type': 'authorization_code',
                'redirect_uri': redirect_uri
            }

            token_response = requests.post(
                token_url,
                data=token_data,
                headers={'Accept': 'application/json'}
            )

            if token_response.status_code != 200:
                return redirect(f"{frontend_url}/login?error=token_exchange_failed")

            tokens = token_response.json()
            access_token = tokens.get('access_token')

            if not access_token:
                return redirect(f"{frontend_url}/login?error=no_access_token")

            # Get user info
            user_info_url = 'https://gitlab.com/api/v4/user'
            user_response = requests.get(user_info_url, headers={
                'Authorization': f'Bearer {access_token}',
                'Accept': 'application/json'
            })

            if user_response.status_code != 200:
                return redirect(f"{frontend_url}/login?error=user_info_failed")

            user_data = user_response.json()
            gitlab_id = str(user_data.get('id'))
            gitlab_username = user_data.get('username')
            email = user_data.get('email')

            if not email:
                return redirect(f"{frontend_url}/login?error=no_email")

            # Get or create user
            user = None
            try:
                user = User.objects.get(email=email)
            except User.DoesNotExist:
                try:
                    user = User.objects.get(username=email)
                except User.DoesNotExist:
                    name = user_data.get('name', '')
                    user = User.objects.create_user(
                        username=email,
                        email=email,
                        first_name=name.split()[0] if name else '',
                        last_name=' '.join(name.split()[1:]) if name and len(name.split()) > 1 else '',
                    )
            except User.MultipleObjectsReturned:
                user = User.objects.filter(email=email).first()

            # Create token
            token, created = Token.objects.get_or_create(user=user)

            # Connect GitLab service
            gitlab_service, created = Service.objects.get_or_create(
                name='gitlab',
                defaults={'description': 'GitLab code collaboration platform'}
            )

            UserService.objects.update_or_create(
                user=user,
                service=gitlab_service,
                defaults={
                    'access_token': access_token,
                    'service_user_id': gitlab_id,
                    'service_username': gitlab_username,
                    'is_active': True
                }
            )

            # Redirect to frontend with token
            return redirect(f"{frontend_url}/login?token={token.key}&user={email}")

        except Exception as e:
            logger.error(f"GitLab OAuth callback error: {str(e)}")
            return redirect(f"{frontend_url}/login?error=server_error")


class GitLabConnectView(APIView):
    """Connect GitLab to existing user account"""
    permission_classes = [IsAuthenticated]

    def post(self, request):
        try:
            code = request.data.get('code')

            if not code:
                return Response({
                    'error': 'Authorization code is required'
                }, status=status.HTTP_400_BAD_REQUEST)

            # Exchange code for token
            token_url = 'https://gitlab.com/oauth/token'
            
            # Use passed redirect_uri (Mobile) or fallback to Web default
            redirect_uri = request.data.get('redirect_uri')
            if not redirect_uri:
                redirect_uri = f"{os.getenv('FRONTEND_URL', 'http://localhost:8081')}/settings"

            token_data = {
                'client_id': os.getenv('GITLAB_CLIENT_ID'),
                'client_secret': os.getenv('GITLAB_CLIENT_SECRET'),
                'code': code,
                'grant_type': 'authorization_code',
                'redirect_uri': redirect_uri
            }

            token_response = requests.post(
                token_url,
                data=token_data,
                headers={'Accept': 'application/json'}
            )

            if token_response.status_code != 200:
                return Response({
                    'error': 'Failed to exchange code for token',
                    'details': token_response.text
                }, status=status.HTTP_400_BAD_REQUEST)

            tokens = token_response.json()
            access_token = tokens.get('access_token')

            if not access_token:
                return Response({
                    'error': 'No access token received'
                }, status=status.HTTP_400_BAD_REQUEST)

            # Get user info
            user_info_url = 'https://gitlab.com/api/v4/user'
            user_response = requests.get(user_info_url, headers={
                'Authorization': f'Bearer {access_token}',
                'Accept': 'application/json'
            })

            if user_response.status_code != 200:
                return Response({
                    'error': 'Failed to get user info'
                }, status=status.HTTP_400_BAD_REQUEST)

            user_data = user_response.json()
            gitlab_id = str(user_data.get('id'))
            gitlab_username = user_data.get('username')

            # Get or create GitLab service
            gitlab_service, created = Service.objects.get_or_create(
                name='gitlab',
                defaults={'description': 'GitLab code collaboration platform'}
            )

            # Update or create UserService
            UserService.objects.update_or_create(
                user=request.user,
                service=gitlab_service,
                defaults={
                    'access_token': access_token,
                    'service_user_id': gitlab_id,
                    'service_username': gitlab_username,
                    'is_active': True
                }
            )

            return Response({
                'message': 'GitLab connected successfully',
                'service': 'gitlab'
            }, status=status.HTTP_200_OK)

        except Exception as e:
            logger.error(f"GitLab connect error: {str(e)}")
            return Response({'error': str(e)}, status=500)


class GitLabLoginView(APIView):
    permission_classes = [AllowAny]

    def post(self, request):
        """GitLab OAuth2 login/signup endpoint"""
        try:
            data = request.data
            code = data.get('code')
            redirect_uri = data.get('redirect_uri')

            if not code:
                return Response({
                    'error': 'Authorization code is required'
                }, status=status.HTTP_400_BAD_REQUEST)

            token_url = 'https://gitlab.com/oauth/token'
            token_data = {
                'client_id': os.getenv('GITLAB_CLIENT_ID'),
                'client_secret': os.getenv('GITLAB_CLIENT_SECRET'),
                'code': code,
                'grant_type': 'authorization_code',
                'redirect_uri': redirect_uri
            }

            token_response = requests.post(
                token_url,
                data=token_data,
                headers={'Accept': 'application/json'}
            )

            if token_response.status_code != 200:
                logger.error(f"GitLab Token Error: {token_response.text}")
                return Response({
                    'error': 'Failed to exchange GitLab code',
                    'details': token_response.json() if token_response.content else {}
                }, status=status.HTTP_400_BAD_REQUEST)

            token_json = token_response.json()
            access_token = token_json.get('access_token')

            if not access_token:
                return Response({
                    'error': 'No access token received from GitLab'
                }, status=status.HTTP_400_BAD_REQUEST)

            user_info_url = 'https://gitlab.com/api/v4/user'
            user_response = requests.get(user_info_url, headers={
                'Authorization': f'Bearer {access_token}',
                'Accept': 'application/json'
            })

            if user_response.status_code != 200:
                logger.error(f"GitLab User Info Error: {user_response.text}")
                return Response({
                    'error': 'Failed to fetch GitLab user info'
                }, status=status.HTTP_400_BAD_REQUEST)

            user_data = user_response.json()
            gitlab_id = str(user_data.get('id'))
            gitlab_username = user_data.get('username')
            email = user_data.get('email')

            if not email:
                return Response({
                    'error': 'Email not provided by GitLab'
                }, status=status.HTTP_400_BAD_REQUEST)

            user = None
            try:
                user = User.objects.get(email=email)
            except User.DoesNotExist:
                try:
                    user = User.objects.get(username=email)
                except User.DoesNotExist:
                    user = User.objects.create_user(
                        username=email,
                        email=email,
                        first_name=user_data.get('name', '').split()[0] if user_data.get('name') else '',
                        last_name=' '.join(user_data.get('name', '').split()[1:]) if user_data.get('name') and len(user_data.get('name', '').split()) > 1 else '',
                    )
            except User.MultipleObjectsReturned:
                user = User.objects.filter(email=email).first()

            token, created = Token.objects.get_or_create(user=user)

            gitlab_service, created = Service.objects.get_or_create(
                name='gitlab',
                defaults={'description': 'GitLab code collaboration platform'}
            )

            UserService.objects.update_or_create(
                user=user,
                service=gitlab_service,
                defaults={
                    'access_token': access_token,
                    'service_user_id': gitlab_id,
                    'service_username': gitlab_username,
                    'is_active': True
                }
            )

            return Response({
                'message': 'GitLab login successful',
                'user': UserSerializer(user).data,
                'token': token.key,
                'gitlab_service_connected': True
            }, status=status.HTTP_200_OK)

        except Exception as e:
            logger.error(f"GitLab login error: {str(e)}")
            return Response({
                'error': str(e)
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)