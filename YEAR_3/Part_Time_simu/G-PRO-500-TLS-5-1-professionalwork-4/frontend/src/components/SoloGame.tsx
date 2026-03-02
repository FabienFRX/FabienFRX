import { useEffect, useState } from "react";
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { Progress } from "@/components/ui/progress";
import { Badge } from "@/components/ui/badge";
import { Trophy, Clock, CheckCircle2, XCircle, Brain } from "lucide-react";
import { devError, devLog } from "@/lib/api.config";

interface Question {
  id: number;
  question: string;
  options: string[];
  correctAnswer: number;
  explanation: string;
  theme: string;
  difficulty: string;
}

interface SoloGameProps {
  theme: string;
  difficulty: string;
  timeLimit: number; // Durée en secondes par question
  onGameEnd: (score: number, correctAnswers: number, totalQuestions: number) => void;
}

type GameState = 
  | { type: 'question' }
  | { type: 'results' }
  | { type: 'ended' };

export const SoloGame = ({ theme, difficulty, timeLimit, onGameEnd }: SoloGameProps) => {
  const [gameState, setGameState] = useState<GameState>({ type: 'question' });
  const [questions, setQuestions] = useState<Question[]>([]);
  const [currentQuestionIndex, setCurrentQuestionIndex] = useState(0);
  const [selectedAnswer, setSelectedAnswer] = useState<number | null>(null);
  const [timeLeft, setTimeLeft] = useState<number>(timeLimit);
  const [score, setScore] = useState(0);
  const [correctAnswers, setCorrectAnswers] = useState(0);
  const [hasAnswered, setHasAnswered] = useState(false);
  const [audioPlayed, setAudioPlayed] = useState(false); // Pour jouer le son qu'une fois
  const [answersHistory, setAnswersHistory] = useState<Array<{
    questionId: number;
    correct: boolean;
    selectedAnswer: number;
  }>>([]);

  // Charger les questions au démarrage
  useEffect(() => {
    const loadQuestions = async () => {
      try {
        const API_URL = import.meta.env.VITE_API_URL || 'http://localhost:3000/api';
        const response = await fetch(`${API_URL}/quiz/solo?theme=${theme}&difficulty=${difficulty}`);
        if (!response.ok) {
          throw new Error('Failed to load questions');
        }
        const data = await response.json();
        setQuestions(data.questions);
      } catch (error) {
        devError('Error loading questions:', error);
        // Fallback: utiliser des questions en dur si l'API échoue
        setQuestions(getFallbackQuestions(theme, difficulty));
      }
    };

    loadQuestions();
  }, [theme, difficulty]);

  // Timer countdown
  useEffect(() => {
    if (gameState.type === 'question' && timeLeft > 0 && !hasAnswered && questions.length > 0) {
      // Jouer le son à 5 secondes
      if (timeLeft === 5 && !audioPlayed) {
        const audio = new Audio('/timer_quizlab.mp3');
        audio.play().catch(err => devLog('Audio play failed:', err));
        setAudioPlayed(true);
      }
      
      const timer = setTimeout(() => {
        setTimeLeft(timeLeft - 1);
      }, 1000);
      return () => clearTimeout(timer);
    }

    // Auto-submit si le temps est écoulé
    if (timeLeft === 0 && !hasAnswered && gameState.type === 'question') {
      handleAnswerSelect(null);
    }
  }, [gameState, timeLeft, hasAnswered, questions, audioPlayed]);

  const handleAnswerSelect = (answerIndex: number | null) => {
    if (hasAnswered) return;

    const currentQuestion = questions[currentQuestionIndex];
    const isCorrect = answerIndex === currentQuestion.correctAnswer;

    setSelectedAnswer(answerIndex);
    setHasAnswered(true);

    if (isCorrect) {
      setScore(score + 100);
      setCorrectAnswers(correctAnswers + 1);
    }

    setAnswersHistory([...answersHistory, {
      questionId: currentQuestion.id,
      correct: isCorrect,
      selectedAnswer: answerIndex ?? -1,
    }]);

    // Passer aux résultats
    setGameState({ type: 'results' });

    // Passer à la question suivante après 5 secondes
    setTimeout(() => {
      if (currentQuestionIndex < questions.length - 1) {
        setCurrentQuestionIndex(currentQuestionIndex + 1);
        setSelectedAnswer(null);
        setHasAnswered(false);
        setAudioPlayed(false); // Réinitialiser pour la prochaine question
        setTimeLeft(timeLimit); // Utiliser le timeLimit choisi
        setGameState({ type: 'question' });
      } else {
        // Fin du jeu
        setGameState({ type: 'ended' });
      }
    }, 5000);
  };

  const getFallbackQuestions = (theme: string, difficulty: string): Question[] => {
    // Questions de secours en cas d'échec de l'API
    const allQuestions: Question[] = [
      {
        id: 1,
        question: "Quelle est la capitale de la France ?",
        options: ["Paris", "Londres", "Berlin", "Madrid"],
        correctAnswer: 0,
        explanation: "Paris est la capitale de la France depuis le 12ème siècle.",
        theme: "geography",
        difficulty: "easy"
      },
      {
        id: 2,
        question: "En quelle année a eu lieu la Révolution française ?",
        options: ["1789", "1799", "1804", "1815"],
        correctAnswer: 0,
        explanation: "La Révolution française a commencé en 1789 avec la prise de la Bastille.",
        theme: "history",
        difficulty: "medium"
      },
      {
        id: 3,
        question: "Qui a peint la Joconde ?",
        options: ["Léonard de Vinci", "Michel-Ange", "Raphaël", "Donatello"],
        correctAnswer: 0,
        explanation: "Léonard de Vinci a peint la Joconde entre 1503 et 1519.",
        theme: "art",
        difficulty: "easy"
      },
      {
        id: 4,
        question: "Quel est le symbole chimique de l'or ?",
        options: ["Au", "Ag", "Fe", "Or"],
        correctAnswer: 0,
        explanation: "Au vient du latin 'aurum' qui signifie or.",
        theme: "science",
        difficulty: "medium"
      },
      {
        id: 5,
        question: "Quel langage de programmation est principalement utilisé pour le développement web frontend ?",
        options: ["JavaScript", "Python", "Java", "C++"],
        correctAnswer: 0,
        explanation: "JavaScript est le langage principal pour l'interactivité web côté client.",
        theme: "technology",
        difficulty: "easy"
      }
    ];

    let filtered = allQuestions;
    if (theme !== 'random') {
      filtered = filtered.filter(q => q.theme === theme);
    }
    if (difficulty !== 'all') {
      filtered = filtered.filter(q => q.difficulty === difficulty);
    }

    return filtered.length > 0 ? filtered.slice(0, 5) : allQuestions.slice(0, 5);
  };

  if (questions.length === 0) {
    return (
      <div className="min-h-screen flex items-center justify-center">
        <Card className="w-full max-w-md">
          <CardContent className="p-6">
            <p className="text-center">Chargement des questions...</p>
          </CardContent>
        </Card>
      </div>
    );
  }

  const currentQuestion = questions[currentQuestionIndex];
  const progress = (timeLeft / timeLimit) * 100; // Utiliser timeLimit dynamique
  const isUrgent = timeLeft <= 5 && timeLeft > 0;
  const isExpired = timeLeft === 0;

  if (gameState.type === 'question') {
    return (
      <div className="min-h-screen flex flex-col p-4 bg-background">
        {/* Header Score */}
        <Card className="w-full max-w-2xl mx-auto mb-6">
          <CardContent className="p-4">
            <div className="flex items-center justify-between">
              <div className="flex items-center gap-2">
                <Brain className="h-5 w-5 text-primary" />
                <span className="font-semibold">Mode Solo</span>
              </div>
              <div className="flex items-center gap-4">
                <div className="flex items-center gap-2">
                  <Trophy className="h-4 w-4 text-yellow-500" />
                  <span className="font-bold">{score} pts</span>
                </div>
                <Badge variant="outline">
                  {correctAnswers}/{currentQuestionIndex} bonnes réponses
                </Badge>
              </div>
            </div>
          </CardContent>
        </Card>

        {/* Question Card */}
        <Card className={`w-full max-w-2xl mx-auto transition-all ${
          isUrgent ? 'ring-4 ring-red-500' : ''
        } ${isExpired ? 'opacity-75' : ''}`}>
          <CardHeader>
            <div className="flex items-center justify-between mb-4">
              <Badge variant="outline">
                Question {currentQuestionIndex + 1} / {questions.length}
              </Badge>
              <div className={`flex items-center gap-2 ${
                isUrgent ? 'text-red-500 animate-pulse' : isExpired ? 'text-muted-foreground' : ''
              }`}>
                <Clock className={`h-5 w-5 ${isUrgent ? 'animate-bounce' : ''}`} />
                <span className="font-bold text-xl">{timeLeft}s</span>
              </div>
            </div>
            <Progress 
              value={progress} 
              className={`mb-4 transition-all ${
                isUrgent ? 'bg-red-100' : ''
              }`}
            />
            {isUrgent && !hasAnswered && (
              <div className="mb-3 p-2 bg-red-500/10 border-2 border-red-500 rounded-lg text-center animate-pulse">
                <span className="text-red-600 font-bold text-sm">⚠️ ATTENTION : Plus que {timeLeft} seconde{timeLeft > 1 ? 's' : ''} !</span>
              </div>
            )}
            {isExpired && !hasAnswered && (
              <div className="mb-3 p-2 bg-gray-500/10 border-2 border-gray-500 rounded-lg text-center">
                <span className="text-gray-600 font-bold text-sm">⏰ Temps écoulé</span>
              </div>
            )}
            <CardTitle className="text-xl">{currentQuestion.question}</CardTitle>
          </CardHeader>
          <CardContent>
            <div className="grid gap-3">
              {currentQuestion.options.map((option, index) => (
                <Button
                  key={index}
                  variant={selectedAnswer === index ? "default" : "outline"}
                  className={`h-auto py-4 px-6 text-left justify-start text-base transition-all ${
                    selectedAnswer === index && isUrgent ? 'ring-2 ring-red-500' : ''
                  }`}
                  onClick={() => handleAnswerSelect(index)}
                  disabled={hasAnswered || isExpired}
                >
                  <span className="font-semibold mr-3">{String.fromCharCode(65 + index)}.</span>
                  {option}
                </Button>
              ))}
            </div>

            {hasAnswered && (
              <div className="mt-6 text-center">
                <Badge variant="secondary" className="text-base py-2 px-4">
                  ✓ Réponse enregistrée
                </Badge>
              </div>
            )}
          </CardContent>
        </Card>
      </div>
    );
  }

  if (gameState.type === 'results') {
    const isCorrect = selectedAnswer === currentQuestion.correctAnswer;

    return (
      <div className="min-h-screen flex flex-col p-4 bg-background">
        {/* Header Score */}
        <Card className="w-full max-w-2xl mx-auto mb-6">
          <CardContent className="p-4">
            <div className="flex items-center justify-between">
              <div className="flex items-center gap-2">
                <Brain className="h-5 w-5 text-primary" />
                <span className="font-semibold">Mode Solo</span>
              </div>
              <div className="flex items-center gap-4">
                <div className="flex items-center gap-2">
                  <Trophy className="h-4 w-4 text-yellow-500" />
                  <span className="font-bold">{score} pts</span>
                </div>
                <Badge variant="outline">
                  {correctAnswers}/{currentQuestionIndex + 1} bonnes réponses
                </Badge>
              </div>
            </div>
          </CardContent>
        </Card>

        {/* Results Card */}
        <Card className="w-full max-w-2xl mx-auto">
          <CardHeader>
            <CardTitle className="text-center text-2xl">
              {isCorrect ? "✅ Bonne réponse !" : "❌ Mauvaise réponse"}
            </CardTitle>
          </CardHeader>
          <CardContent className="space-y-6">
            <div
              className={`flex items-center gap-3 p-4 rounded-lg border-2 ${
                isCorrect
                  ? 'border-green-500 bg-green-500/10'
                  : 'border-red-500 bg-red-500/10'
              }`}
            >
              {isCorrect ? (
                <CheckCircle2 className="h-6 w-6 text-green-500" />
              ) : (
                <XCircle className="h-6 w-6 text-red-500" />
              )}
              <span className="font-medium">
                {isCorrect ? '+100 points' : 'Aucun point'}
              </span>
            </div>

            <div className="p-4 bg-primary/10 rounded-lg">
              <p className="font-semibold mb-2">Bonne réponse :</p>
              <p className="text-lg mb-3">
                <span className="font-bold text-primary">
                  {String.fromCharCode(65 + currentQuestion.correctAnswer)}.
                </span>{" "}
                {currentQuestion.options[currentQuestion.correctAnswer]}
              </p>
              <p className="text-sm text-muted-foreground">
                {currentQuestion.explanation}
              </p>
            </div>

            <p className="text-center text-sm text-muted-foreground">
              {currentQuestionIndex < questions.length - 1
                ? 'Prochaine question dans quelques instants...'
                : 'Calcul des résultats finaux...'}
            </p>
          </CardContent>
        </Card>
      </div>
    );
  }

  if (gameState.type === 'ended') {
    const percentage = Math.round((correctAnswers / questions.length) * 100);
    let message = "Bon travail !";
    let emoji = "👍";

    if (percentage === 100) {
      message = "Parfait ! 100% de bonnes réponses !";
      emoji = "🎉";
    } else if (percentage >= 80) {
      message = "Excellent travail !";
      emoji = "🌟";
    } else if (percentage >= 60) {
      message = "Bien joué !";
      emoji = "👏";
    } else if (percentage >= 40) {
      message = "Pas mal, continue à t'entraîner !";
      emoji = "💪";
    } else {
      message = "Continue à t'entraîner !";
      emoji = "📚";
    }

    return (
      <div className="min-h-screen flex flex-col items-center justify-center p-4 bg-background">
        <Card className="w-full max-w-2xl">
          <CardHeader>
            <CardTitle className="text-center text-3xl">
              {emoji} Quiz terminé !
            </CardTitle>
          </CardHeader>
          <CardContent className="space-y-6">
            <div className="text-center">
              <Trophy className="h-16 w-16 mx-auto mb-4 text-yellow-500" />
              <p className="text-4xl font-bold mb-2">{score} points</p>
              <p className="text-xl text-muted-foreground mb-4">{message}</p>
              <div className="flex items-center justify-center gap-4">
                <Badge variant="outline" className="text-lg py-2 px-4">
                  {correctAnswers} / {questions.length} bonnes réponses
                </Badge>
                <Badge variant="outline" className="text-lg py-2 px-4">
                  {percentage}%
                </Badge>
              </div>
            </div>

            <div className="space-y-3">
              <p className="font-semibold text-center">Détails par question</p>
              {questions.map((question, index) => {
                const answer = answersHistory[index];
                return (
                  <div
                    key={question.id}
                    className={`flex items-center justify-between p-3 rounded-lg border ${
                      answer.correct
                        ? 'border-green-500/30 bg-green-500/5'
                        : 'border-red-500/30 bg-red-500/5'
                    }`}
                  >
                    <div className="flex items-center gap-3">
                      {answer.correct ? (
                        <CheckCircle2 className="h-5 w-5 text-green-500" />
                      ) : (
                        <XCircle className="h-5 w-5 text-red-500" />
                      )}
                      <span className="text-sm">Question {index + 1}</span>
                    </div>
                    <Badge variant={answer.correct ? "default" : "secondary"}>
                      {answer.correct ? '+100 pts' : '0 pts'}
                    </Badge>
                  </div>
                );
              })}
            </div>

            <Button
              onClick={() => onGameEnd(score, correctAnswers, questions.length)}
              className="w-full gradient-primary text-white"
              size="lg"
            >
              Retour au menu
            </Button>
          </CardContent>
        </Card>
      </div>
    );
  }

  return null;
};
