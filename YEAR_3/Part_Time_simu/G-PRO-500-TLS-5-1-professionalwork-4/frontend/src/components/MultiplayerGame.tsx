import { useEffect, useState } from "react";
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { Progress } from "@/components/ui/progress";
import { Badge } from "@/components/ui/badge";
import { Trophy, Users, Clock, CheckCircle2, XCircle, Loader2 } from "lucide-react";
import { Socket } from "socket.io-client";
import { devLog } from "@/lib/api.config";

interface Player {
  username: string;
  score: number;
}

interface Question {
  questionNumber: number;
  totalQuestions: number;
  question: string;
  options: string[];
  timeLimit: number;
}

interface QuestionResult {
  correctAnswer: number;
  explanation: string;
  results: Array<{
    playerId: string;
    username: string;
    correct: boolean;
    score: number;
  }>;
  scores: Player[];
}

interface GameEndResult {
  results: Player[];
  winner: Player;
}

interface MultiplayerGameProps {
  socket: Socket;
  roomId: string;
  players: Player[];
  timeLimit: number; // Durée en secondes par question
  onGameEnd: () => void;
}

type GameState = 
  | { type: 'waiting' }
  | { type: 'question'; data: Question }
  | { type: 'results'; data: QuestionResult }
  | { type: 'ended'; data: GameEndResult };

