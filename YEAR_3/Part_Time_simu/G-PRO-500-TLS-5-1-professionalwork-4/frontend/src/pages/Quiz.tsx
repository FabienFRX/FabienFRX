import { useEffect, useState } from "react";
import { Header } from "@/components/Header";
import { Footer } from "@/components/Footer";
import { Button } from "@/components/ui/button";
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card";
import { Gamepad2, Users, User, Dices, Globe, Palette, Cpu, History, Lightbulb, Loader2, Clock } from "lucide-react";
import { Badge } from "@/components/ui/badge";
import { Alert, AlertDescription } from "@/components/ui/alert";
import { MultiplayerGame } from "@/components/MultiplayerGame";
import { SoloGame } from "@/components/SoloGame";
import { Socket } from "socket.io-client";
import { useAuth } from "@/hooks/useAuth";
import { useNavigate } from "react-router-dom";

const modes = [
  { id: "solo", name: "Solo", description: "Entraînement personnel", icon: User },
  { id: "multi", name: "Multijoueur", description: "Affronte d'autres joueurs", icon: Users },
];

const themes = [
  { id: "history", name: "Histoire", icon: History },
  { id: "science", name: "Sciences", icon: Lightbulb },
  { id: "geography", name: "Géographie", icon: Globe },
  { id: "art", name: "Art & Culture", icon: Palette },
  { id: "technology", name: "Technologie", icon: Cpu },
  { id: "random", name: "Aléatoire", icon: Dices },
];

const difficulties = [
  { id: "easy", name: "Facile", color: "bg-success text-success-foreground" },
  { id: "medium", name: "Moyen", color: "bg-warning text-warning-foreground" },
  { id: "hard", name: "Difficile", color: "bg-destructive text-destructive-foreground" },
];

interface Player {
  username: string;
  score: number;
}

interface MatchFoundPayload {
  roomId: string;
  players: Player[];
  theme: string;
  difficulty: string;
}

