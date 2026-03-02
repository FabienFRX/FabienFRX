from django.core.management.base import BaseCommand
from django.contrib.auth.models import Group, User
from django.contrib.auth.models import Permission
from django.contrib.contenttypes.models import ContentType

class Command(BaseCommand):
    help = 'Crée les groupes d\'utilisateurs et les permissions'

    def handle(self, *args, **options):
        self.stdout.write("Setting up groups...")
