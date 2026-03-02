from rest_framework import serializers
from .models import Service, Action, Reaction, Area, UserService
from django.contrib.auth.models import User


class ActionSerializer(serializers.ModelSerializer):
    class Meta:
        model = Action
        fields = ['id', 'name', 'description']


class ReactionSerializer(serializers.ModelSerializer):
    class Meta:
        model = Reaction
        fields = ['id', 'name', 'description']


class ServiceSerializer(serializers.ModelSerializer):
    actions = ActionSerializer(many=True, read_only=True)
    reactions = ReactionSerializer(many=True, read_only=True)

    class Meta:
        model = Service
        fields = ['id', 'name', 'description', 'actions', 'reactions']


class UserSerializer(serializers.ModelSerializer):
    class Meta:
        model = User
        fields = ['id', 'username', 'email', 'first_name', 'last_name', 'date_joined']


class UserServiceSerializer(serializers.ModelSerializer):
    service_id = serializers.IntegerField(source='service.id', read_only=True)
    service_name = serializers.CharField(source='service.name', read_only=True)
    service_description = serializers.CharField(source='service.description', read_only=True)

    class Meta:
        model = UserService
        fields = [
            'id', 'service_id', 'service_name', 'service_description', 'service_user_id',
            'service_username', 'is_active', 'created_at'
        ]


class AboutSerializer(serializers.Serializer):
    client = serializers.DictField()
    server = serializers.DictField()


class AreaSerializer(serializers.ModelSerializer):
    action_name = serializers.CharField(source='action.name', read_only=True)
    action_description = serializers.CharField(source='action.description', read_only=True)
    action_service = serializers.CharField(source='action.service.name', read_only=True)
    reaction_name = serializers.CharField(source='reaction.name', read_only=True)
    reaction_description = serializers.CharField(source='reaction.description', read_only=True)
    reaction_service = serializers.CharField(source='reaction.service.name', read_only=True)
    is_active = serializers.BooleanField(source='enabled', read_only=True)

    class Meta:
        model = Area
        fields = [
            'id', 'name', 'description', 'enabled', 'is_active', 'created_at',
            'action_name', 'action_description', 'action_service', 'action_config',
            'reaction_name', 'reaction_description', 'reaction_service', 'reaction_config'
        ]