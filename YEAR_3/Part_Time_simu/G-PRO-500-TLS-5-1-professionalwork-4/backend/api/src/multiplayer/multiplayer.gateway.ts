import { Injectable, Logger } from '@nestjs/common';
import { WebSocketGateway, WebSocketServer, SubscribeMessage, MessageBody, ConnectedSocket, OnGatewayDisconnect } from '@nestjs/websockets';
import { Server, Socket } from 'socket.io';
import { QuizService } from './quiz.service';

interface QueuePlayer {
  socket: Socket;
  theme: string;
  difficulty: string;
  username: string;
}

@Injectable()
@WebSocketGateway({
  cors: {
    origin: '*',
    methods: ['GET', 'POST'],
    credentials: true,
  },
  transports: ['websocket', 'polling'],
})
export class MultiplayerGateway implements OnGatewayDisconnect {
  private readonly logger = new Logger(MultiplayerGateway.name);

  @WebSocketServer()
  server: Server;

  private queue: QueuePlayer[] = [];
  private playerRooms: Map<string, string> = new Map(); // socketId -> roomId

  constructor(private readonly quizService: QuizService) {}

  @SubscribeMessage('find_match')
  handleFindMatch(@ConnectedSocket() client: Socket, @MessageBody() payload: { theme: string; difficulty: string; username?: string }) {
    this.logger.log(`Client ${client.id} requested match: ${JSON.stringify(payload)}`);
    
    // Éviter les doublons
    if (this.queue.find((p) => p.socket.id === client.id)) {
      return;
    }

    const queuePlayer: QueuePlayer = {
      socket: client,
      theme: payload.theme || 'random',
      difficulty: payload.difficulty || 'medium',
      username: payload.username || `Joueur ${Math.floor(Math.random() * 1000)}`,
    };

    this.queue.push(queuePlayer);
    client.emit('matching', { status: 'searching', queueSize: this.queue.length });

    // Chercher un adversaire avec les mêmes critères
    this.tryMatchPlayers();
  }

  private tryMatchPlayers() {
    if (this.queue.length < 2) return;

    // Prendre les deux premiers joueurs avec les mêmes préférences
    for (let i = 0; i < this.queue.length - 1; i++) {
      for (let j = i + 1; j < this.queue.length; j++) {
        const player1 = this.queue[i];
        const player2 = this.queue[j];

        // Match si même thème et difficulté
        if (player1.theme === player2.theme && player1.difficulty === player2.difficulty) {
          this.createMatch(player1, player2);
          // Retirer les joueurs de la queue
          this.queue.splice(j, 1);
          this.queue.splice(i, 1);
          // Réessayer pour les joueurs restants
          this.tryMatchPlayers();
          return;
        }
      }
    }
  }

  private createMatch(player1: QueuePlayer, player2: QueuePlayer) {
    const roomId = `room_${Date.now()}_${Math.floor(Math.random() * 10000)}`;
    
    // Créer la room avec le QuizService
    const room = this.quizService.createRoom(roomId, player1.theme, player1.difficulty);
    
    // Ajouter les joueurs
    this.quizService.addPlayerToRoom(roomId, player1.socket.id, player1.username);
    this.quizService.addPlayerToRoom(roomId, player2.socket.id, player2.username);

    // Joindre la room Socket.IO
    player1.socket.join(roomId);
    player2.socket.join(roomId);

    // Mapper les joueurs à leur room
    this.playerRooms.set(player1.socket.id, roomId);
    this.playerRooms.set(player2.socket.id, roomId);

    this.logger.log(`Match created: ${roomId} for ${player1.username} vs ${player2.username}`);

    // Notifier les joueurs
    this.server.to(roomId).emit('match_found', {
      roomId,
      players: room.players.map(p => ({ username: p.username, score: p.score })),
      theme: player1.theme,
      difficulty: player1.difficulty,
    });

    // Démarrer le jeu après 3 secondes
    setTimeout(() => {
      this.startGame(roomId);
    }, 3000);
  }

  private startGame(roomId: string) {
    const room = this.quizService.getRoom(roomId);
    if (!room) return;

    this.logger.log(`Game started in room ${roomId}`);
    this.server.to(roomId).emit('game_start', { totalQuestions: room.questions.length });

    // Envoyer la première question
    this.sendQuestion(roomId);
  }

