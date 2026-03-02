from django.contrib import admin
from api import views as api_views
from django.urls import path, include
from django.http import JsonResponse

def api_root(request):
    return JsonResponse({
        'message': 'JEB Incubator Platform API',
        'version': '1.0.0',
        'endpoints': {
            'api': '/api/',
            'admin': '/admin/',
            'docs': '/api/docs/'
        }
    })

urlpatterns = [
    path('', api_root, name='api-root'),
    path('favicon.ico', api_views.favicon_view, name='favicon'),
    path('admin/', admin.site.urls),
    path('api/', include('api.urls')),
]
