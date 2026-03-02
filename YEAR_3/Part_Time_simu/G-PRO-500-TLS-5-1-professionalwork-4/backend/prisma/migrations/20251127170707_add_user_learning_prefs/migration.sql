/*
  Warnings:

  - You are about to drop the column `isDeleted` on the `User` table. All the data in the column will be lost.

*/
-- CreateEnum
CREATE TYPE "AgeGroup" AS ENUM ('CHILD', 'TEEN', 'ADULT');

-- CreateEnum
CREATE TYPE "Level" AS ENUM ('EASY', 'MEDIUM', 'HARD');

-- AlterTable
ALTER TABLE "User" DROP COLUMN "isDeleted",
ADD COLUMN     "ageGroup" "AgeGroup",
ADD COLUMN     "defaultLevel" "Level",
ADD COLUMN     "favoriteTopics" TEXT[] DEFAULT ARRAY[]::TEXT[],
ADD COLUMN     "trickyAnswersEnabled" BOOLEAN NOT NULL DEFAULT false;
