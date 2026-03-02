from django.urls import path
from . import views, views_redirect
from . import auth_views
from rest_framework.decorators import api_view, permission_classes
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from django.db.models import Count
from django.utils import timezone
from datetime import datetime, timedelta
from .models import Startup, Event, News, UserProfile, Message

urlpatterns = [
    path('auth/login/', auth_views.login_view, name='django-login'),
    path('auth/register/', auth_views.register_view, name='django-register'),
    path('auth/logout/', auth_views.logout_view, name='django-logout'),
    path('auth/user/', auth_views.user_info_view, name='user-info'),
    path('auth/check/', auth_views.check_auth_view, name='check-auth'),
    
    path('startups/', views.StartupListView.as_view(), name='startup-list'),
    path('startups/<int:pk>/', views.StartupDetailView.as_view(), name='startup-detail'),
    
    path('events/', views.EventListView.as_view(), name='event-list'),
    path('events/<int:pk>/', views.EventDetailView.as_view(), name='event-detail'),
    
    path('news/', views.NewsListView.as_view(), name='news-list'),
    path('news/<int:pk>/', views.NewsDetailView.as_view(), name='news-detail'),
    
    # JEB Users
    path('jeb-users/', views.JEBUserListView.as_view(), name='jeb-user-list'),
    path('jeb-users/<int:pk>/', views.JEBUserDetailView.as_view(), name='jeb-user-detail'),
    path('users/', views.UserListView.as_view(), name='user-list'),
    
    path('messages/', views.MessageListView.as_view(), name='message-list'),
    path('messages/<int:pk>/', views.MessageDetailView.as_view(), name='message-detail'),
    
    path('investors/', views.InvestorListView.as_view(), name='investor-list'),
    path('investors/<int:pk>/', views.InvestorDetailView.as_view(), name='investor-detail'),
    
    path('partners/', views.PartnerListView.as_view(), name='partner-list'),
    path('partners/<int:pk>/', views.PartnerDetailView.as_view(), name='partner-detail'),
    
    # Images
    path('news/<int:news_id>/image/', views_redirect.news_image_redirect, name='news-image'),
    path('events/<int:event_id>/image/', views_redirect.event_image_redirect, name='event-image'),
    path('users/<int:user_id>/image/', views_redirect.user_image_redirect, name='user-image'),
    path('startups/<int:startup_id>/image/', views_redirect.startup_image_redirect, name='startup-image'),
    path('startups/<int:startup_id>/founders/<int:founder_id>/image/', views_redirect.founder_image_redirect, name='founder-image'),
    
    # PDF Export endpoints
    path('export/startup/<int:startup_id>/pdf/', views.export_startup_profile_pdf, name='export-startup-pdf'),
    path('export/directory/pdf/', views.export_startup_directory_pdf, name='export-directory-pdf'),
    path('export/sector-analysis/pdf/', views.export_sector_analysis_pdf, name='export-sector-analysis-pdf'),
    
    # Pitch Deck endpoint
    path('pitch-deck/<int:startup_id>/', views.pitch_deck_view, name='pitch-deck'),
    
    path('favicon.ico', views.favicon_view, name='favicon'),
    
    # Dashboard data
    path('dashboard/data/', views.dashboard_data, name='dashboard-data'),
]
