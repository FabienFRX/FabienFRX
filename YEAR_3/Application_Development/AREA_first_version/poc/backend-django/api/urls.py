from django.urls import path
from . import views

urlpatterns = [
    path('about.json', views.about, name='about'),
    path('hello/', views.hello_world, name='hello'),
]