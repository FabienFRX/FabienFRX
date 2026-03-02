from rest_framework.decorators import api_view, permission_classes
from rest_framework.permissions import AllowAny, IsAuthenticated
from rest_framework.response import Response
from rest_framework import status
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.models import User
from .models import UserProfile, Startup
from rest_framework.authtoken.models import Token
from django.core.validators import validate_email
from django.core.exceptions import ValidationError
import re

@api_view(['POST'])
@permission_classes([AllowAny])
def login_view(request):
    """Connexion utilisateur avec authentification Django"""
    username = request.data.get('username')
    password = request.data.get('password')
    
    if not username or not password:
        return Response(
            {'error': 'Username and password required'}, 
            status=status.HTTP_400_BAD_REQUEST
        )
    
    user = authenticate(username=username, password=password)
    if user is not None:
        login(request, user)
        token, created = Token.objects.get_or_create(user=user)
        
        # Récupérer le profil utilisateur
        try:
            profile = user.userprofile
            role = profile.role
            startup_id = profile.startup.id if profile.startup else None
        except UserProfile.DoesNotExist:
            role = 'visitor'
            startup_id = None
        
        return Response({
            'token': token.key,
            'user_id': user.id,
            'username': user.username,
            'email': user.email,
            'first_name': user.first_name,
            'last_name': user.last_name,
            'role': role,
            'startup_id': startup_id,
            'is_staff': user.is_staff,
            'is_superuser': user.is_superuser
        })
    else:
        return Response(
            {'error': 'Invalid credentials'}, 
            status=status.HTTP_401_UNAUTHORIZED
        )

@api_view(['POST'])
@permission_classes([AllowAny])
def register_view(request):
    """Inscription d'un nouvel utilisateur"""
    username = request.data.get('username')
    email = request.data.get('email')
    password = request.data.get('password')
    password_confirm = request.data.get('password_confirm')
    first_name = request.data.get('first_name', '')
    last_name = request.data.get('last_name', '')
    role = request.data.get('role', 'visitor')
    startup_id = request.data.get('startup_id')
    
    # Validation des champs obligatoires
    if not username or not email or not password:
        return Response(
            {'error': 'Username, email and password are required'}, 
            status=status.HTTP_400_BAD_REQUEST
        )
    
    # Validation de la confirmation du mot de passe
    if password != password_confirm:
        return Response(
            {'error': 'Passwords do not match'}, 
            status=status.HTTP_400_BAD_REQUEST
        )
    
    # Validation de l'email
    try:
        validate_email(email)
    except ValidationError:
        return Response(
            {'error': 'Invalid email format'}, 
            status=status.HTTP_400_BAD_REQUEST
        )
    
    # Validation du nom d'utilisateur
    if len(username) < 3:
        return Response(
            {'error': 'Username must be at least 3 characters long'}, 
            status=status.HTTP_400_BAD_REQUEST
        )
    
    if not re.match(r'^[a-zA-Z0-9_]+$', username):
        return Response(
            {'error': 'Username can only contain letters, numbers and underscores'}, 
            status=status.HTTP_400_BAD_REQUEST
        )
    
    # Validation du mot de passe
    if len(password) < 6:
        return Response(
            {'error': 'Password must be at least 6 characters long'}, 
            status=status.HTTP_400_BAD_REQUEST
        )
    
    # Validation du rôle
    valid_roles = ['admin', 'startup', 'investor', 'visitor']
    if role not in valid_roles:
        return Response(
            {'error': f'Invalid role. Must be one of: {", ".join(valid_roles)}'}, 
            status=status.HTTP_400_BAD_REQUEST
        )
    
    # Validation pour les startups
    startup = None
    if role == 'startup':
        if not startup_id:
            return Response(
                {'error': 'Startup selection is required for startup founders'}, 
                status=status.HTTP_400_BAD_REQUEST
            )
        try:
            startup = Startup.objects.get(id=startup_id)
        except Startup.DoesNotExist:
            return Response(
                {'error': 'Selected startup does not exist'}, 
                status=status.HTTP_400_BAD_REQUEST
            )
    
    # Vérifier si l'utilisateur existe déjà
    if User.objects.filter(username=username).exists():
        return Response(
            {'error': 'Username already exists'}, 
            status=status.HTTP_400_BAD_REQUEST
        )
    
    if User.objects.filter(email=email).exists():
        return Response(
            {'error': 'Email already exists'}, 
            status=status.HTTP_400_BAD_REQUEST
        )
    
    try:
        # Créer l'utilisateur
        user = User.objects.create_user(
            username=username,
            email=email,
            password=password,
            first_name=first_name,
            last_name=last_name
        )
        
        # Créer le profil utilisateur
        UserProfile.objects.create(
            user=user,
            role=role,
            startup=startup if role == 'startup' else None
        )
        
        # Créer le token d'authentification
        token, created = Token.objects.get_or_create(user=user)
        
        return Response({
            'message': 'User created successfully',
            'token': token.key,
            'user_id': user.id,
            'username': user.username,
            'email': user.email,
            'first_name': user.first_name,
            'last_name': user.last_name,
            'role': role,
            'startup_id': startup.id if startup else None,
            'is_staff': user.is_staff,
            'is_superuser': user.is_superuser
        }, status=status.HTTP_201_CREATED)
        
    except Exception as e:
        return Response(
            {'error': f'Failed to create user: {str(e)}'}, 
            status=status.HTTP_500_INTERNAL_SERVER_ERROR
        )

@api_view(['POST'])
@permission_classes([IsAuthenticated])
def logout_view(request):
    """Déconnexion utilisateur"""
    try:
        # Supprimer le token d'authentification
        request.user.auth_token.delete()
        logout(request)
        return Response({'message': 'Successfully logged out'})
    except Exception as e:
        return Response(
            {'error': str(e)}, 
            status=status.HTTP_500_INTERNAL_SERVER_ERROR
        )

@api_view(['GET'])
@permission_classes([IsAuthenticated])
def user_info_view(request):
    """Informations sur l'utilisateur connecté"""
    user = request.user
    try:
        profile = user.userprofile
        role = profile.role
        startup_id = profile.startup.id if profile.startup else None
    except UserProfile.DoesNotExist:
        role = 'visitor'
        startup_id = None
    
    return Response({
        'user_id': user.id,
        'username': user.username,
        'email': user.email,
        'first_name': user.first_name,
        'last_name': user.last_name,
        'role': role,
        'startup_id': startup_id,
        'is_staff': user.is_staff,
        'is_superuser': user.is_superuser
    })

@api_view(['GET'])
@permission_classes([AllowAny])
def check_auth_view(request):
    """Vérifier l'état d'authentification"""
    if request.user.is_authenticated:
        try:
            profile = request.user.userprofile
            role = profile.role
            startup_id = profile.startup.id if profile.startup else None
        except UserProfile.DoesNotExist:
            role = 'visitor'
            startup_id = None
        
        return Response({
            'authenticated': True,
            'user_id': request.user.id,
            'username': request.user.username,
            'email': request.user.email,
            'first_name': request.user.first_name,
            'last_name': request.user.last_name,
            'role': role,
            'startup_id': startup_id,
            'is_staff': request.user.is_staff,
            'is_superuser': request.user.is_superuser
        })
    else:
        return Response({
            'authenticated': False
        })
