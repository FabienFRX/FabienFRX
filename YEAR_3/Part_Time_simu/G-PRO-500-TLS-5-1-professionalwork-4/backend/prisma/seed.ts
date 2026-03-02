import { PrismaClient } from '@prisma/client';
import * as bcrypt from 'bcrypt';

const prisma = new PrismaClient();

async function main() {
  const hash = await bcrypt.hash('password123', 12);

  await prisma.user.upsert({
    where: { email: 'admin@quizlab.local' },
    update: {},
    create: {
      email: 'admin@quizlab.local',
      username: 'admin',
      displayName: 'Admin',
      passwordHash: hash,
      roles: ['ADMIN'],
    },
  });
}

main()
  .then(() => prisma.$disconnect())
  .catch(async (e) => { console.error(e); await prisma.$disconnect(); process.exit(1); });
