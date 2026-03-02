import { useState } from "react";
import { Header } from "@/components/Header";
import { Footer } from "@/components/Footer";
import { Button } from "@/components/ui/button";
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card";
import { BookOpen, Dices, Globe, Palette, Cpu, History, Lightbulb } from "lucide-react";
import { Badge } from "@/components/ui/badge";

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

const trapLevels = [
  { id: "gentle", name: "Doux", description: "~5% de réponses ambiguës" },
  { id: "moderate", name: "Modéré", description: "~15% de réponses ambiguës" },
  { id: "vicious", name: "Vicieux", description: "~25% de réponses ambiguës" },
];

const Learn = () => {
  const [selectedTheme, setSelectedTheme] = useState<string | null>(null);
  const [selectedDifficulty, setSelectedDifficulty] = useState<string>("medium");
  const [selectedTrapLevel, setSelectedTrapLevel] = useState<string>("moderate");

  return (
    <div className="min-h-screen flex flex-col bg-background">
      <Header />
      
      <main className="flex-1 flex flex-col items-center px-4 py-12">
        <div className="w-full max-w-4xl space-y-8">
          {/* Title */}
          <div className="text-center animate-fade-in">
            <h1 className="text-3xl md:text-4xl font-bold text-foreground mb-2 flex items-center justify-center gap-2">
              <BookOpen className="h-8 w-8 text-primary" />
              Choisis ton apprentissage
            </h1>
            <p className="text-muted-foreground">
              L'IA adaptera les questions selon ta tranche d'âge
            </p>
          </div>

          {/* Themes */}
          <Card className="animate-fade-in" style={{ animationDelay: "100ms" }}>
            <CardHeader>
              <CardTitle>Thème d'apprentissage</CardTitle>
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
                          ? "border-primary bg-primary/5"
                          : "border-border hover:border-primary/50"
                      }`}
                    >
                      <Icon className={`h-8 w-8 mx-auto mb-2 ${
                        selectedTheme === theme.id ? "text-primary" : "text-muted-foreground"
                      }`} />
                      <p className={`text-sm font-medium ${
                        selectedTheme === theme.id ? "text-primary" : "text-foreground"
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
          <Card className="animate-fade-in" style={{ animationDelay: "200ms" }}>
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

          {/* Trap Level */}
          <Card className="animate-fade-in" style={{ animationDelay: "300ms" }}>
            <CardHeader>
              <CardTitle>Niveau de pièges</CardTitle>
              <CardDescription>Choisis le pourcentage de réponses ambiguës</CardDescription>
            </CardHeader>
            <CardContent>
              <div className="space-y-3">
                {trapLevels.map((trap) => (
                  <button
                    key={trap.id}
                    onClick={() => setSelectedTrapLevel(trap.id)}
                    className={`w-full p-4 rounded-lg border-2 text-left transition-all hover-lift ${
                      selectedTrapLevel === trap.id
                        ? "border-secondary bg-secondary/5"
                        : "border-border hover:border-secondary/50"
                    }`}
                  >
                    <div className="flex items-center justify-between">
                      <div>
                        <p className={`font-medium ${
                          selectedTrapLevel === trap.id ? "text-secondary" : "text-foreground"
                        }`}>
                          {trap.name}
                        </p>
                        <p className="text-sm text-muted-foreground">{trap.description}</p>
                      </div>
                      {selectedTrapLevel === trap.id && (
                        <div className="h-5 w-5 rounded-full bg-secondary flex items-center justify-center">
                          <div className="h-2 w-2 rounded-full bg-white" />
                        </div>
                      )}
                    </div>
                  </button>
                ))}
              </div>
            </CardContent>
          </Card>

          {/* CTA */}
          <div className="flex justify-center animate-fade-in" style={{ animationDelay: "400ms" }}>
            <Button
              size="lg"
              className="gradient-primary text-white px-12 py-6 text-lg hover-glow"
              disabled={!selectedTheme}
            >
              Commencer l'apprentissage
            </Button>
          </div>
        </div>
      </main>

      <Footer />
    </div>
  );
};

export default Learn;
