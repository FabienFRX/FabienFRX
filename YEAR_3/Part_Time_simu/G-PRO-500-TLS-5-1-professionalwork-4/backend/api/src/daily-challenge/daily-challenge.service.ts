import { Injectable, BadRequestException, NotFoundException } from '@nestjs/common';
import { PrismaService } from '../prisma/prisma.service';
import { Level } from '@prisma/client';

interface Question {
  id: number;
  question: string;
  options: string[];
  correctAnswer: number;
  explanation: string;
}

interface SubmitAnswersDto {
  answers: number[]; // Indices des réponses choisies
  timeSpent: number; // Temps passé en secondes
}

@Injectable()
export class DailyChallengeService {
  constructor(private prisma: PrismaService) {}

  // XP rewards basés sur la difficulté
  private readonly XP_REWARDS = {
    EASY: 50,
    MEDIUM: 100,
    HARD: 150,
  };

  // Questions par difficulté
  private readonly QUESTIONS_COUNT = {
    EASY: 5,
    MEDIUM: 5,
    HARD: 5,
  };

  /**
   * Récupère le Daily Challenge du jour
   * Si aucun challenge n'existe pour aujourd'hui, en créé un nouveau
   */
  async getTodayChallenge() {
    const today = this.getTodayDate();
    
    let challenge = await this.prisma.dailyChallenge.findUnique({
      where: { date: today },
    });

    if (!challenge) {
      challenge = await this.createDailyChallenge(today);
    }

    // Retourner le challenge sans les réponses correctes
    const questionsWithoutAnswers = (challenge.questions as unknown as Question[]).map((q) => ({
      id: q.id,
      question: q.question,
      options: q.options,
    }));

    return {
      id: challenge.id,
      date: challenge.date,
      theme: challenge.theme,
      difficulty: challenge.difficulty,
      questions: questionsWithoutAnswers,
      xpReward: challenge.xpReward,
      timeUntilReset: this.getTimeUntilReset(),
    };
  }

  /**
   * Récupère le statut du Daily Challenge pour un utilisateur
   */
  async getUserChallengeStatus(userId: string) {
    const today = this.getTodayDate();
    
    const challenge = await this.prisma.dailyChallenge.findUnique({
      where: { date: today },
    });

    if (!challenge) {
      return {
        challengeAvailable: false,
        completed: false,
        timeUntilReset: this.getTimeUntilReset(),
      };
    }

    const attempt = await this.prisma.dailyChallengeAttempt.findUnique({
      where: {
        userId_challengeId: {
          userId,
          challengeId: challenge.id,
        },
      },
    });

    const stats = await this.getUserStats(userId);

    return {
      challengeAvailable: true,
      completed: !!attempt,
      attempt: attempt || null,
      stats,
      timeUntilReset: this.getTimeUntilReset(),
    };
  }

  /**
   * Soumet les réponses d'un utilisateur au Daily Challenge
   */
  async submitChallenge(userId: string, challengeId: string, dto: SubmitAnswersDto) {
    const challenge = await this.prisma.dailyChallenge.findUnique({
      where: { id: challengeId },
    });

    if (!challenge) {
      throw new NotFoundException('Challenge not found');
    }

    // Vérifier si l'utilisateur a déjà complété ce challenge
    const existingAttempt = await this.prisma.dailyChallengeAttempt.findUnique({
      where: {
        userId_challengeId: {
          userId,
          challengeId,
        },
      },
    });

    if (existingAttempt) {
      throw new BadRequestException('Challenge already completed today');
    }

    // Calculer le score
    const questions = challenge.questions as unknown as Question[];
    const correctAnswers = dto.answers.filter((answer, index) => 
      answer === questions[index].correctAnswer
    ).length;

    const score = Math.round((correctAnswers / questions.length) * 100);

    // Calculer l'XP basé sur la difficulté et la performance
    const baseXp = this.XP_REWARDS[challenge.difficulty];
    const performanceMultiplier = score / 100; // 0.0 à 1.0
    const xpEarned = Math.round(baseXp * performanceMultiplier);

    // Créer la tentative
    const attempt = await this.prisma.dailyChallengeAttempt.create({
      data: {
        userId,
        challengeId,
        score,
        correctAnswers,
        totalQuestions: questions.length,
        timeSpent: dto.timeSpent,
        xpEarned,
      },
    });

    // Mettre à jour les stats de l'utilisateur
    await this.updateUserStats(userId, xpEarned);

    // Retourner les résultats avec les bonnes réponses
    return {
      attempt,
      results: questions.map((q, index) => ({
        question: q.question,
        yourAnswer: dto.answers[index],
        correctAnswer: q.correctAnswer,
        isCorrect: dto.answers[index] === q.correctAnswer,
        explanation: q.explanation,
      })),
    };
  }

  /**
   * Récupère les stats d'un utilisateur
   */
  async getUserStats(userId: string) {
    let stats = await this.prisma.dailyChallengeStats.findUnique({
      where: { userId },
    });

    if (!stats) {
      stats = await this.prisma.dailyChallengeStats.create({
        data: { userId },
      });
    }

    return stats;
  }

  /**
   * Récupère l'historique des tentatives d'un utilisateur
   */
  async getUserHistory(userId: string, limit = 30) {
    const attempts = await this.prisma.dailyChallengeAttempt.findMany({
      where: { userId },
      include: {
        challenge: {
          select: {
            date: true,
            theme: true,
            difficulty: true,
          },
        },
      },
      orderBy: { completedAt: 'desc' },
      take: limit,
    });

    return attempts;
  }

