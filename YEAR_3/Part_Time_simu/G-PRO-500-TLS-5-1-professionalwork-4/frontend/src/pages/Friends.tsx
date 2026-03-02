import { Header } from "@/components/Header";
import { Footer } from "@/components/Footer";
import { Card } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import { Avatar, AvatarFallback, AvatarImage } from "@/components/ui/avatar";
import { Badge } from "@/components/ui/badge";
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs";
import { 
  Dialog,
  DialogContent,
  DialogDescription,
  DialogHeader,
  DialogTitle,
  DialogTrigger,
} from "@/components/ui/dialog";
import { Label } from "@/components/ui/label";
import { Search, UserPlus, MessageCircle, Gamepad2, UserMinus } from "lucide-react";
import { useNavigate } from "react-router-dom";
import { messagesService } from "@/services/messages.service";
import { usersService } from "@/services/users.service";
import { useAuth } from "@/hooks/useAuth";
import { useState, useEffect } from "react";

const Friends = () => {
  const navigate = useNavigate();
  const { user } = useAuth();
  const [allUsers, setAllUsers] = useState<any[]>([]);
  const [loading, setLoading] = useState(true);
  
  useEffect(() => {
    const loadUsers = async () => {
      try {
        const users = await usersService.list();
        // Filtrer pour exclure l'utilisateur actuel
        const otherUsers = users.filter(u => u.id !== user?.id);
        setAllUsers(otherUsers);
      } catch (error) {
        console.error('Failed to load users:', error);
      } finally {
        setLoading(false);
      }
    };
    
    if (user) {
      loadUsers();
    }
  }, [user]);

  // Pour l'instant, on affiche tous les utilisateurs comme suggestions
  // Plus tard, vous pourrez implémenter un vrai système d'amis
  const friends = allUsers.slice(0, 3);
  const pendingRequests: any[] = [];
  const suggestions = allUsers.slice(3);

  const handleOpenChat = async (friendUserId: string) => {
    if (!user) {
      navigate('/auth');
      return;
    }

    try {
      // Créer ou récupérer la conversation
      await messagesService.getOrCreateConversation(friendUserId);
      // Rediriger vers la page messages
      navigate('/messages');
    } catch (error) {
      console.error('Failed to open chat:', error);
    }
  };

  return (
    <div className="min-h-screen flex flex-col bg-background">
      <Header />
      
      <main className="flex-1 px-4 py-8">
        <div className="max-w-4xl mx-auto space-y-6">
          {/* Header Section */}
          <div className="flex flex-col md:flex-row gap-4 items-center justify-between">
            <h1 className="text-3xl font-bold text-foreground">Gestion des amis</h1>
            
            <Dialog>
              <DialogTrigger asChild>
                <Button className="bg-primary text-primary-foreground hover:bg-primary/90">
                  <UserPlus className="h-4 w-4 mr-2" />
                  Ajouter un ami
                </Button>
              </DialogTrigger>
              <DialogContent>
                <DialogHeader>
                  <DialogTitle>Ajouter un ami</DialogTitle>
                  <DialogDescription>
                    Recherchez par pseudo ou code d'ami
                  </DialogDescription>
                </DialogHeader>
                <div className="space-y-4 pt-4">
                  <div className="space-y-2">
                    <Label htmlFor="search-friend">Pseudo ou code</Label>
                    <Input
                      id="search-friend"
                      placeholder="Entrez un pseudo ou code d'ami..."
                    />
                  </div>
                  <Button className="w-full bg-primary text-primary-foreground hover:bg-primary/90">
                    Envoyer une demande
                  </Button>
                </div>
              </DialogContent>
            </Dialog>
          </div>

          {/* Search Bar */}
          <Card className="p-4">
            <div className="relative">
              <Search className="absolute left-3 top-1/2 transform -translate-y-1/2 h-4 w-4 text-muted-foreground" />
              <Input
                placeholder="Rechercher un ami..."
                className="pl-10"
              />
            </div>
          </Card>

          {/* Tabs */}
          <Tabs defaultValue="friends" className="space-y-6">
            <TabsList className="grid w-full grid-cols-3">
              <TabsTrigger value="friends">Amis ({friends.length})</TabsTrigger>
              <TabsTrigger value="requests">Demandes ({pendingRequests.length})</TabsTrigger>
              <TabsTrigger value="sent">Envoyées</TabsTrigger>
            </TabsList>

            {/* Friends List */}
            <TabsContent value="friends" className="space-y-3">
              {loading ? (
                <Card className="p-8 text-center text-muted-foreground">Chargement...</Card>
              ) : friends.length === 0 ? (
                <Card className="p-8 text-center text-muted-foreground">Aucun ami pour le moment</Card>
              ) : (
                friends.map((friend) => (
                  <Card key={friend.id} className="p-4 hover:shadow-medium transition-shadow">
                    <div className="flex items-center gap-4">
                      <div className="relative">
                        <Avatar className="h-12 w-12">
                          <AvatarImage src={friend.avatarUrl || "/placeholder.svg"} />
                          <AvatarFallback className="bg-primary text-primary-foreground">
                            {friend.username.substring(0, 2).toUpperCase()}
                          </AvatarFallback>
                        </Avatar>
                      </div>
                      
                      <div className="flex-1">
                        <h3 className="font-semibold text-foreground">{friend.displayName || friend.username}</h3>
                        <p className="text-sm text-muted-foreground">@{friend.username}</p>
                      </div>

                      <div className="flex gap-2">
                        <Button size="sm" variant="outline" className="border-secondary text-secondary hover:bg-secondary hover:text-secondary-foreground">
                          <Gamepad2 className="h-4 w-4 md:mr-1" />
                          <span className="hidden md:inline">Inviter</span>
                        </Button>
                        <Button 
                          size="sm" 
                          variant="outline" 
                          className="hover:bg-muted"
                          onClick={() => handleOpenChat(friend.id)}
                        >
                          <MessageCircle className="h-4 w-4 md:mr-1 " />
                          <span className="hidden md:inline">Message</span>
                        </Button>
                        <Button size="sm" variant="ghost" className="text-destructive hover:text-destructive hover:bg-destructive/10">
                          <UserMinus className="h-4 w-4" />
                        </Button>
                      </div>
                    </div>
                  </Card>
                ))
              )}
            </TabsContent>

            {/* Pending Requests */}
            <TabsContent value="requests" className="space-y-3">
              {pendingRequests.map((request) => (
                <Card key={request.id} className="p-4 hover:shadow-medium transition-shadow">
                  <div className="flex items-center gap-4">
                    <Avatar className="h-12 w-12">
                      <AvatarImage src="/placeholder.svg" />
                      <AvatarFallback className="bg-primary text-primary-foreground">
                        {request.name.split(' ').map(n => n[0]).join('')}
                      </AvatarFallback>
                    </Avatar>
                    
                    <div className="flex-1">
                      <h3 className="font-semibold text-foreground">{request.name}</h3>
                      <p className="text-sm text-muted-foreground">@{request.username}</p>
                      <div className="flex gap-2 mt-1">
                        <Badge variant="outline" className="text-xs">#{request.rank}</Badge>
                        <Badge variant="outline" className="text-xs">{request.score} pts</Badge>
                      </div>
                    </div>

                    <div className="flex gap-2">
                      <Button size="sm" className="bg-success text-success-foreground hover:bg-success/90">
                        Accepter
                      </Button>
                      <Button size="sm" variant="outline" className="text-destructive border-destructive hover:bg-destructive hover:text-destructive-foreground">
                        Refuser
                      </Button>
                    </div>
                  </div>
                </Card>
              ))}
            </TabsContent>

            {/* Sent Requests */}
            <TabsContent value="sent" className="space-y-3">
              <Card className="p-8 text-center">
                <p className="text-muted-foreground">Aucune demande en attente</p>
              </Card>
            </TabsContent>
          </Tabs>

          {/* Suggestions */}
          <div className="space-y-4">
            <h2 className="text-xl font-bold text-foreground">Suggestions d'amis</h2>
            <p className="text-sm text-muted-foreground">Basé sur vos centres d'intérêt</p>
            
            <div className="space-y-3">
              {loading ? (
                <Card className="p-8 text-center text-muted-foreground">Chargement...</Card>
              ) : suggestions.length === 0 ? (
                <Card className="p-8 text-center text-muted-foreground">Aucune suggestion pour le moment</Card>
              ) : (
                suggestions.map((suggestion) => (
                  <Card key={suggestion.id} className="p-4 hover:shadow-medium transition-shadow">
                    <div className="flex items-center gap-4">
                      <Avatar className="h-12 w-12">
                        <AvatarImage src={suggestion.avatarUrl || "/placeholder.svg"} />
                        <AvatarFallback className="bg-secondary text-secondary-foreground">
                          {suggestion.username.substring(0, 2).toUpperCase()}
                        </AvatarFallback>
                      </Avatar>
                      
                      <div className="flex-1">
                        <h3 className="font-semibold text-foreground">{suggestion.displayName || suggestion.username}</h3>
                        <p className="text-sm text-muted-foreground">@{suggestion.username}</p>
                      </div>

                      <Button 
                        size="sm" 
                        className="bg-primary text-primary-foreground hover:bg-primary/90"
                        onClick={() => handleOpenChat(suggestion.id)}
                      >
                        <MessageCircle className="h-4 w-4 md:mr-1" />
                        <span className="hidden md:inline">Message</span>
                      </Button>
                    </div>
                  </Card>
                ))
              )}
            </div>
          </div>
        </div>
      </main>

      <Footer />
    </div>
  );
};

export default Friends;
