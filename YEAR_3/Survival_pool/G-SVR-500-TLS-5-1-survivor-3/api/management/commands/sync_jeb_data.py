from django.core.management.base import BaseCommand
from api.services import JEBAPIService

class Command(BaseCommand):
    help = 'Synchronise les données depuis l\'API JEB'

    def add_arguments(self, parser):
        parser.add_argument(
            '--type',
            type=str,
            choices=['all', 'startups', 'news', 'events', 'investors', 'partners', 'users'],
            default='all',
            help='Type de données à synchroniser'
        )

    def handle(self, *args, **options):
        self.stdout.write("Début de la synchronisation des données " + options['type'])
        
        service = JEBAPIService()
        
        if options['type'] == 'all':
            self.stdout.write("Starting full data synchronization...")
            service.sync_all_data()
            self.stdout.write("Data synchronization completed!")
        elif options['type'] == 'startups':
            self.stdout.write("Syncing startups...")
            service.sync_startups()
        elif options['type'] == 'news':
            self.stdout.write("Syncing news...")
            service.sync_news()
        elif options['type'] == 'events':
            self.stdout.write("Syncing events...")
            service.sync_events()
        elif options['type'] == 'investors':
            self.stdout.write("Syncing investors...")
            service.sync_investors()
        elif options['type'] == 'partners':
            self.stdout.write("Syncing partners...")
            service.sync_partners()
        elif options['type'] == 'users':
            self.stdout.write("Syncing JEB users...")
            service.sync_users()
        
        self.stdout.write("Synchronisation des données " + options['type'] + " terminée avec succès!")
