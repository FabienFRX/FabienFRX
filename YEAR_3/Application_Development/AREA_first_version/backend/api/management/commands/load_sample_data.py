from django.core.management.base import BaseCommand
from api.models import Service, Action, Reaction


class Command(BaseCommand):
    help = 'Load sample services, actions and reactions'

    def handle(self, *args, **options):
        # Create Facebook service
        facebook, created = Service.objects.get_or_create(
            name='facebook',
            defaults={'description': 'Facebook social media platform'}
        )

        if created:
            self.stdout.write(f"Created service: {facebook.name}")

            # Facebook Actions
            Action.objects.create(
                service=facebook,
                name='new_message_in_group',
                description='A new message is posted in the group'
            )
            Action.objects.create(
                service=facebook,
                name='new_message_inbox',
                description='A new private message is received by the user'
            )
            Action.objects.create(
                service=facebook,
                name='new_like',
                description='The user gains a like from one of their messages'
            )

            # Facebook Reactions
            Reaction.objects.create(
                service=facebook,
                name='like_message',
                description='The user likes a message'
            )

        # Create Google service
        google, created = Service.objects.get_or_create(
            name='google',
            defaults={'description': 'Google services platform'}
        )

        if created:
            self.stdout.write(f"Created service: {google.name}")

            # Google Actions
            Action.objects.create(
                service=google,
                name='new_email_received',
                description='A new email is received in Gmail'
            )
            Action.objects.create(
                service=google,
                name='calendar_event_starts',
                description='A calendar event is about to start'
            )

            # Google Reactions
            Reaction.objects.create(
                service=google,
                name='send_email',
                description='Send an email via Gmail'
            )
            Reaction.objects.create(
                service=google,
                name='create_calendar_event',
                description='Create a new calendar event'
            )

        # Create Timer service
        timer, created = Service.objects.get_or_create(
            name='timer',
            defaults={'description': 'Time-based triggers and actions'}
        )

        if created:
            self.stdout.write(f"Created service: {timer.name}")

            # Timer Actions
            Action.objects.create(
                service=timer,
                name='specific_time',
                description='The current time is HH:MM'
            )
            Action.objects.create(
                service=timer,
                name='specific_date',
                description='The current date is DD/MM'
            )
            Action.objects.create(
                service=timer,
                name='days_until_event',
                description='In X days it will be Y'
            )

        self.stdout.write(
            self.style.SUCCESS('Successfully loaded sample data')
        )