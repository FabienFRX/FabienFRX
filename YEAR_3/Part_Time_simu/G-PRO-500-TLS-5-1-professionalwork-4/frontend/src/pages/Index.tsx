import { Header } from "@/components/Header";
import { ModeCard } from "@/components/ModeCard";
import { DailyChallengeCard } from "@/components/DailyChallengeCard";
import { Podium } from "@/components/Podium";
import { Footer } from "@/components/Footer";
import { BookOpen, Gamepad2 } from "lucide-react";
import { useNavigate } from "react-router-dom";
import { useAuth } from "@/hooks/useAuth";
import { useEffect, useState } from "react";
import { type ChallengeStatus, type DailyChallenge, dailyChallengeService } from "@/services/dailyChallenge.service";
import { devError } from "@/lib/api.config";

const Index = () => {
  const navigate = useNavigate();
  const { user } = useAuth();
  const [challengeStatus, setChallengeStatus] = useState<ChallengeStatus | null>(null);
  const [challenge, setChallenge] = useState<DailyChallenge | null>(null);

  useEffect(() => {
    if (user) {
      loadChallengeData();
    }
  }, [user]);

  const loadChallengeData = async () => {
    try {
      const [status, challengeData] = await Promise.all([
        dailyChallengeService.getChallengeStatus(),
        dailyChallengeService.getTodayChallenge(),
      ]);
      setChallengeStatus(status);
      setChallenge(challengeData);
    } catch (error) {
      devError('Failed to load challenge data:', error);
    }
  };

  const getDifficultyLabel = (difficulty?: string) => {
    switch (difficulty) {
      case 'EASY': return 'facile';
      case 'MEDIUM': return 'moyen';
      case 'HARD': return 'difficile';
      default: return 'moyen';
    }
  };

  return (
    <div className="min-h-screen flex flex-col bg-background">
      <Header />
      
      <main className="flex-1 flex flex-col items-center px-4 py-12">
        <div className="w-full max-w-6xl space-y-12">
          {/* Hero Section */}
          <div className="text-center animate-fade-in">
            <h1 className="text-4xl md:text-5xl font-bold text-foreground mb-4">
              Découvrez QuizLab 🎓
            </h1>
            <p className="text-lg md:text-xl text-muted-foreground max-w-2xl mx-auto">
              Apprenez en vous amusant avec des quiz adaptés à tous les âges
            </p>
          </div>

          {/* Mode Cards */}
          <div className="grid md:grid-cols-2 gap-2 animate-fade-in" style={{ animationDelay: "200ms" }}>
            <ModeCard
              title="Learn"
              description="Découvre, comprends et retiens grâce à notre IA adaptative."
              icon={BookOpen}
              gradient="gradient-primary"
              onClick={() => navigate('/learn')}
            />
            <ModeCard
              title="Quiz"
              description="Teste tes connaissances, progresse et grimpe dans le classement."
              icon={Gamepad2}
              gradient="gradient-secondary"
              onClick={() => navigate('/quiz')}
            />
          </div>

          {/* Daily Challenge */}
          <div className="animate-fade-in" style={{ animationDelay: "400ms" }}>
            <DailyChallengeCard
              theme={challenge?.theme || "Capitales d'Europe"}
              difficulty={getDifficultyLabel(challenge?.difficulty)}
              timeLeft={challenge?.timeUntilReset || 0}
              streak={challengeStatus?.stats.currentStreak || 0}
              xpReward={challenge?.xpReward || 150}
              state={challengeStatus?.completed ? "completed" : "idle"}
              trapsEnabled={true}
              onStart={() => user ? navigate('/daily-challenge') : navigate('/auth')}
              onViewHistory={() => user ? navigate('/daily-challenge/history') : navigate('/auth')}
            />
          </div>

          {/* Podium */}
          <div className="animate-fade-in" style={{ animationDelay: "600ms" }}>
            <Podium />
          </div>
        </div>
      </main>

      <Footer />
    </div>
  );
};

export default Index;
