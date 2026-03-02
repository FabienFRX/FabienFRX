from django.shortcuts import render
from rest_framework.filters import SearchFilter, OrderingFilter
from rest_framework import generics, status
from rest_framework.decorators import api_view  
from rest_framework.authtoken.views import ObtainAuthToken
from rest_framework.authtoken.models import Token
from rest_framework.permissions import IsAuthenticated, AllowAny, IsAuthenticatedOrReadOnly
from django.views.decorators.cache import cache_page
from django.utils.decorators import method_decorator
from django.views.decorators.vary import vary_on_headers
from .models import Message, Investor, Partner, JEBUser
from .serializers import MessageSerializer
from rest_framework.response import Response
from django.contrib.auth.models import User
from .models import Startup, Event, News
from .serializers import (
    StartupSerializer, StartupDetailSerializer,
    EventSerializer, EventDetailSerializer,
    NewsSerializer, UserSerializer, InvestorSerializer, PartnerSerializer, JEBUserSerializer
)
from django.http import HttpResponse
from django.db.models import Count
from django.utils import timezone
from datetime import datetime, timedelta

@method_decorator(cache_page(60 * 15), name='get')  # Cache for 15 minutes
@method_decorator(vary_on_headers('Authorization'), name='get')
class StartupListView(generics.ListCreateAPIView):
    serializer_class = StartupSerializer
    filter_backends = [SearchFilter, OrderingFilter]
    search_fields = ['name', 'description', 'email']
    ordering_fields = ['name', 'created_at', 'updated_at']
    ordering = ['-created_at']
    permission_classes = [IsAuthenticatedOrReadOnly]  # Public read, auth required for create

    def get_queryset(self):
        return Startup.objects.all()

@method_decorator(cache_page(60 * 30), name='get')  # Cache for 30 minutes
class StartupDetailView(generics.RetrieveUpdateDestroyAPIView):
    serializer_class = StartupDetailSerializer
    permission_classes = [IsAuthenticatedOrReadOnly]  # Public read, auth required for modify
    
    def get_queryset(self):
        return Startup.objects.all()

@method_decorator(cache_page(60 * 15), name='get')  # Cache for 15 minutes
class EventListView(generics.ListCreateAPIView):
    serializer_class = EventSerializer
    filter_backends = [SearchFilter, OrderingFilter]
    search_fields = ['name', 'description']
    ordering_fields = ['name', 'dates', 'created_at']
    ordering = ['-dates']
    permission_classes = [IsAuthenticatedOrReadOnly]  # Public read, auth required for create
    
    def get_queryset(self):
        return Event.objects.all()

@method_decorator(cache_page(60 * 30), name='get')  # Cache for 30 minutes
class EventDetailView(generics.RetrieveUpdateDestroyAPIView):
    serializer_class = EventDetailSerializer
    permission_classes = [IsAuthenticatedOrReadOnly]  # Public read, auth required for modify
    
    def get_queryset(self):
        return Event.objects.all()

class NewsListView(generics.ListCreateAPIView):
    queryset = News.objects.all()
    serializer_class = NewsSerializer
    filter_backends = [SearchFilter, OrderingFilter]
    search_fields = ['title', 'content', 'description']
    ordering_fields = ['title', 'news_date', 'created_at']
    ordering = ['-news_date']
    permission_classes = [IsAuthenticatedOrReadOnly]  # Public read, auth required for create

class NewsDetailView(generics.RetrieveUpdateDestroyAPIView):
    queryset = News.objects.all()
    serializer_class = NewsSerializer
    permission_classes = [IsAuthenticatedOrReadOnly]  # Public read, auth required for modify

class MessageListView(generics.ListCreateAPIView):
    queryset = Message.objects.all()
    serializer_class = MessageSerializer
    filter_backends = [SearchFilter, OrderingFilter]
    search_fields = ['subject', 'content']
    ordering_fields = ['sent_at', 'subject']
    ordering = ['-sent_at']
    permission_classes = [IsAuthenticated]

