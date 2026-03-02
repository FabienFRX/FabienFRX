import { Toaster } from "@/components/ui/toaster";
import { Toaster as Sonner } from "@/components/ui/sonner";
import { TooltipProvider } from "@/components/ui/tooltip";
import { QueryClient, QueryClientProvider } from "@tanstack/react-query";
import { BrowserRouter, Routes, Route } from "react-router-dom";
import { AuthProvider } from "@/hooks/useAuth";
import Index from "./pages/Index";
import Learn from "./pages/Learn";
import Quiz from "./pages/Quiz";
import Profile from "./pages/Profile";
import Friends from "./pages/Friends";
import Messages from "./pages/Messages";
import Leaderboard from "./pages/Leaderboard";
import Auth from "./pages/Auth";
import Admin from "./pages/Admin";
import DailyChallenge from "./pages/DailyChallenge";
import DailyChallengeHistory from "./pages/DailyChallengeHistory";
import NotFound from "./pages/NotFound";

const queryClient = new QueryClient();

const App = () => (
  <QueryClientProvider client={queryClient}>
    <AuthProvider>
      <TooltipProvider>
        <Toaster />
        <Sonner />
        <BrowserRouter>
          <Routes>
            <Route path="/" element={<Index />} />
            <Route path="/learn" element={<Learn />} />
            <Route path="/quiz" element={<Quiz />} />
            <Route path="/daily-challenge" element={<DailyChallenge />} />
            <Route path="/daily-challenge/history" element={<DailyChallengeHistory />} />
            <Route path="/profile" element={<Profile />} />
            <Route path="/friends" element={<Friends />} />
            <Route path="/messages" element={<Messages />} />
            <Route path="/leaderboard" element={<Leaderboard />} />
            <Route path="/auth" element={<Auth />} />
            <Route path="/admin" element={<Admin />} />
            {/* ADD ALL CUSTOM ROUTES ABOVE THE CATCH-ALL "*" ROUTE */}
            <Route path="*" element={<NotFound />} />
          </Routes>
        </BrowserRouter>
      </TooltipProvider>
    </AuthProvider>
  </QueryClientProvider>
);

export default App;
