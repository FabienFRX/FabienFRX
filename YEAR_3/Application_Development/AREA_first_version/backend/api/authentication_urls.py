from django.urls import path
from . import auth_views
from . import main_views

urlpatterns = [
    path('signup/', auth_views.SignUpView.as_view(), name='signup'),
    path('login/', auth_views.LoginView.as_view(), name='login'),
    path('logout/', auth_views.LogoutView.as_view(), name='logout'),
    path('profile/', auth_views.ProfileView.as_view(), name='profile'),
    path('google/login/', auth_views.GoogleLoginView.as_view(), name='google_login'),
    path('google/connect/', auth_views.GoogleConnectView.as_view(), name='google_connect'),
    path('services/', auth_views.UserServicesView.as_view(), name='user_services'),
    path('services/<int:service_id>/', main_views.UserServiceDetailView.as_view(), name='disconnect_service'),
]