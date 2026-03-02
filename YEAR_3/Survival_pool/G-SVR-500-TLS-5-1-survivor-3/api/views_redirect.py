import os
from django.http import HttpResponseRedirect, HttpResponse
from django.conf import settings
from rest_framework.decorators import api_view
from rest_framework import generics
from .models import News, Event, Startup, JEBUser, Investor, Partner, Founder
from .serializers import NewsSerializer, EventSerializer, StartupSerializer, JEBUserSerializer, InvestorSerializer, PartnerSerializer, UserSerializer
from django_filters.rest_framework import DjangoFilterBackend
from rest_framework.filters import SearchFilter, OrderingFilter
from rest_framework.permissions import AllowAny
from django.shortcuts import get_object_or_404

# Generic views for data (unchanged from original views.py, but now imported from here)
class NewsListView(generics.ListCreateAPIView):
    queryset = News.objects.all()
    serializer_class = NewsSerializer
    filter_backends = [DjangoFilterBackend, SearchFilter, OrderingFilter]
    filterset_fields = ['category', 'location']
    search_fields = ['title', 'description', 'content']
    ordering_fields = ['news_date', 'title']
    ordering = ['-news_date']
    permission_classes = [AllowAny]

class NewsDetailView(generics.RetrieveUpdateDestroyAPIView):
    queryset = News.objects.all()
    serializer_class = NewsSerializer
    permission_classes = [AllowAny]

class EventListView(generics.ListCreateAPIView):
    queryset = Event.objects.all()
    serializer_class = EventSerializer
    filter_backends = [DjangoFilterBackend, SearchFilter, OrderingFilter]
    filterset_fields = ['event_type', 'location']
    search_fields = ['name', 'description']
    ordering_fields = ['dates', 'name']
    ordering = ['-dates']
    permission_classes = [AllowAny]

class EventDetailView(generics.RetrieveUpdateDestroyAPIView):
    queryset = Event.objects.all()
    serializer_class = EventSerializer
    permission_classes = [AllowAny]

class StartupListView(generics.ListCreateAPIView):
    queryset = Startup.objects.all()
    serializer_class = StartupSerializer
    filter_backends = [DjangoFilterBackend, SearchFilter, OrderingFilter]
    filterset_fields = ['sector', 'maturity', 'project_status']
    search_fields = ['name', 'description', 'needs', 'progress']
    ordering_fields = ['name', 'created_at']
    ordering = ['name']
    permission_classes = [AllowAny]

class StartupDetailView(generics.RetrieveUpdateDestroyAPIView):
    queryset = Startup.objects.all()
    serializer_class = StartupSerializer
    permission_classes = [AllowAny]

class JEBUserListView(generics.ListCreateAPIView):
    queryset = JEBUser.objects.all()
    serializer_class = JEBUserSerializer
    filter_backends = [DjangoFilterBackend, SearchFilter, OrderingFilter]
    filterset_fields = ['role', 'company']
    search_fields = ['name', 'email', 'company', 'position', 'bio']
    ordering_fields = ['name', 'created_at']
    ordering = ['name']
    permission_classes = [AllowAny]

class JEBUserDetailView(generics.RetrieveUpdateDestroyAPIView):
    queryset = JEBUser.objects.all()
    serializer_class = JEBUserSerializer
    permission_classes = [AllowAny]

class InvestorListView(generics.ListCreateAPIView):
    queryset = Investor.objects.all()
    serializer_class = InvestorSerializer
    filter_backends = [DjangoFilterBackend, SearchFilter, OrderingFilter]
    filterset_fields = ['investor_type', 'legal_status']
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
    filter_backends = [DjangoFilterBackend, SearchFilter, OrderingFilter]
    filterset_fields = ['partner_type', 'legal_status']
    search_fields = ['name', 'email', 'description', 'focus_area']
    ordering_fields = ['name', 'created_at']
    ordering = ['name']
    permission_classes = [AllowAny]

class PartnerDetailView(generics.RetrieveUpdateDestroyAPIView):
    queryset = Partner.objects.all()
    serializer_class = PartnerSerializer
    permission_classes = [AllowAny]

