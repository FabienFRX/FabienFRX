"""
Celery tasks for AREA workflow execution
"""
from celery import shared_task
from django.contrib.auth.models import User
from django.utils import timezone
import logging

logger = logging.getLogger(__name__)


@shared_task(bind=True, max_retries=3)
def check_all_workflows(self):
    """
    Periodic task that checks all active AREAs and triggers reactions when actions are detected
    """
    try:
        from api.models import Area
        from api.workflow_executor import executor
        
        active_areas = Area.objects.filter(enabled=True).select_related(
            'user', 'action', 'reaction', 'action__service', 'reaction__service'
        )
        
        logger.info(f"Checking {active_areas.count()} active AREAs")
        
        triggered_count = 0
        for area in active_areas:
            try:
                # Check if action is triggered
                if executor.check_action_triggered(area):
                    logger.info(f"✓ Action triggered for AREA '{area.name}' (ID: {area.id})")
                    
                    # Execute reaction asynchronously
                    execute_reaction_task.delay(area.id)
                    triggered_count += 1
                    
            except Exception as e:
                logger.error(f"Error checking AREA '{area.name}' (ID: {area.id}): {e}")
                continue
        
        if triggered_count > 0:
            logger.info(f"✓ Triggered {triggered_count} AREA(s) in this cycle")
        
        return {
            'checked': active_areas.count(),
            'triggered': triggered_count,
            'timestamp': timezone.now().isoformat()
        }
        
    except Exception as exc:
        logger.error(f"Error in check_all_workflows task: {exc}")
        raise self.retry(exc=exc, countdown=60)


@shared_task(bind=True, max_retries=3)
def execute_reaction_task(self, area_id):
    """
    Execute a reaction for a specific AREA
    """
    from api.models import Area, AreaExecution
    from api.workflow_executor import executor
    
    area = None
    execution = None
    
    try:
        area = Area.objects.select_related(
            'user', 'action', 'reaction', 'action__service', 'reaction__service'
        ).get(id=area_id)
        
        # Create execution record
        execution = AreaExecution.objects.create(
            area=area,
            status='partial',
            action_data={'triggered_at': timezone.now().isoformat()}
        )
        
        logger.info(f"Executing reaction for AREA '{area.name}' (ID: {area.id})")
        executor.execute_reaction(area)
        
        # Update execution as successful
        execution.status = 'success'
        execution.reaction_result = {'completed_at': timezone.now().isoformat()}
        execution.save()
        
        logger.info(f"✓ Successfully executed reaction for AREA '{area.name}'")
        
        return {
            'area_id': area_id,
            'area_name': area.name,
            'status': 'success',
            'timestamp': timezone.now().isoformat()
        }
        
    except Exception as exc:
        logger.error(f"Error executing reaction for AREA ID {area_id}: {exc}")
        
        # Update execution as failed
        if execution:
            execution.status = 'failed'
            execution.error_message = str(exc)
            execution.save()
        
        raise self.retry(exc=exc, countdown=30)



@shared_task
def test_celery_task():
    """
    Test task to verify Celery is working
    """
    logger.info("✓ Celery test task executed successfully!")
    return "Celery is working!"


@shared_task
def cleanup_old_logs():
    """
    Cleanup old workflow execution logs (can be scheduled daily)
    """
    try:
        # Implement log cleanup logic if needed
        logger.info("Cleanup task executed")
        return "Cleanup completed"
    except Exception as e:
        logger.error(f"Error in cleanup task: {e}")
        return f"Cleanup failed: {e}"
