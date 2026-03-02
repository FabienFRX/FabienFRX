#!/usr/bin/env python3
"""
Simple script to add basic services, actions and reactions needed for MVP
"""

import os
import django

# Configure Django
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'area_poc.settings')
django.setup()

from api.models import Service, Action, Reaction

def setup_mvp_data():
    """Create minimal data needed for MVP demo"""

    print("Setting up MVP data...")

    # Create Google service
    google_service, created = Service.objects.get_or_create(
        name='google',
        defaults={'description': 'Google services integration'}
    )
    if created:
        print("✅ Created Google service")
    else:
        print("📌 Google service already exists")

    # Create actions for Google (only working ones)
    actions_data = [
        {'name': 'calendar_event_created', 'description': 'A new calendar event is created'},
        {'name': 'calendar_event_upcoming', 'description': 'A calendar event is starting soon'},
        {'name': 'drive_file_uploaded', 'description': 'A new file is uploaded to Drive'},
    ]

    for action_data in actions_data:
        action, created = Action.objects.get_or_create(
            service=google_service,
            name=action_data['name'],
            defaults={'description': action_data['description']}
        )
        if created:
            print(f"✅ Created action: {action_data['name']}")

    # Create reactions for Google (only working ones)
    reactions_data = [
        {'name': 'send_email', 'description': 'Send an email via Gmail'},
        {'name': 'create_calendar_event', 'description': 'Create a new calendar event'},
    ]

    for reaction_data in reactions_data:
        reaction, created = Reaction.objects.get_or_create(
            service=google_service,
            name=reaction_data['name'],
            defaults={'description': reaction_data['description']}
        )
        if created:
            print(f"✅ Created reaction: {reaction_data['name']}")

    # Create Timer service for time-based triggers
    timer_service, created = Service.objects.get_or_create(
        name='timer',
        defaults={'description': 'Time-based automation triggers'}
    )
    if created:
        print("✅ Created Timer service")
    else:
        print("📌 Timer service already exists")

    # Timer actions (only working ones)
    timer_actions = [
        {'name': 'daily_at_time', 'description': 'The current time is HH:MM'},
        {'name': 'weekly_on_day', 'description': 'Trigger on specific day of the week'},
        {'name': 'every_x_seconds', 'description': 'Trigger every X seconds'},
        {'name': 'every_x_minutes', 'description': 'Trigger every X minutes'},
        {'name': 'every_x_hours', 'description': 'Trigger every X hours'},
    ]

    print("📌 Creating Timer actions...")
    for action_data in timer_actions:
        action, created = Action.objects.get_or_create(
            service=timer_service,
            name=action_data['name'],
            defaults={'description': action_data['description']}
        )

    print("✨ MVP data setup completed!")
    print(f"  - Google Actions: {Action.objects.filter(service=google_service).count()}")
    print(f"  - Google Reactions: {Reaction.objects.filter(service=google_service).count()}")
    print(f"  - Timer Actions: {Action.objects.filter(service=timer_service).count()}")


if __name__ == '__main__':
    setup_mvp_data()
    