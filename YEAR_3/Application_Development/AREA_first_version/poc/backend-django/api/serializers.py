from rest_framework import serializers
from .models import Service, Action, Reaction, Area


class ActionSerializer(serializers.ModelSerializer):
    class Meta:
        model = Action
        fields = ['name', 'description']


class ReactionSerializer(serializers.ModelSerializer):
    class Meta:
        model = Reaction
        fields = ['name', 'description']


class ServiceSerializer(serializers.ModelSerializer):
    actions = ActionSerializer(many=True, read_only=True)
    reactions = ReactionSerializer(many=True, read_only=True)

    class Meta:
        model = Service
        fields = ['name', 'actions', 'reactions']


class AboutSerializer(serializers.Serializer):
    client = serializers.DictField()
    server = serializers.DictField()