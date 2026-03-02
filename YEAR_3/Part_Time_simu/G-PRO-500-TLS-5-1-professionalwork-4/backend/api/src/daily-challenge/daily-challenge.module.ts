import { Module } from '@nestjs/common';
import { DailyChallengeService } from './daily-challenge.service';
import { DailyChallengeController } from './daily-challenge.controller';
import { PrismaModule } from '../prisma/prisma.module';

@Module({
  imports: [PrismaModule],
  controllers: [DailyChallengeController],
  providers: [DailyChallengeService],
  exports: [DailyChallengeService],
})
export class DailyChallengeModule {}
