import axios from 'axios';
import { io, Socket } from 'socket.io-client';
import { API_BASE_URL, getAuthHeaders } from '@/lib/api.config';

const API_URL = `${API_BASE_URL}/api`;
const WS_URL = API_BASE_URL;

export interface User {
  id: string;
  username: string;
  displayName: string | null;
  avatarUrl: string | null;
}

export interface Message {
  id: string;
  conversationId: string;
  senderId: string;
  content: string;
  read: boolean;
  createdAt: Date;
  sender: User;
}

export interface Conversation {
  id: string;
  createdAt: Date;
  updatedAt: Date;
  participants: Array<{
    id: string;
    userId: string;
    user: User;
    lastReadAt: Date;
  }>;
  messages: Message[];
  unreadCount?: number;
}

class MessagesService {
  private socket: Socket | null = null;

  private getAuthHeader() {
    const token = localStorage.getItem('auth_token');
    return token ? { Authorization: `Bearer ${token}` } : {};
  }

  /**
   * Connecte au WebSocket pour les messages en temps réel
   */
  connectSocket(userId: string) {
    if (this.socket?.connected) {
      return this.socket;
    }

    this.socket = io(`${WS_URL}/messages`, {
      transports: ['websocket'],
      reconnection: true,
    });

    this.socket.on('connect', () => {
      console.log('Connected to messages socket');
      this.socket?.emit('authenticate', { userId });
    });

    this.socket.on('disconnect', () => {
      console.log('Disconnected from messages socket');
    });

    return this.socket;
  }

  /**
   * Déconnecte du WebSocket
   */
  disconnectSocket() {
    if (this.socket) {
      this.socket.disconnect();
      this.socket = null;
    }
  }

  /**
   * Récupère le socket actif
   */
  getSocket() {
    return this.socket;
  }

  /**
   * Récupère toutes les conversations de l'utilisateur
   */
  async getConversations(): Promise<Conversation[]> {
    const response = await axios.get(`${API_URL}/messages/conversations`, {
      headers: this.getAuthHeader(),
    });
    return response.data;
  }

  /**
   * Récupère les messages d'une conversation
   */
  async getConversationMessages(
    conversationId: string,
    limit = 50,
  ): Promise<Message[]> {
    const response = await axios.get(
      `${API_URL}/messages/conversations/${conversationId}`,
      {
        headers: this.getAuthHeader(),
        params: { limit },
      },
    );
    return response.data;
  }

  /**
   * Envoie un message via HTTP
   */
  async sendMessage(receiverId: string, content: string): Promise<Message> {
    const response = await axios.post(
      `${API_URL}/messages`,
      { receiverId, content },
      {
        headers: this.getAuthHeader(),
      },
    );
    return response.data;
  }

  /**
   * Envoie un message via WebSocket (plus rapide)
   */
  sendMessageWS(receiverId: string, content: string): Promise<any> {
    return new Promise((resolve, reject) => {
      if (!this.socket?.connected) {
        reject(new Error('Socket not connected'));
        return;
      }

      this.socket.emit(
        'sendMessage',
        { receiverId, content },
        (response: any) => {
          if (response.error) {
            reject(new Error(response.error));
          } else {
            resolve(response);
          }
        },
      );
    });
  }

  /**
   * Marque les messages d'une conversation comme lus
   */
  async markAsRead(conversationId: string): Promise<void> {
    await axios.patch(
      `${API_URL}/messages/conversations/${conversationId}/read`,
      {},
      {
        headers: this.getAuthHeader(),
      },
    );
  }

  /**
   * Supprime une conversation
   */
  async deleteConversation(conversationId: string): Promise<void> {
    await axios.delete(`${API_URL}/messages/conversations/${conversationId}`, {
      headers: this.getAuthHeader(),
    });
  }

  /**
   * Récupère le nombre de messages non lus
   */
  async getUnreadCount(): Promise<number> {
    const response = await axios.get(`${API_URL}/messages/unread-count`, {
      headers: this.getAuthHeader(),
    });
    return response.data.unreadCount;
  }

  /**
   * Récupère ou crée une conversation avec un utilisateur
   */
  async getOrCreateConversation(userId: string): Promise<Conversation> {
    const response = await axios.post(
      `${API_URL}/messages/conversation`,
      { userId },
      {
        headers: this.getAuthHeader(),
      },
    );
    return response.data;
  }

  /**
   * Indique que l'utilisateur est en train de taper
   */
  emitTyping(conversationId: string, receiverId: string) {
    this.socket?.emit('typing', { conversationId, receiverId });
  }

  /**
   * Indique que l'utilisateur a arrêté de taper
   */
  emitStopTyping(conversationId: string, receiverId: string) {
    this.socket?.emit('stopTyping', { conversationId, receiverId });
  }
}

export const messagesService = new MessagesService();
