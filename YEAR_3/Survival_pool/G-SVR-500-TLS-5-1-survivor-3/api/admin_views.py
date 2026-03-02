from django.shortcuts import render
from django.contrib.admin.views.decorators import staff_member_required
from django.db.models import Count, Q, Avg, Sum
from django.utils import timezone
from datetime import datetime, timedelta
from .models import Startup, Event, News, UserProfile, Message, Opportunity, Investor, Partner
from rest_framework.decorators import api_view, permission_classes
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response

@staff_member_required
def admin_dashboard(request):
    """Dashboard principal avec KPIs"""
    
    # Statistiques générales
    total_startups = Startup.objects.count()
    total_events = Event.objects.count()
    total_news = News.objects.count()
    total_users = UserProfile.objects.count()
    total_messages = Message.objects.count()
    total_opportunities = Opportunity.objects.count()
    total_investors = Investor.objects.count()
    total_partners = Partner.objects.count()
    
    # Statistiques par secteur
    startups_by_sector = Startup.objects.values('sector').annotate(
        count=Count('id')
    ).order_by('-count')[:5]
    
    # Statistiques par maturité
    startups_by_maturity = Startup.objects.values('maturity').annotate(
        count=Count('id')
    ).order_by('-count')
    
    # Statistiques par rôle utilisateur
    users_by_role = UserProfile.objects.values('role').annotate(
        count=Count('id')
    ).order_by('-count')
    
    # Événements récents (30 derniers jours)
    thirty_days_ago = timezone.now() - timedelta(days=30)
    recent_events = Event.objects.filter(
        created_at__gte=thirty_days_ago
    ).count()
    
    # Actualités récentes (30 derniers jours)
    recent_news = News.objects.filter(
        created_at__gte=thirty_days_ago
    ).count()
    
    # Startups créées récemment (30 derniers jours)
    recent_startups = Startup.objects.filter(
        created_at__gte=thirty_days_ago
    ).count()
    
    # Messages non lus
    unread_messages = Message.objects.filter(is_read=False).count()
    
    # Opportunités actives (non expirées)
    active_opportunities = Opportunity.objects.filter(
        deadline__gte=timezone.now()
    ).count()
    
    # Statistiques temporelles (derniers 7 jours)
    seven_days_ago = timezone.now() - timedelta(days=7)
    startups_last_week = Startup.objects.filter(
        created_at__gte=seven_days_ago
    ).count()
    
    events_last_week = Event.objects.filter(
        created_at__gte=seven_days_ago
    ).count()
    
    news_last_week = News.objects.filter(
        created_at__gte=seven_days_ago
    ).count()
    
    # Top 5 startups par engagement (basé sur les messages)
    top_startups = Startup.objects.annotate(
        message_count=Count('startup_messages')
    ).order_by('-message_count')[:5]
    
    context = {
        'total_startups': total_startups,
        'total_events': total_events,
        'total_news': total_news,
        'total_users': total_users,
        'total_messages': total_messages,
        'total_opportunities': total_opportunities,
        'total_investors': total_investors,
        'total_partners': total_partners,
        'startups_by_sector': startups_by_sector,
        'startups_by_maturity': startups_by_maturity,
        'users_by_role': users_by_role,
        'recent_events': recent_events,
        'recent_news': recent_news,
        'recent_startups': recent_startups,
        'unread_messages': unread_messages,
        'active_opportunities': active_opportunities,
        'startups_last_week': startups_last_week,
        'events_last_week': events_last_week,
        'news_last_week': news_last_week,
        'top_startups': top_startups,
    }
    
    return render(request, 'admin/dashboard.html', context)

@staff_member_required
def admin_analytics(request):
    """Page d'analyses avancées"""
    
    # Évolution mensuelle des startups
    monthly_startups = []
    for i in range(6):  # 6 derniers mois
        month_start = timezone.now().replace(day=1) - timedelta(days=30*i)
        month_end = month_start + timedelta(days=30)
        count = Startup.objects.filter(
            created_at__gte=month_start,
            created_at__lt=month_end
        ).count()
        monthly_startups.append({
            'month': month_start.strftime('%B %Y'),
            'count': count
        })
    
    # Évolution mensuelle des événements
    monthly_events = []
    for i in range(6):
        month_start = timezone.now().replace(day=1) - timedelta(days=30*i)
        month_end = month_start + timedelta(days=30)
        count = Event.objects.filter(
            created_at__gte=month_start,
            created_at__lt=month_end
        ).count()
        monthly_events.append({
            'month': month_start.strftime('%B %Y'),
            'count': count
        })
    
    # Top secteurs avec pourcentages
    total_startups = Startup.objects.count()
    sectors_with_percentage = []
    for sector in Startup.objects.values('sector').annotate(count=Count('id')).order_by('-count')[:10]:
        percentage = (sector['count'] / total_startups * 100) if total_startups > 0 else 0
        sectors_with_percentage.append({
            'sector': sector['sector'] or 'Non défini',
            'count': sector['count'],
            'percentage': round(percentage, 1)
        })
    
    context = {
        'monthly_startups': monthly_startups,
        'monthly_events': monthly_events,
        'sectors_with_percentage': sectors_with_percentage,
    }
    
    return render(request, 'admin/analytics.html', context)

