from django.db import models
from django.contrib.auth.models import User
from django.utils import timezone


class Service(models.Model):
    name = models.CharField(max_length=100)
    description = models.TextField()

    def __str__(self):
        return self.name


class Action(models.Model):
    service = models.ForeignKey(Service, on_delete=models.CASCADE, related_name='actions')
    name = models.CharField(max_length=100)
    description = models.TextField()

    def __str__(self):
        return f"{self.service.name} - {self.name}"


class Reaction(models.Model):
    service = models.ForeignKey(Service, on_delete=models.CASCADE, related_name='reactions')
    name = models.CharField(max_length=100)
    description = models.TextField()

    def __str__(self):
        return f"{self.service.name} - {self.name}"


class UserService(models.Model):
    """Model to store OAuth2 tokens and user service connections"""
    user = models.ForeignKey(User, on_delete=models.CASCADE, related_name='services')
    service = models.ForeignKey(Service, on_delete=models.CASCADE)
    access_token = models.TextField(blank=True, null=True)
    refresh_token = models.TextField(blank=True, null=True)
    token_expires_at = models.DateTimeField(blank=True, null=True)
    service_user_id = models.CharField(max_length=255, blank=True, null=True)
    service_username = models.CharField(max_length=255, blank=True, null=True)
    is_active = models.BooleanField(default=True)
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)

    class Meta:
        unique_together = ['user', 'service']

    def __str__(self):
        return f"{self.user.username} - {self.service.name}"

    def is_token_expired(self):
        if not self.token_expires_at:
            return False
        return timezone.now() > self.token_expires_at


class Area(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    name = models.CharField(max_length=200)
    action = models.ForeignKey(Action, on_delete=models.CASCADE)
    reaction = models.ForeignKey(Reaction, on_delete=models.CASCADE)
    enabled = models.BooleanField(default=True)
    created_at = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return self.name
