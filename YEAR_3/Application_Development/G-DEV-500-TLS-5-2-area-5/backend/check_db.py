
import os
import django
import sys

# Setup Django environment
sys.path.append('/app')
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'area_poc.settings')
django.setup()

from api.models import Service, Action, Reaction

def check_services():
    services = ['github', 'gitlab']
    for name in services:
        try:
            service = Service.objects.get(name=name)
            action_count = service.actions.count()
            reaction_count = service.reactions.count()
            print(f"Service: {name}")
            print(f"  Actions: {action_count}")
            print(f"  Reactions: {reaction_count}")
            if action_count > 0:
                print(f"  Sample Action: {service.actions.first().name}")
            if reaction_count > 0:
                print(f"  Sample Reaction: {service.reactions.first().name}")
        except Service.DoesNotExist:
            print(f"Service '{name}' NOT FOUND in database")

if __name__ == '__main__':
    check_services()