export const MultiplayerGame = ({ socket, roomId, players: initialPlayers, timeLimit, onGameEnd }: MultiplayerGameProps) => {
  const [gameState, setGameState] = useState<GameState>({ type: 'waiting' });
  const [selectedAnswer, setSelectedAnswer] = useState<number | null>(null);
  const [timeLeft, setTimeLeft] = useState<number>(0);
  const [players, setPlayers] = useState<Player[]>(initialPlayers);
  const [hasAnswered, setHasAnswered] = useState(false);
  const [opponentAnswered, setOpponentAnswered] = useState(false);
  const [audioPlayed, setAudioPlayed] = useState(false); // Pour jouer le son qu'une fois

  useEffect(() => {
    // Écouter les événements du jeu
    socket.on('game_start', ({ totalQuestions }: { totalQuestions: number }) => {
      devLog('Game starting with', totalQuestions, 'questions');
    });

    socket.on('new_question', (question: Question) => {
      devLog('New question:', question);
      setGameState({ type: 'question', data: question });
      setSelectedAnswer(null);
      setHasAnswered(false);
      setOpponentAnswered(false);
      setAudioPlayed(false); // Réinitialiser pour la prochaine question
      setTimeLeft(question.timeLimit);
    });

    socket.on('player_answered', ({ playerId }: { playerId: string }) => {
      if (playerId !== socket.id) {
        setOpponentAnswered(true);
      }
    });

    socket.on('question_results', (results: QuestionResult) => {
      devLog('Question results:', results);
      setGameState({ type: 'results', data: results });
      setPlayers(results.scores);
    });

    socket.on('game_end', (endData: GameEndResult) => {
      devLog('Game ended:', endData);
      setGameState({ type: 'ended', data: endData });
    });

    socket.on('opponent_disconnected', ({ message }: { message: string }) => {
      alert(message);
    });

    return () => {
      socket.off('game_start');
      socket.off('new_question');
      socket.off('player_answered');
      socket.off('question_results');
      socket.off('game_end');
      socket.off('opponent_disconnected');
    };
  }, [socket]);

  // Timer countdown
  useEffect(() => {
    if (gameState.type === 'question' && timeLeft > 0 && !hasAnswered) {
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
    
    // Auto-submit si le temps est écoulé et qu'une réponse a été sélectionnée
    if (gameState.type === 'question' && timeLeft === 0 && !hasAnswered && selectedAnswer !== null) {
      setHasAnswered(true);
      socket.emit('submit_answer', { answerIndex: selectedAnswer });
    }
  }, [gameState, timeLeft, hasAnswered, selectedAnswer, socket, audioPlayed]);

  const handleAnswerSelect = (index: number) => {
    if (hasAnswered || gameState.type !== 'question') return;
    
    setSelectedAnswer(index);
    setHasAnswered(true);
    socket.emit('submit_answer', { answerIndex: index });
  };

  const renderScoreboard = () => (
    <Card className="mb-6">
      <CardHeader>
        <CardTitle className="flex items-center gap-2">
          <Users className="h-5 w-5" />
          Scores
        </CardTitle>
      </CardHeader>
      <CardContent>
        <div className="space-y-3">
          {players.map((player, index) => (
            <div key={index} className="flex items-center justify-between p-3 bg-secondary/20 rounded-lg">
              <div className="flex items-center gap-2">
                <Badge variant={index === 0 ? "default" : "outline"}>
                  {index + 1}
                </Badge>
                <span className="font-medium">{player.username}</span>
              </div>
              <div className="flex items-center gap-2">
                <Trophy className="h-4 w-4 text-yellow-500" />
                <span className="font-bold">{player.score} pts</span>
              </div>
            </div>
          ))}
        </div>
      </CardContent>
    </Card>
  );

  if (gameState.type === 'waiting') {
    return (
      <div className="min-h-screen flex flex-col items-center justify-center p-4">
        <Card className="w-full max-w-md">
          <CardHeader>
            <CardTitle className="text-center">Partie trouvée !</CardTitle>
          </CardHeader>
          <CardContent className="space-y-4">
            <div className="flex justify-center">
              <Loader2 className="h-12 w-12 animate-spin text-primary" />
            </div>
            <p className="text-center text-muted-foreground">
              La partie commence dans quelques instants...
            </p>
            {renderScoreboard()}
          </CardContent>
        </Card>
      </div>
    );
  }

  if (gameState.type === 'question') {
    const { data: question } = gameState;
    const progress = (timeLeft / question.timeLimit) * 100;
    const isUrgent = timeLeft <= 5 && timeLeft > 0; // Mode urgence : 5 secondes ou moins
    const isExpired = timeLeft === 0;

    return (
      <div className="min-h-screen flex flex-col p-4">
        {renderScoreboard()}

        <Card className={`w-full max-w-2xl mx-auto transition-all ${
          isUrgent ? 'ring-4 ring-red-500' : ''
        } ${isExpired ? 'opacity-75' : ''}`}>
          <CardHeader>
            <div className="flex items-center justify-between mb-4">
              <Badge variant="outline">
                Question {question.questionNumber} / {question.totalQuestions}
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
            <CardTitle className="text-xl">{question.question}</CardTitle>
          </CardHeader>
          <CardContent>
            <div className="grid gap-3">
              {question.options.map((option, index) => (
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
                {!opponentAnswered && (
                  <p className="text-sm text-muted-foreground mt-2">
                    En attente de l'adversaire...
                  </p>
                )}
              </div>
            )}

            {isExpired && !hasAnswered && selectedAnswer === null && (
              <div className="mt-6 text-center">
                <Badge variant="destructive" className="text-base py-2 px-4">
                  ⏰ Temps écoulé - Aucune réponse sélectionnée
                </Badge>
                <p className="text-sm text-muted-foreground mt-2">
                  En attente de l'adversaire...
                </p>
              </div>
            )}
          </CardContent>
        </Card>
      </div>
    );
  }

  if (gameState.type === 'results') {
    const { data: results } = gameState;

    return (
      <div className="min-h-screen flex flex-col p-4">
        {renderScoreboard()}

        <Card className="w-full max-w-2xl mx-auto">
          <CardHeader>
            <CardTitle className="text-center text-2xl">Résultats de la question</CardTitle>
          </CardHeader>
          <CardContent className="space-y-6">
            <div className="p-4 bg-primary/10 rounded-lg">
              <p className="font-semibold mb-2">Bonne réponse :</p>
              <p className="text-lg">
                <span className="font-bold text-primary">
                  {String.fromCharCode(65 + results.correctAnswer)}.
                </span>{" "}
                {results.explanation}
              </p>
            </div>

            <div className="space-y-3">
              {results.results.map((result, index) => (
                <div
                  key={index}
                  className={`flex items-center justify-between p-4 rounded-lg border-2 ${
                    result.correct
                      ? 'border-green-500 bg-green-500/10'
                      : 'border-red-500 bg-red-500/10'
                  }`}
                >
                  <div className="flex items-center gap-3">
                    {result.correct ? (
                      <CheckCircle2 className="h-6 w-6 text-green-500" />
                    ) : (
                      <XCircle className="h-6 w-6 text-red-500" />
                    )}
                    <span className="font-medium">{result.username}</span>
                  </div>
                  <Badge variant={result.correct ? "default" : "secondary"}>
                    {result.score} pts
                  </Badge>
                </div>
              ))}
            </div>

            <p className="text-center text-sm text-muted-foreground">
              Prochaine question dans quelques instants...
            </p>
          </CardContent>
        </Card>
      </div>
    );
  }

  if (gameState.type === 'ended') {
    const { data: endData } = gameState;
    const myResult = endData.results.find(r => 
      players.some(p => p.username === r.username)
    );
    const isWinner = myResult?.username === endData.winner.username;

    return (
      <div className="min-h-screen flex flex-col items-center justify-center p-4">
        <Card className="w-full max-w-2xl">
          <CardHeader>
            <CardTitle className="text-center text-3xl">
              {isWinner ? '🎉 Victoire !' : 'Partie terminée'}
            </CardTitle>
          </CardHeader>
          <CardContent className="space-y-6">
            <div className="text-center">
              <Trophy className="h-16 w-16 mx-auto mb-4 text-yellow-500" />
              <p className="text-2xl font-bold mb-2">{endData.winner.username}</p>
              <p className="text-xl text-muted-foreground">
                {endData.winner.score} points
              </p>
            </div>

            <div className="space-y-3">
              <p className="font-semibold text-center">Classement final</p>
              {endData.results.map((player, index) => (
                <div
                  key={index}
                  className={`flex items-center justify-between p-4 rounded-lg ${
                    index === 0
                      ? 'bg-yellow-500/20 border-2 border-yellow-500'
                      : 'bg-secondary/20'
                  }`}
                >
                  <div className="flex items-center gap-3">
                    <Badge variant={index === 0 ? "default" : "outline"} className="text-lg">
                      {index + 1}
                    </Badge>
                    <span className="font-medium text-lg">{player.username}</span>
                  </div>
                  <span className="font-bold text-xl">{player.score} pts</span>
                </div>
              ))}
            </div>

            <Button
              onClick={onGameEnd}
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