class FounderListView(generics.ListCreateAPIView):
    queryset = Founder.objects.all()
    serializer_class = UserSerializer
    filter_backends = [DjangoFilterBackend, SearchFilter, OrderingFilter]
    filterset_fields = ['startup']
    search_fields = ['name']
    ordering_fields = ['name', 'created_at']
    ordering = ['name']
    permission_classes = [AllowAny]

class FounderDetailView(generics.RetrieveUpdateDestroyAPIView):
    queryset = Founder.objects.all()
    serializer_class = UserSerializer
    permission_classes = [AllowAny]

# Image redirection views with authentication
@api_view(['GET'])
def news_image_redirect(request, news_id):
    """Redirige vers l'image d'une actualité depuis l'API JEB avec token"""
    token = settings.JEB_API_TOKEN
    if token:
        redirect_url = f"{settings.JEB_API_BASE_URL}/news/{news_id}/image"
        # On ne peut pas passer les headers dans une redirection, donc on utilise un proxy
        return proxy_image(request, redirect_url, token)
    else:
        redirect_url = f"{settings.JEB_API_BASE_URL}/news/{news_id}/image"
        return HttpResponseRedirect(redirect_url)

@api_view(['GET'])
def event_image_redirect(request, event_id):
    """Redirige vers l'image d'un événement depuis l'API JEB avec token"""
    token = settings.JEB_API_TOKEN
    if token:
        redirect_url = f"{settings.JEB_API_BASE_URL}/events/{event_id}/image"
        return proxy_image(request, redirect_url, token)
    else:
        redirect_url = f"{settings.JEB_API_BASE_URL}/events/{event_id}/image"
        return HttpResponseRedirect(redirect_url)

@api_view(['GET'])
def user_image_redirect(request, user_id):
    """Redirige vers l'image d'un utilisateur depuis l'API JEB avec token"""
    token = settings.JEB_API_TOKEN
    if token:
        redirect_url = f"{settings.JEB_API_BASE_URL}/users/{user_id}/image"
        return proxy_image(request, redirect_url, token)
    else:
        redirect_url = f"{settings.JEB_API_BASE_URL}/users/{user_id}/image"
        return HttpResponseRedirect(redirect_url)

@api_view(['GET'])
def startup_image_redirect(request, startup_id):
    """Redirige vers l'image d'une startup depuis l'API JEB avec token"""
    token = settings.JEB_API_TOKEN
    if token:
        redirect_url = f"{settings.JEB_API_BASE_URL}/startups/{startup_id}/image"
        return proxy_image(request, redirect_url, token)
    else:
        redirect_url = f"{settings.JEB_API_BASE_URL}/startups/{startup_id}/image"
        return HttpResponseRedirect(redirect_url)

@api_view(['GET'])
def founder_image_redirect(request, startup_id, founder_id):
    """Redirige vers l'image d'un fondateur depuis l'API JEB avec token"""
    token = settings.JEB_API_TOKEN
    if token:
        redirect_url = f"{settings.JEB_API_BASE_URL}/startups/{startup_id}/founders/{founder_id}/image"
        return proxy_image(request, redirect_url, token)
    else:
        redirect_url = f"{settings.JEB_API_BASE_URL}/startups/{startup_id}/founders/{founder_id}/image"
        return HttpResponseRedirect(redirect_url)

def proxy_image(request, image_url, token):
    """Proxy pour les images avec authentification"""
    import requests
    try:
        headers = {'X-Group-Authorization': token}
        response = requests.get(image_url, headers=headers, stream=True)
        response.raise_for_status()
        
        # Retourner l'image avec les bons headers
        from django.http import HttpResponse
        http_response = HttpResponse(
            response.content,
            content_type=response.headers.get('content-type', 'image/jpeg')
        )
        return http_response
    except requests.exceptions.RequestException as e:
        print(f"Erreur proxy image {image_url}: {e}")
        return HttpResponse(status=404)

def favicon_view(request):
    favicon_path = os.path.join(settings.BASE_DIR, 'api', 'static', 'favicon', 'favicon.ico')
    if os.path.exists(favicon_path):
        return HttpResponse(open(favicon_path, 'rb'), content_type='image/x-icon')
    return HttpResponse(status=404)
