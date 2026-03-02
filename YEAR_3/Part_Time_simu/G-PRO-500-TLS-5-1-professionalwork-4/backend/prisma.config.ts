import { defineConfig } from 'prisma/config';
import * as path from 'path';
import * as dotenv from 'dotenv';

dotenv.config({ path: path.resolve(__dirname, '.env') });

export default defineConfig({
  schema: './prisma/schema.prisma',
  seeds: {
    main: 'ts-node prisma/seed.ts',
  },
});
