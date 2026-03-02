import axios from 'axios';
import { API_BASE_URL, getAuthHeaders } from '@/lib/api.config';

const API_URL = `${API_BASE_URL}/api`;

export interface Question {
  id: number;
  question: string;
  options: string[];
}

export interface DailyChallenge {
  id: string;
  date: Date;
  theme: string;
  difficulty: 'EASY' | 'MEDIUM' | 'HARD';
  questions: Question[];
  xpReward: number;
  timeUntilReset: number;
}

export interface DailyChallengeAttempt {
  id: string;
  score: number;
  correctAnswers: number;
  totalQuestions: number;
  timeSpent: number;
  xpEarned: number;
  completedAt: Date;
}

export interface DailyChallengeStats {
  currentStreak: number;
  longestStreak: number;
  totalChallengesCompleted: number;
  totalXpEarned: number;
  lastCompletedDate: Date | null;
}

export interface ChallengeStatus {
  challengeAvailable: boolean;
  completed: boolean;
  attempt: DailyChallengeAttempt | null;
  stats: DailyChallengeStats;
  timeUntilReset: number;
}

export interface SubmitChallengeDto {
  challengeId: string;
  answers: number[];
  timeSpent: number;
}

export interface SubmitChallengeResult {
  attempt: DailyChallengeAttempt;
  results: Array<{
    question: string;
    yourAnswer: number;
    correctAnswer: number;
    isCorrect: boolean;
    explanation: string;
  }>;
}

export interface HistoryEntry {
  id: string;
  score: number;
  correctAnswers: number;
  totalQuestions: number;
  timeSpent: number;
  xpEarned: number;
  completedAt: Date;
  challenge: {
    date: Date;
    theme: string;
    difficulty: 'EASY' | 'MEDIUM' | 'HARD';
  };
}

export interface LeaderboardEntry {
  rank: number;
  user: {
    id: string;
    username: string;
    displayName: string;
    avatarUrl: string | null;
  };
  currentStreak: number;
  longestStreak: number;
  totalCompleted: number;
  totalXp: number;
}

class DailyChallengeService {
  /**
   * Récupère le Daily Challenge du jour
   */
  async getTodayChallenge(): Promise<DailyChallenge> {
    const response = await axios.get(`${API_URL}/daily-challenge/today`, {
      headers: getAuthHeaders(),
    });
    return response.data;
  }

  /**
   * Récupère le statut du challenge pour l'utilisateur
   */
  async getChallengeStatus(): Promise<ChallengeStatus> {
    const response = await axios.get(`${API_URL}/daily-challenge/status`, {
      headers: getAuthHeaders(),
    });
    return response.data;
  }

  /**
   * Soumet les réponses au Daily Challenge
   */
  async submitChallenge(dto: SubmitChallengeDto): Promise<SubmitChallengeResult> {
    const response = await axios.post(
      `${API_URL}/daily-challenge/submit`,
      dto,
      {
        headers: getAuthHeaders(),
      }
    );
    return response.data;
  }

  /**
   * Récupère les statistiques de l'utilisateur
   */
  async getUserStats(): Promise<DailyChallengeStats> {
    const response = await axios.get(`${API_URL}/daily-challenge/stats`, {
      headers: getAuthHeaders(),
    });
    return response.data;
  }

  /**
   * Récupère l'historique des tentatives
   */
  async getUserHistory(limit = 30): Promise<HistoryEntry[]> {
    const response = await axios.get(`${API_URL}/daily-challenge/history`, {
      headers: getAuthHeaders(),
      params: { limit },
    });
    return response.data;
  }

  /**
   * Récupère le leaderboard des streaks
   */
  async getStreakLeaderboard(limit = 10): Promise<LeaderboardEntry[]> {
    const response = await axios.get(`${API_URL}/daily-challenge/leaderboard`, {
      params: { limit },
    });
    return response.data;
  }
}

export const dailyChallengeService = new DailyChallengeService();
