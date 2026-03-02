#!/usr/bin/env python
"""
Script to setup Dropbox service with actions and reactions in the database
"""
import os
import sys
import django

# Setup Django environment
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'area_poc.settings')
django.setup()

from api.models import Service, Action, Reaction

def setup_dropbox_service():
    """Create or update Dropbox service with all actions and reactions"""
    
    # Create or get Dropbox service
    dropbox_service, created = Service.objects.get_or_create(
        name='dropbox',
        defaults={
            'description': 'Dropbox file storage and sharing service'
        }
    )
    
    if created:
        print(f"✅ Created Dropbox service")
    else:
        print(f"ℹ️  Dropbox service already exists")
    
    # Define Dropbox actions (triggers)
    actions_data = [
        {
            'name': 'new_file_in_folder',
            'description': 'Trigger when a new file is added to a specific folder'
        },
        {
            'name': 'file_modified',
            'description': 'Trigger when a file is modified'
        },
    ]
    
    # Define Dropbox reactions (only working ones)
    reactions_data = [
        {
            'name': 'create_folder',
            'description': 'Create a new folder in Dropbox'
        },
        {
            'name': 'move_file',
            'description': 'Move a file to another location'
        },
        {
            'name': 'copy_file',
            'description': 'Copy a file to another location'
        },
        {
            'name': 'delete_file',
            'description': 'Delete a file from Dropbox'
        },
    ]
    
    # Create actions
    print("\n📌 Creating Dropbox actions...")
    for action_data in actions_data:
        action, created = Action.objects.get_or_create(
            service=dropbox_service,
            name=action_data['name'],
            defaults={
                'description': action_data['description']
            }
        )
        status = "✅ Created" if created else "ℹ️  Already exists"
        print(f"{status}: {action.name} - {action.description}")
    
    # Create reactions
    print("\n📌 Creating Dropbox reactions...")
    for reaction_data in reactions_data:
        reaction, created = Reaction.objects.get_or_create(
            service=dropbox_service,
            name=reaction_data['name'],
            defaults={
                'description': reaction_data['description']
            }
        )
        status = "✅ Created" if created else "ℹ️  Already exists"
        print(f"{status}: {reaction.name} - {reaction.description}")
    
    print("\n" + "="*60)
    print("✨ Dropbox service setup completed!")
    print("="*60)
    
    # Summary
    total_actions = Action.objects.filter(service=dropbox_service).count()
    total_reactions = Reaction.objects.filter(service=dropbox_service).count()
    
    print(f"\n📊 Summary:")
    print(f"  - Service: {dropbox_service.name}")
    print(f"  - Total Actions: {total_actions}")
    print(f"  - Total Reactions: {total_reactions}")

if __name__ == '__main__':
    setup_dropbox_service()