const Quiz = () => {
  const { user, loading } = useAuth();
  const navigate = useNavigate();
  const [selectedMode, setSelectedMode] = useState<string>("solo");
  const [selectedTheme, setSelectedTheme] = useState<string | null>(null);
  const [selectedDifficulty, setSelectedDifficulty] = useState<string>("medium");
  const [selectedTimeLimit, setSelectedTimeLimit] = useState<number>(15); // Nouveau: durée par question
  const [socket, setSocket] = useState<Socket | null>(null);
  const [isSearching, setIsSearching] = useState(false);
  const [gameStarted, setGameStarted] = useState(false);
  const [matchData, setMatchData] = useState<MatchFoundPayload | null>(null);
  const [soloGameStarted, setSoloGameStarted] = useState(false);

  // Cleanup socket on unmount
  useEffect(() => {
    return () => {
      if (socket) {
        socket.disconnect();
      }
    };
  }, [socket]);

  const startMultiplayerSearch = async () => {
    if (!selectedTheme) {
      alert("Veuillez choisir un thème");
      return;
    }

    if (!user?.username) {
      if (confirm("Vous devez être connecté pour jouer en multijoueur. Voulez-vous vous connecter maintenant ?")) {
        navigate('/auth');
      }
      return;
    }

    setIsSearching(true);

    try {
      const { io } = await import('socket.io-client');
      const newSocket = io('http://localhost:3000', {
        path: '/socket.io',
        transports: ['polling', 'websocket'],
        reconnection: true,
        reconnectionDelay: 1000,
        reconnectionAttempts: 10,
        timeout: 10000,
        forceNew: true,
      });
      
      newSocket.on('connect', () => {
        devLog('Connected to server', newSocket.id);
        // Démarrer la recherche
        newSocket.emit('find_match', {
          theme: selectedTheme,
          difficulty: selectedDifficulty,
          username: user?.username || 'Anonymous',
        });
      });

      newSocket.on('connect_error', (error) => {
        devError('Connection error:', error);
        setIsSearching(false);
        alert('Erreur de connexion au serveur. Vérifiez que le backend est démarré sur le port 3000.');
      });

      newSocket.on('matching', (payload: { status: string; queueSize: number }) => {
        devLog('Searching for match...', payload);
      });

      newSocket.on('match_found', (payload: MatchFoundPayload) => {
        devLog('Match found!', payload);
        setMatchData(payload);
        setIsSearching(false);
        setGameStarted(true);
      });

      newSocket.on('disconnect', () => {
        devLog('Disconnected from server');
      });

      setSocket(newSocket);
    } catch (err) {
      devError('Failed to connect:', err);
      setIsSearching(false);
      alert('Erreur de connexion au serveur');
    }
  };

  const cancelSearch = () => {
    if (socket) {
      socket.emit('leave_queue');
      socket.disconnect();
      setSocket(null);
    }
    setIsSearching(false);
  };

  const handleGameEnd = () => {
    if (socket) {
      socket.disconnect();
      setSocket(null);
    }
    setGameStarted(false);
    setMatchData(null);
    setIsSearching(false);
  };

  const startSoloQuiz = () => {
    if (!selectedTheme) {
      alert("Veuillez choisir un thème");
      return;
    }
    setSoloGameStarted(true);
  };

  const handleSoloGameEnd = (score: number, correctAnswers: number, totalQuestions: number) => {
    devLog(`Solo game ended: ${score} points, ${correctAnswers}/${totalQuestions} correct`);
    setSoloGameStarted(false);
  };

  // Si une partie solo est en cours
  if (soloGameStarted && selectedTheme) {
    return (
      <SoloGame
        theme={selectedTheme}
        difficulty={selectedDifficulty}
        timeLimit={selectedTimeLimit}
        onGameEnd={handleSoloGameEnd}
      />
    );
  }

  // Si une partie multijoueur est en cours, afficher le composant de jeu
  if (gameStarted && matchData && socket) {
    return (
      <MultiplayerGame
        socket={socket}
        roomId={matchData.roomId}
        players={matchData.players}
        timeLimit={selectedTimeLimit}
        onGameEnd={handleGameEnd}
      />
    );
  }

  // Si en recherche d'adversaire
  if (isSearching) {
    return (
      <div className="min-h-screen flex flex-col bg-background">
        <Header />
        <main className="flex-1 flex flex-col items-center justify-center px-4">
          <Card className="w-full max-w-md">
            <CardHeader>
              <CardTitle className="text-center">Recherche d'un adversaire...</CardTitle>
            </CardHeader>
            <CardContent className="space-y-6">
              <div className="flex justify-center">
                <Loader2 className="h-16 w-16 animate-spin text-primary" />
              </div>
              <div className="text-center space-y-2">
                <p className="text-muted-foreground">Thème : <span className="font-semibold">{themes.find(t => t.id === selectedTheme)?.name}</span></p>
                <p className="text-muted-foreground">Difficulté : <span className="font-semibold">{difficulties.find(d => d.id === selectedDifficulty)?.name}</span></p>
              </div>
              <Button
                onClick={cancelSearch}
                variant="outline"
                className="w-full"
              >
                Annuler
              </Button>
            </CardContent>
          </Card>
        </main>
        <Footer />
      </div>
    );
  }

  return (
    <div className="min-h-screen flex flex-col bg-background">
      <Header />
      
      <main className="flex-1 flex flex-col items-center px-4 py-12">
        <div className="w-full max-w-4xl space-y-8">
          {/* Title */}
          <div className="text-center animate-fade-in">
            <h1 className="text-3xl md:text-4xl font-bold text-foreground mb-2 flex items-center justify-center gap-2">
              <Gamepad2 className="h-8 w-8 text-primary" />
              Choisis ton mode de quiz
            </h1>
            <p className="text-muted-foreground">
              Teste tes connaissances et grimpe dans le classement
            </p>
          </div>

          {/* Mode Selection */}
          <Card className="animate-fade-in" style={{ animationDelay: "100ms" }}>
            <CardHeader>
              <CardTitle>Mode de jeu</CardTitle>
              <CardDescription>Joue seul ou défie d'autres joueurs</CardDescription>
            </CardHeader>
            <CardContent>
              <div className="grid md:grid-cols-2 gap-4">
                {modes.map((mode) => {
                  const Icon = mode.icon;
                  return (
                    <button
                      key={mode.id}
                      onClick={() => setSelectedMode(mode.id)}
                      className={`p-6 rounded-lg border-2 transition-all hover-lift ${
                        selectedMode === mode.id
                          ? "border-primary bg-primary/5"
                          : "border-border hover:border-primary/50"
                      }`}
                    >
                      <Icon className={`h-10 w-10 mx-auto mb-3 ${
                        selectedMode === mode.id ? "text-primary" : "text-muted-foreground"
                      }`} />
                      <p className={`font-semibold mb-1 ${
                        selectedMode === mode.id ? "text-primary" : "text-foreground"
                      }`}>
                        {mode.name}
                      </p>
                      <p className="text-sm text-muted-foreground">{mode.description}</p>
                    </button>
                  );
                })}
              </div>
            </CardContent>
          </Card>

          {/* Themes */}
          <Card className="animate-fade-in" style={{ animationDelay: "200ms" }}>
            <CardHeader>
              <CardTitle>Thème du quiz</CardTitle>
              <CardDescription>Sélectionne un domaine de connaissance</CardDescription>
            </CardHeader>
            <CardContent>
              <div className="grid grid-cols-2 md:grid-cols-3 gap-4">
                {themes.map((theme) => {
                  const Icon = theme.icon;
                  return (
                    <button
                      key={theme.id}
                      onClick={() => setSelectedTheme(theme.id)}
                      className={`p-4 rounded-lg border-2 transition-all hover-lift ${
                        selectedTheme === theme.id
                          ? "border-secondary bg-secondary/5"
                          : "border-border hover:border-secondary/50"
                      }`}
                    >
                      <Icon className={`h-8 w-8 mx-auto mb-2 ${
                        selectedTheme === theme.id ? "text-secondary" : "text-muted-foreground"
                      }`} />
                      <p className={`text-sm font-medium ${
                        selectedTheme === theme.id ? "text-secondary" : "text-foreground"
                      }`}>
                        {theme.name}
                      </p>
                    </button>
                  );
                })}
              </div>
            </CardContent>
          </Card>

          {/* Difficulty */}
          <Card className="animate-fade-in" style={{ animationDelay: "300ms" }}>
            <CardHeader>
              <CardTitle>Niveau de difficulté</CardTitle>
              <CardDescription>Adapte le challenge à ton niveau</CardDescription>
            </CardHeader>
            <CardContent>
              <div className="flex flex-wrap gap-3">
                {difficulties.map((diff) => (
                  <Badge
                    key={diff.id}
                    variant={selectedDifficulty === diff.id ? "default" : "outline"}
                    className={`cursor-pointer px-6 py-2 text-base transition-all hover:scale-105 ${
                      selectedDifficulty === diff.id ? diff.color : ""
                    }`}
                    onClick={() => setSelectedDifficulty(diff.id)}
                  >
                    {diff.name}
                  </Badge>
                ))}
              </div>
            </CardContent>
          </Card>

          {/* Time Limit */}
          <Card className="animate-fade-in" style={{ animationDelay: "350ms" }}>
            <CardHeader>
              <CardTitle className="flex items-center gap-2">
                <Clock className="h-5 w-5" />
                Durée par question
              </CardTitle>
              <CardDescription>Choisis le temps disponible pour répondre</CardDescription>
            </CardHeader>
            <CardContent>
              <div className="flex flex-wrap gap-3">
                {[10, 15, 20].map((time) => (
                  <Badge
                    key={time}
                    variant={selectedTimeLimit === time ? "default" : "outline"}
                    className={`cursor-pointer px-6 py-2 text-base transition-all hover:scale-105 ${
                      selectedTimeLimit === time ? "bg-blue-500 text-white hover:bg-blue-600" : ""
                    }`}
                    onClick={() => setSelectedTimeLimit(time)}
                  >
                    {time} secondes
                  </Badge>
                ))}
              </div>
            </CardContent>
          </Card>

          {/* Info Alert */}
          <Alert className="animate-fade-in border-accent/50 bg-accent/5" style={{ animationDelay: "400ms" }}>
            <AlertDescription className="text-foreground">
              <strong>Important :</strong> Les questions incluront parfois des pièges. 
              Les scores seront ajoutés à ton classement.
            </AlertDescription>
          </Alert>

          {/* Message si non connecté en mode multijoueur */}
          {selectedMode === 'multi' && !loading && !user && (
            <Alert className="animate-fade-in border-yellow-500/50 bg-yellow-500/5" style={{ animationDelay: "350ms" }}>
              <AlertDescription className="text-foreground">
                <strong>Connexion requise :</strong> Vous devez être connecté pour jouer en mode multijoueur.
                Cliquez sur "Connexion" en haut à droite.
              </AlertDescription>
            </Alert>
          )}

          {/* Indicateur de chargement */}
          {selectedMode === 'multi' && loading && (
            <Card className="animate-fade-in" style={{ animationDelay: "350ms" }}>
              <CardContent className="p-4">
                <div className="flex items-center gap-2">
                  <Loader2 className="h-4 w-4 animate-spin" />
                  <span className="text-muted-foreground">Vérification de votre connexion...</span>
                </div>
              </CardContent>
            </Card>
          )}

          {/* Afficher le pseudo qui sera utilisé */}
          {selectedMode === 'multi' && user && (
            <Card className="animate-fade-in" style={{ animationDelay: "350ms" }}>
              <CardContent className="p-4">
                <div className="flex items-center justify-between">
                  <span className="text-muted-foreground">Pseudo utilisé :</span>
                  <Badge variant="outline" className="text-base py-1 px-3">
                    {user.username}
                  </Badge>
                </div>
              </CardContent>
            </Card>
          )}

          {/* CTA */}
          <div className="flex justify-center animate-fade-in" style={{ animationDelay: "500ms" }}>
            <Button
              size="lg"
              className="gradient-primary text-white px-12 py-6 text-lg hover-glow"
              disabled={!selectedTheme || (selectedMode === 'multi' && !user)}
              onClick={selectedMode === 'multi' ? startMultiplayerSearch : startSoloQuiz}
            >
              {selectedMode === 'multi' ? 'Rechercher un adversaire' : 'Lancer le quiz'}
            </Button>
          </div>
        </div>
      </main>

      <Footer />
    </div>
  );
};

export default Quiz;
