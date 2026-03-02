import { Clock, Flame, Trophy, Info, History } from "lucide-react";
import { Card, CardContent } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { Badge } from "@/components/ui/badge";
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogHeader,
  DialogTitle,
  DialogTrigger,
} from "@/components/ui/dialog";
import { useState, useEffect } from "react";

type ChallengeState = 'idle' | 'running' | 'completed' | 'missed' | 'loading';

interface DailyChallengeCardProps {
  theme: string;
  difficulty: 'facile' | 'moyen' | 'difficile';
  timeLeft: number;
  streak: number;
  xpReward: number;
  rankToday?: number;
  state: ChallengeState;
  trapsEnabled: boolean;
  onStart?: () => void;
  onViewHistory?: () => void;
}

export const DailyChallengeCard = ({
  theme,
  difficulty,
  timeLeft,
  streak,
  xpReward,
  rankToday,
  state,
  trapsEnabled,
  onStart,
  onViewHistory,
}: DailyChallengeCardProps) => {
  const [countdown, setCountdown] = useState(timeLeft);

  // Mettre à jour le countdown quand timeLeft change
  useEffect(() => {
    setCountdown(timeLeft);
  }, [timeLeft]);

  useEffect(() => {
    if (state === 'idle' || state === 'completed') {
      const timer = setInterval(() => {
        setCountdown((prev) => (prev > 0 ? prev - 1000 : 0));
      }, 1000);
      return () => clearInterval(timer);
    }
  }, [state]);

  const formatTime = (milliseconds: number) => {
    const totalSeconds = Math.floor(milliseconds / 1000);
    const h = Math.floor(totalSeconds / 3600);
    const m = Math.floor((totalSeconds % 3600) / 60);
    const s = totalSeconds % 60;
    return `${h}h ${m}m ${s}s`;
  };

  const getDifficultyColor = () => {
    switch (difficulty) {
      case 'facile':
        return 'bg-success text-success-foreground';
      case 'moyen':
        return 'bg-warning text-warning-foreground';
      case 'difficile':
        return 'bg-destructive text-destructive-foreground';
    }
  };

  return (
    <Card className="w-full border-2 hover:shadow-medium transition-shadow">
      <div className="h-1 bg-primary w-full rounded-t-lg" />
      <CardContent className="p-6">
        <div className="grid md:grid-cols-2 gap-6">
          {/* Left: Info */}
          <div className="space-y-4">
            <div>
              <h3 className="text-2xl font-bold text-foreground mb-2">
                Défi du jour
              </h3>
              <div className="flex flex-wrap gap-2 items-center text-sm text-muted-foreground">
                <span>Thème : <strong className="text-foreground">{theme}</strong></span>
                <span>•</span>
                <Badge className={getDifficultyColor()}>
                  {difficulty}
                </Badge>
              </div>
            </div>

            <div className="flex flex-wrap gap-4 text-sm">
              <div className="flex items-center gap-1.5">
                <Clock className="h-4 w-4 text-muted-foreground" />
                <span className="text-muted-foreground">
                  {state === 'completed' ? 'Prochain dans' : 'Temps restant'} : 
                  <strong className="ml-1 text-foreground">{formatTime(countdown)}</strong>
                </span>
              </div>
              <div className="flex items-center gap-1.5">
                <Flame className="h-4 w-4 text-accent" />
                <span className="text-muted-foreground">
                  Série : <strong className="text-accent">{streak} jours</strong>
                </span>
              </div>
              <div className="flex items-center gap-1.5">
                <Trophy className="h-4 w-4 text-secondary" />
                <span className="text-muted-foreground">
                  Récompenses : <strong className="text-foreground">+{xpReward} XP</strong>
                </span>
              </div>
            </div>

            <p className="text-xs text-muted-foreground">
              Adapté à ta tranche d'âge. {trapsEnabled ? '15–20% de réponses pièges.' : ''}
            </p>
          </div>

          {/* Right: Actions */}
          <div className="flex flex-col justify-center gap-3">
            {state === 'idle' && (
              <>
                <Button 
                  className="gradient-primary text-primary-foreground hover:opacity-90 transition-opacity"
                  size="lg"
                  onClick={onStart}
                >
                  Lancer le défi
                </Button>
                <div className="flex gap-2">
                  <Dialog>
                    <DialogTrigger asChild>
                      <Button variant="ghost" size="sm" className="flex-1 border-secondary text-secondary hover:bg-secondary hover:text-secondary-foreground">
                        <Info className="h-4 w-4 mr-1" />
                        Règles
                      </Button>
                    </DialogTrigger>
                    <DialogContent className="max-w-2xl max-h-[80vh] overflow-y-auto">
                      <DialogHeader>
                        <DialogTitle>Règles du Défi quotidien</DialogTitle>
                        <DialogDescription className="space-y-4 text-left pt-4">
                          <div>
                            <h4 className="font-semibold text-foreground mb-2">Format</h4>
                            <p>10 questions / 120 secondes</p>
                          </div>
                          
                          <div>
                            <h4 className="font-semibold text-foreground mb-2">Tentatives</h4>
                            <p>1 seule tentative par jour pour le classement. Rejouable en mode entraînement (sans points).</p>
                          </div>

                          <div>
                            <h4 className="font-semibold text-foreground mb-2">Barème</h4>
                            <ul className="list-disc list-inside space-y-1">
                              <li>Bonne réponse : +10 pts</li>
                              <li>Réponse rapide (&lt;5s) : +3 pts bonus</li>
                              <li>Série de 3 bonnes : +5 pts bonus</li>
                              <li>Mauvaise réponse : 0 pt</li>
                            </ul>
                          </div>

                          <div>
                            <h4 className="font-semibold text-foreground mb-2">Pièges</h4>
                            <p>15–20% des questions intègrent des propositions proches/ambiguës pour tester votre vigilance.</p>
                          </div>

                          <div>
                            <h4 className="font-semibold text-foreground mb-2">Adaptation IA</h4>
                            <p>Thème et difficulté pondérés par tranche d'âge + historique de performance.</p>
                          </div>

                          <div>
                            <h4 className="font-semibold text-foreground mb-2">Anti-triche</h4>
                            <p>Pas de pause. Si focus perdu &gt; 10 secondes → reprise à la question suivante sans bonus temps.</p>
                          </div>

                          <div>
                            <h4 className="font-semibold text-foreground mb-2">Récompenses</h4>
                            <p>XP profil, points classement quotidien, progression défis hebdomadaires.</p>
                          </div>
                        </DialogDescription>
                      </DialogHeader>
                    </DialogContent>
                  </Dialog>

                  <Button 
                    variant="ghost" 
                    size="sm" 
                    className="flex-1 border-secondary text-secondary hover:bg-secondary hover:text-secondary-foreground"
                    onClick={onViewHistory}
                  >
                    <History className="h-4 w-4 mr-1" />
                    Historique
                  </Button>
                </div>
              </>
            )}

            {state === 'completed' && (
              <>
                <Badge className="bg-success text-success-foreground text-base py-2 justify-center">
                  ✓ Défi terminé
                </Badge>
                <div className="text-center space-y-1">
                  <p className="text-sm text-muted-foreground">Ton rang du jour</p>
                  <p className="text-3xl font-bold text-primary">#{rankToday}</p>
                </div>
                <Button 
                  variant="outline"
                  className="border-secondary text-secondary hover:bg-secondary hover:text-secondary-foreground"
                  onClick={onStart}
                >
                  Rejouer en entraînement
                </Button>
              </>
            )}

            {state === 'missed' && (
              <>
                <Badge className="bg-warning text-warning-foreground text-base py-2 justify-center">
                  Défi manqué
                </Badge>
                <p className="text-sm text-center text-muted-foreground">Reviens demain !</p>
                <Button 
                  variant="outline"
                  onClick={onStart}
                >
                  S'entraîner
                </Button>
              </>
            )}
          </div>
        </div>
      </CardContent>
    </Card>
  );
};
