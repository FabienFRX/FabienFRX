import requests
import os
import urllib.parse
from django.core.files.base import ContentFile

from django.conf import settings
from .models import Startup, Event, News, Investor, Partner, JEBUser
from datetime import datetime
from django.utils import timezone

class JEBAPIService:
    def __init__(self):
        self.base_url = settings.JEB_API_BASE_URL
        self.headers = {'X-Group-Authorization': settings.JEB_API_TOKEN}

    def fetch_events(self):
        """Récupère les événements depuis l'API JEB"""
        try:
            response = requests.get(f"{self.base_url}/events", headers=self.headers)
            response.raise_for_status()
            return response.json()
        except requests.exceptions.RequestException as e:
            print(f"Erreur API JEB events: {e}")
            return []

    def fetch_startups(self):
        """Récupère les startups depuis l'API JEB"""
        try:
            response = requests.get(f"{self.base_url}/startups", headers=self.headers)
            response.raise_for_status()
            return response.json()
        except requests.exceptions.RequestException as e:
            print(f"Erreur API JEB startups: {e}")
            return []

    def fetch_startup_details(self, startup_id):
        """Récupère les détails complets d'une startup"""
        try:
            response = requests.get(f"{self.base_url}/startups/{startup_id}", headers=self.headers)
            response.raise_for_status()
            return response.json()
        except requests.exceptions.RequestException as e:
            print(f"Erreur API JEB startup details {startup_id}: {e}")
            return None

    def fetch_news(self):
        """Récupère les actualités depuis l'API JEB"""
        try:
            response = requests.get(f"{self.base_url}/news", headers=self.headers)
            response.raise_for_status()
            return response.json()
        except requests.exceptions.RequestException as e:
            print(f"Erreur API JEB news: {e}")
            return []

    def fetch_investors(self):
        """Récupère les investisseurs depuis l'API JEB"""
        try:
            response = requests.get(f"{self.base_url}/investors", headers=self.headers)
            response.raise_for_status()
            return response.json()
        except requests.exceptions.RequestException as e:
            print(f"Erreur API JEB investors: {e}")
            return []

    def fetch_partners(self):
        """Récupère les partenaires depuis l'API JEB"""
        try:
            response = requests.get(f"{self.base_url}/partners", headers=self.headers)
            response.raise_for_status()
            return response.json()
        except requests.exceptions.RequestException as e:
            print(f"Erreur API JEB partners: {e}")
            return []

    def fetch_news_details(self, news_id):
        """Récupère les détails complets d'une news"""
        try:
            response = requests.get(f"{self.base_url}/news/{news_id}", headers=self.headers)
            response.raise_for_status()
            return response.json()
        except requests.exceptions.RequestException as e:
            print(f"Erreur API JEB news details {news_id}: {e}")
            return None

    def fetch_event_details(self, event_id):
        """Récupère les détails complets d'un event"""
        try:
            response = requests.get(f"{self.base_url}/events/{event_id}", headers=self.headers)
            response.raise_for_status()
            return response.json()
        except requests.exceptions.RequestException as e:
            print(f"Erreur API JEB event details {event_id}: {e}")
            return None

    def fetch_investor_details(self, investor_id):
        """Récupère les détails complets d'un investor"""
        try:
            response = requests.get(f"{self.base_url}/investors/{investor_id}", headers=self.headers)
            response.raise_for_status()
            return response.json()
        except requests.exceptions.RequestException as e:
            print(f"Erreur API JEB investor details {investor_id}: {e}")
            return None

    def fetch_partner_details(self, partner_id):
        """Récupère les détails complets d'un partner"""
        try:
            response = requests.get(f"{self.base_url}/partners/{partner_id}", headers=self.headers)
            response.raise_for_status()
            return response.json()
        except requests.exceptions.RequestException as e:
            print(f"Erreur API JEB partner details {partner_id}: {e}")
            return None

    def fetch_user_details(self, user_id):
        """Récupère les détails complets d'un utilisateur JEB"""
        try:
            response = requests.get(f"{self.base_url}/users/{user_id}", headers=self.headers)
            response.raise_for_status()
            return response.json()
        except requests.exceptions.RequestException as e:
            print(f"Erreur API JEB user details {user_id}: {e}")
            return None

    def sync_all_data(self):
        """Synchronise toutes les données"""
        self.sync_startups()
        self.sync_events()
        self.sync_news()
        self.sync_investors()
        self.sync_partners()
        self.sync_users()

    def sync_startups(self):
        """Synchronise les startups depuis l'API JEB avec tous les détails"""
        try:
            # Récupérer la liste des startups avec pagination
            startups_data = []
            skip = 0
            limit = 100

            while True:
                response = requests.get(
                    f"{self.base_url}/startups",
                    headers=self.headers,
                    params={'skip': skip, 'limit': limit}
                )

                if response.status_code == 200:
                    batch = response.json()
                    if not batch:
                        break
                    startups_data.extend(batch)
                    skip += limit
                else:
                    break

            print(f"Récupération de {len(startups_data)} startups...")

            # Pour chaque startup, récupérer les détails complets
            for startup_data in startups_data:
                startup_id = startup_data['id']
                print(f"Récupération des détails pour startup {startup_id}...")

                # Récupérer les détails complets
                detailed_data = self.fetch_startup_details(startup_id)
                if detailed_data:
                    # Utiliser les données détaillées
                    startup, created = Startup.objects.update_or_create(
                        jeb_id=startup_id,
                        defaults={
                            'name': detailed_data.get('name', ''),
                            'legal_status': detailed_data.get('legal_status', ''),
                            'address': detailed_data.get('address', ''),
                            'email': detailed_data.get('email', ''),
                            'phone': detailed_data.get('phone', ''),
                            'sector': detailed_data.get('sector', ''),
                            'maturity': detailed_data.get('maturity', ''),
                            'description': detailed_data.get('description', ''),
                            'website_url': detailed_data.get('website_url'),
                            'social_media_url': detailed_data.get('social_media_url'),
                            'project_status': detailed_data.get('project_status', ''),
                            'needs': detailed_data.get('needs', ''),
                            'last_synced': timezone.now()
                        }
                    )
                    print(f"Startup {startup_id} synchronisée: {startup.name}")
                else:
                    # Fallback sur les données de base
                    startup, created = Startup.objects.update_or_create(
                        jeb_id=startup_id,
                        defaults={
                            'name': startup_data.get('name', ''),
                            'legal_status': startup_data.get('legal_status', ''),
                            'address': startup_data.get('address', ''),
                            'email': startup_data.get('email', ''),
                            'phone': startup_data.get('phone', ''),
                            'sector': startup_data.get('sector', ''),
                            'maturity': startup_data.get('maturity', ''),
                            'last_synced': timezone.now()
                        }
                    )

            return {
                'success': True,
                'message': f'{len(startups_data)} startups synchronisées avec détails',
                'count': len(startups_data)
            }

        except Exception as e:
            return {
                'success': False,
                'message': f'Erreur lors de la synchronisation: {str(e)}'
            }

    def sync_events(self):
        """Synchronise les événements"""
        events_data = self.fetch_events()
        for event_data in events_data:
            Event.objects.update_or_create(
                jeb_id=event_data['id'],
                defaults={
                    'name': event_data['name'],
                    'dates': datetime.strptime(event_data['dates'], '%Y-%m-%d').date(),
                    'location': event_data.get('location', ''),
                    'description': event_data.get('description', ''),
                    'event_type': event_data.get('event_type', ''),
                    'target_audience': event_data.get('target_audience', ''),
                    'last_synced': timezone.now(),
                }
            )

    def sync_news(self):
        """Synchronise les actualités avec liaison aux startups et détails complets"""
        news_data = self.fetch_news()
        print(f"Récupération de {len(news_data)} news...")
        
        for news_item in news_data:
            news_id = news_item['id']
            print(f"Récupération des détails pour news {news_id}...")
            
            # Récupérer les détails complets
            detailed_data = self.fetch_news_details(news_id)
            if detailed_data:
                # Utiliser les données détaillées
                news_data_to_save = detailed_data
            else:
                # Fallback sur les données de base
                news_data_to_save = news_item
            
            # Trouver la startup liée si elle existe
            related_startup = None
            if news_data_to_save.get('startup_id'):
                try:
                    related_startup = Startup.objects.get(jeb_id=news_data_to_save['startup_id'])
                except Startup.DoesNotExist:
                    pass
            
            News.objects.update_or_create(
                jeb_id=news_id,
                defaults={
                    'news_date': datetime.strptime(news_data_to_save['news_date'], '%Y-%m-%d').date(),
                    'location': news_data_to_save.get('location', ''),
                    'title': news_data_to_save['title'],
                    'category': news_data_to_save.get('category', ''),
                    'startup_id': news_data_to_save.get('startup_id'),
                    'description': news_data_to_save.get('description', ''),
                    'content': news_data_to_save.get('content', ''),
                    'related_startup': related_startup,
                    'last_synced': timezone.now(),
                }
            )
            print(f"News {news_id} synchronisée: {news_data_to_save['title']}")

    def sync_investors(self):
        """Synchronise les investisseurs"""
        try:
            investors_data = self.fetch_investors()
            print(f"Récupération de {len(investors_data)} investisseurs...")

            for investor_data in investors_data:
                investor, created = Investor.objects.update_or_create(
                    jeb_id=investor_data['id'],
                    defaults={
                        'name': investor_data.get('name', ''),
                        'legal_status': investor_data.get('legal_status', ''),
                        'address': investor_data.get('address', ''),
                        'email': investor_data.get('email', ''),
                        'phone': investor_data.get('phone', ''),
                        'description': investor_data.get('description', ''),
                        'investor_type': investor_data.get('investor_type', ''),
                        'investment_focus': investor_data.get('investment_focus', ''),
                        'last_synced': timezone.now()
                    }
                )
                print(f"Investisseur {investor_data['id']} synchronisé: {investor.name}")

            return {
                'success': True,
                'message': f'{len(investors_data)} investisseurs synchronisés',
                'count': len(investors_data)
            }
        except Exception as e:
            return {
                'success': False,
                'message': f'Erreur lors de la synchronisation des investisseurs: {str(e)}'
            }

    def sync_partners(self):
        """Synchronise les partenaires"""
        try:
            partners_data = self.fetch_partners()
            print(f"Récupération de {len(partners_data)} partenaires...")

            for partner_data in partners_data:
                partner, created = Partner.objects.update_or_create(
                    jeb_id=partner_data['id'],
                    defaults={
                        'name': partner_data.get('name', ''),
                        'legal_status': partner_data.get('legal_status', ''),
                        'address': partner_data.get('address', ''),
                        'email': partner_data.get('email', ''),
                        'phone': partner_data.get('phone', ''),
                        'description': partner_data.get('description', ''),
                        'partner_type': partner_data.get('partner_type', ''),
                        'focus_area': partner_data.get('focus_area', ''),
                        'last_synced': timezone.now()
                    }
                )
                print(f"Partenaire {partner_data['id']} synchronisé: {partner.name}")

            return {
                'success': True,
                'message': f'{len(partners_data)} partenaires synchronisés',
                'count': len(partners_data)
            }
        except Exception as e:
            return {
                'success': False,
                'message': f'Erreur lors de la synchronisation des partenaires: {str(e)}'
            }

    def fetch_users(self):
        """Récupère les utilisateurs depuis l'API JEB"""
        try:
            response = requests.get(f"{self.base_url}/users", headers=self.headers)
            response.raise_for_status()
            return response.json()
        except requests.exceptions.RequestException as e:
            print(f"Erreur API JEB users: {e}")
            return []

    def sync_users(self):
        """Synchronise les utilisateurs depuis l'API JEB"""
        try:
            users_data = self.fetch_users()
            print(f"Récupération de {len(users_data)} utilisateurs...")
            for user_data in users_data:
                user, created = JEBUser.objects.update_or_create(
                    jeb_id=user_data['id'],
                    defaults={
                        'name': user_data['name'],
                        'email': user_data['email'],
                        'role': user_data.get('role', ''),
                        'company': user_data.get('company', ''),
                        'position': user_data.get('position', ''),
                        'bio': user_data.get('bio', ''),
                        'linkedin_url': user_data.get('linkedin_url'),
                        'twitter_url': user_data.get('twitter_url'),
                        'created_at': datetime.strptime(user_data['created_at'], '%Y-%m-%d').replace(tzinfo=timezone.utc) if 'created_at' in user_data else timezone.now(),
                        'updated_at': datetime.strptime(user_data['updated_at'], '%Y-%m-%d').replace(tzinfo=timezone.utc) if 'updated_at' in user_data else timezone.now(),
                        'last_synced': timezone.now()
                    }
                )
                print(f"Utilisateur {user.id} synchronisé: {user.name}")
            return {
                'success': True,
                'message': f'{len(users_data)} utilisateurs synchronisés',
                'count': len(users_data)
            }
        except Exception as e:
            return {
                'success': False,
                'message': f'Erreur lors de la synchronisation des utilisateurs: {str(e)}'
            }

    def download_image(self, image_url, model_instance, field_name):
        """Télécharge une image depuis l'API JEB et la stocke localement"""
        if not image_url:
            return None
            
        try:
            response = requests.get(image_url, headers=self.headers, timeout=30)
            response.raise_for_status()
            
            # Extraire le nom du fichier de l'URL
            parsed_url = urllib.parse.urlparse(image_url)
            filename = os.path.basename(parsed_url.path)
            if not filename or '.' not in filename:
                filename = f"image_{model_instance.jeb_id}.jpg"
            
            # Créer le fichier Django
            image_file = ContentFile(response.content, name=filename)
            
            # Sauvegarder l'image
            getattr(model_instance, field_name).save(filename, image_file, save=False)
            return filename
            
        except Exception as e:
            print(f"Erreur téléchargement image {image_url}: {e}")
            return None

# Fonctions de compatibilité pour les commandes de gestion
def sync_all_data():
    """Synchronise toutes les données (pour les commandes de gestion)"""
    service = JEBAPIService()
    service.sync_all_data()
    print("Synchronisation des données all terminée avec succès!")

def sync_startups():
    """Synchronise les startups (pour les commandes de gestion)"""
    service = JEBAPIService()
    service.sync_startups()

def sync_news():
    """Synchronise les news (pour les commandes de gestion)"""
    service = JEBAPIService()
    service.sync_news()

def sync_events():
    """Synchronise les événements (pour les commandes de gestion)"""
    service = JEBAPIService()
    service.sync_events()

def sync_investors():
    """Synchronise les investisseurs (pour les commandes de gestion)"""
    service = JEBAPIService()
    service.sync_investors()

def sync_partners():
    """Synchronise les partenaires (pour les commandes de gestion)"""
    service = JEBAPIService()
    service.sync_partners()

def sync_users():
    """Synchronise les utilisateurs (pour les commandes de gestion)"""
    service = JEBAPIService()
    service.sync_users()
