"""
Celery configuration for AREA project
"""
from __future__ import absolute_import, unicode_literals
import os
from celery import Celery
from celery.schedules import crontab

# Set the default Django settings module
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'area_poc.settings')

# Create Celery app
app = Celery('area')

# Load config from Django settings with 'CELERY_' prefix
app.config_from_object('django.conf:settings', namespace='CELERY')

# Auto-discover tasks in all installed apps
app.autodiscover_tasks()

# Configure Celery Beat schedule for periodic tasks
app.conf.beat_schedule = {
    'check-workflows-every-minute': {
        'task': 'api.tasks.check_all_workflows',
        'schedule': 60.0,  # Run every 60 seconds
    },
    'check-workflows-frequent': {
        'task': 'api.tasks.check_all_workflows',
        'schedule': 30.0,  # Also check every 30 seconds for faster response
    },
}

app.conf.timezone = 'UTC'

@app.task(bind=True)
def debug_task(self):
    print(f'Request: {self.request!r}')
