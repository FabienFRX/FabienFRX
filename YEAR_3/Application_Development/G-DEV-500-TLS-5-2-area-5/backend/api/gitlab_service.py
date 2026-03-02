import requests
import logging
from urllib.parse import quote
from .service_utils import get_user_service_token

logger = logging.getLogger(__name__)


class GitLabService:
    def __init__(self, access_token):
        self.access_token = access_token
        self.base_url = 'https://gitlab.com/api/v4'
        self.headers = {
            'Authorization': f'Bearer {access_token}',
            'Content-Type': 'application/json'
        }

    def _encode_project_id(self, project_id):
        """Encode project ID for URL (handles both numeric IDs and paths like username/project)"""
        return quote(str(project_id), safe='')

    def _make_request(self, method, endpoint, **kwargs):
        url = f"{self.base_url}{endpoint}"
        try:
            response = requests.request(method, url, headers=self.headers, **kwargs)
            response.raise_for_status()
            return response.json() if response.content else {}
        except requests.exceptions.RequestException as e:
            logger.error(f"GitLab API request failed: {e}")
            return None

    def get_user(self):
        """Get authenticated user info"""
        return self._make_request('GET', '/user')

    def list_projects(self, per_page=30, order_by='updated_at'):
        """List user projects"""
        return self._make_request('GET', '/projects', params={
            'membership': 'true',
            'per_page': per_page,
            'order_by': order_by
        })

    def get_project(self, project_id):
        """Get project details"""
        encoded_id = self._encode_project_id(project_id)
        return self._make_request('GET', f'/projects/{encoded_id}')

    def create_project(self, name, description=None, visibility='private'):
        """Create a new project"""
        data = {
            'name': name,
            'description': description or '',
            'visibility': visibility
        }
        return self._make_request('POST', '/projects', json=data)

    def list_issues(self, project_id, state='opened'):
        """List project issues"""
        encoded_id = self._encode_project_id(project_id)
        return self._make_request('GET', f'/projects/{encoded_id}/issues', params={'state': state, 'order_by': 'created_at', 'sort': 'desc'})

    def create_issue(self, project_id, title, description=None, labels=None):
        """Create a new issue"""
        encoded_id = self._encode_project_id(project_id)
        data = {
            'title': title,
            'description': description or '',
        }
        if labels:
            data['labels'] = labels
        return self._make_request('POST', f'/projects/{encoded_id}/issues', json=data)

    def add_comment(self, project_id, issue_iid, body):
        """Add comment to issue"""
        encoded_id = self._encode_project_id(project_id)
        data = {'body': body}
        return self._make_request('POST', f'/projects/{encoded_id}/issues/{issue_iid}/notes', json=data)

    def add_merge_request_comment(self, project_id, merge_request_iid, body):
        """Add comment to merge request"""
        encoded_id = self._encode_project_id(project_id)
        data = {'body': body}
        return self._make_request('POST', f'/projects/{encoded_id}/merge_requests/{merge_request_iid}/notes', json=data)

    def list_merge_requests(self, project_id, state='opened'):
        """List project merge requests"""
        encoded_id = self._encode_project_id(project_id)
        return self._make_request('GET', f'/projects/{encoded_id}/merge_requests', params={'state': state})

    def create_merge_request(self, project_id, title, source_branch, target_branch, description=None):
        """Create a new merge request"""
        encoded_id = self._encode_project_id(project_id)
        data = {
            'title': title,
            'source_branch': source_branch,
            'target_branch': target_branch,
            'description': description or ''
        }
        return self._make_request('POST', f'/projects/{encoded_id}/merge_requests', json=data)

    def star_project(self, project_id):
        """Star a project"""
        encoded_id = self._encode_project_id(project_id)
        response = requests.post(
            f"{self.base_url}/projects/{encoded_id}/star",
            headers=self.headers
        )
        return response.status_code in [200, 201, 304]

    def unstar_project(self, project_id):
        """Unstar a project"""
        encoded_id = self._encode_project_id(project_id)
        response = requests.post(
            f"{self.base_url}/projects/{encoded_id}/unstar",
            headers=self.headers
        )
        return response.status_code in [200, 201, 304]

    def list_commits(self, project_id, ref_name=None, per_page=30):
        """List project commits"""
        encoded_id = self._encode_project_id(project_id)
        params = {'per_page': per_page}
        if ref_name:
            params['ref_name'] = ref_name
        return self._make_request('GET', f'/projects/{encoded_id}/repository/commits', params=params)

    def list_releases(self, project_id):
        """List project releases"""
        encoded_id = self._encode_project_id(project_id)
        return self._make_request('GET', f'/projects/{encoded_id}/releases')

    def create_release(self, project_id, tag_name, name, description=None):
        """Create a new release"""
        encoded_id = self._encode_project_id(project_id)
        data = {
            'tag_name': tag_name,
            'name': name,
            'description': description or ''
        }
        return self._make_request('POST', f'/projects/{encoded_id}/releases', json=data)

    def get_user_starred_projects(self, per_page=30):
        """List starred projects"""
        return self._make_request('GET', '/projects', params={
            'starred': 'true',
            'per_page': per_page
        })


def get_gitlab_service(user):
    """Factory function to create GitLab service instance"""
    _, access_token = get_user_service_token(user, 'gitlab')
    if not access_token:
        return None
    
    return GitLabService(access_token=access_token)
