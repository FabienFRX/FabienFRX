import { Module } from '@nestjs/common';
import { ConfigModule } from '@nestjs/config';
import { PrismaModule } from './prisma/prisma.module';
import { AuthModule } from './auth/auth.module';
import { UsersModule } from './users/users.module';
import { MultiplayerModule } from './multiplayer/multiplayer.module';
import { DailyChallengeModule } from './daily-challenge/daily-challenge.module';
import { MessagesModule } from './messages/messages.module';

@Module({
  imports: [
    ConfigModule.forRoot({ isGlobal: true }),
    PrismaModule,
    AuthModule,
    UsersModule,
    MultiplayerModule,
    DailyChallengeModule,
    MessagesModule,
  ],
})
export class AppModule {}
