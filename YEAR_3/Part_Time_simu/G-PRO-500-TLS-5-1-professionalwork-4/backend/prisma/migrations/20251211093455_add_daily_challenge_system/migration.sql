-- CreateTable
CREATE TABLE "DailyChallenge" (
    "id" TEXT NOT NULL,
    "date" DATE NOT NULL,
    "theme" TEXT NOT NULL,
    "difficulty" "Level" NOT NULL,
    "questions" JSONB NOT NULL,
    "xpReward" INTEGER NOT NULL,
    "createdAt" TIMESTAMP(3) NOT NULL DEFAULT CURRENT_TIMESTAMP,

    CONSTRAINT "DailyChallenge_pkey" PRIMARY KEY ("id")
);

-- CreateTable
CREATE TABLE "DailyChallengeAttempt" (
    "id" TEXT NOT NULL,
    "userId" TEXT NOT NULL,
    "challengeId" TEXT NOT NULL,
    "score" INTEGER NOT NULL,
    "correctAnswers" INTEGER NOT NULL,
    "totalQuestions" INTEGER NOT NULL,
    "timeSpent" INTEGER NOT NULL,
    "xpEarned" INTEGER NOT NULL,
    "completedAt" TIMESTAMP(3) NOT NULL DEFAULT CURRENT_TIMESTAMP,

    CONSTRAINT "DailyChallengeAttempt_pkey" PRIMARY KEY ("id")
);

-- CreateTable
CREATE TABLE "DailyChallengeStats" (
    "id" TEXT NOT NULL,
    "userId" TEXT NOT NULL,
    "currentStreak" INTEGER NOT NULL DEFAULT 0,
    "longestStreak" INTEGER NOT NULL DEFAULT 0,
    "totalChallengesCompleted" INTEGER NOT NULL DEFAULT 0,
    "totalXpEarned" INTEGER NOT NULL DEFAULT 0,
    "lastCompletedDate" DATE,
    "createdAt" TIMESTAMP(3) NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "updatedAt" TIMESTAMP(3) NOT NULL,

    CONSTRAINT "DailyChallengeStats_pkey" PRIMARY KEY ("id")
);

-- CreateIndex
CREATE UNIQUE INDEX "DailyChallenge_date_key" ON "DailyChallenge"("date");

-- CreateIndex
CREATE INDEX "DailyChallenge_date_idx" ON "DailyChallenge"("date");

-- CreateIndex
CREATE INDEX "DailyChallengeAttempt_userId_idx" ON "DailyChallengeAttempt"("userId");

-- CreateIndex
CREATE INDEX "DailyChallengeAttempt_challengeId_idx" ON "DailyChallengeAttempt"("challengeId");

-- CreateIndex
CREATE INDEX "DailyChallengeAttempt_completedAt_idx" ON "DailyChallengeAttempt"("completedAt");

-- CreateIndex
CREATE UNIQUE INDEX "DailyChallengeAttempt_userId_challengeId_key" ON "DailyChallengeAttempt"("userId", "challengeId");

-- CreateIndex
CREATE UNIQUE INDEX "DailyChallengeStats_userId_key" ON "DailyChallengeStats"("userId");

-- CreateIndex
CREATE INDEX "DailyChallengeStats_userId_idx" ON "DailyChallengeStats"("userId");

-- CreateIndex
CREATE INDEX "DailyChallengeStats_currentStreak_idx" ON "DailyChallengeStats"("currentStreak");

-- AddForeignKey
ALTER TABLE "DailyChallengeAttempt" ADD CONSTRAINT "DailyChallengeAttempt_userId_fkey" FOREIGN KEY ("userId") REFERENCES "User"("id") ON DELETE CASCADE ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "DailyChallengeAttempt" ADD CONSTRAINT "DailyChallengeAttempt_challengeId_fkey" FOREIGN KEY ("challengeId") REFERENCES "DailyChallenge"("id") ON DELETE CASCADE ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "DailyChallengeStats" ADD CONSTRAINT "DailyChallengeStats_userId_fkey" FOREIGN KEY ("userId") REFERENCES "User"("id") ON DELETE CASCADE ON UPDATE CASCADE;
