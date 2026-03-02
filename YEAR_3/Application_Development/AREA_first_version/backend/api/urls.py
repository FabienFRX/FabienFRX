from django.urls import path
from . import views
from . import main_views

urlpatterns = [
    path('about.json', views.about, name='about'),
    path('hello/', views.hello_world, name='hello'),
    path('services/', main_views.ServicesListView.as_view(), name='services_list'),
    path('areas/', main_views.UserAreasView.as_view(), name='user_areas'),
    path('areas/<int:area_id>/', main_views.UserAreaDetailView.as_view(), name='area_detail'),
]