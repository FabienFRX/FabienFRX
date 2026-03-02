from io import BytesIO
from google.oauth2.credentials import Credentials
from googleapiclient.discovery import build
from googleapiclient.http import MediaIoBaseUpload
import logging

logger = logging.getLogger(__name__)


class GoogleDriveService:
    def __init__(self, access_token, refresh_token=None):
        self.access_token = access_token
        self.refresh_token = refresh_token

    def _get_service(self):
        """Get authenticated Google Drive service"""
        try:
            # Create credentials object
            credentials = Credentials(
                token=self.access_token,
                refresh_token=self.refresh_token,
                token_uri='https://oauth2.googleapis.com/token',
        client_id = "REMOVED",
        client_secret = "REMOVED",
            )

            # Build service
            service = build('drive', 'v3', credentials=credentials)
            return service
        except Exception as e:
            logger.error(f"Failed to create Drive service: {e}")
            return None

    def list_files(self, max_results=10, query=None):
        """List files in Google Drive"""
        try:
            service = self._get_service()
            if not service:
                logger.error("Could not create Drive service")
                return []

            params = {
                'pageSize': max_results,
                'fields': 'files(id, name, mimeType, createdTime, modifiedTime, owners, shared)'
            }

            if query:
                params['q'] = query

            results = service.files().list(**params).execute()
            files = results.get('files', [])
            return files

        except Exception as e:
            logger.error(f"Failed to list files: {e}")
            return []

    def get_recent_uploads(self, max_results=10):
        """Get recently uploaded files"""
        # Simplifié: liste juste les fichiers non-dossiers sans filtre complexe
        try:
            service = self._get_service()
            if not service:
                return []

            params = {
                'pageSize': max_results,
                'orderBy': 'createdTime desc',
                'fields': 'files(id, name, mimeType, createdTime)',
                'q': "mimeType != 'application/vnd.google-apps.folder'"
            }

            results = service.files().list(**params).execute()
            files = results.get('files', [])
            return files

        except Exception as e:
            logger.error(f"Failed to get recent uploads: {e}")
            return []

    def get_shared_files(self, max_results=10):
        """Get recently shared files"""
        query = "sharedWithMe = true"
        return self.list_files(max_results=max_results, query=query)

    def create_folder(self, folder_name, parent_id=None):
        """Create a new folder in Google Drive"""
        try:
            service = self._get_service()
            if not service:
                logger.error("Could not create Drive service")
                return None

            file_metadata = {
                'name': folder_name,
                'mimeType': 'application/vnd.google-apps.folder'
            }

            if parent_id:
                file_metadata['parents'] = [parent_id]

            folder = service.files().create(
                body=file_metadata,
                fields='id, name, webViewLink'
            ).execute()

            logger.info(f"Folder created: {folder.get('name')} ({folder.get('id')})")
            return folder

        except Exception as e:
            logger.error(f"Failed to create folder: {e}")
            return None

    def upload_file(self, filename, content, mime_type='text/plain', parent_id=None):
        """Upload a file to Google Drive"""
        try:
            service = self._get_service()
            if not service:
                logger.error("Could not create Drive service")
                return None

            file_metadata = {'name': filename}

            if parent_id:
                file_metadata['parents'] = [parent_id]

            # Convert content to bytes if it's a string
            if isinstance(content, str):
                content = content.encode('utf-8')

            # Create file stream
            fh = BytesIO(content)
            media = MediaIoBaseUpload(fh, mimetype=mime_type, resumable=True)

            file = service.files().create(
                body=file_metadata,
                media_body=media,
                fields='id, name, webViewLink'
            ).execute()

            logger.info(f"File uploaded: {file.get('name')} ({file.get('id')})")
            return file

        except Exception as e:
            logger.error(f"Failed to upload file: {e}")
            return None

    def share_file(self, file_id, email, role='reader'):
        """Share a file with another user"""
        try:
            service = self._get_service()
            if not service:
                logger.error("Could not create Drive service")
                return False

            permission = {
                'type': 'user',
                'role': role,  # 'reader', 'writer', 'commenter'
                'emailAddress': email
            }

            service.permissions().create(
                fileId=file_id,
                body=permission,
                fields='id'
            ).execute()

            logger.info(f"File {file_id} shared with {email} as {role}")
            return True

        except Exception as e:
            logger.error(f"Failed to share file: {e}")
            return False

    def delete_file(self, file_id):
        """Delete a file from Google Drive"""
        try:
            service = self._get_service()
            if not service:
                logger.error("Could not create Drive service")
                return False

            service.files().delete(fileId=file_id).execute()
            logger.info(f"File {file_id} deleted successfully")
            return True

        except Exception as e:
            logger.error(f"Failed to delete file: {e}")
            return False

    def save_workflow_log(self, area_name, action_description, reaction_description):
        """Save a workflow log file to Drive"""
        from datetime import datetime

        filename = f"AREA_Log_{area_name}_{datetime.now().strftime('%Y%m%d_%H%M%S')}.txt"
        content = f"""
AREA Workflow Log
=================

Workflow: {area_name}
Timestamp: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}

Action: {action_description}
Reaction: {reaction_description}

This log was automatically created by the AREA system.
"""

        return self.upload_file(filename, content)

    @staticmethod
    def get_drive_service_for_user(user):
        """Static method to get Drive service for a user"""
        try:
            from .models import UserService

            user_service = UserService.objects.filter(
                user=user,
                service__name='google'
            ).first()

            if not user_service or not user_service.access_token:
                logger.warning(f"No Google service found for user {user.username}")
                return None

            return GoogleDriveService(
                access_token=user_service.access_token,
                refresh_token=user_service.refresh_token
            )

        except Exception as e:
            logger.error(f"Failed to get Drive service for user: {e}")
            return None
