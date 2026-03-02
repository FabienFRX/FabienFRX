import {
  Controller,
  Get,
  Post,
  Delete,
  Patch,
  Body,
  Param,
  Query,
  UseGuards,
  Request,
} from '@nestjs/common';
import { MessagesService } from './messages.service';
import { JwtAuthGuard } from '../auth/jwt-auth.guard';

interface SendMessageDto {
  receiverId: string;
  content: string;
}

@Controller('messages')
@UseGuards(JwtAuthGuard)
export class MessagesController {
  constructor(private readonly messagesService: MessagesService) {}

  /**
   * GET /api/messages/conversations
   * Récupère toutes les conversations de l'utilisateur
   */
  @Get('conversations')
  async getConversations(@Request() req: any) {
    return this.messagesService.getUserConversations(req.user.id);
  }

  /**
   * GET /api/messages/conversations/:id
   * Récupère les messages d'une conversation
   */
  @Get('conversations/:id')
  async getConversationMessages(
    @Request() req: any,
    @Param('id') conversationId: string,
    @Query('limit') limit?: string,
  ) {
    const limitNum = limit ? parseInt(limit, 10) : 50;
    return this.messagesService.getConversationMessages(
      conversationId,
      req.user.id,
      limitNum,
    );
  }

  /**
   * POST /api/messages
   * Envoie un message
   */
  @Post()
  async sendMessage(@Request() req: any, @Body() dto: SendMessageDto) {
    return this.messagesService.sendMessage(req.user.id, dto);
  }

  /**
   * PATCH /api/messages/conversations/:id/read
   * Marque les messages d'une conversation comme lus
   */
  @Patch('conversations/:id/read')
  async markAsRead(@Request() req: any, @Param('id') conversationId: string) {
    return this.messagesService.markAsRead(conversationId, req.user.id);
  }

  /**
   * DELETE /api/messages/conversations/:id
   * Supprime une conversation
   */
  @Delete('conversations/:id')
  async deleteConversation(
    @Request() req: any,
    @Param('id') conversationId: string,
  ) {
    return this.messagesService.deleteConversation(conversationId, req.user.id);
  }

  /**
   * GET /api/messages/unread-count
   * Récupère le nombre de messages non lus
   */
  @Get('unread-count')
  async getUnreadCount(@Request() req: any) {
    return this.messagesService.getUnreadCount(req.user.id);
  }

  /**
   * POST /api/messages/conversation
   * Récupère ou crée une conversation avec un utilisateur
   */
  @Post('conversation')
  async getOrCreateConversation(
    @Request() req: any,
    @Body() dto: { userId: string },
  ) {
    return this.messagesService.getOrCreateConversation(req.user.id, dto.userId);
  }
}
