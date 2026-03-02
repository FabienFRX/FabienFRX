from django.urls import path
from . import views
from . import main_views
from . import execution_views

urlpatterns = [
    path('about.json', views.about, name='about'),
    path('hello/', views.hello_world, name='hello'),
    path('services/', main_views.ServicesListView.as_view(), name='services_list'),
    path('areas/', main_views.UserAreasView.as_view(), name='user_areas'),
    path('areas/<int:area_id>/', main_views.UserAreaDetailView.as_view(), name='area_detail'),
    path('executor/', main_views.WorkflowExecutorView.as_view(), name='workflow_executor'),
    path('executions/', execution_views.AreaExecutionsView.as_view(), name='area_executions'),
    path('executions/<int:area_id>/', execution_views.AreaExecutionsView.as_view(), name='area_executions_detail'),
    path('health/celery/', execution_views.CeleryHealthView.as_view(), name='celery_health'),
]