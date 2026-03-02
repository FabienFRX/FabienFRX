#!/usr/bin/env python
"""
Script to setup LinkedIn service with actions and reactions in the database
"""
import os
import sys
import django

# Setup Django environment
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'area_poc.settings')
django.setup()

from api.models import Service, Action, Reaction

def setup_linkedin_service():
    """Create or update LinkedIn service with all actions and reactions"""
    
    # Create or get LinkedIn service
    linkedin_service, created = Service.objects.get_or_create(
        name='linkedin',
        defaults={
            'description': 'LinkedIn professional networking'
        }
    )
    
    if created:
        print(f"✅ Created LinkedIn service")
    else:
        print(f"ℹ️  LinkedIn service already exists")
    
    # Define LinkedIn actions (triggers)
    actions_data = [
        {
            'name': 'new_post_by_user',
            'description': 'Trigger when you create a new post on LinkedIn'
        },
    ]
    
    # Define LinkedIn reactions (only working ones)
    reactions_data = [
        {
            'name': 'share_text_post',
            'description': 'Share a text post on LinkedIn'
        },
    ]
    
    # Create actions
    print("\n📌 Creating LinkedIn actions...")
    for action_data in actions_data:
        action, created = Action.objects.get_or_create(
            service=linkedin_service,
            name=action_data['name'],
            defaults={
                'description': action_data['description']
            }
        )
        status = "✅ Created" if created else "ℹ️  Already exists"
        print(f"{status}: {action.name} - {action.description}")
    
    # Create reactions
    print("\n📌 Creating LinkedIn reactions...")
    for reaction_data in reactions_data:
        reaction, created = Reaction.objects.get_or_create(
            service=linkedin_service,
            name=reaction_data['name'],
            defaults={
                'description': reaction_data['description']
            }
        )
        status = "✅ Created" if created else "ℹ️  Already exists"
        print(f"{status}: {reaction.name} - {reaction.description}")
    
    print("\n" + "="*60)
    print("✨ LinkedIn service setup completed!")
    print("="*60)
    
    # Summary
    total_actions = Action.objects.filter(service=linkedin_service).count()
    total_reactions = Reaction.objects.filter(service=linkedin_service).count()
    
    print(f"\n📊 Summary:")
    print(f"  - Service: {linkedin_service.name}")
    print(f"  - Total Actions: {total_actions}")
    print(f"  - Total Reactions: {total_reactions}")

if __name__ == '__main__':
    setup_linkedin_service()
