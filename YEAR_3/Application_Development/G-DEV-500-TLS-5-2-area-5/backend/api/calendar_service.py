from datetime import datetime, timedelta
from google.oauth2.credentials import Credentials
from googleapiclient.discovery import build
import logging

logger = logging.getLogger(__name__)


class GoogleCalendarService:
    def __init__(self, access_token, refresh_token=None):
        self.access_token = access_token
        self.refresh_token = refresh_token

    def _get_service(self):
        """Get authenticated Google Calendar service"""
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
            service = build('calendar', 'v3', credentials=credentials)
            return service
        except Exception as e:
            logger.error(f"Failed to create Calendar service: {e}")
            return None

    def list_events(self, max_results=10, time_min=None):
        """List upcoming calendar events"""
        try:
            service = self._get_service()
            if not service:
                logger.error("Could not create Calendar service")
                return []

            if time_min is None:
                time_min = datetime.utcnow().isoformat() + 'Z'

            events_result = service.events().list(
                calendarId='primary',
                timeMin=time_min,
                maxResults=max_results,
                singleEvents=True,
                orderBy='startTime'
            ).execute()

            events = events_result.get('items', [])
            return events

        except Exception as e:
            logger.error(f"Failed to list calendar events: {e}")
            return []

    def get_upcoming_events(self, minutes=15):
        """Get events starting in the next N minutes"""
        try:
            service = self._get_service()
            if not service:
                logger.error("Could not create Calendar service")
                return []

            now = datetime.utcnow()
            time_min = now.isoformat() + 'Z'
            time_max = (now + timedelta(minutes=minutes)).isoformat() + 'Z'

            events_result = service.events().list(
                calendarId='primary',
                timeMin=time_min,
                timeMax=time_max,
                singleEvents=True,
                orderBy='startTime'
            ).execute()

            events = events_result.get('items', [])
            return events

        except Exception as e:
            logger.error(f"Failed to get upcoming events: {e}")
            return []

    def create_event(self, summary, start_time, end_time, description=None, location=None, attendees=None, send_notifications=True):
        """Create a new calendar event

        Args:
            summary: Event title
            start_time: Start datetime
            end_time: End datetime
            description: Event description
            location: Event location
            attendees: List of email addresses to invite (e.g. ['user1@example.com', 'user2@example.com'])
            send_notifications: Whether to send email notifications to attendees
        """
        try:
            service = self._get_service()
            if not service:
                logger.error("Could not create Calendar service")
                return None

            event = {
                'summary': summary,
                'start': {
                    'dateTime': start_time.isoformat(),
                    'timeZone': 'UTC',
                },
                'end': {
                    'dateTime': end_time.isoformat(),
                    'timeZone': 'UTC',
                },
            }

            if description:
                event['description'] = description

            if location:
                event['location'] = location

            # Add attendees if provided
            if attendees:
                event['attendees'] = [{'email': email.strip()} for email in attendees if email.strip()]

            created_event = service.events().insert(
                calendarId='primary',
                body=event,
                sendUpdates='all' if send_notifications else 'none'
            ).execute()

            logger.info(f"Event created: {created_event.get('htmlLink')}")
            return created_event

        except Exception as e:
            logger.error(f"Failed to create calendar event: {e}")
            return None

    def create_simple_event(self, title, duration_minutes=60, description=None, start_datetime=None, attendees=None, send_notifications=True):
        """Create a simple event starting now or at specified time

        Args:
            title: Event title
            duration_minutes: Duration in minutes (default 60)
            description: Event description
            start_datetime: ISO format datetime string (e.g. "2026-01-15T14:30:00") or None for now
            attendees: List of email addresses or comma-separated string
            send_notifications: Whether to send email notifications to attendees
        """
        if start_datetime:
            # Parse ISO format datetime string
            try:
                from dateutil import parser
                start_time = parser.parse(start_datetime)
                # Convert to UTC if needed
                if start_time.tzinfo is None:
                    start_time = start_time.replace(tzinfo=None)
            except Exception as e:
                logger.warning(f"Failed to parse start_datetime '{start_datetime}', using now: {e}")
                start_time = datetime.utcnow()
        else:
            start_time = datetime.utcnow()

        end_time = start_time + timedelta(minutes=duration_minutes)

        # Parse attendees if provided as string
        attendees_list = None
        if attendees:
            if isinstance(attendees, str):
                # Split by comma and clean up
                attendees_list = [email.strip() for email in attendees.split(',') if email.strip()]
            elif isinstance(attendees, list):
                attendees_list = attendees

        return self.create_event(title, start_time, end_time, description, attendees=attendees_list, send_notifications=send_notifications)

    def delete_event(self, event_id):
        """Delete a calendar event"""
        try:
            service = self._get_service()
            if not service:
                logger.error("Could not create Calendar service")
                return False

            service.events().delete(
                calendarId='primary',
                eventId=event_id
            ).execute()

            logger.info(f"Event {event_id} deleted")
            return True

        except Exception as e:
            logger.error(f"Failed to delete event: {e}")
            return False

    def create_workflow_event(self, area_name, action_description, reaction_description):
        """Create an event for workflow notification"""
        summary = f"AREA Workflow: {area_name}"
        description = f"""
        Workflow triggered:

        Action: {action_description}
        Reaction: {reaction_description}

        Created automatically by AREA system.
        """

        start_time = datetime.utcnow()
        end_time = start_time + timedelta(minutes=30)

        return self.create_event(summary, start_time, end_time, description)

    @staticmethod
    def get_calendar_service_for_user(user):
        """Static method to get Calendar service for a user"""
        try:
            from .models import UserService

            user_service = UserService.objects.filter(
                user=user,
                service__name='google'
            ).first()

            if not user_service or not user_service.access_token:
                logger.warning(f"No Google service found for user {user.username}")
                return None

            return GoogleCalendarService(
                access_token=user_service.access_token,
                refresh_token=user_service.refresh_token
            )

        except Exception as e:
            logger.error(f"Failed to get Calendar service for user: {e}")
            return None
