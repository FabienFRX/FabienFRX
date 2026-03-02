import { useState, useEffect } from 'react';
import { useNavigate } from 'react-router-dom';
import { useAuth } from '../hooks/useAuth';
import { Question, type DailyChallenge, type ChallengeStatus, dailyChallengeService } from '../services/dailyChallenge.service';
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from '../components/ui/card';
import { Button } from '../components/ui/button';
import { Badge } from '../components/ui/badge';
import { Progress } from '../components/ui/progress';
import { Flame, Trophy, Clock, Star, ChevronRight, History } from 'lucide-react';
import { Alert, AlertDescription } from '../components/ui/alert';
import { devError } from '@/lib/api.config';

export default function DailyChallenge() {
  const { user } = useAuth();
  const navigate = useNavigate();
  
  const [loading, setLoading] = useState(true);
  const [status, setStatus] = useState<ChallengeStatus | null>(null);
  const [challenge, setChallenge] = useState<DailyChallenge | null>(null);
  
  // État du jeu
  const [playing, setPlaying] = useState(false);
  const [currentQuestionIndex, setCurrentQuestionIndex] = useState(0);
  const [selectedAnswers, setSelectedAnswers] = useState<number[]>([]);
  const [timeLeft, setTimeLeft] = useState(20); // 20 secondes par question
  const [startTime] = useState(Date.now());
  const [showResults, setShowResults] = useState(false);
  const [results, setResults] = useState<any>(null);

  // Timer countdown
  const [timeUntilReset, setTimeUntilReset] = useState('');

  useEffect(() => {
    if (!user) {
      navigate('/auth');
      return;
    }
    loadStatus();
  }, [user, navigate]);

  useEffect(() => {
    if (playing && !showResults) {
      const timer = setInterval(() => {
        setTimeLeft((prev) => {
          if (prev <= 1) {
            handleNextQuestion();
            return 20;
          }
          return prev - 1;
        });
      }, 1000);

      return () => clearInterval(timer);
    }
  }, [playing, showResults, currentQuestionIndex]);

  useEffect(() => {
    if (status?.timeUntilReset) {
      updateCountdown(status.timeUntilReset);
      const interval = setInterval(() => {
        updateCountdown(status.timeUntilReset - (Date.now() - startTime));
      }, 1000);
      return () => clearInterval(interval);
    }
  }, [status]);

  const updateCountdown = (ms: number) => {
    const hours = Math.floor(ms / (1000 * 60 * 60));
    const minutes = Math.floor((ms % (1000 * 60 * 60)) / (1000 * 60));
    const seconds = Math.floor((ms % (1000 * 60)) / 1000);
    setTimeUntilReset(`${hours}h ${minutes}m ${seconds}s`);
  };

  const loadStatus = async () => {
    try {
      const [statusData, challengeData] = await Promise.all([
        dailyChallengeService.getChallengeStatus(),
        dailyChallengeService.getTodayChallenge(),
      ]);
      setStatus(statusData);
      setChallenge(challengeData);
    } catch (error) {
      devError('Failed to load challenge:', error);
    } finally {
      setLoading(false);
    }
  };

  const handleStartChallenge = () => {
    setPlaying(true);
    setCurrentQuestionIndex(0);
    setSelectedAnswers([]);
    setTimeLeft(20);
    setShowResults(false);
  };

  const handleAnswerSelect = (answerIndex: number) => {
    const newAnswers = [...selectedAnswers];
    newAnswers[currentQuestionIndex] = answerIndex;
    setSelectedAnswers(newAnswers);
  };

  const handleNextQuestion = () => {
    if (challenge && currentQuestionIndex < challenge.questions.length - 1) {
      setCurrentQuestionIndex(currentQuestionIndex + 1);
      setTimeLeft(20);
    } else {
      handleSubmit();
    }
  };

  const handleSubmit = async () => {
    if (!challenge) return;

    try {
      const timeSpent = Math.floor((Date.now() - startTime) / 1000);
      const result = await dailyChallengeService.submitChallenge({
        challengeId: challenge.id,
        answers: selectedAnswers,
        timeSpent,
      });
      
      setResults(result);
      setShowResults(true);
      setPlaying(false);
      
      // Recharger le statut
      await loadStatus();
    } catch (error: any) {
      devError('Failed to submit challenge:', error);
      if (error.response?.data?.message) {
        alert(error.response.data.message);
      }
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

  if (loading) {
    return (
      <div className="container mx-auto p-6 flex items-center justify-center min-h-[60vh]">
        <div className="text-center">
          <div className="animate-spin rounded-full h-12 w-12 border-b-2 border-primary mx-auto mb-4"></div>
          <p>Chargement du défi...</p>
        </div>
      </div>
    );
  }

  if (!status || !challenge) {
    return (
      <div className="container mx-auto p-6">
        <Alert>
          <AlertDescription>Impossible de charger le défi du jour.</AlertDescription>
        </Alert>
      </div>
    );
  }

  // Affichage des résultats
  if (showResults && results) {
    return (
      <div className="container mx-auto p-6 max-w-4xl">
        <Card>
          <CardHeader>
            <CardTitle className="flex items-center gap-2">
              <Trophy className="h-6 w-6 text-yellow-500" />
              Résultats du Défi Quotidien
            </CardTitle>
          </CardHeader>
          <CardContent className="space-y-6">
            {/* Stats */}
            <div className="grid grid-cols-2 md:grid-cols-4 gap-4">
              <div className="text-center p-4 bg-muted rounded-lg">
                <div className="text-3xl font-bold text-primary">{results.attempt.score}%</div>
                <div className="text-sm text-muted-foreground">Score</div>
              </div>
              <div className="text-center p-4 bg-muted rounded-lg">
                <div className="text-3xl font-bold text-green-500">
                  {results.attempt.correctAnswers}/{results.attempt.totalQuestions}
                </div>
                <div className="text-sm text-muted-foreground">Bonnes réponses</div>
              </div>
              <div className="text-center p-4 bg-muted rounded-lg">
                <div className="text-3xl font-bold text-blue-500">+{results.attempt.xpEarned}</div>
                <div className="text-sm text-muted-foreground">XP gagnés</div>
              </div>
              <div className="text-center p-4 bg-muted rounded-lg">
                <div className="text-3xl font-bold flex items-center justify-center gap-1">
                  <Flame className="h-6 w-6 text-orange-500" />
                  {status.stats.currentStreak}
                </div>
                <div className="text-sm text-muted-foreground">Série</div>
              </div>
            </div>

            {/* Détails des réponses */}
            <div className="space-y-4">
              <h3 className="font-semibold text-lg">Détails des réponses</h3>
              {results.results.map((result: any, index: number) => (
                <Card key={index} className={result.isCorrect ? 'border-green-500' : 'border-red-500'}>
                  <CardContent className="pt-6">
                    <div className="flex items-start gap-2 mb-2">
                      {result.isCorrect ? (
                        <div className="flex-shrink-0 w-6 h-6 bg-green-500 rounded-full flex items-center justify-center text-white text-sm">✓</div>
                      ) : (
                        <div className="flex-shrink-0 w-6 h-6 bg-red-500 rounded-full flex items-center justify-center text-white text-sm">✗</div>
                      )}
                      <div className="flex-1">
                        <p className="font-medium mb-2">{result.question}</p>
                        {!result.isCorrect && (
                          <p className="text-sm text-muted-foreground mb-1">
                            Bonne réponse: {challenge.questions[index].options[result.correctAnswer]}
                          </p>
                        )}
                        <p className="text-sm text-muted-foreground italic">{result.explanation}</p>
                      </div>
                    </div>
                  </CardContent>
                </Card>
              ))}
            </div>

            <div className="flex gap-2">
              <Button onClick={() => navigate('/quiz')} className="flex-1">
                Jouer en Solo
              </Button>
              <Button onClick={() => navigate('/leaderboard')} variant="outline" className="flex-1">
                Voir le classement
              </Button>
            </div>
          </CardContent>
        </Card>
      </div>
    );
  }

  // Mode jeu
  if (playing && challenge) {
    const currentQuestion = challenge.questions[currentQuestionIndex];
    const progress = ((timeLeft / 20) * 100);
    const isUrgent = timeLeft <= 5;

    return (
      <div className="container mx-auto p-6 max-w-3xl">
        <Card className={isUrgent ? 'ring-4 ring-red-500' : ''}>
          <CardHeader>
            <div className="flex justify-between items-center mb-2">
              <Badge variant="outline">
                Question {currentQuestionIndex + 1}/{challenge.questions.length}
              </Badge>
              <div className={`flex items-center gap-2 ${isUrgent ? 'text-red-500 font-bold' : ''}`}>
                <Clock className="h-4 w-4" />
                <span>{timeLeft}s</span>
              </div>
            </div>
            <Progress value={progress} className={isUrgent ? 'bg-red-200' : ''} />
          </CardHeader>
          <CardContent className="space-y-6">
            <h2 className="text-xl font-semibold">{currentQuestion.question}</h2>
            
            <div className="space-y-3">
              {currentQuestion.options.map((option, index) => (
                <Button
                  key={index}
                  variant={selectedAnswers[currentQuestionIndex] === index ? 'default' : 'outline'}
                  className="w-full justify-start text-left h-auto py-4"
                  onClick={() => handleAnswerSelect(index)}
                >
                  <span className="font-semibold mr-2">{String.fromCharCode(65 + index)}.</span>
                  {option}
                </Button>
              ))}
            </div>

            <Button
              onClick={handleNextQuestion}
              disabled={selectedAnswers[currentQuestionIndex] === undefined}
              className="w-full"
            >
              {currentQuestionIndex < challenge.questions.length - 1 ? 'Question suivante' : 'Terminer'}
              <ChevronRight className="ml-2 h-4 w-4" />
            </Button>
          </CardContent>
        </Card>
      </div>
    );
  }

  // Écran d'accueil
  return (
    <div className="container mx-auto p-6 max-w-4xl space-y-6">
      {/* En-tête avec stats */}
      <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
        <Card>
          <CardContent className="pt-6 flex items-center gap-4">
            <Flame className="h-10 w-10 text-orange-500" />
            <div>
              <div className="text-2xl font-bold">{status.stats.currentStreak}</div>
              <div className="text-sm text-muted-foreground">Série actuelle</div>
            </div>
          </CardContent>
        </Card>
        <Card>
          <CardContent className="pt-6 flex items-center gap-4">
            <Star className="h-10 w-10 text-yellow-500" />
            <div>
              <div className="text-2xl font-bold">{status.stats.totalXpEarned}</div>
              <div className="text-sm text-muted-foreground">XP total</div>
            </div>
          </CardContent>
        </Card>
        <Card>
          <CardContent className="pt-6 flex items-center gap-4">
            <Trophy className="h-10 w-10 text-blue-500" />
            <div>
              <div className="text-2xl font-bold">{status.stats.longestStreak}</div>
              <div className="text-sm text-muted-foreground">Meilleure série</div>
            </div>
          </CardContent>
        </Card>
      </div>

      {/* Défi du jour */}
      <Card>
        <CardHeader>
          <div className="flex justify-between items-start">
            <div>
              <CardTitle className="flex items-center gap-2">
                <Trophy className="h-6 w-6 text-yellow-500" />
                Défi Quotidien
              </CardTitle>
              <CardDescription className="mt-2">
                Thème: {challenge.theme} • {challenge.questions.length} questions
              </CardDescription>
            </div>
            <Badge className={getDifficultyColor(challenge.difficulty)}>
              {getDifficultyLabel(challenge.difficulty)}
            </Badge>
          </div>
        </CardHeader>
        <CardContent className="space-y-4">
          {status.completed ? (
            <Alert>
              <AlertDescription className="flex items-center justify-between">
                <span>✅ Défi complété aujourd'hui ! Score: {status.attempt?.score}%</span>
                <span className="text-sm text-muted-foreground flex items-center gap-1">
                  <Clock className="h-4 w-4" />
                  Prochain défi dans {timeUntilReset}
                </span>
              </AlertDescription>
            </Alert>
          ) : (
            <>
              <div className="flex items-center justify-between p-4 bg-muted rounded-lg">
                <div className="flex items-center gap-2">
                  <Star className="h-5 w-5 text-yellow-500" />
                  <span className="font-semibold">Récompense:</span>
                </div>
                <Badge variant="secondary" className="text-lg">
                  +{challenge.xpReward} XP max
                </Badge>
              </div>
              
              <Button onClick={handleStartChallenge} className="w-full" size="lg">
                Commencer le défi
                <ChevronRight className="ml-2 h-5 w-5" />
              </Button>
              
              <p className="text-sm text-center text-muted-foreground flex items-center justify-center gap-1">
                <Clock className="h-4 w-4" />
                Nouveau défi dans {timeUntilReset}
              </p>
            </>
          )}
        </CardContent>
      </Card>

      {/* Historique */}
      <Button
        variant="outline"
        onClick={() => navigate('/daily-challenge/history')}
        className="w-full"
      >
        <History className="mr-2 h-4 w-4" />
        Voir l'historique
      </Button>
    </div>
  );
}
