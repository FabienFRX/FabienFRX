import {
  Controller,
  Get,
  Post,
  Body,
  UseGuards,
  Request,
  Query,
} from '@nestjs/common';
import { DailyChallengeService } from './daily-challenge.service';
import { JwtAuthGuard } from '../auth/jwt-auth.guard';

interface SubmitAnswersDto {
  challengeId: string;
  answers: number[];
  timeSpent: number;
}

@Controller('daily-challenge')
export class DailyChallengeController {
  constructor(private readonly dailyChallengeService: DailyChallengeService) {}

  /**
   * GET /api/daily-challenge/today
   * Récupère le Daily Challenge du jour
   */
  @Get('today')
  @UseGuards(JwtAuthGuard)
  async getTodayChallenge() {
    return this.dailyChallengeService.getTodayChallenge();
  }

  /**
   * GET /api/daily-challenge/status
   * Récupère le statut du challenge pour l'utilisateur connecté
   */
  @Get('status')
  @UseGuards(JwtAuthGuard)
  async getUserStatus(@Request() req: any) {
    return this.dailyChallengeService.getUserChallengeStatus(req.user.id);
  }

  /**
   * POST /api/daily-challenge/submit
   * Soumet les réponses au Daily Challenge
   */
  @Post('submit')
  @UseGuards(JwtAuthGuard)
  async submitChallenge(@Request() req: any, @Body() dto: SubmitAnswersDto) {
    return this.dailyChallengeService.submitChallenge(
      req.user.id,
      dto.challengeId,
      {
        answers: dto.answers,
        timeSpent: dto.timeSpent,
      },
    );
  }

  /**
   * GET /api/daily-challenge/stats
   * Récupère les statistiques de l'utilisateur connecté
   */
  @Get('stats')
  @UseGuards(JwtAuthGuard)
  async getUserStats(@Request() req: any) {
    return this.dailyChallengeService.getUserStats(req.user.id);
  }

  /**
   * GET /api/daily-challenge/history
   * Récupère l'historique des tentatives de l'utilisateur
   */
  @Get('history')
  @UseGuards(JwtAuthGuard)
  async getUserHistory(@Request() req: any, @Query('limit') limit?: string) {
    const limitNum = limit ? parseInt(limit, 10) : 30;
    return this.dailyChallengeService.getUserHistory(req.user.id, limitNum);
  }

  /**
   * GET /api/daily-challenge/leaderboard
   * Récupère le leaderboard des streaks
   */
  @Get('leaderboard')
  async getStreakLeaderboard(@Query('limit') limit?: string) {
    const limitNum = limit ? parseInt(limit, 10) : 10;
    return this.dailyChallengeService.getStreakLeaderboard(limitNum);
  }
}
