from django.db import models
from django.contrib.auth.models import User

class Startup(models.Model):
    # Champs de l'API JEB
    jeb_id = models.IntegerField(unique=True)
    name = models.CharField(max_length=200)
    legal_status = models.CharField(max_length=50, blank=True)
    address = models.TextField(blank=True)
    email = models.EmailField()
    phone = models.CharField(max_length=20, blank=True)
    sector = models.CharField(max_length=100)
    maturity = models.CharField(max_length=50)
    
    # Champs locaux supplémentaires (alignés avec l'API)
    description = models.TextField(blank=True)
    website_url = models.URLField(blank=True, null=True)
    social_media_url = models.URLField(blank=True, null=True)
    project_status = models.CharField(max_length=50, blank=True)
    needs = models.TextField(blank=True)
    progress = models.TextField(blank=True)
    
    
    # Métadonnées
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)
    last_synced = models.DateTimeField(null=True, blank=True)

    def __str__(self):
        return self.name

class News(models.Model):
    # Champs de l'API JEB
    jeb_id = models.IntegerField(unique=True)
    news_date = models.DateField()
    location = models.CharField(max_length=200, blank=True)
    title = models.CharField(max_length=200)
    category = models.CharField(max_length=50)
    startup_id = models.IntegerField(null=True, blank=True)
    description = models.TextField(blank=True)
    
    # Champs locaux
    content = models.TextField(blank=True)
    related_startup = models.ForeignKey(Startup, on_delete=models.SET_NULL, null=True, blank=True)

    # Métadonnées
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)
    last_synced = models.DateTimeField(null=True, blank=True)

    def __str__(self):
        return self.title

class Event(models.Model):
    # Champs de l'API JEB
    jeb_id = models.IntegerField(unique=True)
    name = models.CharField(max_length=200)
    dates = models.DateField()
    location = models.CharField(max_length=200)
    description = models.TextField()
    event_type = models.CharField(max_length=50)
    target_audience = models.CharField(max_length=100, blank=True)
    
    # Champs locaux
    max_participants = models.IntegerField(null=True, blank=True)

    # Métadonnées
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)
    last_synced = models.DateTimeField(null=True, blank=True)

    def __str__(self):
        return self.name

class Message(models.Model):
    sender = models.ForeignKey(User, on_delete=models.CASCADE, related_name='sent_messages')
    recipient = models.ForeignKey(User, on_delete=models.CASCADE, related_name='received_messages')
    subject = models.CharField(max_length=200)
    content = models.TextField()
    sent_at = models.DateTimeField(auto_now_add=True)
    is_read = models.BooleanField(default=False)

    def __str__(self):
        return f"{self.subject} - {self.sender.username} to {self.recipient.username}"

class Opportunity(models.Model):
    OPPORTUNITY_TYPES = [
        ('funding', 'Funding Opportunity'),
        ('partnership', 'Partnership'),
        ('call', 'Call for Projects')
    ]
    
    title = models.CharField(max_length=200)
    description = models.TextField()
    opportunity_type = models.CharField(max_length=20, choices=OPPORTUNITY_TYPES)
    deadline = models.DateTimeField(null=True, blank=True)
    contact_info = models.TextField()
    created_at = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return self.title

class UserProfile(models.Model):
    ROLE_CHOICES = [
        ('admin', 'Administrator'),
        ('startup', 'Startup'),
        ('investor', 'Investor'),
        ('visitor', 'Visitor')
    ]
    
    user = models.OneToOneField(User, on_delete=models.CASCADE)
    role = models.CharField(max_length=20, choices=ROLE_CHOICES, default='visitor')
    startup = models.ForeignKey(Startup, on_delete=models.CASCADE, null=True, blank=True)
    created_at = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return f"{self.user.username} - {self.role}"

class PitchDeck(models.Model):
    startup = models.OneToOneField(Startup, on_delete=models.CASCADE, related_name='pitch_deck')
    
    # Financial data
    current_funding = models.CharField(max_length=100, blank=True)
    target_funding = models.CharField(max_length=100, blank=True)
    revenue = models.CharField(max_length=100, blank=True)
    projected_revenue = models.CharField(max_length=100, blank=True)
    
    # Metrics data
    users = models.IntegerField(default=0)
    growth = models.CharField(max_length=100, blank=True)
    market_size = models.CharField(max_length=100, blank=True)
    
    # Competitive advantages (stored as JSON)
    advantages = models.JSONField(default=list, blank=True)
    
    # Metadata
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)
    
    def __str__(self):
        return f"Pitch Deck for {self.startup.name}"

class Founder(models.Model):
    jeb_id = models.IntegerField(unique=True)
    name = models.CharField(max_length=200)
    startup = models.ForeignKey(Startup, on_delete=models.CASCADE, related_name='founders')
    image = models.ImageField(upload_to='founder_images/', blank=True, null=True)
    image_url = models.URLField(blank=True, null=True)
    
    # Métadonnées
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)
    last_synced = models.DateTimeField(null=True, blank=True)

    def __str__(self):
        return f"{self.name} ({self.startup.name})"

class Investor(models.Model):
    # Champs de l'API JEB
    jeb_id = models.IntegerField(unique=True)
    name = models.CharField(max_length=200)
    legal_status = models.CharField(max_length=50, blank=True)
    address = models.TextField(blank=True)
    email = models.EmailField(blank=True)
    phone = models.CharField(max_length=20, blank=True)
    description = models.TextField(blank=True)
    investor_type = models.CharField(max_length=50, blank=True)
    investment_focus = models.CharField(max_length=200, blank=True)
    
    # Métadonnées
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)
    last_synced = models.DateTimeField(null=True, blank=True)

    def __str__(self):
        return self.name

class Partner(models.Model):
    # Champs de l'API JEB
    jeb_id = models.IntegerField(unique=True)
    name = models.CharField(max_length=200)
    legal_status = models.CharField(max_length=50, blank=True)
    address = models.TextField(blank=True)
    email = models.EmailField(blank=True)
    phone = models.CharField(max_length=20, blank=True)
    description = models.TextField(blank=True)
    partner_type = models.CharField(max_length=50, blank=True)
    focus_area = models.CharField(max_length=200, blank=True)
    
    # Métadonnées
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)
    last_synced = models.DateTimeField(null=True, blank=True)

    def __str__(self):
        return self.name

class JEBUser(models.Model):
    # Champs de l'API JEB
    jeb_id = models.IntegerField(unique=True)
    name = models.CharField(max_length=200)
    email = models.EmailField()
    role = models.CharField(max_length=50, blank=True)
    company = models.CharField(max_length=200, blank=True)
    position = models.CharField(max_length=100, blank=True)
    bio = models.TextField(blank=True)
    linkedin_url = models.URLField(blank=True, null=True)
    twitter_url = models.URLField(blank=True, null=True)

    # Métadonnées
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)
    last_synced = models.DateTimeField(null=True, blank=True)

    def __str__(self):
        return self.name
