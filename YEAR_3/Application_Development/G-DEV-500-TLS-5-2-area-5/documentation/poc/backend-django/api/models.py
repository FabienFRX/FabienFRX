from django.db import models
from django.contrib.auth.models import User


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


class Area(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    name = models.CharField(max_length=200)
    action = models.ForeignKey(Action, on_delete=models.CASCADE)
    reaction = models.ForeignKey(Reaction, on_delete=models.CASCADE)
    enabled = models.BooleanField(default=True)
    created_at = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return self.name
