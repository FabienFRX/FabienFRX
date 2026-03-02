"""
Utility functions for service management
Centralizes common logic to avoid code duplication
"""
from .models import UserService, Service
import logging

logger = logging.getLogger(__name__)


def get_user_service_token(user, service_name):
    """
    Generic function to get UserService and access token for a given service
    
    Args:
        user: Django user instance
        service_name: Name of the service (google, dropbox, linkedin, github, gitlab)
    
    Returns:
        tuple: (user_service, access_token) or (None, None) if not found
    """
    try:
        service = Service.objects.filter(name=service_name).first()
        if not service:
            logger.error(f"{service_name} service not found in database")
            return None, None

        user_service = UserService.objects.filter(
            user=user,
            service=service,
            is_active=True
        ).first()

        if not user_service:
            logger.warning(f"No {service_name} service for user {user.email}")
            return None, None

        if not user_service.access_token:
            logger.error(f"No access token for user {user.email}")
            return None, None

        return user_service, user_service.access_token
    except Exception as e:
        logger.error(f"Failed to get {service_name} service for {user.email}: {e}")
        return None, None
