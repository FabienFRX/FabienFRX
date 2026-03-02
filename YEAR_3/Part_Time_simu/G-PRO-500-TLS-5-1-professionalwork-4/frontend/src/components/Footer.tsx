import { Instagram, Facebook, Send } from "lucide-react";
import { Button } from "@/components/ui/button";

export const Footer = () => {
  return (
    <footer className="w-full py-8 px-4 border-t border-border bg-background">
      <div className="max-w-7xl mx-auto">
        <div className="flex flex-col md:flex-row items-center justify-between gap-4">
          <div className="flex gap-6 text-sm text-muted-foreground">
            <a
              href="#"
              className="hover:text-primary transition-colors"
            >
              Conditions d'utilisation
            </a>
          </div>

          <div className="flex gap-3">
            <Button
              variant="ghost"
              size="icon"
              className="rounded-full hover:bg-primary hover:text-primary-foreground transition-colors"
              aria-label="Instagram"
            >
              <Instagram className="h-5 w-5" />
            </Button>
            <Button
              variant="ghost"
              size="icon"
              className="rounded-full hover:bg-primary hover:text-primary-foreground transition-colors"
              aria-label="Facebook"
            >
              <Facebook className="h-5 w-5" />
            </Button>
            <Button
              variant="ghost"
              size="icon"
              className="rounded-full hover:bg-primary hover:text-primary-foreground transition-colors"
              aria-label="Telegram"
            >
              <Send className="h-5 w-5" />
            </Button>
          </div>
        </div>

        <div className="mt-6 text-center text-sm text-muted-foreground">
          <p>© 2025 QuizLab - Apprendre en s'amusant 🎓</p>
        </div>
      </div>
    </footer>
  );
};