class MessageDetailView(generics.RetrieveUpdateDestroyAPIView):
    queryset = Message.objects.all()
    serializer_class = MessageSerializer
    permission_classes = [IsAuthenticated]

class UserListView(generics.ListCreateAPIView):
    queryset = User.objects.all()
    serializer_class = UserSerializer
    filter_backends = [SearchFilter, OrderingFilter]
    search_fields = ['username', 'email', 'first_name', 'last_name']
    ordering_fields = ['username', 'date_joined']
    ordering = ['-date_joined']
    permission_classes = [IsAuthenticated]

class UserDetailView(generics.RetrieveUpdateDestroyAPIView):
    queryset = User.objects.all()
    serializer_class = UserSerializer
    permission_classes = [IsAuthenticated]

class InvestorListView(generics.ListCreateAPIView):
    queryset = Investor.objects.all()
    serializer_class = InvestorSerializer
    filter_backends = [SearchFilter, OrderingFilter]
    search_fields = ['name', 'email', 'description', 'investment_focus']
    ordering_fields = ['name', 'created_at']
    ordering = ['name']
    permission_classes = [AllowAny]

class InvestorDetailView(generics.RetrieveUpdateDestroyAPIView):
    queryset = Investor.objects.all()
    serializer_class = InvestorSerializer
    permission_classes = [AllowAny]

class PartnerListView(generics.ListCreateAPIView):
    queryset = Partner.objects.all()
    serializer_class = PartnerSerializer
    filter_backends = [SearchFilter, OrderingFilter]
    search_fields = ['name', 'email', 'description', 'focus_area']
    ordering_fields = ['name', 'created_at']
    ordering = ['name']
    permission_classes = [AllowAny]

class PartnerDetailView(generics.RetrieveUpdateDestroyAPIView):
    queryset = Partner.objects.all()
    serializer_class = PartnerSerializer
    permission_classes = [AllowAny]

class JEBUserListView(generics.ListCreateAPIView):
    queryset = JEBUser.objects.all()
    serializer_class = JEBUserSerializer
    filter_backends = [SearchFilter, OrderingFilter]
    search_fields = ['name', 'email', 'company', 'position', 'bio']
    ordering_fields = ['name', 'created_at']
    ordering = ['name']
    permission_classes = [AllowAny]

class JEBUserDetailView(generics.RetrieveUpdateDestroyAPIView):
    queryset = JEBUser.objects.all()
    serializer_class = JEBUserSerializer
    permission_classes = [AllowAny]

@api_view(['GET'])
def news_image(request, news_id):
    """Serve news image, fallback to API if not found locally"""
    try:
        news = News.objects.get(id=news_id)
        if news.image and news.image.url:
            return HttpResponse(open(news.image.path, 'rb').read(), content_type='image/jpeg')
    except News.DoesNotExist:
        pass
    
    # Fallback to API
    import requests
    from django.conf import settings
    try:
        response = requests.get(f"{settings.JEB_API_BASE_URL}/news/{news_id}/image/", 
                              headers={'X-Group-Authorization': settings.JEB_API_TOKEN})
        if response.status_code == 200:
            return HttpResponse(response.content, content_type='image/jpeg')
    except:
        pass
    
    return HttpResponse(status=404)

@api_view(['GET'])
def event_image(request, event_id):
    """Serve event image, fallback to API if not found locally"""
    try:
        event = Event.objects.get(id=event_id)
        if event.image and event.image.url:
            return HttpResponse(open(event.image.path, 'rb').read(), content_type='image/jpeg')
    except Event.DoesNotExist:
        pass
    
    # Fallback to API
    import requests
    from django.conf import settings
    try:
        response = requests.get(f"{settings.JEB_API_BASE_URL}/events/{event_id}/image/", 
                              headers={'X-Group-Authorization': settings.JEB_API_TOKEN})
        if response.status_code == 200:
            return HttpResponse(response.content, content_type='image/jpeg')
    except:
        pass
    
    return HttpResponse(status=404)

