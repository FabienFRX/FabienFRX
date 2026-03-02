from .gmail_service import GmailService
from .calendar_service import GoogleCalendarService
from .drive_service import GoogleDriveService
from .dropbox_service import DropboxService
from .service_utils import get_user_service_token
import logging

logger = logging.getLogger(__name__)

def get_google_calendar_service(user):
    """Get Google Calendar service for user"""
    user_service, _ = get_user_service_token(user, 'google')
    if not user_service:
        return None
    
    calendar_service = GoogleCalendarService(
        access_token=user_service.access_token,
        refresh_token=user_service.refresh_token
    )
    logger.info(f"✅ Calendar service created for user {user.email}")
    return calendar_service

def get_google_drive_service(user):
    """Get Google Drive service for user"""
    user_service, _ = get_user_service_token(user, 'google')
    if not user_service:
        return None
    
    drive_service = GoogleDriveService(
        access_token=user_service.access_token,
        refresh_token=user_service.refresh_token
    )
    logger.info(f"✅ Drive service created for user {user.email}")
    return drive_service

def get_google_gmail_service(user):
    """Get Gmail service for user"""
    user_service, _ = get_user_service_token(user, 'google')
    if not user_service:
        return None
    
    gmail_service = GmailService(
        access_token=user_service.access_token,
        refresh_token=user_service.refresh_token
    )
    logger.info(f"✅ Gmail service created for user {user.email}")
    return gmail_service

def get_dropbox_service(user):
    """Get Dropbox service for user"""
    user_service, _ = get_user_service_token(user, 'dropbox')
    if not user_service:
        return None
    
    dropbox_svc = DropboxService(
        access_token=user_service.access_token,
        refresh_token=user_service.refresh_token
    )
    logger.info(f"✅ Dropbox service created for user {user.email}")
    return dropbox_svc
