from django.urls import path
from . import auth_views
from . import main_views
from . import dropbox_auth_views

urlpatterns = [
    path('signup/', auth_views.SignUpView.as_view(), name='signup'),
    path('login/', auth_views.LoginView.as_view(), name='login'),
    path('logout/', auth_views.LogoutView.as_view(), name='logout'),
    path('profile/', auth_views.ProfileView.as_view(), name='profile'),
    
    # Google OAuth - New flow (GET redirects)
    path('google/init/', auth_views.GoogleOAuthInitView.as_view(), name='google_oauth_init'),
    path('google/callback/', auth_views.GoogleOAuthCallbackView.as_view(), name='google_oauth_callback'),
    # Google OAuth - Legacy endpoints (POST with code)
    path('google/login/', auth_views.GoogleLoginView.as_view(), name='google_login'),
    path('google/connect/', auth_views.GoogleConnectView.as_view(), name='google_connect'),
    
    # LinkedIn OAuth - New flow (GET redirects)
    path('linkedin/init/', auth_views.LinkedInOAuthInitView.as_view(), name='linkedin_oauth_init'),
    path('linkedin/callback/', auth_views.LinkedInOAuthCallbackView.as_view(), name='linkedin_oauth_callback'),
    # LinkedIn OAuth - Legacy endpoints (POST with code)
    path('linkedin/login/', auth_views.LinkedInLoginView.as_view(), name='linkedin_login'),
    path('linkedin/connect/', auth_views.LinkedInConnectView.as_view(), name='linkedin_connect'),
    
    # GitHub OAuth - New flow (GET redirects)
    path('github/init/', auth_views.GitHubOAuthInitView.as_view(), name='github_oauth_init'),
    path('github/callback/', auth_views.GitHubOAuthCallbackView.as_view(), name='github_oauth_callback'),
    # GitHub OAuth - Legacy endpoints (POST with code)
    path('github/login/', auth_views.GitHubLoginView.as_view(), name='github_login'),
    path('github/connect/', auth_views.GitHubConnectView.as_view(), name='github_connect'),

    # GitLab OAuth - New flow (GET redirects)
    path('gitlab/init/', auth_views.GitLabOAuthInitView.as_view(), name='gitlab_oauth_init'),
    path('gitlab/callback/', auth_views.GitLabOAuthCallbackView.as_view(), name='gitlab_oauth_callback'),
    # GitLab OAuth - Legacy endpoints (POST with code)
    path('gitlab/login/', auth_views.GitLabLoginView.as_view(), name='gitlab_login'),
    path('gitlab/connect/', auth_views.GitLabConnectView.as_view(), name='gitlab_connect'),

    # Dropbox OAuth
    path('dropbox/init/', dropbox_auth_views.dropbox_auth_init, name='dropbox_auth_init'),
    path('dropbox/callback/', dropbox_auth_views.dropbox_auth_callback, name='dropbox_callback'),
    path('dropbox/disconnect/', dropbox_auth_views.dropbox_disconnect, name='dropbox_disconnect'),
    path('dropbox/status/', dropbox_auth_views.dropbox_status, name='dropbox_status'),
    
    # User services
    path('services/', auth_views.UserServicesView.as_view(), name='user_services'),
    path('services/<int:service_id>/', main_views.UserServiceDetailView.as_view(), name='disconnect_service'),
]