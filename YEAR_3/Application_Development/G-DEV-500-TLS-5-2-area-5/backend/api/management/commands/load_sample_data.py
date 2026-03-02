from django.core.management.base import BaseCommand
from api.models import Service, Action, Reaction


class Command(BaseCommand):
    help = 'Load sample services, actions and reactions'

    def handle(self, *args, **options):
        # Create Facebook service
        facebook, created = Service.objects.get_or_create(
            name='facebook',
            defaults={'description': 'Facebook social media platform'}
        )

        if created:
            self.stdout.write(f"Created service: {facebook.name}")

            # Facebook Actions
            Action.objects.create(
                service=facebook,
                name='new_message_in_group',
                description='A new message is posted in the group'
            )
            Action.objects.create(
                service=facebook,
                name='new_message_inbox',
                description='A new private message is received by the user'
            )
            Action.objects.create(
                service=facebook,
                name='new_like',
                description='The user gains a like from one of their messages'
            )

            # Facebook Reactions
            Reaction.objects.create(
                service=facebook,
                name='like_message',
                description='The user likes a message'
            )

        # Create Google service
        google, created = Service.objects.get_or_create(
            name='google',
            defaults={'description': 'Google services platform'}
        )

        if created or google:
            self.stdout.write(f"Updating service: {google.name}")

            # Google Actions (Enhanced)
            google_actions = [
                {'name': 'new_email_received', 'description': 'New email received in Gmail'},
                {'name': 'new_email_from_sender', 'description': 'New email from specific sender'},
                {'name': 'new_email_with_label', 'description': 'New email with specific label'},
                {'name': 'calendar_event_created', 'description': 'New calendar event created'},
                {'name': 'calendar_event_upcoming', 'description': 'Calendar event upcoming (in 15 minutes)'},
                {'name': 'drive_file_uploaded', 'description': 'New file uploaded to Google Drive'},
                {'name': 'drive_file_shared', 'description': 'File shared on Google Drive'},
                {'name': 'sheets_new_row', 'description': 'New row added to Google Sheets'},
            ]

            for action_data in google_actions:
                Action.objects.get_or_create(
                    service=google,
                    name=action_data['name'],
                    defaults={'description': action_data['description']}
                )

            # Google Reactions (Enhanced)
            google_reactions = [
                {'name': 'send_email', 'description': 'Send email via Gmail'},
                {'name': 'forward_email', 'description': 'Forward email to another address'},
                {'name': 'mark_email_as_read', 'description': 'Mark Gmail email as read'},
                {'name': 'add_email_label', 'description': 'Add label to Gmail email'},
                {'name': 'create_calendar_event', 'description': 'Create calendar event'},
                {'name': 'delete_calendar_event', 'description': 'Delete calendar event'},
                {'name': 'save_to_drive', 'description': 'Save file to Google Drive'},
                {'name': 'create_drive_folder', 'description': 'Create folder on Google Drive'},
                {'name': 'share_drive_file', 'description': 'Share Google Drive file'},
                {'name': 'add_sheets_row', 'description': 'Add row to Google Sheets'},
            ]

            for reaction_data in google_reactions:
                Reaction.objects.get_or_create(
                    service=google,
                    name=reaction_data['name'],
                    defaults={'description': reaction_data['description']}
                )

        # Create LinkedIn service
        linkedin, created = Service.objects.get_or_create(
            name='linkedin',
            defaults={'description': 'LinkedIn professional network'}
        )

        if created or linkedin:
            self.stdout.write(f"Updating service: {linkedin.name}")

            # LinkedIn Actions
            linkedin_actions = [
                {'name': 'new_post_by_user', 'description': 'User shares a new post'},
                {'name': 'new_connection', 'description': 'User gets a new connection'},
                {'name': 'new_comment_on_post', 'description': 'New comment on a user post'},
                {'name': 'new_like_on_post', 'description': 'New like/reaction on a user post'},
            ]

            for action_data in linkedin_actions:
                Action.objects.get_or_create(
                    service=linkedin,
                    name=action_data['name'],
                    defaults={'description': action_data['description']}
                )

            # LinkedIn Reactions
            linkedin_reactions = [
                {'name': 'share_text_post', 'description': 'Share a text post on LinkedIn'},
                {'name': 'share_article', 'description': 'Share an article connection on LinkedIn'},
                {'name': 'post_image', 'description': 'Upload and share an image on LinkedIn'},
                {'name': 'delete_post', 'description': 'Delete a post on LinkedIn'},
            ]

            for reaction_data in linkedin_reactions:
                Reaction.objects.get_or_create(
                    service=linkedin,
                    name=reaction_data['name'],
                    defaults={'description': reaction_data['description']}
                )

        # Create Timer service
        timer, created = Service.objects.get_or_create(
            name='timer',
            defaults={'description': 'Time-based triggers and actions'}
        )

        if created or timer:
            self.stdout.write(f"Updating service: {timer.name}")

            # Timer Actions
            timer_actions = [
                {'name': 'daily_at_time', 'description': 'Trigger daily at specific time'},
                {'name': 'weekly_on_day', 'description': 'Trigger weekly on specific day'},
                {'name': 'every_x_seconds', 'description': 'Trigger every X seconds'},
                {'name': 'every_x_minutes', 'description': 'Trigger every X minutes'},
                {'name': 'every_x_hours', 'description': 'Trigger every X hours'},
                {'name': 'specific_time', 'description': 'The current time is HH:MM'},
                {'name': 'specific_date', 'description': 'The current date is DD/MM'},
                {'name': 'days_until_event', 'description': 'In X days it will be Y'},
            ]

            for action_data in timer_actions:
                Action.objects.get_or_create(
                    service=timer,
                    name=action_data['name'],
                    defaults={'description': action_data['description']}
                )

        # Create Email service
        email, created = Service.objects.get_or_create(
            name='email',
            defaults={'description': 'General email service'}
        )

        if created or email:
            self.stdout.write(f"Updating service: {email.name}")

            # Email Reactions
            email_reactions = [
                {'name': 'send_notification_email', 'description': 'Send notification email'},
                {'name': 'send_alert_email', 'description': 'Send alert email'},
            ]

            for reaction_data in email_reactions:
                Reaction.objects.get_or_create(
                    service=email,
                    name=reaction_data['name'],
                    defaults={'description': reaction_data['description']}
                )



        # Create GitHub service
        github, created = Service.objects.get_or_create(
            name='github',
            defaults={'description': 'GitHub code hosting platform'}
        )

        if created or github:
            self.stdout.write(f"Updating service: {github.name}")

            # GitHub Actions
            github_actions = [
                {'name': 'new_repository', 'description': 'New repository created'},
                {'name': 'new_issue', 'description': 'New issue opened'},
                {'name': 'new_pull_request', 'description': 'New pull request opened'},
                {'name': 'new_star', 'description': 'Repository starred'},
                {'name': 'new_commit', 'description': 'New commit pushed'},
                {'name': 'new_release', 'description': 'New release published'},
            ]

            for action_data in github_actions:
                Action.objects.get_or_create(
                    service=github,
                    name=action_data['name'],
                    defaults={'description': action_data['description']}
                )

            # GitHub Reactions
            github_reactions = [
                {'name': 'create_issue', 'description': 'Create new issue'},
                {'name': 'create_repository', 'description': 'Create new repository'},
                {'name': 'add_comment', 'description': 'Add comment to issue or PR'},
                {'name': 'star_repository', 'description': 'Star a repository'},
                {'name': 'create_pull_request', 'description': 'Create pull request'},
            ]

            for reaction_data in github_reactions:
                Reaction.objects.get_or_create(
                    service=github,
                    name=reaction_data['name'],
                    defaults={'description': reaction_data['description']}
                )

        # Create GitLab service
        gitlab, created = Service.objects.get_or_create(
            name='gitlab',
            defaults={'description': 'GitLab DevOps platform'}
        )

        if created or gitlab:
            self.stdout.write(f"Updating service: {gitlab.name}")

            # GitLab Actions
            gitlab_actions = [
                {'name': 'new_project', 'description': 'New project created'},
                {'name': 'new_issue', 'description': 'New issue opened'},
                {'name': 'new_merge_request', 'description': 'New merge request opened'},
                {'name': 'new_star', 'description': 'Project starred'},
                {'name': 'new_push', 'description': 'New push to repository'},
            ]

            for action_data in gitlab_actions:
                Action.objects.get_or_create(
                    service=gitlab,
                    name=action_data['name'],
                    defaults={'description': action_data['description']}
                )

            # GitLab Reactions
            gitlab_reactions = [
                {'name': 'create_issue', 'description': 'Create new issue (Notify Error)'},
                {'name': 'create_project', 'description': 'Create new project'},
                {'name': 'add_comment', 'description': 'Add comment to issue or MR'},
                {'name': 'star_project', 'description': 'Star a project'},
                {'name': 'create_merge_request', 'description': 'Create merge request'},
            ]

            for reaction_data in gitlab_reactions:
                Reaction.objects.get_or_create(
                    service=gitlab,
                    name=reaction_data['name'],
                    defaults={'description': reaction_data['description']}
                )

        self.stdout.write(
            self.style.SUCCESS('Successfully loaded sample data')
        )