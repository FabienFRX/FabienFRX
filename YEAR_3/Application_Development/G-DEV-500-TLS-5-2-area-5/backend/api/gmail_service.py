import base64
import json
from email.mime.text import MIMEText
from google.oauth2.credentials import Credentials
from googleapiclient.discovery import build
import logging

logger = logging.getLogger(__name__)

class GmailService:
    def __init__(self, access_token, refresh_token=None):
        self.access_token = access_token
        self.refresh_token = refresh_token

    def _get_service(self):
        """Get authenticated Gmail service"""
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
            service = build('gmail', 'v1', credentials=credentials)
            return service
        except Exception as e:
            logger.error(f"Failed to create Gmail service: {e}")
            return None

    def send_email(self, to_email, subject, body):
        """Send an email via Gmail API"""
        try:
            service = self._get_service()
            if not service:
                logger.error("Could not create Gmail service")
                return False

            # Create message
            message = MIMEText(body)
            message['to'] = to_email
            message['subject'] = subject

            # Encode message
            raw_message = base64.urlsafe_b64encode(message.as_bytes()).decode()

            # Send email
            send_message = service.users().messages().send(
                userId='me',
                body={'raw': raw_message}
            ).execute()

            logger.info(f"Email sent successfully. Message ID: {send_message.get('id')}")
            return True

        except Exception as e:
            logger.error(f"Failed to send email: {e}")
            return False

    def send_workflow_notification(self, area_name, action_description, reaction_description):
        """Send a workflow notification email (compatibility method)"""
        subject = f"AREA Workflow: {area_name}"
        body = f"""
        Your AREA workflow "{area_name}" has been triggered!

        Action: {action_description}
        Reaction: {reaction_description}

        This is an automated message from your AREA system.
        """

        # For now, send to 'me' (the authenticated user)
        # In a real implementation, this could be configurable
        return self.send_email("me", subject, body)

    def search_emails(self, subject_contains=None, from_email=None, max_results=10, only_unread=True):
        """Search for emails matching filters"""
        try:
            service = self._get_service()
            if not service:
                logger.error("Could not create Gmail service")
                return []

            # Build Gmail API query
            query_parts = []
            if only_unread:
                query_parts.append('is:unread')
            if subject_contains:
                query_parts.append(f'subject:"{subject_contains}"')
            if from_email:
                query_parts.append(f'from:{from_email}')
            
            query = ' '.join(query_parts) if query_parts else 'is:unread'
            
            logger.info(f"Searching emails with query: {query}")

            # Search for messages
            results = service.users().messages().list(
                userId='me',
                q=query,
                maxResults=max_results
            ).execute()

            messages = results.get('messages', [])
            logger.info(f"Found {len(messages)} matching emails")
            
            # Log first few message IDs for debugging
            if messages:
                logger.info(f"First message IDs: {[msg['id'] for msg in messages[:3]]}")
            
            return [msg['id'] for msg in messages]

        except Exception as e:
            logger.error(f"Failed to search emails: {e}")
            return []

    def forward_email(self, message_id, to_email):
        """Forward an email to another address"""
        try:
            service = self._get_service()
            if not service:
                return False

            # Get the original message
            message = service.users().messages().get(
                userId='me',
                id=message_id,
                format='raw'
            ).execute()

            # Forward it
            forward_message = {
                'raw': message['raw']
            }
            
            service.users().messages().send(
                userId='me',
                body=forward_message
            ).execute()

            logger.info(f"Email {message_id} forwarded to {to_email}")
            return True

        except Exception as e:
            logger.error(f"Failed to forward email: {e}")
            return False

    def mark_as_read(self, message_id):
        """Mark an email as read"""
        try:
            service = self._get_service()
            if not service:
                return False

            service.users().messages().modify(
                userId='me',
                id=message_id,
                body={'removeLabelIds': ['UNREAD']}
            ).execute()

            logger.info(f"Email {message_id} marked as read")
            return True

        except Exception as e:
            logger.error(f"Failed to mark email as read: {e}")
            return False

    def add_label(self, message_id, label_name):
        """Add a label to an email"""
        try:
            service = self._get_service()
            if not service:
                return False

            # Get or create label
            labels = service.users().labels().list(userId='me').execute()
            label_id = None
            
            for label in labels.get('labels', []):
                if label['name'] == label_name:
                    label_id = label['id']
                    break
            
            if not label_id:
                # Create new label
                label_obj = service.users().labels().create(
                    userId='me',
                    body={
                        'name': label_name,
                        'labelListVisibility': 'labelShow',
                        'messageListVisibility': 'show'
                    }
                ).execute()
                label_id = label_obj['id']
                logger.info(f"Created new label: {label_name}")

            # Add label to message
            service.users().messages().modify(
                userId='me',
                id=message_id,
                body={'addLabelIds': [label_id]}
            ).execute()

            logger.info(f"Label '{label_name}' added to email {message_id}")
            return True

        except Exception as e:
            logger.error(f"Failed to add label: {e}")
            return False

    def get_unread_messages(self, max_results=10):
        """Get unread messages"""
        try:
            service = self._get_service()
            if not service:
                logger.error("Could not create Gmail service")
                return []

            results = service.users().messages().list(
                userId='me',
                q='is:unread',
                maxResults=max_results
            ).execute()

            messages = results.get('messages', [])
            logger.info(f"Found {len(messages)} unread messages")
            return messages

        except Exception as e:
            logger.error(f"Failed to get unread messages: {e}")
            return []

    def get_messages_from_sender(self, sender_email, max_results=10):
        """Get messages from a specific sender"""
        try:
            service = self._get_service()
            if not service:
                logger.error("Could not create Gmail service")
                return []

            query = f'from:{sender_email} is:unread'
            results = service.users().messages().list(
                userId='me',
                q=query,
                maxResults=max_results
            ).execute()

            messages = results.get('messages', [])
            logger.info(f"Found {len(messages)} unread messages from {sender_email}")
            return messages

        except Exception as e:
            logger.error(f"Failed to get messages from sender: {e}")
            return []
