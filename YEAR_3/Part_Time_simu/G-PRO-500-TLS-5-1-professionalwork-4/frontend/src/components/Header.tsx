import { Users, User, LogOut, LogIn, Trophy } from "lucide-react";
import { Button } from "@/components/ui/button";
import { Avatar, AvatarImage, AvatarFallback } from "@/components/ui/avatar";
import { useNavigate } from "react-router-dom";
import { useAuth } from "@/hooks/useAuth";
import { getRolesFromToken } from "@/services/users.service";
import { useEffect, useState } from "react";
import { usersService, getUserIdFromToken } from "@/services/users.service";
import {
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuItem,
  DropdownMenuSeparator,
  DropdownMenuTrigger,
} from "@/components/ui/dropdown-menu";

export const Header = () => {
  const navigate = useNavigate();
  const { isAuthenticated, logout } = useAuth();
  const [avatarUrl, setAvatarUrl] = useState<string>("");

  useEffect(() => {
    if (isAuthenticated) {
      const userId = getUserIdFromToken();
      if (userId) {
        usersService.getById(userId)
          .then(user => setAvatarUrl(user.avatarUrl || ""))
          .catch(() => {});
      }
    }
  }, [isAuthenticated]);

  const handleLogout = () => {
    logout();
    navigate('/');
  };

  const isAdmin = getRolesFromToken().includes('ADMIN');
  return (
    <header className="w-full px-4 py-6 md:px-8 bg-background border-b border-border">
      <div className="max-w-7xl mx-auto flex items-center justify-between">
        <Button
          variant="ghost"
          size="icon"
          className="rounded-full hover:bg-muted transition-colors"
          aria-label="Amis"
          onClick={() => navigate('/friends')}
        >
          <Users className="h-6 w-6 text-primary" />
        </Button>

        <h1 
          className="text-2xl md:text-3xl font-bold text-primary cursor-pointer"
          onClick={() => navigate('/')}
        >
          QuizLab
        </h1>

        {isAuthenticated ? (
          <DropdownMenu>
            <DropdownMenuTrigger asChild>
              <Button
                variant="ghost"
                size="icon"
                className="rounded-full hover:bg-muted transition-colors p-0"
                aria-label="Profil"
              >
                {avatarUrl ? (
                  <Avatar className="h-10 w-10">
                    <AvatarImage src={avatarUrl} alt="Avatar" />
                    <AvatarFallback>
                      <User className="h-5 w-5 text-primary" />
                    </AvatarFallback>
                  </Avatar>
                ) : (
                  <User className="h-6 w-6 text-primary" />
                )}
              </Button>
            </DropdownMenuTrigger>
            <DropdownMenuContent align="end">
              <DropdownMenuItem onClick={() => navigate('/profile')}>
                <User className="mr-2 h-4 w-4" />
                Mon profil
              </DropdownMenuItem>
              <DropdownMenuItem onClick={() => navigate('/daily-challenge')}>
                <Trophy className="mr-2 h-4 w-4" />
                Défi quotidien
              </DropdownMenuItem>
              {isAdmin && (
                <DropdownMenuItem onClick={() => navigate('/admin')}>
                  <User className="mr-2 h-4 w-4" />
                  Admin Panel
                </DropdownMenuItem>
              )}
              <DropdownMenuSeparator />
              <DropdownMenuItem onClick={handleLogout}>
                <LogOut className="mr-2 h-4 w-4" />
                Se déconnecter
              </DropdownMenuItem>
            </DropdownMenuContent>
          </DropdownMenu>
        ) : (
          <Button
            variant="ghost"
            size="icon"
            className="rounded-full hover:bg-muted transition-colors"
            aria-label="Se connecter"
            onClick={() => navigate('/auth')}
          >
            <LogIn className="h-6 w-6 text-primary" />
          </Button>
        )}
      </div>
    </header>
  );
};
