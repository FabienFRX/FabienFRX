
import os
import sys
import django

sys.path.append(os.path.dirname(os.path.abspath(__file__)))
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'area_poc.settings')
django.setup()

from api.models import Area

def inspect_latest_areas():
    # Get last 5 areas
    areas = Area.objects.order_by('-id')[:5]
    for area in areas:
        print(f"AREA {area.id}: {area.name}")
        print(f"  Trigger: {area.action.service.name}.{area.action.name}")
        print(f"  Reaction: {area.reaction.service.name}.{area.reaction.name}")
        print(f"  Action Config: {area.action_config}")
        print(f"  Reaction Config: {area.reaction_config}")
        print("-" * 20)

if __name__ == '__main__':
    inspect_latest_areas()
