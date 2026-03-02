// Usage:
// 1. Assurez-vous que Docker Postgres est démarré: `docker compose up -d`
// 2. Vérifiez que `.env` contient DATABASE_URL
// 3. Exécutez: `npx ts-node scripts/set-admin-role.ts admin@quizlab.com`
//    (ou: `node --loader ts-node/esm scripts/set-admin-role.ts <email>` si ESM)
// Le script ajoute ADMIN au tableau roles s'il n'y est pas.

import { PrismaClient, Role } from '@prisma/client';

async function main() {
  const email = process.argv[2];
  if (!email) {
    console.error('Email manquant. Utilisation: npx ts-node scripts/set-admin-role.ts <email>');
    process.exit(1);
  }
  const prisma = new PrismaClient();
  try {
    const user = await prisma.user.findFirst({ where: { email } });
    if (!user) {
      console.error(`Utilisateur introuvable pour ${email}`);
      process.exit(1);
    }
    const roles: Role[] = user.roles.includes(Role.ADMIN) ? user.roles : [...user.roles, Role.ADMIN];
    await prisma.user.update({ where: { id: user.id }, data: { roles } });
    console.log(`Rôle ADMIN appliqué à ${email}. Rôles actuels:`, roles);
  } catch (e) {
    console.error('Erreur mise à jour rôle:', e);
    process.exit(1);
  } finally {
    process.exit(0);
  }
}

main();
