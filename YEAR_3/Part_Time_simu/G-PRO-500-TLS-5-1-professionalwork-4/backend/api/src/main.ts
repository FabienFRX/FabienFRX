import { NestFactory } from '@nestjs/core';
import { AppModule } from './app.module';
import { ValidationPipe } from '@nestjs/common';

async function bootstrap() {
  const app = await NestFactory.create(AppModule, {
    cors: {
      origin: ['http://localhost:8080', 'http://localhost:8081', 'http://localhost:8082', 'http://localhost:5173'],
      methods: ['GET', 'POST', 'PUT', 'DELETE', 'PATCH', 'OPTIONS'],
      credentials: true,
      allowedHeaders: ['Content-Type', 'Authorization'],
    },
  });
  
  // Préfixe global pour toutes les routes
  app.setGlobalPrefix('api');
  
  app.useGlobalPipes(new ValidationPipe({ whitelist: true, forbidNonWhitelisted: true }));
  await app.listen(process.env.PORT || 3000);
  console.log(`Application is running on: http://localhost:${process.env.PORT || 3000}`);
}
bootstrap();
