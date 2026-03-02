import { Trophy, Medal, Plus } from "lucide-react";
import { Card } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { useNavigate } from "react-router-dom";

interface PodiumPlayer {
  rank: number;
  name: string;
  score: number;
}

const podiumData: PodiumPlayer[] = [
  { rank: 2, name: "Abdel M.", score: 18150 },
  { rank: 1, name: "Andi B.", score: 19820 },
  { rank: 3, name: "Fabien F.", score: 17320 },
];

export const Podium = () => {
  const navigate = useNavigate();
  const getHeight = (rank: number) => {
    switch (rank) {
      case 1:
        return "h-48";
      case 2:
        return "h-40";
      case 3:
        return "h-32";
      default:
        return "h-32";
    }
  };

  const getColor = (rank: number) => {
    switch (rank) {
      case 1:
        return "gradient-primary";
      case 2:
        return "gradient-secondary";
      case 3:
        return "gradient-accent";
      default:
        return "bg-muted";
    }
  };

  return (
    <div className="w-full max-w-4xl mx-auto px-4 py-12">
      <div className="flex items-center justify-center gap-2 mb-8">
        <h2 className="text-3xl font-bold text-foreground flex items-center gap-2">
          <Trophy className="h-8 w-8 text-primary" />
          Top Mondial
        </h2>
        <Button
          variant="ghost"
          size="sm"
          className="text-secondary hover:text-secondary hover:bg-secondary/10"
          onClick={() => navigate('/leaderboard')}
          aria-label="Voir tout le classement"
        >
          <Plus className="h-5 w-5" />
        </Button>
      </div>
      <p className="text-center text-muted-foreground mb-8">Les meilleurs joueurs du moment</p>

      <div className="flex items-end justify-center gap-4 mb-8">
        {podiumData.map((player) => (
          <div
            key={player.rank}
            className="flex flex-col items-center animate-slide-up"
            style={{ animationDelay: `${player.rank * 100}ms` }}
          >
            <div className="mb-3 text-center">
              <div className="mb-2">
                {player.rank === 1 ? (
                  <Trophy className="h-8 w-8 text-primary mx-auto animate-pulse-soft" />
                ) : (
                  <Medal className="h-6 w-6 text-muted-foreground mx-auto" />
                )}
              </div>
              <p className="font-semibold text-foreground">{player.name}</p>
              <p className="text-sm text-muted-foreground">{player.score} pts</p>
            </div>
            <div
              className={`w-24 ${getHeight(player.rank)} ${getColor(
                player.rank
              )} rounded-t-2xl flex items-center justify-center shadow-soft`}
            >
              <span className="text-4xl font-bold text-white">{player.rank}</span>
            </div>
          </div>
        ))}
      </div>

      <Card className="bg-card border-2 border-border p-4 max-w-xs mx-auto">
        <div className="flex items-center justify-between">
          <div>
            <p className="text-sm text-muted-foreground">Ton rang</p>
            <p className="text-xl font-bold text-primary">#462</p>
          </div>
          <div className="text-right">
            <p className="text-sm text-muted-foreground">Ton score</p>
            <p className="text-xl font-bold text-foreground">3,420 pts</p>
          </div>
        </div>
      </Card>
    </div>
  );
};