@api_view(['GET'])
@permission_classes([IsAuthenticated])
def admin_dashboard_api(request):
    """API endpoint pour récupérer les données du dashboard admin"""
    
    # Vérifier que l'utilisateur est admin
    if not request.user.is_staff:
        return Response({'error': 'Access denied'}, status=403)
    
    now = timezone.now()
    last_30_days = now - timedelta(days=30)
    last_7_days = now - timedelta(days=7)
    
    # === KPIs PRINCIPAUX ===
    total_startups = Startup.objects.count()
    total_events = Event.objects.count()
    total_news = News.objects.count()
    total_users = UserProfile.objects.count()
    total_messages = Message.objects.count()
    total_opportunities = Opportunity.objects.count()
    total_investors = Investor.objects.count()
    total_partners = Partner.objects.count()
    
    # === KPIs TEMPORELS ===
    recent_startups = Startup.objects.filter(created_at__gte=last_30_days).count()
    recent_events = Event.objects.filter(created_at__gte=last_30_days).count()
    recent_news = News.objects.filter(created_at__gte=last_30_days).count()
    recent_users = UserProfile.objects.filter(created_at__gte=last_30_days).count()
    
    # Messages non lus
    unread_messages = Message.objects.filter(is_read=False).count()
    
    # Opportunités actives
    active_opportunities = Opportunity.objects.filter(
        deadline__gte=now
    ).count()
    
    # === ANALYSES PAR SECTEUR ===
    startups_by_sector = list(Startup.objects.values('sector').annotate(
        count=Count('id')
    ).order_by('-count')[:10])
    
    # === ANALYSES PAR MATURITÉ ===
    startups_by_maturity = list(Startup.objects.values('maturity').annotate(
        count=Count('id')
    ).order_by('-count'))
    
    # === ANALYSES PAR RÔLE ===
    users_by_role = list(UserProfile.objects.values('role').annotate(
        count=Count('id')
    ).order_by('-count'))
    
    # === ÉVÉNEMENTS À VENIR ===
    upcoming_events = list(Event.objects.filter(
        dates__gte=now
    ).order_by('dates')[:5].values('id', 'name', 'dates', 'location', 'event_type'))
    
    # === ACTUALITÉS RÉCENTES ===
    recent_news_list = list(News.objects.filter(
        news_date__gte=last_7_days
    ).order_by('-news_date')[:5].values('id', 'title', 'news_date', 'category'))
    
    # === ÉVOLUTION TEMPORELLE (30 derniers jours) ===
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
        
        daily_messages = Message.objects.filter(
            sent_at__gte=date,
            sent_at__lt=next_date
        ).count()
        
        daily_data.append({
            'date': date.strftime('%Y-%m-%d'),
            'startups': daily_startups,
            'events': daily_events,
            'news': daily_news,
            'messages': daily_messages
        })
    
    daily_data.reverse()  # Du plus ancien au plus récent
    
    # === MÉTRIQUES D'ENGAGEMENT ===
    # Top startups par nombre de messages
    top_startups_engagement = list(Startup.objects.annotate(
        message_count=Count('startup_messages')
    ).order_by('-message_count')[:5].values('id', 'name', 'message_count'))
    
    # Messages par jour de la semaine
    messages_by_weekday = []
    weekdays = ['Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sunday']
    for i, day in enumerate(weekdays):
        count = Message.objects.filter(
            sent_at__week_day=i+2  # Django week_day: 1=Sunday, 2=Monday, etc.
        ).count()
        messages_by_weekday.append({'day': day, 'count': count})
    
    # === TAUX DE CROISSANCE ===
    # Croissance des startups (30 derniers jours vs 30 jours précédents)
    previous_30_days = last_30_days - timedelta(days=30)
    startups_previous_30 = Startup.objects.filter(
        created_at__gte=previous_30_days,
        created_at__lt=last_30_days
    ).count()
    
    growth_rate_startups = 0
    if startups_previous_30 > 0:
        growth_rate_startups = ((recent_startups - startups_previous_30) / startups_previous_30) * 100
    
    # === STATISTIQUES DE PERFORMANCE ===
    # Taux de participation aux événements
    total_event_capacity = Event.objects.aggregate(
        total_capacity=Sum('max_participants')
    )['total_capacity'] or 0
    
    # === DONNÉES POUR GRAPHIQUES ===
    chart_data = {
        'daily_activity': daily_data,
        'sector_distribution': startups_by_sector,
        'maturity_distribution': startups_by_maturity,
        'role_distribution': users_by_role,
        'messages_by_weekday': messages_by_weekday
    }
    
    return Response({
        'kpis': {
            'total_startups': total_startups,
            'total_events': total_events,
            'total_news': total_news,
            'total_users': total_users,
            'total_messages': total_messages,
            'total_opportunities': total_opportunities,
            'total_investors': total_investors,
            'total_partners': total_partners,
            'recent_startups': recent_startups,
            'recent_events': recent_events,
            'recent_news': recent_news,
            'recent_users': recent_users,
            'unread_messages': unread_messages,
            'active_opportunities': active_opportunities,
            'growth_rate_startups': round(growth_rate_startups, 2),
            'total_event_capacity': total_event_capacity
        },
        'analytics': {
            'startups_by_sector': startups_by_sector,
            'startups_by_maturity': startups_by_maturity,
            'users_by_role': users_by_role,
            'top_startups_engagement': top_startups_engagement,
            'messages_by_weekday': messages_by_weekday
        },
        'recent_activity': {
            'upcoming_events': upcoming_events,
            'recent_news': recent_news_list
        },
        'charts': chart_data,
        'last_updated': now.isoformat()
    })
