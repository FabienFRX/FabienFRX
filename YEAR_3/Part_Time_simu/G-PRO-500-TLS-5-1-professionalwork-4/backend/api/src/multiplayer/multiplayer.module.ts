import { Module } from '@nestjs/common';
import { MultiplayerGateway } from './multiplayer.gateway';
import { QuizService } from './quiz.service';
import { QuizController } from './quiz.controller';

@Module({
  controllers: [QuizController],
  providers: [MultiplayerGateway, QuizService],
  exports: [QuizService],
})
export class MultiplayerModule {}
