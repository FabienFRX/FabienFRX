import { useState, useEffect, useRef } from 'react';
import { useAuth } from '../hooks/useAuth';
import { messagesService, type Conversation, type Message } from '../services/messages.service';
import { Header } from '../components/Header';
import { Footer } from '../components/Footer';
import { Card, CardContent } from '../components/ui/card';
import { Button } from '../components/ui/button';
import { Input } from '../components/ui/input';
import { Avatar, AvatarFallback, AvatarImage } from '../components/ui/avatar';
import { Badge } from '../components/ui/badge';
import { ScrollArea } from '../components/ui/scroll-area';
import { Send, Search, MoreVertical, Trash2, ArrowLeft } from 'lucide-react';
import {
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuItem,
  DropdownMenuTrigger,
} from '../components/ui/dropdown-menu';
import { useNavigate } from 'react-router-dom';

export default function Messages() {
  const { user } = useAuth();
  const navigate = useNavigate();
  
  const [conversations, setConversations] = useState<Conversation[]>([]);
  const [selectedConversation, setSelectedConversation] = useState<Conversation | null>(null);
  const [messages, setMessages] = useState<Message[]>([]);
  const [newMessage, setNewMessage] = useState('');
  const [searchQuery, setSearchQuery] = useState('');
  const [isTyping, setIsTyping] = useState(false);
  const [loading, setLoading] = useState(true);
  
  const messagesEndRef = useRef<HTMLDivElement>(null);
  const typingTimeoutRef = useRef<NodeJS.Timeout>();

  useEffect(() => {
    if (!user) {
      navigate('/auth');
      return;
    }

    loadConversations();
    
    // Connexion WebSocket
    const socket = messagesService.connectSocket(user.id);

    // Écouter les nouveaux messages
    socket.on('newMessage', (message: Message) => {
      if (selectedConversation && message.conversationId === selectedConversation.id) {
        setMessages((prev) => [...prev, message]);
        messagesService.markAsRead(selectedConversation.id);
      }
      loadConversations(); // Mettre à jour la liste des conversations
    });

    // Écouter l'indicateur de saisie
    socket.on('userTyping', (data: { conversationId: string }) => {
      if (selectedConversation && data.conversationId === selectedConversation.id) {
        setIsTyping(true);
      }
    });

    socket.on('userStoppedTyping', (data: { conversationId: string }) => {
      if (selectedConversation && data.conversationId === selectedConversation.id) {
        setIsTyping(false);
      }
    });

    return () => {
      messagesService.disconnectSocket();
    };
  }, [user, navigate]);

  useEffect(() => {
    scrollToBottom();
  }, [messages]);

  const loadConversations = async () => {
    try {
      const data = await messagesService.getConversations();
      setConversations(data);
    } catch (error) {
      console.error('Failed to load conversations:', error);
    } finally {
      setLoading(false);
    }
  };

  const loadMessages = async (conversation: Conversation) => {
    try {
      const data = await messagesService.getConversationMessages(conversation.id);
      setMessages(data);
      setSelectedConversation(conversation);
      await messagesService.markAsRead(conversation.id);
      loadConversations(); // Mettre à jour le compteur de non-lus
    } catch (error) {
      console.error('Failed to load messages:', error);
    }
  };

  const handleSendMessage = async () => {
    if (!newMessage.trim() || !selectedConversation || !user) return;

    const otherParticipant = selectedConversation.participants.find(
      (p) => p.userId !== user.id,
    );
    if (!otherParticipant) return;

    try {
      // Envoyer via WebSocket pour temps réel
      await messagesService.sendMessageWS(otherParticipant.userId, newMessage);
      
      // Ajouter le message localement
      const tempMessage: Message = {
        id: Date.now().toString(),
        conversationId: selectedConversation.id,
        senderId: user.id,
        content: newMessage,
        read: false,
        createdAt: new Date(),
        sender: {
          id: user.id,
          username: user.username,
          displayName: user.displayName,
          avatarUrl: user.avatarUrl || null,
        },
      };
      
      setMessages((prev) => [...prev, tempMessage]);
      setNewMessage('');
      
      // Arrêter l'indicateur de saisie
      messagesService.emitStopTyping(selectedConversation.id, otherParticipant.userId);
    } catch (error) {
      console.error('Failed to send message:', error);
    }
  };

  const handleTyping = () => {
    if (!selectedConversation || !user) return;

    const otherParticipant = selectedConversation.participants.find(
      (p) => p.userId !== user.id,
    );
    if (!otherParticipant) return;

    messagesService.emitTyping(selectedConversation.id, otherParticipant.userId);

    // Arrêter après 3 secondes d'inactivité
    if (typingTimeoutRef.current) {
      clearTimeout(typingTimeoutRef.current);
    }
    typingTimeoutRef.current = setTimeout(() => {
      messagesService.emitStopTyping(selectedConversation.id, otherParticipant.userId);
    }, 3000);
  };

  const handleDeleteConversation = async (conversationId: string) => {
    try {
      await messagesService.deleteConversation(conversationId);
      setConversations((prev) => prev.filter((c) => c.id !== conversationId));
      if (selectedConversation?.id === conversationId) {
        setSelectedConversation(null);
        setMessages([]);
      }
    } catch (error) {
      console.error('Failed to delete conversation:', error);
    }
  };

  const scrollToBottom = () => {
    messagesEndRef.current?.scrollIntoView({ behavior: 'smooth' });
  };

  const getOtherParticipant = (conversation: Conversation) => {
    return conversation.participants.find((p) => p.userId !== user?.id)?.user;
  };

  const filteredConversations = conversations.filter((conv) => {
    const other = getOtherParticipant(conv);
    if (!other) return false;
    const searchLower = searchQuery.toLowerCase();
    return (
      other.username.toLowerCase().includes(searchLower) ||
      other.displayName?.toLowerCase().includes(searchLower)
    );
  });

  const formatTime = (date: Date) => {
    const now = new Date();
    const messageDate = new Date(date);
    const diffInHours = (now.getTime() - messageDate.getTime()) / (1000 * 60 * 60);

    if (diffInHours < 24) {
      return messageDate.toLocaleTimeString('fr-FR', {
        hour: '2-digit',
        minute: '2-digit',
      });
    } else if (diffInHours < 48) {
      return 'Hier';
    } else {
      return messageDate.toLocaleDateString('fr-FR', {
        day: 'numeric',
        month: 'short',
      });
    }
  };

  if (loading) {
    return (
      <div className="min-h-screen flex flex-col bg-background">
        <Header />
        <div className="flex-1 flex items-center justify-center">
          <div className="animate-spin rounded-full h-12 w-12 border-b-2 border-primary"></div>
        </div>
        <Footer />
      </div>
    );
  }

  return (
    <div className="min-h-screen flex flex-col bg-background">
      <Header />
      
      <main className="flex-1 flex overflow-hidden">
        <div className="w-full max-w-7xl mx-auto flex h-[calc(100vh-140px)]">
          {/* Liste des conversations */}
          <div className={`${selectedConversation ? 'hidden md:flex' : 'flex'} w-full md:w-1/3 flex-col border-r border-border`}>
            <div className="p-4 border-b border-border">
              <h2 className="text-2xl font-bold mb-4">Messages</h2>
              <div className="relative">
                <Search className="absolute left-3 top-1/2 transform -translate-y-1/2 h-4 w-4 text-muted-foreground" />
                <Input
                  placeholder="Rechercher une conversation..."
                  value={searchQuery}
                  onChange={(e) => setSearchQuery(e.target.value)}
                  className="pl-10"
                />
              </div>
            </div>

            <ScrollArea className="flex-1">
              {filteredConversations.length === 0 ? (
                <div className="p-8 text-center text-muted-foreground">
                  <p>Aucune conversation</p>
                  <Button
                    onClick={() => navigate('/friends')}
                    variant="outline"
                    className="mt-4"
                  >
                    Ajouter des amis
                  </Button>
                </div>
              ) : (
                filteredConversations.map((conversation) => {
                  const other = getOtherParticipant(conversation);
                  if (!other) return null;

                  const lastMessage = conversation.messages[0];
                  const initials = (other.displayName || other.username)
                    .split(' ')
                    .map((n) => n[0])
                    .join('')
                    .toUpperCase()
                    .substring(0, 2);

                  return (
                    <div
                      key={conversation.id}
                      onClick={() => loadMessages(conversation)}
                      className={`p-4 flex items-center gap-3 cursor-pointer hover:bg-muted transition-colors ${
                        selectedConversation?.id === conversation.id ? 'bg-muted' : ''
                      }`}
                    >
                      <Avatar className="h-12 w-12">
                        <AvatarImage src={other.avatarUrl || undefined} />
                        <AvatarFallback>{initials}</AvatarFallback>
                      </Avatar>
                      <div className="flex-1 min-w-0">
                        <div className="flex items-center justify-between mb-1">
                          <span className="font-semibold truncate">
                            {other.displayName || other.username}
                          </span>
                          {lastMessage && (
                            <span className="text-xs text-muted-foreground">
                              {formatTime(lastMessage.createdAt)}
                            </span>
                          )}
                        </div>
                        {lastMessage && (
                          <p className="text-sm text-muted-foreground truncate">
                            {lastMessage.senderId === user?.id ? 'Vous: ' : ''}
                            {lastMessage.content}
                          </p>
                        )}
                      </div>
                      {(conversation.unreadCount || 0) > 0 && (
                        <Badge className="bg-primary text-primary-foreground">
                          {conversation.unreadCount}
                        </Badge>
                      )}
                    </div>
                  );
                })
              )}
            </ScrollArea>
          </div>

          {/* Zone de chat */}
          {selectedConversation ? (
            <div className={`${selectedConversation ? 'flex' : 'hidden md:flex'} w-full md:w-2/3 flex-col`}>
              {/* En-tête du chat */}
              <div className="p-4 border-b border-border flex items-center justify-between">
                <div className="flex items-center gap-3">
                  <Button
                    variant="ghost"
                    size="icon"
                    className="md:hidden"
                    onClick={() => setSelectedConversation(null)}
                  >
                    <ArrowLeft className="h-5 w-5" />
                  </Button>
                  <Avatar className="h-10 w-10">
                    <AvatarImage
                      src={getOtherParticipant(selectedConversation)?.avatarUrl || undefined}
                    />
                    <AvatarFallback>
                      {(getOtherParticipant(selectedConversation)?.displayName ||
                        getOtherParticipant(selectedConversation)?.username ||
                        '')
                        .split(' ')
                        .map((n) => n[0])
                        .join('')
                        .toUpperCase()
                        .substring(0, 2)}
                    </AvatarFallback>
                  </Avatar>
                  <div>
                    <p className="font-semibold">
                      {getOtherParticipant(selectedConversation)?.displayName ||
                        getOtherParticipant(selectedConversation)?.username}
                    </p>
                    {isTyping && (
                      <p className="text-xs text-muted-foreground">en train d'écrire...</p>
                    )}
                  </div>
                </div>
                <DropdownMenu>
                  <DropdownMenuTrigger asChild>
                    <Button variant="ghost" size="icon">
                      <MoreVertical className="h-5 w-5" />
                    </Button>
                  </DropdownMenuTrigger>
                  <DropdownMenuContent align="end">
                    <DropdownMenuItem
                      onClick={() => handleDeleteConversation(selectedConversation.id)}
                      className="text-destructive"
                    >
                      <Trash2 className="h-4 w-4 mr-2" />
                      Supprimer la conversation
                    </DropdownMenuItem>
                  </DropdownMenuContent>
                </DropdownMenu>
              </div>

              {/* Messages */}
              <ScrollArea className="flex-1 p-4">
                <div className="space-y-4">
                  {messages.map((message) => {
                    const isOwn = message.senderId === user?.id;
                    return (
                      <div
                        key={message.id}
                        className={`flex ${isOwn ? 'justify-end' : 'justify-start'}`}
                      >
                        <div
                          className={`max-w-[70%] rounded-lg p-3 ${
                            isOwn
                              ? 'bg-primary text-primary-foreground'
                              : 'bg-muted'
                          }`}
                        >
                          <p className="break-words">{message.content}</p>
                          <p
                            className={`text-xs mt-1 ${
                              isOwn ? 'text-primary-foreground/70' : 'text-muted-foreground'
                            }`}
                          >
                            {new Date(message.createdAt).toLocaleTimeString('fr-FR', {
                              hour: '2-digit',
                              minute: '2-digit',
                            })}
                          </p>
                        </div>
                      </div>
                    );
                  })}
                  <div ref={messagesEndRef} />
                </div>
              </ScrollArea>

              {/* Input de message */}
              <div className="p-4 border-t border-border">
                <div className="flex gap-2">
                  <Input
                    placeholder="Écrivez votre message..."
                    value={newMessage}
                    onChange={(e) => {
                      setNewMessage(e.target.value);
                      handleTyping();
                    }}
                    onKeyPress={(e) => {
                      if (e.key === 'Enter' && !e.shiftKey) {
                        e.preventDefault();
                        handleSendMessage();
                      }
                    }}
                    className="flex-1"
                  />
                  <Button onClick={handleSendMessage} disabled={!newMessage.trim()}>
                    <Send className="h-4 w-4" />
                  </Button>
                </div>
              </div>
            </div>
          ) : (
            <div className="hidden md:flex w-2/3 items-center justify-center text-muted-foreground">
              <div className="text-center">
                <p className="text-lg">Sélectionnez une conversation</p>
                <p className="text-sm">pour commencer à chatter</p>
              </div>
            </div>
          )}
        </div>
      </main>

      <Footer />
    </div>
  );
}
