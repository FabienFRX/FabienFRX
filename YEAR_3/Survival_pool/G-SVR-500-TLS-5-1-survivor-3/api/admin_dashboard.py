from django.contrib import admin
from django.urls import path
from django.shortcuts import render
from django.db.models import Count, Q
from django.utils import timezone
from datetime import datetime, timedelta
from .models import Startup, Event, News, UserProfile, Message, Opportunity

class AdminDashboard:
    """Dashboard personnalisé pour l'admin Django"""
    
    def __init__(self):
        self.title = "JEB Incubator Platform - Dashboard"
        self.template = "admin/dashboard.html"
    
    def get_kpis(self):
        """Récupère les KPIs principaux"""
        now = timezone.now()
        last_30_days = now - timedelta(days=30)
        last_7_days = now - timedelta(days=7)
        
        # Statistiques générales
        total_startups = Startup.objects.count()
        total_events = Event.objects.count()
        total_news = News.objects.count()
        total_users = UserProfile.objects.count()
        
        # Statistiques récentes
        recent_startups = Startup.objects.filter(created_at__gte=last_30_days).count()
        recent_events = Event.objects.filter(dates__gte=last_30_days).count()
        recent_news = News.objects.filter(news_date__gte=last_30_days).count()
        
        # Engagement (messages et opportunités)
        total_messages = Message.objects.count()
        recent_messages = Message.objects.filter(created_at__gte=last_7_days).count()
        total_opportunities = Opportunity.objects.count()
        active_opportunities = Opportunity.objects.filter(is_active=True).count()
        
        # Répartition par secteur
        sector_stats = Startup.objects.values('sector').annotate(
            count=Count('id')
        ).order_by('-count')[:5]
        
        # Répartition par maturité
        maturity_stats = Startup.objects.values('maturity').annotate(
            count=Count('id')
        ).order_by('-count')
        
        # Répartition par rôle utilisateur
        role_stats = UserProfile.objects.values('role').annotate(
            count=Count('id')
        ).order_by('-count')
        
        # Événements à venir
        upcoming_events = Event.objects.filter(
            dates__gte=now
        ).order_by('dates')[:5]
        
        # Actualités récentes
        recent_news_list = News.objects.filter(
            news_date__gte=last_7_days
        ).order_by('-news_date')[:5]
        
        return {
            'total_startups': total_startups,
            'total_events': total_events,
            'total_news': total_news,
            'total_users': total_users,
            'recent_startups': recent_startups,
            'recent_events': recent_events,
            'recent_news': recent_news,
            'total_messages': total_messages,
            'recent_messages': recent_messages,
            'total_opportunities': total_opportunities,
            'active_opportunities': active_opportunities,
            'sector_stats': list(sector_stats),
            'maturity_stats': list(maturity_stats),
            'role_stats': list(role_stats),
            'upcoming_events': list(upcoming_events),
            'recent_news_list': list(recent_news_list),
            'last_30_days': last_30_days,
            'last_7_days': last_7_days,
        }
    
    def get_chart_data(self):
        """Prépare les données pour les graphiques"""
        now = timezone.now()
        
        # Données pour graphique temporel (30 derniers jours)
        daily_data = []
        for i in range(30):
            date = now - timedelta(days=i)
            next_date = date + timedelta(days=1)
            
            daily_startups = Startup.objects.filter(
                created_at__gte=date,
                created_at__lt=next_date
            ).count()
            
            daily_events = Event.objects.filter(
                dates__gte=date,
                dates__lt=next_date
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
        
        return {
            'daily_data': daily_data,
        }

# Instance globale du dashboard
admin_dashboard = AdminDashboard()
