import {
  WebSocketGateway,
  WebSocketServer,
  SubscribeMessage,
  OnGatewayConnection,
  OnGatewayDisconnect,
  ConnectedSocket,
  MessageBody,
} from '@nestjs/websockets';
import { Server, Socket } from 'socket.io';
import { MessagesService } from './messages.service';

interface AuthenticatedSocket extends Socket {
  userId?: string;
}

@WebSocketGateway({
  cors: {
    origin: ['http://localhost:8080', 'http://localhost:8081', 'http://localhost:8082', 'http://localhost:5173'],
    credentials: true,
  },
  namespace: '/messages',
})
export class MessagesGateway implements OnGatewayConnection, OnGatewayDisconnect {
  @WebSocketServer()
  server: Server;

  private userSockets: Map<string, string> = new Map(); // userId -> socketId

  constructor(private messagesService: MessagesService) {}

  handleConnection(client: AuthenticatedSocket) {
    console.log(`Client connected to messages: ${client.id}`);
  }

  handleDisconnect(client: AuthenticatedSocket) {
    console.log(`Client disconnected from messages: ${client.id}`);
    if (client.userId) {
      this.userSockets.delete(client.userId);
    }
  }

  @SubscribeMessage('authenticate')
  handleAuthenticate(
    @ConnectedSocket() client: AuthenticatedSocket,
    @MessageBody() data: { userId: string },
  ) {
    client.userId = data.userId;
    this.userSockets.set(data.userId, client.id);
    console.log(`User ${data.userId} authenticated on messages socket`);
    return { success: true };
  }

  @SubscribeMessage('sendMessage')
  async handleMessage(
    @ConnectedSocket() client: AuthenticatedSocket,
    @MessageBody() data: { receiverId: string; content: string },
  ) {
    if (!client.userId) {
      return { error: 'Not authenticated' };
    }

    try {
      // Sauvegarder le message en base de données
      const message = await this.messagesService.sendMessage(client.userId, {
        receiverId: data.receiverId,
        content: data.content,
      });

      // Envoyer au destinataire s'il est connecté
      const receiverSocketId = this.userSockets.get(data.receiverId);
      if (receiverSocketId) {
        this.server.to(receiverSocketId).emit('newMessage', message);
      }

      // Confirmer l'envoi à l'émetteur
      return { success: true, message };
    } catch (error) {
      console.error('Error sending message:', error);
      return { error: 'Failed to send message' };
    }
  }

  @SubscribeMessage('typing')
  handleTyping(
    @ConnectedSocket() client: AuthenticatedSocket,
    @MessageBody() data: { conversationId: string; receiverId: string },
  ) {
    const receiverSocketId = this.userSockets.get(data.receiverId);
    if (receiverSocketId) {
      this.server.to(receiverSocketId).emit('userTyping', {
        conversationId: data.conversationId,
        userId: client.userId,
      });
    }
  }

  @SubscribeMessage('stopTyping')
  handleStopTyping(
    @ConnectedSocket() client: AuthenticatedSocket,
    @MessageBody() data: { conversationId: string; receiverId: string },
  ) {
    const receiverSocketId = this.userSockets.get(data.receiverId);
    if (receiverSocketId) {
      this.server.to(receiverSocketId).emit('userStoppedTyping', {
        conversationId: data.conversationId,
        userId: client.userId,
      });
    }
  }
}