  /**
   * Récupère le leaderboard global des streaks
   */
  async getStreakLeaderboard(limit = 10) {
    const topStreaks = await this.prisma.dailyChallengeStats.findMany({
      where: {
        currentStreak: { gt: 0 },
      },
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
      orderBy: [
        { currentStreak: 'desc' },
        { longestStreak: 'desc' },
      ],
      take: limit,
    });

    return topStreaks.map((stat, index) => ({
      rank: index + 1,
      user: stat.user,
      currentStreak: stat.currentStreak,
      longestStreak: stat.longestStreak,
      totalCompleted: stat.totalChallengesCompleted,
      totalXp: stat.totalXpEarned,
    }));
  }

  // ==================== PRIVATE METHODS ====================

  /**
   * Crée un nouveau Daily Challenge pour une date donnée
   */
  private async createDailyChallenge(date: Date) {
    // Choisir une difficulté aléatoire (avec distribution)
    const difficulties: Level[] = ['EASY', 'MEDIUM', 'HARD'];
    const weights = [0.3, 0.5, 0.2]; // 30% easy, 50% medium, 20% hard
    const difficulty = this.weightedRandom(difficulties, weights);

    // Choisir un thème aléatoire
    const themes = ['history', 'science', 'geography', 'art', 'technology'];
    const theme = themes[Math.floor(Math.random() * themes.length)];

    // Générer les questions (dans un vrai système, ces questions viendraient d'une base de données ou d'une API LLM)
    const questions = this.generateQuestions(theme, difficulty);

    return await this.prisma.dailyChallenge.create({
      data: {
        date,
        theme,
        difficulty,
        questions: questions as any,
        xpReward: this.XP_REWARDS[difficulty],
      },
    });
  }

  /**
   * Met à jour les statistiques d'un utilisateur après une tentative
   */
  private async updateUserStats(userId: string, xpEarned: number) {
    const today = this.getTodayDate();
    
    let stats = await this.prisma.dailyChallengeStats.findUnique({
      where: { userId },
    });

    if (!stats) {
      stats = await this.prisma.dailyChallengeStats.create({
        data: { userId },
      });
    }

    // Calculer le nouveau streak
    let newStreak = 1;
    if (stats.lastCompletedDate) {
      const yesterday = new Date(today);
      yesterday.setDate(yesterday.getDate() - 1);
      
      const lastDate = new Date(stats.lastCompletedDate);
      
      if (lastDate.toDateString() === yesterday.toDateString()) {
        // Continuation du streak
        newStreak = stats.currentStreak + 1;
      } else if (lastDate.toDateString() !== today.toDateString()) {
        // Streak cassé
        newStreak = 1;
      } else {
        // Déjà complété aujourd'hui (ne devrait pas arriver)
        newStreak = stats.currentStreak;
      }
    }

    const longestStreak = Math.max(newStreak, stats.longestStreak);

    await this.prisma.dailyChallengeStats.update({
      where: { userId },
      data: {
        currentStreak: newStreak,
        longestStreak,
        totalChallengesCompleted: stats.totalChallengesCompleted + 1,
        totalXpEarned: stats.totalXpEarned + xpEarned,
        lastCompletedDate: today,
      },
    });
  }

  /**
   * Génère des questions pour un thème et une difficulté donnés
   * TODO: Remplacer par une vraie génération via LLM ou base de données
   */
  private generateQuestions(theme: string, difficulty: Level): Question[] {
    const count = this.QUESTIONS_COUNT[difficulty];
    
    // Questions d'exemple (à remplacer par votre système de questions)
    const sampleQuestions: Question[] = [
      {
        id: 1,
        question: `Quelle est la capitale de la France ?`,
        options: ['Paris', 'Londres', 'Berlin', 'Madrid'],
        correctAnswer: 0,
        explanation: 'Paris est la capitale de la France depuis le 12ème siècle.',
      },
      {
        id: 2,
        question: `En quelle année a eu lieu la Révolution française ?`,
        options: ['1789', '1799', '1804', '1815'],
        correctAnswer: 0,
        explanation: 'La Révolution française a commencé en 1789.',
      },
      {
        id: 3,
        question: `Quel est le symbole chimique de l'or ?`,
        options: ['Au', 'Ag', 'Fe', 'Or'],
        correctAnswer: 0,
        explanation: "Au vient du latin 'aurum'.",
      },
      {
        id: 4,
        question: `Qui a peint la Joconde ?`,
        options: ['Léonard de Vinci', 'Michel-Ange', 'Raphaël', 'Donatello'],
        correctAnswer: 0,
        explanation: 'Léonard de Vinci a peint la Joconde entre 1503 et 1519.',
      },
      {
        id: 5,
        question: `Quelle est la plus grande planète du système solaire ?`,
        options: ['Jupiter', 'Saturne', 'Neptune', 'Terre'],
        correctAnswer: 0,
        explanation: 'Jupiter est la plus grande planète.',
      },
    ];

    return sampleQuestions.slice(0, count);
  }

  /**
   * Retourne la date d'aujourd'hui à minuit
   */
  private getTodayDate(): Date {
    const now = new Date();
    return new Date(now.getFullYear(), now.getMonth(), now.getDate());
  }

  /**
   * Calcule le temps jusqu'au prochain reset (minuit)
   */
  private getTimeUntilReset(): number {
    const now = new Date();
    const tomorrow = new Date(now.getFullYear(), now.getMonth(), now.getDate() + 1);
    return tomorrow.getTime() - now.getTime(); // Millisecondes jusqu'à minuit
  }

  /**
   * Sélection aléatoire pondérée
   */
  private weightedRandom<T>(items: T[], weights: number[]): T {
    const total = weights.reduce((sum, w) => sum + w, 0);
    let random = Math.random() * total;
    
    for (let i = 0; i < items.length; i++) {
      random -= weights[i];
      if (random <= 0) {
        return items[i];
      }
    }
    
    return items[items.length - 1];
  }
}