@api_view(['GET'])
def user_image(request, user_id):
    """Serve user image, fallback to API if not found locally"""
    try:
        user = JEBUser.objects.get(id=user_id)
        if user.profile_image and user.profile_image.url:
            return HttpResponse(open(user.profile_image.path, 'rb').read(), content_type='image/jpeg')
    except JEBUser.DoesNotExist:
        pass
    
    # Fallback to API
    import requests
    from django.conf import settings
    try:
        response = requests.get(f"{settings.JEB_API_BASE_URL}/users/{user_id}/image/", 
                              headers={'X-Group-Authorization': settings.JEB_API_TOKEN})
        if response.status_code == 200:
            return HttpResponse(response.content, content_type='image/jpeg')
    except:
        pass
    
    return HttpResponse(status=404)

@api_view(['POST'])
def user_view(request):
    """Get user information"""
    if request.user.is_authenticated:
        return Response({
            'id': request.user.id,
            'username': request.user.username,
            'email': request.user.email,
            'first_name': request.user.first_name,
            'last_name': request.user.last_name,
            'is_staff': request.user.is_staff,
            'is_superuser': request.user.is_superuser,
        })
    return Response({'error': 'Not authenticated'}, status=401)

@api_view(['GET'])
def check_auth_view(request):
    """Check if user is authenticated"""
    if request.user.is_authenticated:
        return Response({
            'authenticated': True,
            'user': {
                'id': request.user.id,
                'username': request.user.username,
                'email': request.user.email,
                'first_name': request.user.first_name,
                'last_name': request.user.last_name,
                'is_staff': request.user.is_staff,
                'is_superuser': request.user.is_superuser,
            }
        })
    return Response({'authenticated': False})

@api_view(['GET'])
def startup_image(request, startup_id):
    """Serve startup image, fallback to API if not found locally"""
    try:
        startup = Startup.objects.get(id=startup_id)
        if startup.image and startup.image.url:
            return HttpResponse(open(startup.image.path, 'rb').read(), content_type='image/jpeg')
    except Startup.DoesNotExist:
        pass

    # Fallback to API
    import requests
    from django.conf import settings
    try:
        response = requests.get(f"{settings.JEB_API_BASE_URL}/startups/{startup_id}/image/",
                              headers={'X-Group-Authorization': settings.JEB_API_TOKEN})
        if response.status_code == 200:
            return HttpResponse(response.content, content_type='image/jpeg')
    except:
        pass

    return HttpResponse(status=404)

@api_view(['GET'])
def founder_image(request, startup_id, founder_id):
    """Serve founder image, fallback to API if not found locally"""
    try:
        from .models import Founder
        founder = Founder.objects.get(id=founder_id, startup__id=startup_id)
        if founder.image and founder.image.url:
            return HttpResponse(open(founder.image.path, 'rb').read(), content_type='image/jpeg')
    except:
        pass

    # Fallback to API
    import requests
    from django.conf import settings
    try:
        response = requests.get(f"{settings.JEB_API_BASE_URL}/startups/{startup_id}/founders/{founder_id}/image/",
                              headers={'X-Group-Authorization': settings.JEB_API_TOKEN})
        if response.status_code == 200:
            return HttpResponse(response.content, content_type='image/jpeg')
    except:
        pass

    return HttpResponse(status=404)

def favicon_view(request):
    """Serve favicon.ico"""
    try:
        with open('/app/api/static/favicon/favicon.ico', 'rb') as f:
            return HttpResponse(f.read(), content_type='image/x-icon')
    except:
        return HttpResponse(status=404)

# PDF Export Views
from django.http import HttpResponse
from rest_framework.decorators import api_view, permission_classes
from rest_framework.permissions import AllowAny
from .pdf_service import PDFExportService

