import time
import logging
from datetime import datetime, timedelta
from typing import Dict, Any, Optional
from django.contrib.auth.models import User
from .models import Area, UserService
from .gmail_service import GmailService
from .google_services import (
    get_google_calendar_service,
    get_google_drive_service,
    get_google_gmail_service
)
from .github_service import get_github_service
from .linkedin_service import get_linkedin_service

logger = logging.getLogger(__name__)

class WorkflowExecutor:

    def __init__(self):
        self.running = False
        self.last_check = {}  # Store last check times for different triggers
        self.last_check_data = {}  # Store additional data for stateful checks (like file lists)

    def start_monitoring(self):
        self.running = True
        logger.info("Workflow executor started")

        while self.running:
            try:
                self.check_all_workflows()
                time.sleep(30)
            except Exception as e:
                logger.error(f"Error in workflow monitoring: {e}")
                time.sleep(60)

    def stop_monitoring(self):
        self.running = False
        logger.info("Workflow executor stopped")

    def check_all_workflows(self):
        active_areas = Area.objects.filter(enabled=True)

        for area in active_areas:
            try:
                if self.check_action_triggered(area):
                    logger.info(f"Action triggered for workflow: {area.name}")
                    area.refresh_from_db()
                    self.execute_reaction(area)
            except Exception as e:
                logger.error(f"Error checking workflow {area.name}: {e}")

    def check_action_triggered(self, area: Area) -> bool:
        action_name = area.action.name
        service_name = area.action.service.name

        if service_name == "timer":
            return self._check_timer_action(area, action_name)
        elif service_name == "google":
            return self._check_google_action(area, action_name)
        elif service_name == "dropbox":
            return self._check_dropbox_action(area, action_name)
        elif service_name == "github":
            return self._check_github_action(area, action_name)
        elif service_name == "gitlab":
            return self._check_gitlab_action(area, action_name)
        elif service_name == "linkedin":
            return self._check_linkedin_action(area, action_name)

        return False

    def _check_timer_action(self, area: Area, action_name: str) -> bool:
        from django.utils import timezone
        import pytz
        
        now = timezone.now()
        paris_tz = pytz.timezone('Europe/Paris')
        now_paris = now.astimezone(paris_tz)
        area_key = f"timer_{area.id}"

        if action_name == "specific_time":
            try:
                action_config = area.action_config or {}
                target_date = action_config.get('date')
                target_time = action_config.get('time', '12:00')
                
                if not target_date:
                    logger.warning(f"specific_time for AREA {area.id} missing 'date' configuration")
                    return False
                
                from datetime import datetime as dt
                target_datetime_str = f"{target_date} {target_time}"
                target_datetime_naive = dt.strptime(target_datetime_str, "%Y-%m-%d %H:%M")
                target_datetime = paris_tz.localize(target_datetime_naive)
                
                logger.info(f"AREA {area.id} specific_time check: now={now_paris.strftime('%Y-%m-%d %H:%M:%S %Z')}, target={target_datetime.strftime('%Y-%m-%d %H:%M:%S %Z')}")
                
                if area_key in self.last_check:
                    logger.info(f"AREA {area.id} specific_time already executed")
                    return False
                
                time_diff = (now_paris - target_datetime).total_seconds()
                logger.info(f"AREA {area.id} time_diff: {time_diff} seconds")
                
                if -300 <= time_diff <= 300:
                    logger.info(f"✅ Specific time trigger for AREA {area.id}: target {target_datetime_str} (Paris) reached!")
                    self.last_check[area_key] = now
                    return True
                    
            except Exception as e:
                logger.error(f"Error in specific_time for AREA {area.id}: {e}")
                import traceback
                logger.error(traceback.format_exc())
                return False

        elif action_name == "daily_at_time":
            try:
                action_config = area.action_config or {}
                target_time = action_config.get('time', '12:00')
                target_hour, target_minute = map(int, target_time.split(':'))

                if area_key not in self.last_check:
                    self.last_check[area_key] = now - timedelta(hours=2)

                time_diff = now - self.last_check[area_key]

                current_time_minutes = now.hour * 60 + now.minute
                target_time_minutes = target_hour * 60 + target_minute

                time_diff_minutes = abs(current_time_minutes - target_time_minutes)

                last_trigger_date = getattr(self, f'{area_key}_last_date', None)
                today = now.date()

                if (time_diff_minutes <= 1 and
                    last_trigger_date != today and
                    time_diff.total_seconds() > 30):

                    logger.info(f"Timer trigger for AREA {area.id}: target time {target_time} reached (current: {now.strftime('%H:%M')})")
                    self.last_check[area_key] = now
                    setattr(self, f'{area_key}_last_date', today)
                    return True

            except Exception as e:
                logger.error(f"Error parsing timer configuration for AREA {area.id}: {e}")
                if area_key not in self.last_check:
                    self.last_check[area_key] = now - timedelta(hours=2)
                time_diff = now - self.last_check[area_key]
                if time_diff.total_seconds() > 3600:
                    self.last_check[area_key] = now
                    return True

        elif action_name == "weekly_on_day":
            if area_key not in self.last_check:
                self.last_check[area_key] = now - timedelta(hours=3)

            time_diff = now - self.last_check[area_key]
            if time_diff.total_seconds() > 7200:
                self.last_check[area_key] = now
                return True

        elif action_name in ["every_x_seconds", "every_x_minutes", "every_x_hours"]:
            return self._check_interval_timer(area, action_name, area_key, now)

        return False

    def _check_interval_timer(self, area, action_name, area_key, now):
        """Generic interval timer checker"""
        try:
            action_config = area.action_config or {}
            
            # Get interval based on action type
            if action_name == "every_x_seconds":
                interval_value = int(action_config.get('seconds', 30))
                interval_seconds = interval_value
                unit = "s"
            elif action_name == "every_x_minutes":
                interval_value = int(action_config.get('minutes', 5))
                interval_seconds = interval_value * 60
                unit = "min"
            else:  # every_x_hours
                interval_value = int(action_config.get('hours', 1))
                interval_seconds = interval_value * 3600
                unit = "h"

            # Initialize last check if needed
            if area_key not in self.last_check:
                self.last_check[area_key] = now - timedelta(seconds=interval_seconds + 1)

            # Check if interval has elapsed
            time_diff = now - self.last_check[area_key]
            if time_diff.total_seconds() >= interval_seconds:
                logger.info(f"Interval timer ({interval_value}{unit}) triggered for AREA {area.id}")
                self.last_check[area_key] = now
                return True
                
        except Exception as e:
            logger.error(f"Error in {action_name} for AREA {area.id}: {e}")
        
        return False

    def _check_google_action(self, area: Area, action_name: str) -> bool:
        area_key = f"google_{area.id}_{action_name}"
        now = datetime.now()

        if area_key not in self.last_check:
            self.last_check[area_key] = now - timedelta(minutes=5)

        time_diff = now - self.last_check[area_key]

        if time_diff.total_seconds() < 120:
            return False

        user = area.user
        action_config = area.action_config or {}

        if action_name == "new_email_received":
            gmail_service = get_google_gmail_service(user)
            if gmail_service:
                # Use filters if provided, otherwise check for any unread
                subject_contains = action_config.get('subject_contains', '')
                from_email = action_config.get('from_email', '')
                
                if subject_contains or from_email:
                    # Use search_emails with filters
                    message_ids = gmail_service.search_emails(
                        subject_contains=subject_contains if subject_contains else None,
                        from_email=from_email if from_email else None,
                        only_unread=True,
                        max_results=1
                    )
                    if message_ids:
                        logger.info(f"New email matching filters detected for user {user.username}")
                        self.last_check[area_key] = now
                        return True
                else:
                    # Check for any unread
                    messages = gmail_service.get_unread_messages(max_results=1)
                    if messages:
                        logger.info(f"New unread email detected for user {user.username}")
                        self.last_check[area_key] = now
                        return True

        elif action_name == "new_email_from_sender":
            sender = action_config.get('sender_email', '')
            if not sender:
                logger.warning(f"No sender_email configured for AREA {area.id}")
                return False

            gmail_service = get_google_gmail_service(user)
            if gmail_service:
                messages = gmail_service.get_messages_from_sender(sender, max_results=1)
                if messages:
                    message_id = messages[0]['id']
                    last_message_key = f"{area_key}_last_message"
                    
                    # Use last_check_data dict for persistence
                    if area_key not in self.last_check_data:
                        self.last_check_data[area_key] = {}
                    
                    if self.last_check_data[area_key].get('last_message_id') != message_id:
                        logger.info(f"New email from {sender} detected for user {user.username}")
                        self.last_check_data[area_key]['last_message_id'] = message_id
                        self.last_check[area_key] = now
                        return True

        elif action_name == "calendar_event_created":
            calendar_service = get_google_calendar_service(user)
            if calendar_service:
                events = calendar_service.list_events(max_results=1)
                if events:
                    event_id = events[0]['id']
                    
                    # Use last_check_data dict for persistence
                    if area_key not in self.last_check_data:
                        self.last_check_data[area_key] = {}
                    
                    if self.last_check_data[area_key].get('last_event_id') != event_id:
                        logger.info(f"New calendar event detected for user {user.username}")
                        self.last_check_data[area_key]['last_event_id'] = event_id
                        self.last_check[area_key] = now
                        return True

        elif action_name == "calendar_event_starting":
            minutes = action_config.get('minutes_before', 15)
            calendar_service = get_google_calendar_service(user)
            if calendar_service:
                upcoming_events = calendar_service.get_upcoming_events(minutes=minutes)
                if upcoming_events:
                    event_id = upcoming_events[0]['id']
                    
                    # Use last_check_data dict for persistence
                    if area_key not in self.last_check_data:
                        self.last_check_data[area_key] = {}
                    
                    if self.last_check_data[area_key].get('last_upcoming_id') != event_id:
                        logger.info(f"Event starting in {minutes} minutes detected for user {user.username}")
                        self.last_check_data[area_key]['last_upcoming_id'] = event_id
                        self.last_check[area_key] = now
                        return True

        # Drive actions
        elif action_name == "new_drive_file":
            drive_service = get_google_drive_service(user)
            if drive_service:
                files = drive_service.get_recent_uploads(max_results=1)
                if files:
                    file_id = files[0]['id']
                    
                    # Use last_check_data dict for persistence
                    if area_key not in self.last_check_data:
                        self.last_check_data[area_key] = {}
                    
                    if self.last_check_data[area_key].get('last_file_id') != file_id:
                        logger.info(f"New file uploaded to Drive for user {user.username}")
                        self.last_check_data[area_key]['last_file_id'] = file_id
                        self.last_check[area_key] = now
                        return True

        elif action_name == "drive_file_shared":
            drive_service = get_google_drive_service(user)
            if drive_service:
                files = drive_service.get_shared_files(max_results=1)
                if files:
                    file_id = files[0]['id']
                    
                    # Use last_check_data dict for persistence
                    if area_key not in self.last_check_data:
                        self.last_check_data[area_key] = {}
                    
                    if self.last_check_data[area_key].get('last_shared_id') != file_id:
                        logger.info(f"New shared file detected for user {user.username}")
                        self.last_check_data[area_key]['last_shared_id'] = file_id
                        self.last_check[area_key] = now
                        return True

        self.last_check[area_key] = now
        return False

    def _check_github_action(self, area: Area, action_name: str) -> bool:
        from .github_service import get_github_service

        area_key = f"github_{area.id}_{action_name}"
        now = datetime.now()

        if area_key not in self.last_check:
            self.last_check[area_key] = now - timedelta(minutes=2)

        time_diff = now - self.last_check[area_key]

        if time_diff.total_seconds() < 60:
            return False

        try:
            github_service = get_github_service(area.user)
            if not github_service:
                logger.error(f"No GitHub service found for user {area.user.email}")
                return False

            reaction_config = area.reaction_config or {}
            owner = reaction_config.get('owner')
            repo = reaction_config.get('repo')

            if not owner or not repo:
                logger.warning(f"GitHub action for AREA {area.id} missing owner/repo configuration")
                return False

            if action_name == "new_issue":
                issues_url = f'/repos/{owner}/{repo}/issues'
                since_time = (now - timedelta(hours=1)).strftime('%Y-%m-%dT%H:%M:%SZ')
                response = github_service._make_request('GET', issues_url, params={'since': since_time, 'state': 'all'})

                if response and isinstance(response, list):
                    for issue in response:
                        issue_created_at = datetime.strptime(issue['created_at'], '%Y-%m-%dT%H:%M:%SZ')
                        if issue_created_at > self.last_check[area_key]:
                            logger.info(f"New issue detected: #{issue['number']} - {issue['title']}")
                            action_config = area.action_config or {}
                            action_config['_triggered_issue'] = {
                                'issue_number': issue['number'],
                                'issue_title': issue['title'],
                                'issue_url': issue['html_url'],
                                'owner': owner,
                                'repo': repo
                            }
                            area.action_config = action_config
                            area.save()
                            self.last_check[area_key] = now
                            return True

            elif action_name == "new_commit":
                # Get recent commits (last 1 hour)
                commits_url = f'/repos/{owner}/{repo}/commits'
                since_time = (now - timedelta(hours=1)).strftime('%Y-%m-%dT%H:%M:%SZ')
                response = github_service._make_request('GET', commits_url, params={'since': since_time})

                if response and isinstance(response, list):
                    # Check if there are new commits since last check
                    for commit in response:
                        commit_date_str = commit['commit']['author']['date']
                        commit_created_at = datetime.strptime(commit_date_str, '%Y-%m-%dT%H:%M:%SZ')
                        if commit_created_at > self.last_check[area_key]:
                            logger.info(f"New commit detected: {commit['sha'][:7]} - {commit['commit']['message'][:50]}")
                            # Store commit data for reaction to use
                            action_config = area.action_config or {}
                            action_config['_triggered_commit'] = {
                                'commit_sha': commit['sha'],
                                'commit_message': commit['commit']['message'],
                                'commit_url': commit['html_url'],
                                'author': commit['commit']['author']['name'],
                                'owner': owner,
                                'repo': repo
                            }
                            area.action_config = action_config
                            area.save()
                            self.last_check[area_key] = now
                            return True

            elif action_name == "new_pull_request":
                # Get recent pull requests (last 1 hour)
                prs_url = f'/repos/{owner}/{repo}/pulls'
                response = github_service._make_request('GET', prs_url, params={'state': 'all', 'sort': 'created', 'direction': 'desc'})

                if response and isinstance(response, list):
                    # Check if there are new PRs since last check
                    for pr in response:
                        pr_created_at = datetime.strptime(pr['created_at'], '%Y-%m-%dT%H:%M:%SZ')
                        if pr_created_at > self.last_check[area_key]:
                            logger.info(f"New PR detected: #{pr['number']} - {pr['title']}")
                            # Store PR data for reaction to use
                            action_config = area.action_config or {}
                            action_config['_triggered_pr'] = {
                                'pr_number': pr['number'],
                                'pr_title': pr['title'],
                                'pr_url': pr['html_url'],
                                'owner': owner,
                                'repo': repo
                            }
                            area.action_config = action_config
                            area.save()
                            self.last_check[area_key] = now
                            return True

            elif action_name == "new_star":
                # Get stargazers (last 100)
                stargazers_url = f'/repos/{owner}/{repo}/stargazers'
                response = github_service._make_request('GET', stargazers_url, params={'per_page': 100}, headers={'Accept': 'application/vnd.github.v3.star+json'})

                if response and isinstance(response, list):
                    # Check if there are new stars since last check
                    for star in response:
                        starred_at = datetime.strptime(star['starred_at'], '%Y-%m-%dT%H:%M:%SZ')
                        if starred_at > self.last_check[area_key]:
                            logger.info(f"New star detected from: {star['user']['login']}")
                            # Store star data for reaction to use
                            action_config = area.action_config or {}
                            action_config['_triggered_star'] = {
                                'starred_by': star['user']['login'],
                                'starred_at': star['starred_at'],
                                'owner': owner,
                                'repo': repo
                            }
                            area.action_config = action_config
                            area.save()
                            self.last_check[area_key] = now
                            return True

            elif action_name == "new_release":
                # Get recent releases
                releases_url = f'/repos/{owner}/{repo}/releases'
                response = github_service._make_request('GET', releases_url)

                if response and isinstance(response, list):
                    # Check if there are new releases since last check
                    for release in response:
                        published_at = datetime.strptime(release['published_at'], '%Y-%m-%dT%H:%M:%SZ')
                        if published_at > self.last_check[area_key]:
                            logger.info(f"New release detected: {release['tag_name']} - {release['name']}")
                            # Store release data for reaction to use
                            action_config = area.action_config or {}
                            action_config['_triggered_release'] = {
                                'release_tag': release['tag_name'],
                                'release_name': release['name'],
                                'release_url': release['html_url'],
                                'owner': owner,
                                'repo': repo
                            }
                            area.action_config = action_config
                            area.save()
                            self.last_check[area_key] = now
                            return True

            elif action_name == "new_repository":
                # Get user's repositories (check for new repos)
                # Note: This checks authenticated user's repos, not a specific owner
                repos_url = f'/user/repos'
                response = github_service._make_request('GET', repos_url, params={'sort': 'created', 'direction': 'desc', 'per_page': 10})

                if response and isinstance(response, list):
                    # Check if there are new repos since last check
                    for repo_data in response:
                        created_at = datetime.strptime(repo_data['created_at'], '%Y-%m-%dT%H:%M:%SZ')
                        if created_at > self.last_check[area_key]:
                            logger.info(f"New repository detected: {repo_data['full_name']}")
                            # Store repo data for reaction to use
                            action_config = area.action_config or {}
                            action_config['_triggered_repo'] = {
                                'repo_name': repo_data['name'],
                                'repo_full_name': repo_data['full_name'],
                                'repo_url': repo_data['html_url'],
                                'owner': repo_data['owner']['login']
                            }
                            area.action_config = action_config
                            area.save()
                            self.last_check[area_key] = now
                            return True

            self.last_check[area_key] = now

        except Exception as e:
            logger.error(f"Error checking GitHub action {action_name}: {e}")
            return False

    def _check_gitlab_action(self, area: Area, action_name: str) -> bool:
        from .gitlab_service import get_gitlab_service

        area_key = f"gitlab_{area.id}_{action_name}"
        now = datetime.now()

        if area_key not in self.last_check:
            self.last_check[area_key] = now - timedelta(minutes=2)

        time_diff = now - self.last_check[area_key]

        if time_diff.total_seconds() < 60:
            return False

        try:
            gitlab_service = get_gitlab_service(area.user)
            if not gitlab_service:
                logger.error(f"No GitLab service found for user {area.user.email}")
                return False

            action_config = area.action_config or {}
            project_id = action_config.get('project_id')

            if not project_id:
                logger.warning(f"GitLab action for AREA {area.id} missing project_id configuration")
                return False

            if action_name == "new_issue":
                issues_url = f'/projects/{project_id}/issues'
                response = gitlab_service._make_request('GET', issues_url, params={'state': 'opened', 'order_by': 'created_at', 'sort': 'desc'})

                if response and isinstance(response, list):
                    for issue in response:
                        issue_created_at = datetime.strptime(issue['created_at'], '%Y-%m-%dT%H:%M:%S.%fZ')
                        if issue_created_at > self.last_check[area_key]:
                            logger.info(f"New GitLab issue detected: #{issue['iid']} - {issue['title']}")
                            action_config = area.action_config or {}
                            action_config['_triggered_issue'] = {
                                'issue_iid': issue['iid'],
                                'issue_title': issue['title'],
                                'issue_url': issue['web_url'],
                                'project_id': project_id
                            }
                            area.action_config = action_config
                            area.save()
                            self.last_check[area_key] = now
                            return True

            elif action_name == "new_commit":
                commits_url = f'/projects/{project_id}/repository/commits'
                since_time = (now - timedelta(hours=1)).strftime('%Y-%m-%dT%H:%M:%SZ')
                response = gitlab_service._make_request('GET', commits_url, params={'since': since_time})

                if response and isinstance(response, list):
                    for commit in response:
                        commit_date_str = commit['created_at']
                        commit_created_at = datetime.strptime(commit_date_str, '%Y-%m-%dT%H:%M:%S.%fZ')
                        if commit_created_at > self.last_check[area_key]:
                            logger.info(f"New GitLab commit detected: {commit['short_id']} - {commit['title'][:50]}")
                            action_config = area.action_config or {}
                            action_config['_triggered_commit'] = {
                                'commit_sha': commit['id'],
                                'commit_message': commit['message'],
                                'commit_url': commit['web_url'],
                                'author': commit['author_name'],
                                'project_id': project_id
                            }
                            area.action_config = action_config
                            area.save()
                            self.last_check[area_key] = now
                            return True

            elif action_name == "new_merge_request":
                mrs_url = f'/projects/{project_id}/merge_requests'
                response = gitlab_service._make_request('GET', mrs_url, params={'state': 'opened', 'order_by': 'created_at', 'sort': 'desc'})

                if response and isinstance(response, list):
                    for mr in response:
                        mr_created_at = datetime.strptime(mr['created_at'], '%Y-%m-%dT%H:%M:%S.%fZ')
                        if mr_created_at > self.last_check[area_key]:
                            logger.info(f"New GitLab MR detected: !{mr['iid']} - {mr['title']}")
                            action_config = area.action_config or {}
                            action_config['_triggered_mr'] = {
                                'mr_iid': mr['iid'],
                                'mr_title': mr['title'],
                                'mr_url': mr['web_url'],
                                'project_id': project_id
                            }
                            area.action_config = action_config
                            area.save()
                            self.last_check[area_key] = now
                            return True

            elif action_name == "new_star":
                project_url = f'/projects/{project_id}'
                response = gitlab_service._make_request('GET', project_url)

                if response:
                    star_count = response.get('star_count', 0)
                    last_star_count = self.last_check.get(f"{area_key}_count", 0)

                    if star_count > last_star_count:
                        logger.info(f"New GitLab star detected: {star_count} (was {last_star_count})")
                        action_config = area.action_config or {}
                        action_config['_triggered_star'] = {
                            'star_count': star_count,
                            'project_id': project_id
                        }
                        area.action_config = action_config
                        area.save()
                        self.last_check[f"{area_key}_count"] = star_count
                        self.last_check[area_key] = now
                        return True
                    self.last_check[f"{area_key}_count"] = star_count

            elif action_name == "new_release":
                releases_url = f'/projects/{project_id}/releases'
                response = gitlab_service._make_request('GET', releases_url)

                if response and isinstance(response, list):
                    for release in response:
                        released_at = datetime.strptime(release['created_at'], '%Y-%m-%dT%H:%M:%S.%fZ')
                        if released_at > self.last_check[area_key]:
                            logger.info(f"New GitLab release detected: {release['tag_name']} - {release['name']}")
                            action_config = area.action_config or {}
                            action_config['_triggered_release'] = {
                                'release_tag': release['tag_name'],
                                'release_name': release['name'],
                                'release_url': release['_links']['self'],
                                'project_id': project_id
                            }
                            area.action_config = action_config
                            area.save()
                            self.last_check[area_key] = now
                            return True

            self.last_check[area_key] = now

        except Exception as e:
            logger.error(f"Error checking GitLab action {action_name}: {e}")
            return False

    def _check_dropbox_action(self, area: Area, action_name: str) -> bool:
        """Check Dropbox-based actions"""
        from .google_services import get_dropbox_service
        
        user = area.user
        dropbox_service = get_dropbox_service(user)
        
        if not dropbox_service:
            logger.warning(f"No Dropbox service for user {user.username}")
            return False

        area_key = f"dropbox_{area.id}_{action_name}"
        config = area.action_config or {}

        if action_name == "new_file_in_folder":
            # Check for new files in specified folder
            folder_path = config.get('folder_path', '')
            
            # Get current files
            current_files = dropbox_service.list_folder(folder_path)
            current_file_names = {f['name'] for f in current_files if f['type'] == 'file'}
            
            # Check against last known state
            if area_key in self.last_check_data:
                last_files = self.last_check_data[area_key].get('files', set())
                new_files = current_file_names - last_files
                
                if new_files:
                    logger.info(f"New file(s) detected in Dropbox folder '{folder_path}': {new_files}")
                    self.last_check_data[area_key] = {'files': current_file_names}
                    return True
            
            # First check - initialize state
            self.last_check_data[area_key] = {'files': current_file_names}
            return False

        elif action_name == "file_modified":
            # Check if specific file was modified
            file_path = config.get('file_path', '')
            
            if not file_path:
                return False
            
            metadata = dropbox_service.get_file_metadata(file_path)
            if not metadata:
                return False
            
            current_rev = metadata.get('rev')
            
            if area_key in self.last_check_data:
                last_rev = self.last_check_data[area_key].get('rev')
                if last_rev and current_rev != last_rev:
                    logger.info(f"File modified in Dropbox: {file_path}")
                    self.last_check_data[area_key] = {'rev': current_rev}
                    return True
            
            # First check
            self.last_check_data[area_key] = {'rev': current_rev}
            return False

        return False

    def _check_linkedin_action(self, area: Area, action_name: str) -> bool:
        area_key = f"linkedin_{area.id}_{action_name}"
        now = datetime.now()

        if area_key not in self.last_check:
            self.last_check[area_key] = now - timedelta(minutes=5)
            # Initialize data storage if needed
            if area_key not in self.last_check_data:
                self.last_check_data[area_key] = {}

        time_diff = now - self.last_check[area_key]
        if time_diff.total_seconds() < 60:
            return False
            
        try:
            linkedin_service = get_linkedin_service(area.user)
            if not linkedin_service:
                return False

            if action_name == "new_post_by_user":
                posts = linkedin_service.get_recent_posts()
                if posts:
                    # Sort by creation time (descending)
                    # LinkedIn timestamps are usually in 'created' or specificContent
                    latest_post = posts[0]
                    post_urn = latest_post.get('id')
                    
                    last_post_urn = self.last_check_data[area_key].get('last_post_urn')
                    
                    if last_post_urn and last_post_urn != post_urn:
                        logger.info(f"New LinkedIn post detected: {post_urn}")
                        self.last_check_data[area_key]['last_post_urn'] = post_urn
                        self.last_check[area_key] = now
                        return True
                    
                    # Store for next time
                    if not last_post_urn:
                        self.last_check_data[area_key]['last_post_urn'] = post_urn

            elif action_name == "new_like_on_post":
                logger.debug("Likedin 'new_like_on_post' checks are disabled.")
                return False

            self.last_check[area_key] = now

        except Exception as e:
            logger.error(f"Error checking LinkedIn action {action_name}: {e}")
            
        return False

    def execute_reaction(self, area: Area):
        reaction_name = area.reaction.name
        service_name = area.reaction.service.name

        logger.info(f"Executing reaction: {service_name}.{reaction_name} for workflow: {area.name}")

        try:
            if service_name == "email":
                self._execute_email_reaction(area, reaction_name)
            elif service_name == "google":
                self._execute_google_reaction(area, reaction_name)
            elif service_name == "dropbox":
                self._execute_dropbox_reaction(area, reaction_name)
            elif service_name == "github":
                self._execute_github_reaction(area, reaction_name)
            elif service_name == "gitlab":
                self._execute_gitlab_reaction(area, reaction_name)
            elif service_name == "linkedin":
                self._execute_linkedin_reaction(area, reaction_name)
            elif service_name == "timer":
                self._execute_timer_reaction(area, reaction_name)
            else:
                logger.info(f"Unknown reaction service: {service_name}")

        except Exception as e:
            logger.error(f"Failed to execute reaction {reaction_name}: {e}")

    def _execute_timer_reaction(self, area: Area, reaction_name: str):
        """Execute Timer reactions (fallback/simulation)"""
        logger.info(f"Executing Timer reaction: {reaction_name}")
        
        if reaction_name == "send_notification":
            # Map to email notification if possible
            self._execute_email_reaction(area, "send_notification_email")

    def _execute_linkedin_reaction(self, area: Area, reaction_name: str):
        from .linkedin_service import get_linkedin_service

        user = area.user
        linkedin_service = get_linkedin_service(user)
        
        if not linkedin_service:
            logger.warning(f"No LinkedIn service available for user {user.username}")
            return

        reaction_config = area.reaction_config or {}

        if reaction_name == "share_text_post":
            text = reaction_config.get('text', '')
            if not text and area.action.service.name == "github":
                action_config = area.action_config or {}
                issue_info = action_config.get('_triggered_issue', {})
                if issue_info:
                    text = f"Keep an eye on this issue: {issue_info.get('issue_title')} ({issue_info.get('issue_url')})"
            
            if not text:
                text = f"Automated post from AREA for workflow '{area.name}'"

            post_id = linkedin_service.create_post(text)
            if post_id:
                logger.info(f"LinkedIn post created: {post_id}")
            else:
                logger.error("Failed to create LinkedIn post")

        elif reaction_name == "share_article_post":
            text = reaction_config.get('text', f"New article via AREA workflow '{area.name}'")
            article_url = reaction_config.get('article_url', '')
            
            if article_url:
                post_id = linkedin_service.create_post(text, article_url=article_url)
                if post_id:
                    logger.info(f"LinkedIn article post created: {post_id}")
                else:
                    logger.error("Failed to create LinkedIn article post")
            else:
                logger.warning("No article_url provided for share_article_post")

    def _execute_email_reaction(self, area: Area, reaction_name: str):
        user = area.user

        gmail_service = get_google_gmail_service(user)
        if not gmail_service:
            logger.warning(f"No Gmail service available for user {user.username}")
            return

        if reaction_name == "send_notification_email":
            success = gmail_service.send_workflow_notification(
                area_name=area.name,
                action_description=f"{area.action.service.name} - {area.action.description}",
                reaction_description=f"{area.reaction.service.name} - {area.reaction.description}"
            )

            if success:
                logger.info(f"Notification email sent successfully to {user.email} for workflow {area.name}")
            else:
                logger.error(f"Failed to send notification email to {user.email}")

        elif reaction_name == "send_alert_email":
            subject = f"AREA Alert: {area.name}"
            body = f"""
            <html>
                <body style="font-family: Arial, sans-serif; color: #333;">
                    <h2 style="color: #DC2626;">AREA Alert</h2>
                    <p>Workflow "<strong>{area.name}</strong>" has been triggered!</p>
                    <div style="background-color: #FEF2F2; padding: 15px; border-left: 4px solid #DC2626; margin: 20px 0;">
                        <p><strong>User:</strong> {user.username}</p>
                        <p><strong>Time:</strong> {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}</p>
                        <p><strong>Action:</strong> {area.action.description}</p>
                    </div>
                </body>
            </html>
            """

            success = gmail_service.send_email(
                to_email=user.email,
                subject=subject,
                body=body,
                is_html=True
            )

            if success:
                logger.info(f"Alert email sent successfully to {user.email} for workflow {area.name}")
            else:
                logger.error(f"Failed to send alert email to {user.email}")

    def _execute_google_reaction(self, area: Area, reaction_name: str):
        user = area.user
        config = area.reaction_config or {}

        if reaction_name == "send_email":
            gmail_service = get_google_gmail_service(user)
            if gmail_service:
                to_email = config.get('to_email', user.email)
                subject = config.get('subject', f"AREA Notification: {area.name}")
                body = config.get('body', f"Your AREA '{area.name}' has been triggered!")

                success = gmail_service.send_email(to_email, subject, body)
                if success:
                    logger.info(f"Gmail email sent to {to_email} for workflow {area.name}")
                else:
                    logger.error(f"Failed to send Gmail email to {to_email}")
            else:
                logger.warning(f"No Gmail service for user {user.username}")

        elif reaction_name == "forward_email":
            gmail_service = get_google_gmail_service(user)
            if gmail_service:
                to_email = config.get('to_email', user.email)
                subject_contains = config.get('subject_contains', '')
                from_email = config.get('from_email', '')

                message_ids = gmail_service.search_emails(
                    subject_contains=subject_contains if subject_contains else None,
                    from_email=from_email if from_email else None,
                    max_results=5
                )

                if message_ids:
                    logger.info(f"Found {len(message_ids)} emails matching filters")
                    for message_id in message_ids:
                        success = gmail_service.forward_email(message_id, to_email)
                        if success:
                            logger.info(f"Email {message_id} forwarded to {to_email}")
                        else:
                            logger.error(f"Failed to forward email {message_id}")
                else:
                    logger.warning(f"No emails found matching filters")
            else:
                logger.warning(f"No Gmail service for user {user.username}")

        elif reaction_name == "mark_email_as_read":
            gmail_service = get_google_gmail_service(user)
            if gmail_service:
                subject_contains = config.get('subject_contains', '')
                from_email = config.get('from_email', '')

                logger.info(f"mark_email_as_read - Filters: subject_contains='{subject_contains}', from_email='{from_email}'")

                message_ids = gmail_service.search_emails(
                    subject_contains=subject_contains if subject_contains else None,
                    from_email=from_email if from_email else None,
                    max_results=10,
                    only_unread=True
                )

                if message_ids:
                    logger.info(f"Found {len(message_ids)} unread emails matching filters")
                    success_count = 0
                    for message_id in message_ids:
                        success = gmail_service.mark_as_read(message_id)
                        if success:
                            success_count += 1
                            logger.info(f"Email {message_id} marked as read successfully")
                        else:
                            logger.error(f"Failed to mark email {message_id} as read")
                    logger.info(f"Marked {success_count}/{len(message_ids)} emails as read")
                else:
                    logger.warning(f"No unread emails found matching filters (subject: '{subject_contains}', from: '{from_email}')")
            else:
                logger.warning(f"No Gmail service for user {user.username}")

        elif reaction_name == "add_email_label":
            gmail_service = get_google_gmail_service(user)
            if gmail_service:
                subject_contains = config.get('subject_contains', '')
                from_email = config.get('from_email', '')
                label_name = config.get('label_name', 'AREA')

                message_ids = gmail_service.search_emails(
                    subject_contains=subject_contains if subject_contains else None,
                    from_email=from_email if from_email else None,
                    max_results=10
                )

                if message_ids:
                    logger.info(f"Found {len(message_ids)} emails matching filters")
                    for message_id in message_ids:
                        success = gmail_service.add_label(message_id, label_name)
                        if success:
                            logger.info(f"Label '{label_name}' added to email {message_id}")
                        else:
                            logger.error(f"Failed to add label to email {message_id}")
                else:
                    logger.info(f"No emails found matching filters")
            else:
                logger.warning(f"No Gmail service for user {user.username}")

        elif reaction_name == "create_calendar_event":
            calendar_service = get_google_calendar_service(user)
            if calendar_service:
                title = config.get('title', f"AREA Event: {area.name}")
                duration = config.get('duration', 60)
                description = config.get('description', f"Event created by AREA '{area.name}'")
                start_datetime = config.get('start_datetime')
                attendees = config.get('attendees')
                send_notifications = config.get('send_notifications', True)

                event = calendar_service.create_simple_event(
                    title,
                    duration_minutes=duration,
                    description=description,
                    start_datetime=start_datetime,
                    attendees=attendees,
                    send_notifications=send_notifications
                )
                if event:
                    logger.info(f"Calendar event '{title}' created for user {user.username}")
                    
                    if send_notifications:
                        gmail_service = get_google_gmail_service(user)
                        if gmail_service:
                            email_subject = f"nouvel event : {title}"
                            email_body = f"""
                            Un nouvel événement a été créé dans votre calendrier :
                            
                            Titre : {title}
                            Durée : {duration} minutes
                            Description : {description or 'Aucune description'}
                            
                            Lien vers l'événement : {event.get('htmlLink', 'N/A')}
                            """
                            
                            email_sent = gmail_service.send_email(
                                to_email=user.email,
                                subject=email_subject,
                                body=email_body
                            )
                            
                            if email_sent:
                                logger.info(f"Email notification sent to {user.email} for calendar event '{title}'")
                            else:
                                logger.error(f"Failed to send email notification for calendar event")
                else:
                    logger.error(f"Failed to create calendar event")
            else:
                logger.warning(f"No Calendar service for user {user.username}")

        elif reaction_name == "delete_calendar_event":
            calendar_service = get_google_calendar_service(user)
            if calendar_service:
                event_id = config.get('event_id', '')

                if event_id:
                    success = calendar_service.delete_event(event_id)
                    if success:
                        logger.info(f"Calendar event {event_id} deleted")
                    else:
                        logger.error(f"Failed to delete calendar event")
                else:
                    logger.warning(f"No event_id provided for delete_calendar_event")
            else:
                logger.warning(f"No Calendar service for user {user.username}")

        elif reaction_name == "save_to_drive":
            drive_service = get_google_drive_service(user)
            if drive_service:
                filename = config.get('filename', f"AREA_Log_{area.name}.txt")
                content = config.get('content', f"Log from AREA '{area.name}' at {datetime.now()}")

                file = drive_service.upload_file(filename, content)
                if file:
                    logger.info(f"File '{filename}' saved to Drive for user {user.username}")
                else:
                    logger.error(f"Failed to save file to Drive")
            else:
                logger.warning(f"No Drive service for user {user.username}")

        elif reaction_name == "create_drive_folder":
            drive_service = get_google_drive_service(user)
            if drive_service:
                folder_name = config.get('folder_name', f"AREA Folder - {area.name}")

                folder = drive_service.create_folder(folder_name)
                if folder:
                    logger.info(f"Drive folder '{folder_name}' created for user {user.username}")
                else:
                    logger.error(f"Failed to create Drive folder")
            else:
                logger.warning(f"No Drive service for user {user.username}")

        elif reaction_name == "share_drive_file":
            drive_service = get_google_drive_service(user)
            if drive_service:
                file_id = config.get('file_id', '')
                share_email = config.get('share_email', '')
                role = config.get('role', 'reader')

                if file_id and share_email:
                    success = drive_service.share_file(file_id, share_email, role)
                    if success:
                        logger.info(f"Drive file {file_id} shared with {share_email}")
                    else:
                        logger.error(f"Failed to share Drive file")
                else:
                    logger.warning(f"Missing file_id or share_email for share_drive_file")
            else:
                logger.warning(f"No Drive service for user {user.username}")

        elif reaction_name == "add_sheets_row":
            logger.info(f"[NOT IMPLEMENTED] Sheets API - add_sheets_row for user {user.username}")

    def _execute_dropbox_reaction(self, area: Area, reaction_name: str):
        """Execute Dropbox reactions"""
        from .google_services import get_dropbox_service
        
        user = area.user
        config = area.reaction_config or {}
        
        dropbox_service = get_dropbox_service(user)
        if not dropbox_service:
            logger.warning(f"No Dropbox service for user {user.username}")
            return

        if reaction_name == "upload_file":
            file_path = config.get('file_path', '/AREA/file.txt')
            content = config.get('content', 'File created by AREA workflow')
            
            success = dropbox_service.upload_file(file_path, content)
            if success:
                logger.info(f"File uploaded to Dropbox: {file_path}")
            else:
                logger.error(f"Failed to upload file to Dropbox")

        elif reaction_name == "create_folder":
            folder_path = config.get('folder_path', '/AREA_Folder')
            
            success = dropbox_service.create_folder(folder_path)
            if success:
                logger.info(f"Folder created in Dropbox: {folder_path}")
            else:
                logger.error(f"Failed to create folder in Dropbox")

        elif reaction_name == "move_file":
            from_path = config.get('from_path', '')
            to_path = config.get('to_path', '')
            
            if from_path and to_path:
                success = dropbox_service.move_file(from_path, to_path)
                if success:
                    logger.info(f"File moved in Dropbox: {from_path} -> {to_path}")
                else:
                    logger.error(f"Failed to move file in Dropbox")
            else:
                logger.warning(f"Missing from_path or to_path for move_file")

        elif reaction_name == "copy_file":
            from_path = config.get('from_path', '')
            to_path = config.get('to_path', '')
            
            if from_path and to_path:
                success = dropbox_service.copy_file(from_path, to_path)
                if success:
                    logger.info(f"File copied in Dropbox: {from_path} -> {to_path}")
                else:
                    logger.error(f"Failed to copy file in Dropbox")
            else:
                logger.warning(f"Missing from_path or to_path for copy_file")

        elif reaction_name == "delete_file":
            file_path = config.get('file_path', '')
            
            if file_path:
                success = dropbox_service.delete_file(file_path)
                if success:
                    logger.info(f"File deleted from Dropbox: {file_path}")
                else:
                    logger.error(f"Failed to delete file from Dropbox")
            else:
                logger.warning(f"Missing file_path for delete_file")

        elif reaction_name == "share_file":
            file_path = config.get('file_path', '')
            
            if file_path:
                share_link = dropbox_service.create_shared_link(file_path)
                if share_link:
                    logger.info(f"Shared link created for {file_path}: {share_link}")
                else:
                    logger.error(f"Failed to create shared link for {file_path}")
            else:
                logger.warning(f"Missing file_path for share_file")

    def _send_demo_email(self, to_email: str, subject: str, body: str):
        """Send demo email (for MVP demonstration)"""
        try:
            # For MVP demo, just log the email instead of actually sending
            logger.info(f"[DEMO EMAIL] To: {to_email}")
            logger.info(f"[DEMO EMAIL] Subject: {subject}")
            logger.info(f"[DEMO EMAIL] Body: {body[:100]}...")

            # In a real implementation, you would configure SMTP here
            # smtp_server = smtplib.SMTP('smtp.gmail.com', 587)
            # smtp_server.starttls()
            # smtp_server.login('your_email@gmail.com', 'your_password')
            # smtp_server.send_message(msg)
            # smtp_server.quit()

        except Exception as e:
            logger.error(f"Failed to send demo email: {e}")

    def _execute_github_reaction(self, area: Area, reaction_name: str):
        """Execute GitHub reactions"""
        from .github_service import get_github_service

        user = area.user
        github_service = get_github_service(user)

        if not github_service:
            logger.warning(f"No GitHub service available for user {user.username}")
            return

        reaction_config = area.reaction_config or {}
        action_config = area.action_config or {}

        # Get data from action_config (stored by the action trigger)
        triggered_issue = action_config.get('_triggered_issue', {})
        triggered_pr = action_config.get('_triggered_pr', {})
        triggered_commit = action_config.get('_triggered_commit', {})

        if reaction_name == "add_comment":
            owner = reaction_config.get('owner')
            repo = reaction_config.get('repo')
            body = reaction_config.get('body', 'Automated comment from AREA')

            # Get issue/PR number from triggered data
            # GitHub API uses same endpoint for issues and PRs
            issue_number = triggered_issue.get('issue_number') or triggered_pr.get('pr_number')

            if not owner or not repo:
                logger.error(f"Missing owner/repo in reaction config for add_comment")
                return

            if not issue_number:
                logger.error(f"Missing issue/PR number from action data for add_comment")
                return

            logger.info(f"Adding comment to {owner}/{repo}#{issue_number}")
            result = github_service.add_comment(owner, repo, issue_number, body)

            if result:
                logger.info(f"Successfully added comment to issue #{issue_number}")
            else:
                logger.error(f"Failed to add comment to issue #{issue_number}")

        elif reaction_name == "create_issue":
            owner = reaction_config.get('owner')
            repo = reaction_config.get('repo')
            title = reaction_config.get('title', 'Automated Issue')
            body = reaction_config.get('body', '')
            labels_str = reaction_config.get('labels', '')
            labels = [l.strip() for l in labels_str.split(',') if l.strip()] if labels_str else None

            if not owner or not repo:
                logger.error(f"Missing owner/repo in reaction config for create_issue")
                return

            logger.info(f"Creating issue in {owner}/{repo}: {title}")
            result = github_service.create_issue(owner, repo, title, body, labels)

            if result:
                logger.info(f"Successfully created issue #{result.get('number')}")
            else:
                logger.error(f"Failed to create issue")

        elif reaction_name == "create_repository":
            name = reaction_config.get('name')
            description = reaction_config.get('description', '')
            private = reaction_config.get('private', False)

            if not name:
                logger.error(f"Missing name in reaction config for create_repository")
                return

            logger.info(f"Creating repository: {name}")
            result = github_service.create_repository(name, description, private)

            if result:
                logger.info(f"Successfully created repository: {result.get('html_url')}")
            else:
                logger.error(f"Failed to create repository")

        elif reaction_name == "star_repository":
            owner = reaction_config.get('owner')
            repo = reaction_config.get('repo')

            if not owner or not repo:
                logger.error(f"Missing owner/repo in reaction config for star_repository")
                return

            logger.info(f"Starring repository {owner}/{repo}")
            result = github_service.star_repository(owner, repo)

            if result:
                logger.info(f"Successfully starred {owner}/{repo}")
            else:
                logger.error(f"Failed to star repository")

        elif reaction_name == "create_pull_request":
            owner = reaction_config.get('owner')
            repo = reaction_config.get('repo')
            title = reaction_config.get('title', 'Automated PR')
            head = reaction_config.get('head')
            base = reaction_config.get('base', 'main')
            body = reaction_config.get('body', '')

            if not owner or not repo or not head:
                logger.error(f"Missing owner/repo/head in reaction config for create_pull_request")
                return

            logger.info(f"Creating PR in {owner}/{repo}: {title}")
            result = github_service.create_pull_request(owner, repo, title, head, base, body)

            if result:
                logger.info(f"Successfully created PR #{result.get('number')}")
            else:
                logger.error(f"Failed to create pull request")

    def _execute_gitlab_reaction(self, area: Area, reaction_name: str):
        """Execute GitLab reactions"""
        from .gitlab_service import get_gitlab_service

        user = area.user
        gitlab_service = get_gitlab_service(user)

        if not gitlab_service:
            logger.warning(f"No GitLab service available for user {user.username}")
            return

        reaction_config = area.reaction_config or {}
        action_config = area.action_config or {}

        # Get data from action_config (stored by the action trigger)
        triggered_issue = action_config.get('_triggered_issue', {})
        triggered_mr = action_config.get('_triggered_mr', {})
        triggered_commit = action_config.get('_triggered_commit', {})

        if reaction_name == "add_comment":
            # Use comment from config (supports both 'comment' and 'body' for backward compatibility)
            comment = reaction_config.get('comment') or reaction_config.get('body', 'Automated comment from AREA')

            # Get project_id and issue/MR details from triggered data
            project_id = triggered_issue.get('project_id') or triggered_mr.get('project_id') or reaction_config.get('project_id')
            issue_iid = triggered_issue.get('issue_iid')
            mr_iid = triggered_mr.get('mr_iid')

            if not project_id:
                logger.error(f"Missing project_id - neither in triggered data nor reaction config")
                return

            if not issue_iid and not mr_iid:
                logger.info(f"No specific issue/MR triggered, fetching last issue for project {project_id}")
                issues = gitlab_service.list_issues(project_id)
                if issues and len(issues) > 0:
                    issue_iid = issues[0]['iid']
                    logger.info(f"Found last issue: #{issue_iid}")
                else:
                    logger.error(f"No issues found in project {project_id} to comment on")
                    return

            if issue_iid:
                logger.info(f"Adding comment to GitLab issue #{issue_iid} in project {project_id}")
                result = gitlab_service.add_comment(project_id, issue_iid, comment)
                if result:
                    logger.info(f"Successfully added comment to issue #{issue_iid}")
                else:
                    logger.error(f"Failed to add comment to issue #{issue_iid}")
            elif mr_iid:
                logger.info(f"Adding comment to GitLab MR !{mr_iid} in project {project_id}")
                result = gitlab_service.add_merge_request_comment(project_id, mr_iid, comment)
                if result:
                    logger.info(f"Successfully added comment to MR !{mr_iid}")
                else:
                    logger.error(f"Failed to add comment to MR !{mr_iid}")

        elif reaction_name == "create_issue":
            project_id = reaction_config.get('project_id')
            title = reaction_config.get('title', 'Automated Issue')
            description = reaction_config.get('description', '')
            labels_str = reaction_config.get('labels', '')
            labels = labels_str if labels_str else None

            if not project_id:
                logger.error(f"Missing project_id in reaction config for create_issue")
                return

            logger.info(f"Creating issue in GitLab project {project_id}: {title}")
            result = gitlab_service.create_issue(project_id, title, description, labels)

            if result:
                logger.info(f"Successfully created issue #{result.get('iid')}")
            else:
                logger.error(f"Failed to create issue")

        elif reaction_name == "create_project":
            name = reaction_config.get('name')
            description = reaction_config.get('description', '')
            visibility = reaction_config.get('visibility', 'private')

            if not name:
                logger.error(f"Missing name in reaction config for create_project")
                return

            logger.info(f"Creating GitLab project: {name}")
            result = gitlab_service.create_project(name, description, visibility)

            if result:
                logger.info(f"Successfully created project: {result.get('web_url')}")
            else:
                logger.error(f"Failed to create project")

        elif reaction_name == "star_project":
            project_id = reaction_config.get('project_id')

            if not project_id:
                logger.error(f"Missing project_id in reaction config for star_project")
                return

            logger.info(f"Starring GitLab project {project_id}")
            result = gitlab_service.star_project(project_id)

            if result:
                logger.info(f"Successfully starred project {project_id}")
            else:
                logger.error(f"Failed to star project")

        elif reaction_name == "create_merge_request":
            project_id = reaction_config.get('project_id')
            title = reaction_config.get('title', 'Automated MR')
            source_branch = reaction_config.get('source_branch')
            target_branch = reaction_config.get('target_branch', 'main')
            description = reaction_config.get('description', '')

            if not project_id or not source_branch:
                logger.error(f"Missing project_id/source_branch in reaction config for create_merge_request")
                return

            logger.info(f"Creating MR in GitLab project {project_id}: {title}")
            result = gitlab_service.create_merge_request(project_id, title, source_branch, target_branch, description)

            if result:
                logger.info(f"Successfully created MR !{result.get('iid')}")
            else:
                logger.error(f"Failed to create merge request")


# Global executor instance
executor = WorkflowExecutor()

def start_workflow_executor():
    import threading
    if not executor.running:
        thread = threading.Thread(target=executor.start_monitoring, daemon=True)
        thread.start()
    return executor

def stop_workflow_executor():
    executor.stop_monitoring()
