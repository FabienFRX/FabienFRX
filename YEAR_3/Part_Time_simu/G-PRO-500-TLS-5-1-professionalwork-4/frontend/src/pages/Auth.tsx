import { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import { useAuth } from '@/hooks/useAuth';
import { Button } from '@/components/ui/button';
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from '@/components/ui/card';
import { Input } from '@/components/ui/input';
import { Label } from '@/components/ui/label';
import { Tabs, TabsContent, TabsList, TabsTrigger } from '@/components/ui/tabs';
import { Brain, Loader2, ArrowLeft } from 'lucide-react';
import { Header } from '@/components/Header';
import { Footer } from '@/components/Footer';

const Auth = () => {
  const navigate = useNavigate();
  const { login, register, loading } = useAuth();

  const [loginData, setLoginData] = useState({
    email: '',
    password: '',
  });

  const [registerData, setRegisterData] = useState({
    email: '',
    username: '',
    password: '',
    confirmPassword: '',
    displayName: '',
  });

  const handleLogin = async (e: React.FormEvent) => {
    e.preventDefault();
    const success = await login({
      email: loginData.email,
      password: loginData.password,
    });

    if (success) {
      navigate('/');
    }
  };

  const handleRegister = async (e: React.FormEvent) => {
    e.preventDefault();
    
    if (registerData.password !== registerData.confirmPassword) {
      return;
    }

    const success = await register({
      email: registerData.email,
      username: registerData.username,
      password: registerData.password,
      displayName: registerData.displayName || undefined,
    });

    if (success) {
      navigate('/');
    }
  };

  return (
    <div className="min-h-screen bg-gradient-to-br from-purple-50 via-blue-50 to-indigo-50 dark:from-purple-950 dark:via-blue-950 dark:to-indigo-950">
      <Header />
      
      <main className="container mx-auto px-4 py-8 flex-1">
        <div className="max-w-md mx-auto">
          {/* Header de la page */}
          <div className="text-center mb-8">
            <Button 
              variant="ghost" 
              onClick={() => navigate('/')}
              className="mb-4 text-muted-foreground hover:text-foreground"
            >
              <ArrowLeft className="mr-2 h-4 w-4" />
              Retour à l'accueil
            </Button>
            
            <div className="flex items-center justify-center gap-2 mb-4">
              <Brain className="h-10 w-10 text-purple-600" />
              <h1 className="text-4xl font-bold bg-gradient-to-r from-purple-600 to-blue-600 bg-clip-text text-transparent">
                QuizLab
              </h1>
            </div>
            <p className="text-muted-foreground">
              Rejoignez la communauté et défiez vos connaissances !
            </p>
          </div>

          <Tabs defaultValue="login" className="w-full">
            <TabsList className="grid w-full grid-cols-2 mb-6">
              <TabsTrigger value="login" className="data-[state=active]:bg-gradient-to-r data-[state=active]:from-purple-500 data-[state=active]:to-blue-500 data-[state=active]:text-white">
                Connexion
              </TabsTrigger>
              <TabsTrigger value="register" className="data-[state=active]:bg-gradient-to-r data-[state=active]:from-purple-500 data-[state=active]:to-blue-500 data-[state=active]:text-white">
                Inscription
              </TabsTrigger>
            </TabsList>

            {/* Formulaire de connexion */}
            <TabsContent value="login">
              <Card className="border-2 shadow-lg">
                <CardHeader className="text-center">
                  <CardTitle className="text-2xl bg-gradient-to-r from-purple-600 to-blue-600 bg-clip-text text-transparent">
                    Se connecter
                  </CardTitle>
                  <CardDescription>
                    Connectez-vous à votre compte QuizLab
                  </CardDescription>
                </CardHeader>
                <CardContent>
                  <form onSubmit={handleLogin} className="space-y-4">
                    <div className="space-y-2">
                      <Label htmlFor="login-email">Email</Label>
                      <Input
                        id="login-email"
                        type="email"
                        placeholder="votre@email.com"
                        value={loginData.email}
                        onChange={(e) => setLoginData(prev => ({ ...prev, email: e.target.value }))}
                        className="border-2 focus:border-purple-500"
                        required
                      />
                    </div>
                    <div className="space-y-2">
                      <Label htmlFor="login-password">Mot de passe</Label>
                      <Input
                        id="login-password"
                        type="password"
                        placeholder="••••••••"
                        value={loginData.password}
                        onChange={(e) => setLoginData(prev => ({ ...prev, password: e.target.value }))}
                        className="border-2 focus:border-purple-500"
                        required
                      />
                    </div>
                    <Button 
                      type="submit" 
                      className="w-full bg-gradient-to-r from-purple-500 to-blue-500 hover:from-purple-600 hover:to-blue-600 text-white font-semibold py-2 px-4 rounded-lg transition-all duration-300 transform hover:scale-105 shadow-lg" 
                      disabled={loading}
                    >
                      {loading && <Loader2 className="mr-2 h-4 w-4 animate-spin" />}
                      Se connecter
                    </Button>
                  </form>
                </CardContent>
              </Card>
            </TabsContent>

            {/* Formulaire d'inscription */}
            <TabsContent value="register">
              <Card className="border-2 shadow-lg">
                <CardHeader className="text-center">
                  <CardTitle className="text-2xl bg-gradient-to-r from-purple-600 to-blue-600 bg-clip-text text-transparent">
                    S'inscrire
                  </CardTitle>
                  <CardDescription>
                    Créez votre compte QuizLab gratuitement
                  </CardDescription>
                </CardHeader>
                <CardContent>
                  <form onSubmit={handleRegister} className="space-y-4">
                    <div className="space-y-2">
                      <Label htmlFor="register-email">Email</Label>
                      <Input
                        id="register-email"
                        type="email"
                        placeholder="votre@email.com"
                        value={registerData.email}
                        onChange={(e) => setRegisterData(prev => ({ ...prev, email: e.target.value }))}
                        className="border-2 focus:border-purple-500"
                        required
                      />
                    </div>
                    <div className="space-y-2">
                      <Label htmlFor="register-username">Nom d'utilisateur</Label>
                      <Input
                        id="register-username"
                        placeholder="pseudounique"
                        value={registerData.username}
                        onChange={(e) => setRegisterData(prev => ({ ...prev, username: e.target.value }))}
                        className="border-2 focus:border-purple-500"
                        required
                      />
                    </div>
                    <div className="space-y-2">
                      <Label htmlFor="register-displayName">Nom d'affichage (optionnel)</Label>
                      <Input
                        id="register-displayName"
                        placeholder="Votre nom"
                        value={registerData.displayName}
                        onChange={(e) => setRegisterData(prev => ({ ...prev, displayName: e.target.value }))}
                        className="border-2 focus:border-purple-500"
                      />
                    </div>
                    <div className="space-y-2">
                      <Label htmlFor="register-password">Mot de passe</Label>
                      <Input
                        id="register-password"
                        type="password"
                        placeholder="••••••••"
                        value={registerData.password}
                        onChange={(e) => setRegisterData(prev => ({ ...prev, password: e.target.value }))}
                        className="border-2 focus:border-purple-500"
                        required
                      />
                    </div>
                    <div className="space-y-2">
                      <Label htmlFor="register-confirm">Confirmer le mot de passe</Label>
                      <Input
                        id="register-confirm"
                        type="password"
                        placeholder="••••••••"
                        value={registerData.confirmPassword}
                        onChange={(e) => setRegisterData(prev => ({ ...prev, confirmPassword: e.target.value }))}
                        className="border-2 focus:border-purple-500"
                        required
                      />
                      {registerData.password !== registerData.confirmPassword && registerData.confirmPassword && (
                        <p className="text-sm text-red-500">Les mots de passe ne correspondent pas</p>
                      )}
                    </div>
                    <Button 
                      type="submit" 
                      className="w-full bg-gradient-to-r from-green-500 to-emerald-500 hover:from-green-600 hover:to-emerald-600 text-white font-semibold py-2 px-4 rounded-lg transition-all duration-300 transform hover:scale-105 shadow-lg" 
                      disabled={loading || registerData.password !== registerData.confirmPassword}
                    >
                      {loading && <Loader2 className="mr-2 h-4 w-4 animate-spin" />}
                      S'inscrire
                    </Button>
                  </form>
                </CardContent>
              </Card>
            </TabsContent>
          </Tabs>
        </div>
      </main>
      
      <Footer />
    </div>
  );
};

export default Auth;