  private sendQuestion(roomId: string) {
    const question = this.quizService.getCurrentQuestion(roomId);
    const room = this.quizService.getRoom(roomId);
    
    if (!question || !room) {
      this.endGame(roomId);
      return;
    }

    this.logger.log(`Sending question ${room.currentQuestionIndex + 1} to room ${roomId}`);
    
    this.server.to(roomId).emit('new_question', {
      questionNumber: room.currentQuestionIndex + 1,
      totalQuestions: room.questions.length,
      question: question.question,
      options: question.options,
      timeLimit: 15, // 15 secondes par question
    });

    // Timer pour passer à la question suivante même si tous n'ont pas répondu
    setTimeout(() => {
      this.processAnswers(roomId);
    }, 16000); // 15s + 1s de marge
  }

  @SubscribeMessage('submit_answer')
  handleSubmitAnswer(@ConnectedSocket() client: Socket, @MessageBody() data: { answerIndex: number }) {
    const roomId = this.playerRooms.get(client.id);
    if (!roomId) return;

    this.logger.log(`Player ${client.id} answered ${data.answerIndex} in room ${roomId}`);

    const allAnswered = this.quizService.submitAnswer(roomId, client.id, data.answerIndex);

    // Notifier que le joueur a répondu
    this.server.to(roomId).emit('player_answered', { playerId: client.id });

    // Si tous ont répondu, traiter immédiatement
    if (allAnswered) {
      this.processAnswers(roomId);
    }
  }

  private processAnswers(roomId: string) {
    const results = this.quizService.evaluateAnswers(roomId);
    const questionWithAnswer = this.quizService.getQuestionWithAnswer(roomId);
    const room = this.quizService.getRoom(roomId);

    if (!room || !questionWithAnswer) return;

    // Envoyer les résultats avec la bonne réponse et l'explication
    this.server.to(roomId).emit('question_results', {
      correctAnswer: questionWithAnswer.correctAnswer,
      explanation: questionWithAnswer.explanation,
      results: results.map(r => ({
        playerId: r.socketId,
        username: room.players.find(p => p.socketId === r.socketId)?.username,
        correct: r.correct,
        score: r.score,
      })),
      scores: room.players.map(p => ({ username: p.username, score: p.score })),
    });

    // Passer à la question suivante après 5 secondes
    setTimeout(() => {
      const hasMoreQuestions = this.quizService.nextQuestion(roomId);
      
      if (hasMoreQuestions) {
        this.sendQuestion(roomId);
      } else {
        this.endGame(roomId);
      }
    }, 5000);
  }

  private endGame(roomId: string) {
    const finalResults = this.quizService.getFinalResults(roomId);
    
    this.logger.log(`Game ended in room ${roomId}`);
    
    // Marquer la room comme terminée pour éviter la notification de déconnexion
    this.quizService.markRoomAsEnded(roomId);
    
    this.server.to(roomId).emit('game_end', {
      results: finalResults,
      winner: finalResults[0],
    });

    // Nettoyer après 10 secondes
    setTimeout(() => {
      const room = this.quizService.getRoom(roomId);
      if (room) {
        room.players.forEach(p => {
          this.playerRooms.delete(p.socketId);
        });
      }
      this.quizService.deleteRoom(roomId);
      this.logger.log(`Room ${roomId} cleaned up`);
    }, 10000);
  }

  @SubscribeMessage('leave_queue')
  handleLeaveQueue(@ConnectedSocket() client: Socket) {
    this.queue = this.queue.filter((p) => p.socket.id !== client.id);
    client.emit('left_queue');
    this.logger.log(`Client ${client.id} left queue`);
  }

  handleDisconnect(client: Socket) {
    this.logger.log(`Client disconnected: ${client.id}`);
    
    // Retirer de la queue
    this.queue = this.queue.filter((p) => p.socket.id !== client.id);
    
    // Gérer la déconnexion pendant une partie
    const roomId = this.playerRooms.get(client.id);
    if (roomId) {
      const room = this.quizService.getRoom(roomId);
      if (room && !room.isEnded) {
        // Ne notifier que si la partie n'est pas terminée normalement
        this.logger.log(`Player disconnected during active game in room ${roomId}`);
        this.server.to(roomId).emit('opponent_disconnected', {
          message: 'Votre adversaire s\'est déconnecté',
        });
        
        // Terminer la partie après 5 secondes
        setTimeout(() => {
          this.endGame(roomId);
        }, 5000);
      }
      this.playerRooms.delete(client.id);
    }
  }
}
