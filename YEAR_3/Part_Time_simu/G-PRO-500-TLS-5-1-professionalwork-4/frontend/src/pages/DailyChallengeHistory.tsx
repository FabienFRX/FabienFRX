import { useState, useEffect } from 'react';
import { useNavigate } from 'react-router-dom';
import { useAuth } from '../hooks/useAuth';
import { HistoryEntry, dailyChallengeService } from '../services/dailyChallenge.service';
import { Card, CardContent, CardHeader, CardTitle } from '../components/ui/card';
import { Button } from '../components/ui/button';
import { Badge } from '../components/ui/badge';
import { ArrowLeft, Calendar, Clock, Star, Trophy } from 'lucide-react';
import { devError } from '@/lib/api.config';

export default function DailyChallengeHistory() {
  const { user } = useAuth();
  const navigate = useNavigate();
  const [history, setHistory] = useState<HistoryEntry[]>([]);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    if (!user) {
      navigate('/auth');
      return;
    }
    loadHistory();
  }, [user, navigate]);

  const loadHistory = async () => {
    try {
      const data = await dailyChallengeService.getUserHistory(30);
      setHistory(data);
    } catch (error) {
      devError('Failed to load history:', error);
    } finally {
      setLoading(false);
    }
  };

  const getDifficultyColor = (difficulty: string) => {
    switch (difficulty) {
      case 'EASY': return 'bg-green-500';
      case 'MEDIUM': return 'bg-yellow-500';
      case 'HARD': return 'bg-red-500';
      default: return 'bg-gray-500';
    }
  };

  const getDifficultyLabel = (difficulty: string) => {
    switch (difficulty) {
      case 'EASY': return 'Facile';
      case 'MEDIUM': return 'Moyen';
      case 'HARD': return 'Difficile';
      default: return difficulty;
    }
  };

  const formatDate = (date: Date) => {
    return new Date(date).toLocaleDateString('fr-FR', {
      day: 'numeric',
      month: 'long',
      year: 'numeric',
    });
  };

  const formatTime = (seconds: number) => {
    const mins = Math.floor(seconds / 60);
    const secs = seconds % 60;
    return `${mins}m ${secs}s`;
  };

  if (loading) {
    return (
      <div className="container mx-auto p-6 flex items-center justify-center min-h-[60vh]">
        <div className="text-center">
          <div className="animate-spin rounded-full h-12 w-12 border-b-2 border-primary mx-auto mb-4"></div>
          <p>Chargement de l'historique...</p>
        </div>
      </div>
    );
  }

  return (
    <div className="container mx-auto p-6 max-w-4xl space-y-6">
      <div className="flex items-center gap-4">
        <Button variant="ghost" onClick={() => navigate('/daily-challenge')}>
          <ArrowLeft className="h-4 w-4" />
        </Button>
        <h1 className="text-3xl font-bold">Historique des Défis</h1>
      </div>

      {history.length === 0 ? (
        <Card>
          <CardContent className="py-12 text-center text-muted-foreground">
            <Trophy className="h-12 w-12 mx-auto mb-4 opacity-50" />
            <p>Aucun défi complété pour le moment.</p>
            <Button onClick={() => navigate('/daily-challenge')} className="mt-4">
              Commencer aujourd'hui
            </Button>
          </CardContent>
        </Card>
      ) : (
        <div className="space-y-4">
          {history.map((entry) => (
            <Card key={entry.id}>
              <CardHeader>
                <div className="flex justify-between items-start">
                  <div>
                    <CardTitle className="flex items-center gap-2 text-lg">
                      <Calendar className="h-5 w-5" />
                      {formatDate(entry.challenge.date)}
                    </CardTitle>
                    <p className="text-sm text-muted-foreground mt-1">
                      Thème: {entry.challenge.theme}
                    </p>
                  </div>
                  <Badge className={getDifficultyColor(entry.challenge.difficulty)}>
                    {getDifficultyLabel(entry.challenge.difficulty)}
                  </Badge>
                </div>
              </CardHeader>
              <CardContent>
                <div className="grid grid-cols-2 md:grid-cols-4 gap-4">
                  <div className="flex items-center gap-2">
                    <Trophy className="h-5 w-5 text-yellow-500" />
                    <div>
                      <div className="font-bold">{entry.score}%</div>
                      <div className="text-xs text-muted-foreground">Score</div>
                    </div>
                  </div>
                  <div className="flex items-center gap-2">
                    <div className="h-5 w-5 flex items-center justify-center bg-green-500 rounded-full text-white text-xs">✓</div>
                    <div>
                      <div className="font-bold">
                        {entry.correctAnswers}/{entry.totalQuestions}
                      </div>
                      <div className="text-xs text-muted-foreground">Bonnes réponses</div>
                    </div>
                  </div>
                  <div className="flex items-center gap-2">
                    <Star className="h-5 w-5 text-blue-500" />
                    <div>
                      <div className="font-bold">+{entry.xpEarned}</div>
                      <div className="text-xs text-muted-foreground">XP</div>
                    </div>
                  </div>
                  <div className="flex items-center gap-2">
                    <Clock className="h-5 w-5 text-gray-500" />
                    <div>
                      <div className="font-bold">{formatTime(entry.timeSpent)}</div>
                      <div className="text-xs text-muted-foreground">Temps</div>
                    </div>
                  </div>
                </div>
              </CardContent>
            </Card>
          ))}
        </div>
      )}
    </div>
  );
}
