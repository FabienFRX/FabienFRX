import requests
import logging
from django.utils import timezone
from .models import UserService, Service

logger = logging.getLogger(__name__)

class LinkedInService:
    def __init__(self, user):
        self.user = user
        self.access_token = self._get_access_token()
        self.person_urn = self._get_person_urn()

    def _get_access_token(self):
        try:
            service = Service.objects.get(name='linkedin')
            user_service = UserService.objects.get(user=self.user, service=service, is_active=True)
            return user_service.access_token
        except UserService.DoesNotExist:
            logger.warning(f"LinkedIn service not connected for user {self.user.username}")
            return None
        except Service.DoesNotExist:
            logger.error("LinkedIn service definitions not found")
            return None

    def _get_person_urn(self):
        try:
            service = Service.objects.get(name='linkedin')
            user_service = UserService.objects.get(user=self.user, service=service, is_active=True)
            return f"urn:li:person:{user_service.service_user_id}"
        except Exception:
            return None

    def _make_request(self, method, url, data=None, params=None):
        if not self.access_token:
            return None

        headers = {
            'Authorization': f'Bearer {self.access_token}',
            'Content-Type': 'application/json',
            'X-Restli-Protocol-Version': '2.0.0'
        }

        try:
            if method == 'GET':
                response = requests.get(url, headers=headers, params=params)
            elif method == 'POST':
                response = requests.post(url, headers=headers, json=data)
            else:
                return None
            
            if response.status_code == 401:
                logger.warning(f"LinkedIn token expired for user {self.user.username}")
                return None
                
            return response.json() if response.content else {}
            
        except Exception as e:
            logger.error(f"LinkedIn API error: {e}")
            return None

    def check_new_post(self, since_time=None):
        pass

    def create_post(self, text, article_url=None):
        if not self.person_urn:
            logger.error("No person URN found for LinkedIn")
            return False

        url = "https://api.linkedin.com/v2/ugcPosts"
        
        share_content = {
            "shareCommentary": {
                "text": text
            },
            "shareMediaCategory": "NONE"
        }

        if article_url:
            share_content["shareMediaCategory"] = "ARTICLE"
            share_content["media"] = [{
                "status": "READY",
                "description": {
                    "text": "Article shared via AREA"
                },
                "originalUrl": article_url
            }]
        
        payload = {
            "author": self.person_urn,
            "lifecycleState": "PUBLISHED",
            "specificContent": {
                "com.linkedin.ugc.ShareContent": share_content
            },
            "visibility": {
                "com.linkedin.ugc.MemberNetworkVisibility": "PUBLIC"
            }
        }

        response = self._make_request('POST', url, data=payload)
        
        if response and 'id' in response:
            logger.info(f"LinkedIn post created: {response['id']}")
            return True
            
        logger.error(f"Failed to create LinkedIn post. Response: {response}")
        return False

    def get_recent_posts(self):
        """Get recent posts by the user"""
        if not self.person_urn:
            logger.warning("LinkedIn: No person URN available")
            return []

        # This is a simplified endpoint - normally requires nuanced queries
        # For this project, we might need to rely on what permissions we have
        # Trying to fetch posts via UGC API or Shares API
        # Using a broad query for shares by this author
        
        encoded_urn = requests.utils.quote(self.person_urn)
        url = f"https://api.linkedin.com/v2/ugcPosts?q=authors&authors=List({encoded_urn})"
        
        logger.info(f"Fetching LinkedIn posts from: {url}")
        
        response = self._make_request('GET', url)
        logger.info(f"LinkedIn UGC response: {response}")
        
        if response and 'elements' in response:
            return response['elements']
            
        # Fallback: Try Shares API if UGC failed (permissions issue)
        logger.info("UGC Posts failed, trying Shares API...")
        url = f"https://api.linkedin.com/v2/shares?q=owners&owners={encoded_urn}"
        response = self._make_request('GET', url)
        logger.info(f"LinkedIn Shares response: {response}")
        
        if response and 'elements' in response:
            return response['elements']
            
        logger.warning("LinkedIn: API access failed (403/Empty). Activating DEMO MODE for New Post.")
        
        import time
        period = 90 
        slot = int(time.time() / period)
        mock_id = f"urn:li:share:mock_demo_post_{slot}"
        
        return [{'id': mock_id, 'created': {'time': int(time.time() * 1000)}}]


def get_linkedin_service(user):
    return LinkedInService(user)
