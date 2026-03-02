import { Injectable } from '@nestjs/common';

export interface Question {
  id: number;
  question: string;
  options: string[];
  correctAnswer: number;
  explanation: string;
  theme: string;
  difficulty: string;
}

export interface GameRoom {
  roomId: string;
  players: Array<{ socketId: string; username: string; score: number }>;
  questions: Question[];
  currentQuestionIndex: number;
  answers: Map<string, number>; // socketId -> answer index
  theme: string;
  difficulty: string;
  startTime?: number;
  isEnded?: boolean; // Indique si la partie est terminée normalement
}

@Injectable()
export class QuizService {
  private rooms: Map<string, GameRoom> = new Map();

  // Mock questions - En production, ces questions viendraient d'une API LLM ou d'une base de données
  private questionBank: Question[] = [
    {
      id: 1,
      question: "Quelle est la capitale de la France ?",
      options: ["Paris", "Londres", "Berlin", "Madrid"],
      correctAnswer: 0,
      explanation: "Paris est la capitale de la France depuis le 12ème siècle.",
      theme: "geography",
      difficulty: "easy"
    },
    {
      id: 2,
      question: "En quelle année a eu lieu la Révolution française ?",
      options: ["1789", "1799", "1804", "1815"],
      correctAnswer: 0,
      explanation: "La Révolution française a commencé en 1789 avec la prise de la Bastille.",
      theme: "history",
      difficulty: "medium"
    },
    {
      id: 3,
      question: "Quel est le symbole chimique de l'or ?",
      options: ["Au", "Ag", "Fe", "Or"],
      correctAnswer: 0,
      explanation: "Au vient du latin 'aurum' qui signifie or.",
      theme: "science",
      difficulty: "medium"
    },
    {
      id: 4,
      question: "Qui a peint la Joconde ?",
      options: ["Léonard de Vinci", "Michel-Ange", "Raphaël", "Donatello"],
      correctAnswer: 0,
      explanation: "Léonard de Vinci a peint la Joconde entre 1503 et 1519.",
      theme: "art",
      difficulty: "easy"
    },
    {
      id: 5,
      question: "Quel langage de programmation est principalement utilisé pour le développement web frontend ?",
      options: ["JavaScript", "Python", "Java", "C++"],
      correctAnswer: 0,
      explanation: "JavaScript est le langage principal pour l'interactivité web côté client.",
      theme: "technology",
      difficulty: "easy"
    },
    {
      id: 6,
      question: "Quelle est la plus grande planète du système solaire ?",
      options: ["Jupiter", "Saturne", "Neptune", "Terre"],
      correctAnswer: 0,
      explanation: "Jupiter est la plus grande planète avec un diamètre de 142 984 km.",
      theme: "science",
      difficulty: "easy"
    },
    {
      id: 7,
      question: "Quel pays a remporté le plus de Coupes du Monde de football ?",
      options: ["Brésil", "Allemagne", "Argentine", "Italie"],
      correctAnswer: 0,
      explanation: "Le Brésil a remporté 5 Coupes du Monde (1958, 1962, 1970, 1994, 2002).",
      theme: "random",
      difficulty: "medium"
    },
    {
      id: 8,
      question: "Combien de continents y a-t-il sur Terre ?",
      options: ["7", "5", "6", "8"],
      correctAnswer: 0,
      explanation: "Les 7 continents sont : Afrique, Antarctique, Asie, Europe, Amérique du Nord, Océanie et Amérique du Sud.",
      theme: "geography",
      difficulty: "easy"
    },
    {
      id: 9,
      question: "Quelle est la couleur du ciel par temps clair ?",
      options: ["Bleu", "Vert", "Rouge", "Jaune"],
      correctAnswer: 0,
      explanation: "Le ciel apparaît bleu à cause de la diffusion de la lumière solaire par l'atmosphère.",
      theme: "science",
      difficulty: "easy"
    },
    {
      id: 10,
      question: "Combien font 5 + 3 ?",
      options: ["7", "8", "9", "10"],
      correctAnswer: 1,
      explanation: "5 + 3 = 8. C'est une addition simple.",
      theme: "random",
      difficulty: "easy"
    },
    {
      id: 11,
      question: "Dans quel pays se trouve la Tour Eiffel ?",
      options: ["Italie", "France", "Espagne", "Allemagne"],
      correctAnswer: 1,
      explanation: "La Tour Eiffel est située à Paris, en France. Elle a été construite en 1889.",
      theme: "geography",
      difficulty: "easy"
    },
    {
      id: 12,
      question: "Quel instrument de musique a des touches noires et blanches ?",
      options: ["Guitare", "Piano", "Violon", "Flûte"],
      correctAnswer: 1,
      explanation: "Le piano possède 88 touches : 52 blanches et 36 noires.",
      theme: "art",
      difficulty: "easy"
    },
    {
      id: 13,
      question: "Quel est l'animal le plus rapide sur terre ?",
      options: ["Lion", "Guépard", "Cheval", "Léopard"],
      correctAnswer: 1,
      explanation: "Le guépard peut atteindre 110-120 km/h en pointe, ce qui en fait l'animal terrestre le plus rapide.",
      theme: "science",
      difficulty: "medium"
    },
    {
      id: 14,
      question: "Combien de jours y a-t-il dans une année non bissextile ?",
      options: ["364", "365", "366", "367"],
      correctAnswer: 1,
      explanation: "Une année normale compte 365 jours, tandis qu'une année bissextile en compte 366.",
      theme: "random",
      difficulty: "easy"
    },
    {
      id: 15,
      question: "Quelle est la capitale du Japon ?",
      options: ["Osaka", "Kyoto", "Tokyo", "Yokohama"],
      correctAnswer: 2,
      explanation: "Tokyo est la capitale du Japon et l'une des plus grandes métropoles du monde.",
      theme: "geography",
      difficulty: "medium"
    }
  ];