@api_view(['GET'])
@permission_classes([AllowAny])
def export_startup_profile_pdf(request, startup_id):
    """Export a single startup profile as PDF"""
    try:
        pdf_service = PDFExportService()
        pdf_buffer = pdf_service.generate_startup_profile_pdf(startup_id)
        
        if pdf_buffer is None:
            return HttpResponse('Startup not found', status=404)
        
        startup = Startup.objects.get(id=startup_id)
        filename = f"{startup.name.replace(' ', '_').lower()}_profile.pdf"
        
        response = HttpResponse(pdf_buffer.getvalue(), content_type='application/pdf')
        response['Content-Disposition'] = f'attachment; filename="{filename}"'
        return response
        
    except Exception as e:
        return HttpResponse(f'Error generating PDF: {str(e)}', status=500)

@api_view(['GET'])
@permission_classes([AllowAny])
def export_startup_directory_pdf(request):
    """Export all startups as a directory PDF"""
    try:
        pdf_service = PDFExportService()
        
        # Apply filters if provided
        startups = Startup.objects.all()
        sector = request.GET.get('sector')
        maturity = request.GET.get('maturity')
        status = request.GET.get('status')
        
        if sector:
            startups = startups.filter(sector=sector)
        if maturity:
            startups = startups.filter(maturity=maturity)
        if status:
            startups = startups.filter(project_status=status)
        
        pdf_buffer = pdf_service.generate_startup_directory_pdf(startups)
        
        response = HttpResponse(pdf_buffer.getvalue(), content_type='application/pdf')
        response['Content-Disposition'] = 'attachment; filename="jeb_startup_directory.pdf"'
        return response
        
    except Exception as e:
        return HttpResponse(f'Error generating PDF: {str(e)}', status=500)

@api_view(['GET'])
@permission_classes([AllowAny])
def export_sector_analysis_pdf(request):
    """Export sector analysis as PDF"""
    try:
        pdf_service = PDFExportService()
        pdf_buffer = pdf_service.generate_sector_analysis_pdf()
        
        response = HttpResponse(pdf_buffer.getvalue(), content_type='application/pdf')
        response['Content-Disposition'] = 'attachment; filename="jeb_sector_analysis.pdf"'
        return response
        
    except Exception as e:
        return HttpResponse(f'Error generating PDF: {str(e)}', status=500)

# Pitch Deck Views
@api_view(['GET', 'POST', 'PUT'])
def pitch_deck_view(request, startup_id):
    """Handle pitch deck data for a specific startup"""
    from .models import PitchDeck
    
    try:
        startup = Startup.objects.get(id=startup_id)
    except Startup.DoesNotExist:
        return Response({'error': 'Startup not found'}, status=status.HTTP_404_NOT_FOUND)
    
    if request.method == 'GET':
        # Anyone can view pitch deck data
        try:
            pitch_deck = PitchDeck.objects.get(startup=startup)
            data = {
                'financials': {
                    'currentFunding': pitch_deck.current_funding,
                    'targetFunding': pitch_deck.target_funding,
                    'revenue': pitch_deck.revenue,
                    'projectedRevenue': pitch_deck.projected_revenue
                },
                'metrics': {
                    'users': pitch_deck.users,
                    'growth': pitch_deck.growth,
                    'marketSize': pitch_deck.market_size
                },
                'competitive': {
                    'advantages': pitch_deck.advantages
                },
                'last_saved': pitch_deck.updated_at.isoformat()
            }
            return Response(data)
        except PitchDeck.DoesNotExist:
            return Response({'error': 'No pitch deck data found'}, status=status.HTTP_404_NOT_FOUND)
    
    elif request.method in ['POST', 'PUT']:
        # Only startup owners can edit
        if not request.user.is_authenticated:
            return Response({'error': 'Authentication required'}, status=status.HTTP_401_UNAUTHORIZED)
        
        try:
            user_profile = request.user.userprofile
            if user_profile.role != 'startup' or user_profile.startup_id != startup.id:
                return Response({'error': 'Permission denied'}, status=status.HTTP_403_FORBIDDEN)
        except:
            return Response({'error': 'Invalid user profile'}, status=status.HTTP_400_BAD_REQUEST)
        
        # Get or create pitch deck
        pitch_deck, created = PitchDeck.objects.get_or_create(startup=startup)
        
        # Update data
        data = request.data
        if 'financials' in data:
            pitch_deck.current_funding = data['financials'].get('currentFunding', '')
            pitch_deck.target_funding = data['financials'].get('targetFunding', '')
            pitch_deck.revenue = data['financials'].get('revenue', '')
            pitch_deck.projected_revenue = data['financials'].get('projectedRevenue', '')
        
        if 'metrics' in data:
            pitch_deck.users = data['metrics'].get('users', 0)
            pitch_deck.growth = data['metrics'].get('growth', '')
            pitch_deck.market_size = data['metrics'].get('marketSize', '')
        
        if 'competitive' in data:
            pitch_deck.advantages = data['competitive'].get('advantages', [])
        
        pitch_deck.save()
        
        return Response({
            'message': 'Pitch deck saved successfully',
            'last_saved': pitch_deck.updated_at.isoformat()
        })

