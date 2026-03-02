"""
Django management command to run the workflow executor
"""
from django.core.management.base import BaseCommand
from api.workflow_executor import WorkflowExecutor
import signal
import sys

class Command(BaseCommand):
    help = 'Run the AREA workflow executor'

    def add_arguments(self, parser):
        parser.add_argument(
            '--daemon',
            action='store_true',
            help='Run as daemon in background',
        )

    def handle(self, *args, **options):
        executor = WorkflowExecutor()

        def signal_handler(sig, frame):
            self.stdout.write(self.style.WARNING('Stopping workflow executor...'))
            executor.stop_monitoring()
            sys.exit(0)

        # Handle Ctrl+C gracefully
        signal.signal(signal.SIGINT, signal_handler)
        signal.signal(signal.SIGTERM, signal_handler)

        self.stdout.write(self.style.SUCCESS('Starting AREA workflow executor...'))
        self.stdout.write('Press Ctrl+C to stop')

        try:
            executor.start_monitoring()
        except KeyboardInterrupt:
            self.stdout.write(self.style.WARNING('Workflow executor stopped by user'))
        except Exception as e:
            self.stdout.write(
                self.style.ERROR(f'Error running workflow executor: {e}')
            )