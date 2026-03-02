import { Controller, Get, Query } from '@nestjs/common';
import { QuizService } from './quiz.service';

@Controller('quiz')
export class QuizController {
  constructor(private readonly quizService: QuizService) {}

  @Get('solo')
  getSoloQuestions(
    @Query('theme') theme: string = 'random',
    @Query('difficulty') difficulty: string = 'medium',
    @Query('count') count: string = '5',
  ) {
    const questionCount = parseInt(count, 10) || 5;
    const questions = this.quizService.getQuestionsForSolo(theme, difficulty, questionCount);
    
    return {
      questions: questions.map(q => ({
        id: q.id,
        question: q.question,
        options: q.options,
        correctAnswer: q.correctAnswer,
        explanation: q.explanation,
        theme: q.theme,
        difficulty: q.difficulty,
      })),
      theme,
      difficulty,
      totalQuestions: questions.length,
    };
  }
}