@api_view(['GET'])
@permission_classes([IsAuthenticated])
def dashboard_data(request):
    """API endpoint pour récupérer les données du dashboard"""
    
    now = timezone.now()
    last_30_days = now - timedelta(days=30)
    last_7_days = now - timedelta(days=7)
    
    # Statistiques générales
    total_startups = Startup.objects.count()
    total_events = Event.objects.count()
    total_news = News.objects.count()
    total_users = User.objects.count() # Assuming UserProfile is User
    total_messages = Message.objects.count()
    
    # Statistiques récentes
    recent_startups = Startup.objects.filter(created_at__gte=last_30_days).count()
    recent_events = Event.objects.filter(created_at__gte=last_30_days).count()
    recent_news = News.objects.filter(created_at__gte=last_30_days).count()
    
    # Répartition par secteur
    startups_by_sector = list(Startup.objects.values('sector').annotate(
        count=Count('id')
    ).order_by('-count')[:10])
    
    # Répartition par maturité
    startups_by_maturity = list(Startup.objects.values('maturity').annotate(
        count=Count('id')
    ).order_by('-count'))
    
    # Répartition par rôle utilisateur
    users_by_role = list(User.objects.values('is_staff').annotate( # Assuming UserProfile is User
        count=Count('id')
    ).order_by('-count'))
    
    # Évolution temporelle (30 derniers jours)
    daily_data = []
    for i in range(30):
        date = now - timedelta(days=i)
        next_date = date + timedelta(days=1)
        
        daily_startups = Startup.objects.filter(
            created_at__gte=date,
            created_at__lt=next_date
        ).count()
        
        daily_events = Event.objects.filter(
            created_at__gte=date,
            created_at__lt=next_date
        ).count()
        
        daily_news = News.objects.filter(
            news_date__gte=date,
            news_date__lt=next_date
        ).count()
        
        daily_data.append({
            'date': date.strftime('%Y-%m-%d'),
            'startups': daily_startups,
            'events': daily_events,
            'news': daily_news
        })
    
    daily_data.reverse()  # Du plus ancien au plus récent
    
    return Response({
        'stats': {
            'total_startups': total_startups,
            'total_events': total_events,
            'total_news': total_news,
            'total_users': total_users,
            'total_messages': total_messages,
            'recent_startups': recent_startups,
            'recent_events': recent_events,
            'recent_news': recent_news
        },
        'charts': {
            'startups_by_sector': startups_by_sector,
            'startups_by_maturity': startups_by_maturity,
            'users_by_role': users_by_role,
            'daily_activity': daily_data
        }
    })
