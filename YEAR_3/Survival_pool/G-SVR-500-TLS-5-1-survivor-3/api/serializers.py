from rest_framework import serializers
from .models import Startup, Event, News, Message, UserProfile, Investor, Partner, JEBUser, Founder
from django.contrib.auth.models import User

class FounderSerializer(serializers.ModelSerializer):
    class Meta:
        model = Founder
        fields = ['id', 'jeb_id', 'name', 'image', 'image_url']

class StartupSerializer(serializers.ModelSerializer):
    founders = FounderSerializer(many=True, read_only=True)
    
    class Meta:
        model = Startup
        fields = ['id', 'jeb_id', 'name', 'description', 'sector', 'maturity', 'legal_status', 
                 'address', 'email', 'phone', 'website_url', 'social_media_url', 
                 'project_status', 'needs', 'progress', 'founders', 'created_at', 'updated_at']

class StartupDetailSerializer(serializers.ModelSerializer):
    founders = FounderSerializer(many=True, read_only=True)
    
    class Meta:
        model = Startup
        fields = '__all__'

class EventSerializer(serializers.ModelSerializer):
    class Meta:
        model = Event
        fields = ['id', 'jeb_id', 'name', 'description', 'event_type', 'location', 
                 'dates', 'target_audience', 'max_participants', 'created_at', 'updated_at']

class EventDetailSerializer(serializers.ModelSerializer):
    class Meta:
        model = Event
        fields = '__all__'

class NewsSerializer(serializers.ModelSerializer):
    class Meta:
        model = News
        fields = ['id', 'jeb_id', 'title', 'description', 'content', 'category', 
                 'news_date', 'location', 'created_at', 'updated_at']

class NewsDetailSerializer(serializers.ModelSerializer):
    class Meta:
        model = News
        fields = '__all__'

class MessageSerializer(serializers.ModelSerializer):
    sender_username = serializers.CharField(source='sender.username', read_only=True)
    recipient_username = serializers.CharField(source='recipient.username', read_only=True)
    
    class Meta:
        model = Message
        fields = ['id', 'sender', 'sender_username', 'recipient', 'recipient_username', 
                 'subject', 'content', 'sent_at', 'is_read']

class InvestorSerializer(serializers.ModelSerializer):
    class Meta:
        model = Investor
        fields = ['id', 'jeb_id', 'name', 'legal_status', 'address', 'email', 
                 'phone', 'description', 'investor_type', 'investment_focus', 
                 'created_at', 'updated_at']

class InvestorDetailSerializer(serializers.ModelSerializer):
    class Meta:
        model = Investor
        fields = '__all__'

class PartnerSerializer(serializers.ModelSerializer):
    class Meta:
        model = Partner
        fields = ['id', 'jeb_id', 'name', 'legal_status', 'address', 'email', 
                 'phone', 'description', 'partner_type', 'focus_area', 
                 'created_at', 'updated_at']

class PartnerDetailSerializer(serializers.ModelSerializer):
    class Meta:
        model = Partner
        fields = '__all__'

class JEBUserSerializer(serializers.ModelSerializer):
    class Meta:
        model = JEBUser
        fields = ['id', 'jeb_id', 'name', 'email', 'role', 'company', 'position', 
                 'bio', 'linkedin_url', 'twitter_url', 'created_at', 'updated_at']

class JEBUserDetailSerializer(serializers.ModelSerializer):
    class Meta:
        model = JEBUser
        fields = '__all__'

class UserSerializer(serializers.ModelSerializer):
    class Meta:
        model = User
        fields = ['id', 'username', 'email', 'first_name', 'last_name', 'is_staff', 'is_superuser']
