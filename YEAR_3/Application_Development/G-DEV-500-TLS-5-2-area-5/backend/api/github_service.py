import requests
import logging
from .service_utils import get_user_service_token

logger = logging.getLogger(__name__)


class GitHubService:
    def __init__(self, access_token):
        self.access_token = access_token
        self.base_url = 'https://api.github.com'
        self.headers = {
            'Authorization': f'Bearer {access_token}',
            'Accept': 'application/vnd.github+json',
            'X-GitHub-Api-Version': '2022-11-28'
        }

    def _make_request(self, method, endpoint, **kwargs):
        url = f"{self.base_url}{endpoint}"
        try:
            response = requests.request(method, url, headers=self.headers, **kwargs)
            response.raise_for_status()
            return response.json() if response.content else {}
        except requests.exceptions.RequestException as e:
            logger.error(f"GitHub API request failed: {e}")
            return None

    def get_user(self):
        """Get authenticated user info"""
        return self._make_request('GET', '/user')

    def list_repositories(self, per_page=30, sort='updated'):
        """List user repositories"""
        return self._make_request('GET', '/user/repos', params={'per_page': per_page, 'sort': sort})

    def get_repository(self, owner, repo):
        """Get repository details"""
        return self._make_request('GET', f'/repos/{owner}/{repo}')

    def create_repository(self, name, description=None, private=False):
        """Create a new repository"""
        data = {
            'name': name,
            'description': description or '',
            'private': private
        }
        return self._make_request('POST', '/user/repos', json=data)

    def list_issues(self, owner, repo, state='open'):
        """List repository issues"""
        return self._make_request('GET', f'/repos/{owner}/{repo}/issues', params={'state': state})

    def create_issue(self, owner, repo, title, body=None, labels=None):
        """Create a new issue"""
        data = {
            'title': title,
            'body': body or '',
        }
        if labels:
            data['labels'] = labels
        return self._make_request('POST', f'/repos/{owner}/{repo}/issues', json=data)

    def add_comment(self, owner, repo, issue_number, body):
        """Add comment to issue or pull request"""
        data = {'body': body}
        return self._make_request('POST', f'/repos/{owner}/{repo}/issues/{issue_number}/comments', json=data)

    def list_pull_requests(self, owner, repo, state='open'):
        """List repository pull requests"""
        return self._make_request('GET', f'/repos/{owner}/{repo}/pulls', params={'state': state})

    def create_pull_request(self, owner, repo, title, head, base, body=None):
        """Create a new pull request"""
        data = {
            'title': title,
            'head': head,
            'base': base,
            'body': body or ''
        }
        return self._make_request('POST', f'/repos/{owner}/{repo}/pulls', json=data)

    def star_repository(self, owner, repo):
        """Star a repository"""
        response = requests.put(
            f"{self.base_url}/user/starred/{owner}/{repo}",
            headers=self.headers
        )
        return response.status_code == 204

    def unstar_repository(self, owner, repo):
        """Unstar a repository"""
        response = requests.delete(
            f"{self.base_url}/user/starred/{owner}/{repo}",
            headers=self.headers
        )
        return response.status_code == 204

    def list_commits(self, owner, repo, sha=None, per_page=30):
        """List repository commits"""
        params = {'per_page': per_page}
        if sha:
            params['sha'] = sha
        return self._make_request('GET', f'/repos/{owner}/{repo}/commits', params=params)

    def list_releases(self, owner, repo):
        """List repository releases"""
        return self._make_request('GET', f'/repos/{owner}/{repo}/releases')

    def create_release(self, owner, repo, tag_name, name, body=None, draft=False, prerelease=False):
        """Create a new release"""
        data = {
            'tag_name': tag_name,
            'name': name,
            'body': body or '',
            'draft': draft,
            'prerelease': prerelease
        }
        return self._make_request('POST', f'/repos/{owner}/{repo}/releases', json=data)

    def get_user_starred_repos(self, per_page=30):
        """List starred repositories"""
        return self._make_request('GET', '/user/starred', params={'per_page': per_page})


def get_github_service(user):
    """Factory function to create GitHub service instance"""
    _, access_token = get_user_service_token(user, 'github')
    if not access_token:
        return None
    
    return GitHubService(access_token=access_token)