  createRoom(roomId: string, theme: string, difficulty: string): GameRoom {
    // Utiliser 3 questions pour des tests plus rapides (changez à 5 ou plus pour production)
    const questions = this.generateQuestions(theme, difficulty, 3);
    const room: GameRoom = {
      roomId,
      players: [],
      questions,
      currentQuestionIndex: 0,
      answers: new Map(),
      theme,
      difficulty,
    };
    this.rooms.set(roomId, room);
    return room;
  }

  getRoom(roomId: string): GameRoom | undefined {
    return this.rooms.get(roomId);
  }

  addPlayerToRoom(roomId: string, socketId: string, username: string): void {
    const room = this.rooms.get(roomId);
    if (room) {
      room.players.push({ socketId, username: username || `Player ${room.players.length + 1}`, score: 0 });
    }
  }

  submitAnswer(roomId: string, socketId: string, answerIndex: number): boolean {
    const room = this.rooms.get(roomId);
    if (!room) return false;

    room.answers.set(socketId, answerIndex);

    // Check si tous les joueurs ont répondu
    return room.answers.size === room.players.length;
  }

  evaluateAnswers(roomId: string): Array<{ socketId: string; correct: boolean; score: number }> {
    const room = this.rooms.get(roomId);
    if (!room) return [];

    const currentQuestion = room.questions[room.currentQuestionIndex];
    if (!currentQuestion) return [];
    
    const results = [];

    for (const player of room.players) {
      const answer = room.answers.get(player.socketId);
      const correct = answer === currentQuestion.correctAnswer;
      
      if (correct) {
        player.score += 100; // Points par bonne réponse
      }

      results.push({
        socketId: player.socketId,
        correct,
        score: player.score,
      });
    }

    return results;
  }

  nextQuestion(roomId: string): boolean {
    const room = this.rooms.get(roomId);
    if (!room) return false;

    room.answers.clear();
    room.currentQuestionIndex++;

    return room.currentQuestionIndex < room.questions.length;
  }

  getCurrentQuestion(roomId: string): Question | null {
    const room = this.rooms.get(roomId);
    if (!room || room.currentQuestionIndex >= room.questions.length) {
      return null;
    }
    // Ne pas envoyer la réponse correcte au client
    const question = room.questions[room.currentQuestionIndex];
    return {
      ...question,
      correctAnswer: -1, // Cache la réponse
      explanation: '', // Cache l'explication jusqu'à la fin
    };
  }

  getQuestionWithAnswer(roomId: string): Question | null {
    const room = this.rooms.get(roomId);
    if (!room || room.currentQuestionIndex >= room.questions.length) {
      return null;
    }
    return room.questions[room.currentQuestionIndex];
  }

  getFinalResults(roomId: string): Array<{ username: string; score: number }> {
    const room = this.rooms.get(roomId);
    if (!room) return [];

    return room.players
      .map(p => ({ username: p.username, score: p.score }))
      .sort((a, b) => b.score - a.score);
  }

  deleteRoom(roomId: string): void {
    this.rooms.delete(roomId);
  }

  markRoomAsEnded(roomId: string): void {
    const room = this.rooms.get(roomId);
    if (room) {
      room.isEnded = true;
    }
  }

  // Méthode publique pour le mode solo
  getQuestionsForSolo(theme: string, difficulty: string, count: number): Question[] {
    return this.generateQuestions(theme, difficulty, count);
  }

  private generateQuestions(theme: string, difficulty: string, count: number): Question[] {
    // Filtre par thème et difficulté
    let filtered = this.questionBank;

    if (theme !== 'random') {
      filtered = filtered.filter(q => q.theme === theme);
    }

    if (difficulty !== 'all') {
      filtered = filtered.filter(q => q.difficulty === difficulty);
    }

    // Si pas assez de questions filtrées, prendre toutes les questions
    if (filtered.length < count) {
      filtered = this.questionBank;
    }

    // Mélanger et prendre les N premières
    const shuffled = [...filtered].sort(() => Math.random() - 0.5);
    return shuffled.slice(0, Math.min(count, shuffled.length));
  }
}
