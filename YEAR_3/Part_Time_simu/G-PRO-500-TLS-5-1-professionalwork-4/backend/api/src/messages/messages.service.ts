import { Injectable, NotFoundException, BadRequestException } from '@nestjs/common';
import { PrismaService } from '../prisma/prisma.service';

interface CreateMessageDto {
  receiverId: string;
  content: string;
}

@Injectable()
export class MessagesService {
  constructor(private prisma: PrismaService) {}

  /**
   * Récupère ou crée une conversation entre deux utilisateurs
   */
  async getOrCreateConversation(user1Id: string, user2Id: string) {
    // Chercher une conversation existante
    const existingConversation = await this.prisma.conversation.findFirst({
      where: {
        AND: [
          { participants: { some: { userId: user1Id } } },
          { participants: { some: { userId: user2Id } } },
        ],
      },
      include: {
        participants: {
          include: {
            user: {
              select: {
                id: true,
                username: true,
                displayName: true,
                avatarUrl: true,
              },
            },
          },
        },
        messages: {
          orderBy: { createdAt: 'desc' },
          take: 1,
        },
      },
    });

    if (existingConversation) {
      return existingConversation;
    }

    // Créer une nouvelle conversation
    const conversation = await this.prisma.conversation.create({
      data: {
        participants: {
          create: [
            { userId: user1Id },
            { userId: user2Id },
          ],
        },
      },
      include: {
        participants: {
          include: {
            user: {
              select: {
                id: true,
                username: true,
                displayName: true,
                avatarUrl: true,
              },
            },
          },
        },
        messages: true,
      },
    });

    return conversation;
  }

  /**
   * Récupère toutes les conversations d'un utilisateur
   */
  async getUserConversations(userId: string) {
    const conversations = await this.prisma.conversation.findMany({
      where: {
        participants: {
          some: { userId },
        },
      },
      include: {
        participants: {
          include: {
            user: {
              select: {
                id: true,
                username: true,
                displayName: true,
                avatarUrl: true,
              },
            },
          },
        },
        messages: {
          orderBy: { createdAt: 'desc' },
          take: 1,
        },
      },
      orderBy: { updatedAt: 'desc' },
    });

    // Compter les messages non lus pour chaque conversation
    const conversationsWithUnreadCount = await Promise.all(
      conversations.map(async (conv) => {
        const unreadCount = await this.prisma.message.count({
          where: {
            conversationId: conv.id,
            senderId: { not: userId },
            read: false,
          },
        });

        return {
          ...conv,
          unreadCount,
        };
      }),
    );

    return conversationsWithUnreadCount;
  }

  /**
   * Récupère les messages d'une conversation
   */
  async getConversationMessages(conversationId: string, userId: string, limit = 50) {
    // Vérifier que l'utilisateur fait partie de la conversation
    const participant = await this.prisma.conversationParticipant.findUnique({
      where: {
        conversationId_userId: {
          conversationId,
          userId,
        },
      },
    });

    if (!participant) {
      throw new NotFoundException('Conversation not found');
    }

    const messages = await this.prisma.message.findMany({
      where: { conversationId },
      include: {
        sender: {
          select: {
            id: true,
            username: true,
            displayName: true,
            avatarUrl: true,
          },
        },
      },
      orderBy: { createdAt: 'asc' },
      take: limit,
    });

    return messages;
  }

  /**
   * Envoie un message
   */
  async sendMessage(senderId: string, dto: CreateMessageDto) {
    // Récupérer ou créer la conversation
    const conversation = await this.getOrCreateConversation(
      senderId,
      dto.receiverId,
    );

    // Créer le message
    const message = await this.prisma.message.create({
      data: {
        conversationId: conversation.id,
        senderId,
        content: dto.content,
      },
      include: {
        sender: {
          select: {
            id: true,
            username: true,
            displayName: true,
            avatarUrl: true,
          },
        },
      },
    });

    // Mettre à jour la conversation
    await this.prisma.conversation.update({
      where: { id: conversation.id },
      data: { updatedAt: new Date() },
    });

    return message;
  }

  /**
   * Marque les messages comme lus
   */
  async markAsRead(conversationId: string, userId: string) {
    // Vérifier que l'utilisateur fait partie de la conversation
    const participant = await this.prisma.conversationParticipant.findUnique({
      where: {
        conversationId_userId: {
          conversationId,
          userId,
        },
      },
    });

    if (!participant) {
      throw new NotFoundException('Conversation not found');
    }

    // Marquer tous les messages reçus comme lus
    await this.prisma.message.updateMany({
      where: {
        conversationId,
        senderId: { not: userId },
        read: false,
      },
      data: { read: true },
    });

    // Mettre à jour lastReadAt
    await this.prisma.conversationParticipant.update({
      where: {
        conversationId_userId: {
          conversationId,
          userId,
        },
      },
      data: { lastReadAt: new Date() },
    });

    return { success: true };
  }

  /**
   * Supprime une conversation
   */
  async deleteConversation(conversationId: string, userId: string) {
    // Vérifier que l'utilisateur fait partie de la conversation
    const participant = await this.prisma.conversationParticipant.findUnique({
      where: {
        conversationId_userId: {
          conversationId,
          userId,
        },
      },
    });

    if (!participant) {
      throw new NotFoundException('Conversation not found');
    }

    await this.prisma.conversation.delete({
      where: { id: conversationId },
    });

    return { success: true };
  }

  /**
   * Compte le total de messages non lus pour un utilisateur
   */
  async getUnreadCount(userId: string) {
    const conversations = await this.prisma.conversation.findMany({
      where: {
        participants: {
          some: { userId },
        },
      },
      select: { id: true },
    });

    const conversationIds = conversations.map((c) => c.id);

    const unreadCount = await this.prisma.message.count({
      where: {
        conversationId: { in: conversationIds },
        senderId: { not: userId },
        read: false,
      },
    });

    return { unreadCount };
  }
}
