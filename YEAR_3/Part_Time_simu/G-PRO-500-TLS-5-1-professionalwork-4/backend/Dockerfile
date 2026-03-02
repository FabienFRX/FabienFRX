# Multi-stage build for production
FROM node:20-alpine AS builder

WORKDIR /app

# Copy package files
COPY package*.json ./
COPY prisma.config.ts ./
COPY api/package*.json ./api/

# Install dependencies
RUN npm install
RUN cd api && npm install

# Copy source code
COPY . .

# Generate Prisma Client
RUN npx prisma generate

# Build the application
RUN cd api && npm run build

# Production stage
FROM node:20-alpine

WORKDIR /app

# Copy package files
COPY package*.json ./
COPY prisma.config.ts ./
COPY api/package*.json ./api/

# Install production dependencies only
RUN npm install --production
RUN cd api && npm install --production

# Copy built application and Prisma files
COPY --from=builder /app/api/dist ./api/dist
COPY --from=builder /app/prisma ./prisma
COPY --from=builder /app/node_modules/.prisma ./node_modules/.prisma

# Generate Prisma Client in production
RUN npx prisma generate

EXPOSE 3000

# Wait for DB and run migrations
CMD npx prisma migrate deploy && cd api && node dist/main.js
