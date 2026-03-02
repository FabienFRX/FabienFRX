import dropbox
from dropbox.exceptions import ApiError, AuthError
import logging
from datetime import datetime

logger = logging.getLogger(__name__)

class DropboxService:
    def __init__(self, access_token, refresh_token=None):
        self.access_token = access_token
        self.refresh_token = refresh_token

    def _get_client(self):
        """Get authenticated Dropbox client"""
        try:
            dbx = dropbox.Dropbox(self.access_token)
            # Test connection
            dbx.users_get_current_account()
            return dbx
        except AuthError as e:
            logger.error(f"Dropbox authentication error: {e}")
            return None
        except Exception as e:
            logger.error(f"Failed to create Dropbox client: {e}")
            return None

    def list_folder(self, path=""):
        """List files and folders in a directory"""
        try:
            dbx = self._get_client()
            if not dbx:
                return []

            result = dbx.files_list_folder(path if path else "")
            entries = []
            
            for entry in result.entries:
                entries.append({
                    'name': entry.name,
                    'path': entry.path_display,
                    'type': 'folder' if isinstance(entry, dropbox.files.FolderMetadata) else 'file',
                    'modified': entry.client_modified.isoformat() if isinstance(entry, dropbox.files.FileMetadata) else None,
                    'size': entry.size if isinstance(entry, dropbox.files.FileMetadata) else None
                })
            
            logger.info(f"Listed {len(entries)} items in folder '{path}'")
            return entries

        except ApiError as e:
            logger.error(f"Failed to list folder: {e}")
            return []

    def upload_file(self, file_path, content, mode='overwrite'):
        """Upload file to Dropbox
        
        Args:
            file_path: Path where to save the file (e.g., '/documents/file.txt')
            content: File content as bytes or string
            mode: 'overwrite' or 'add' (fail if exists)
        """
        try:
            dbx = self._get_client()
            if not dbx:
                return False

            # Convert string to bytes if needed
            if isinstance(content, str):
                content = content.encode('utf-8')

            write_mode = dropbox.files.WriteMode.overwrite if mode == 'overwrite' else dropbox.files.WriteMode.add

            result = dbx.files_upload(
                content,
                file_path,
                mode=write_mode
            )

            logger.info(f"File uploaded successfully: {result.path_display}")
            return True

        except ApiError as e:
            logger.error(f"Failed to upload file: {e}")
            return False

    def create_folder(self, folder_path):
        """Create a new folder"""
        try:
            dbx = self._get_client()
            if not dbx:
                return False

            result = dbx.files_create_folder_v2(folder_path)
            logger.info(f"Folder created: {result.metadata.path_display}")
            return True

        except ApiError as e:
            if e.error.is_path() and e.error.get_path().is_conflict():
                logger.warning(f"Folder already exists: {folder_path}")
                return True
            logger.error(f"Failed to create folder: {e}")
            return False

    def move_file(self, from_path, to_path):
        """Move or rename a file/folder"""
        try:
            dbx = self._get_client()
            if not dbx:
                return False

            result = dbx.files_move_v2(from_path, to_path)
            logger.info(f"Moved {from_path} to {result.metadata.path_display}")
            return True

        except ApiError as e:
            logger.error(f"Failed to move file: {e}")
            return False

    def copy_file(self, from_path, to_path):
        """Copy a file/folder"""
        try:
            dbx = self._get_client()
            if not dbx:
                return False

            result = dbx.files_copy_v2(from_path, to_path)
            logger.info(f"Copied {from_path} to {result.metadata.path_display}")
            return True

        except ApiError as e:
            logger.error(f"Failed to copy file: {e}")
            return False

    def delete_file(self, path):
        """Delete a file or folder"""
        try:
            dbx = self._get_client()
            if not dbx:
                return False

            result = dbx.files_delete_v2(path)
            logger.info(f"Deleted: {result.metadata.path_display}")
            return True

        except ApiError as e:
            logger.error(f"Failed to delete file: {e}")
            return False

    def create_shared_link(self, path):
        """Create a shared link for a file/folder"""
        try:
            dbx = self._get_client()
            if not dbx:
                return None

            # Try to get existing shared link first
            try:
                links = dbx.sharing_list_shared_links(path=path)
                if links.links:
                    logger.info(f"Using existing shared link for {path}")
                    return links.links[0].url
            except:
                pass

            # Create new shared link
            link = dbx.sharing_create_shared_link_with_settings(path)
            logger.info(f"Created shared link: {link.url}")
            return link.url

        except ApiError as e:
            logger.error(f"Failed to create shared link: {e}")
            return None

    def download_file(self, path):
        """Download file content"""
        try:
            dbx = self._get_client()
            if not dbx:
                return None

            metadata, response = dbx.files_download(path)
            content = response.content
            
            logger.info(f"Downloaded file: {metadata.path_display} ({metadata.size} bytes)")
            return content

        except ApiError as e:
            logger.error(f"Failed to download file: {e}")
            return None

    def search_files(self, query, folder_path=""):
        """Search for files matching query"""
        try:
            dbx = self._get_client()
            if not dbx:
                return []

            result = dbx.files_search_v2(query, options=dropbox.files.SearchOptions(
                path=folder_path if folder_path else None,
                max_results=20
            ))

            matches = []
            for match in result.matches:
                metadata = match.metadata.get_metadata()
                if isinstance(metadata, dropbox.files.FileMetadata):
                    matches.append({
                        'name': metadata.name,
                        'path': metadata.path_display,
                        'modified': metadata.client_modified.isoformat()
                    })

            logger.info(f"Found {len(matches)} files matching '{query}'")
            return matches

        except ApiError as e:
            logger.error(f"Failed to search files: {e}")
            return []

    def get_file_metadata(self, path):
        """Get metadata for a specific file"""
        try:
            dbx = self._get_client()
            if not dbx:
                return None

            metadata = dbx.files_get_metadata(path)
            
            if isinstance(metadata, dropbox.files.FileMetadata):
                return {
                    'name': metadata.name,
                    'path': metadata.path_display,
                    'size': metadata.size,
                    'modified': metadata.client_modified.isoformat(),
                    'rev': metadata.rev
                }
            elif isinstance(metadata, dropbox.files.FolderMetadata):
                return {
                    'name': metadata.name,
                    'path': metadata.path_display,
                    'type': 'folder'
                }

            return None

        except ApiError as e:
            logger.error(f"Failed to get file metadata: {e}")
            return None
