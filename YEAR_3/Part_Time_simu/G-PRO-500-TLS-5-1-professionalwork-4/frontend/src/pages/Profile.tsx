import { Header } from "@/components/Header";
import { Footer } from "@/components/Footer";
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import { Label } from "@/components/ui/label";
import { Switch } from "@/components/ui/switch";
import { Badge } from "@/components/ui/badge";
import { Avatar, AvatarFallback, AvatarImage } from "@/components/ui/avatar";
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from "@/components/ui/select";
import { Separator } from "@/components/ui/separator";
import { Shield, Trash2 } from "lucide-react";
import { useEffect, useState } from "react";
import { usersService, getUserIdFromToken, type User } from "@/services/users.service";
import { toast } from "@/hooks/use-toast";
import { AvatarSelector } from "@/components/AvatarSelector";

const Profile = () => {
  const [loading, setLoading] = useState(false);
  const [user, setUser] = useState<User | null>(null);
  const [form, setForm] = useState({
    displayName: "",
    avatarUrl: "",
    birthYear: "",
    country: "",
  });
  const [pw, setPw] = useState({ currentPassword: "", newPassword: "", confirm: "", changing: false });
  const [prefs, setPrefs] = useState({ ageGroup: 'ADULT', defaultLevel: 'MEDIUM', favoriteTopics: [] as string[], trickyAnswersEnabled: true });
  const [showAvatarSelector, setShowAvatarSelector] = useState(false);

  useEffect(() => {
    const id = getUserIdFromToken();
    if (!id) return;
    usersService
      .getById(id)
      .then((u) => {
        setUser(u);
        setForm({
          displayName: u.displayName || "",
          avatarUrl: u.avatarUrl || "",
          birthYear: u.birthYear ? String(u.birthYear) : "",
          country: u.country || "",
        });
        setPrefs({
          ageGroup: (u as any).ageGroup || 'ADULT',
          defaultLevel: (u as any).defaultLevel || 'MEDIUM',
          favoriteTopics: Array.isArray((u as any).favoriteTopics) ? (u as any).favoriteTopics.slice(0,2) : [],
          trickyAnswersEnabled: !!(u as any).trickyAnswersEnabled,
        });
      })
      .catch(() => {
        toast({ title: "Erreur", description: "Impossible de charger le profil", variant: "destructive" });
      });
  }, []);

  const onSave = async () => {
    if (!user) return;
    setLoading(true);
    try {
      const updated = await usersService.update(user.id, {
        displayName: form.displayName || undefined,
        avatarUrl: form.avatarUrl || undefined,
        country: form.country || undefined,
        birthYear: form.birthYear ? Number(form.birthYear) : undefined,
      });
      setUser(updated);
      toast({ title: "Profil mis à jour", description: "Vos informations ont été enregistrées." });
    } catch (e) {
      toast({ title: "Échec", description: e instanceof Error ? e.message : "Erreur inconnue", variant: "destructive" });
    } finally {
      setLoading(false);
    }
  };
  return (
    <div className="min-h-screen flex flex-col bg-background">
      <Header />
      
      <main className="flex-1 px-4 py-8">
        <div className="max-w-4xl mx-auto space-y-6">
          {/* Profile Header */}
          <Card>
            <CardContent className="pt-6">
              <div className="flex flex-col md:flex-row items-center md:items-start gap-6">
                <Avatar className="h-24 w-24">
                  <AvatarImage src={form.avatarUrl || "/placeholder.svg"} />
                  <AvatarFallback className="text-2xl bg-primary text-primary-foreground">AM</AvatarFallback>
                </Avatar>
                <div className="flex-1 text-center md:text-left">
                  <h1 className="text-2xl font-bold text-foreground mb-1">{form.displayName || user?.username || "Profil"}</h1>
                  <p className="text-muted-foreground mb-2">{user?.email || ""}</p>
                  <div className="flex flex-wrap gap-2 justify-center md:justify-start">
                    <Badge className="bg-primary text-primary-foreground">Rang #462</Badge>
                    <Badge variant="outline">3,420 pts</Badge>
                  </div>
                </div>
              </div>
            </CardContent>
          </Card>

          {/* Personal Info */}
          <Card>
            <CardHeader>
              <CardTitle>Informations personnelles</CardTitle>
              <CardDescription>Gérez vos informations de profil</CardDescription>
            </CardHeader>
            <CardContent className="space-y-4">
              <div className="grid md:grid-cols-2 gap-4">
                <div className="space-y-2">
                  <Label htmlFor="displayName">Nom d'affichage</Label>
                  <Input
                    id="displayName"
                    value={form.displayName}
                    onChange={(e) => setForm((f) => ({ ...f, displayName: e.target.value }))}
                  />
                </div>
                <div className="space-y-2">
                  <Label htmlFor="birthYear">Année de naissance</Label>
                  <Input
                    id="birthYear"
                    type="number"
                    placeholder="1998"
                    value={form.birthYear}
                    onChange={(e) => setForm((f) => ({ ...f, birthYear: e.target.value }))}
                  />
                </div>
              </div>
              <div className="space-y-2">
                <Label htmlFor="avatarUrl">Avatar</Label>
                <div className="flex gap-3 items-center">
                  <Avatar className="h-16 w-16">
                    <AvatarImage src={form.avatarUrl || "/placeholder.svg"} />
                    <AvatarFallback>?</AvatarFallback>
                  </Avatar>
                  <Button 
                    type="button" 
                    variant="outline" 
                    onClick={() => setShowAvatarSelector(!showAvatarSelector)}
                  >
                    {showAvatarSelector ? "Masquer les avatars" : "Choisir un avatar"}
                  </Button>
                </div>
              </div>
              <div className="space-y-2">
                <Label htmlFor="country">Pays (code ISO2)</Label>
                <Input
                  id="country"
                  placeholder="FR"
                  value={form.country}
                  onChange={(e) => setForm((f) => ({ ...f, country: e.target.value.toUpperCase() }))}
                />
              </div>

              {showAvatarSelector && (
                <div className="mt-4">
                  <AvatarSelector
                    currentAvatar={form.avatarUrl}
                    onSelect={(avatarUrl) => {
                      setForm((f) => ({ ...f, avatarUrl }));
                      setShowAvatarSelector(false);
                    }}
                    onCancel={() => setShowAvatarSelector(false)}
                  />
                </div>
              )}

              <Button onClick={onSave} disabled={loading} className="bg-primary text-primary-foreground hover:bg-primary/90">
                {loading ? "Sauvegarde..." : "Sauvegarder les modifications"}
              </Button>
            </CardContent>
          </Card>

          {/* Security */}
          <Card>
            <CardHeader>
              <CardTitle className="flex items-center gap-2">
                <Shield className="h-5 w-5" />
                Sécurité
              </CardTitle>
              <CardDescription>Protégez votre compte</CardDescription>
            </CardHeader>
            <CardContent className="space-y-6">
              <div className="space-y-4">
                <div className="space-y-2">
                  <Label htmlFor="currentPassword">Mot de passe actuel</Label>
                  <Input
                    id="currentPassword"
                    type="password"
                    placeholder="••••••••"
                    value={pw.currentPassword}
                    onChange={(e) => setPw((p) => ({ ...p, currentPassword: e.target.value }))}
                  />
                </div>
                <div className="space-y-2">
                  <Label htmlFor="newPassword">Nouveau mot de passe</Label>
                  <Input
                    id="newPassword"
                    type="password"
                    placeholder="Min 8 caractères"
                    value={pw.newPassword}
                    onChange={(e) => setPw((p) => ({ ...p, newPassword: e.target.value }))}
                  />
                </div>
                <div className="space-y-2">
                  <Label htmlFor="confirmPassword">Confirmer le nouveau mot de passe</Label>
                  <Input
                    id="confirmPassword"
                    type="password"
                    placeholder="Répétez le mot de passe"
                    value={pw.confirm}
                    onChange={(e) => setPw((p) => ({ ...p, confirm: e.target.value }))}
                  />
                  {pw.confirm && pw.confirm !== pw.newPassword && (
                    <p className="text-sm text-destructive">Les mots de passe ne correspondent pas.</p>
                  )}
                </div>
                <Button
                  variant="outline"
                  disabled={pw.changing || !pw.currentPassword || !pw.newPassword || pw.newPassword !== pw.confirm || pw.newPassword.length < 8}
                  onClick={async () => {
                    if (!user) return;
                    setPw((p) => ({ ...p, changing: true }));
                    try {
                      await usersService.changePassword(user.id, pw.currentPassword, pw.newPassword);
                      toast({ title: 'Mot de passe mis à jour', description: 'Modification réussie.' });
                      setPw({ currentPassword: "", newPassword: "", confirm: "", changing: false });
                    } catch (e) {
                      toast({ title: 'Échec', description: e instanceof Error ? e.message : 'Erreur inconnue', variant: 'destructive' });
                      setPw((p) => ({ ...p, changing: false }));
                    }
                  }}
                  className="mt-2 hover:bg-muted"
                >
                  {pw.changing ? 'Mise à jour...' : 'Mettre à jour'}
                </Button>
              </div>
              
              <Separator />
              
              <div className="flex items-center justify-between">
                <div className="space-y-0.5">
                  <Label>Authentification à deux facteurs (2FA)</Label>
                  <p className="text-sm text-muted-foreground">Sécurité renforcée pour votre compte</p>
                </div>
                <Switch />
              </div>

              <Separator />


            </CardContent>
          </Card>

          {/* Learning Preferences */}
          <Card>
            <CardHeader>
              <CardTitle>Préférences d'apprentissage</CardTitle>
              <CardDescription>Personnalisez votre expérience</CardDescription>
            </CardHeader>
            <CardContent className="space-y-4">
              <div className="grid md:grid-cols-2 gap-4">
                <div className="space-y-2">
                  <Label>Tranche d'âge</Label>
                  <Select value={prefs.ageGroup} onValueChange={(v) => setPrefs((p) => ({ ...p, ageGroup: v as any }))}>
                    <SelectTrigger>
                      <SelectValue />
                    </SelectTrigger>
                    <SelectContent>
                      <SelectItem value="CHILD">Enfant (6-12 ans)</SelectItem>
                      <SelectItem value="TEEN">Adolescent (13-17 ans)</SelectItem>
                      <SelectItem value="ADULT">Adulte (18+ ans)</SelectItem>
                    </SelectContent>
                  </Select>
                </div>
                <div className="space-y-2">
                  <Label>Niveau par défaut</Label>
                  <Select value={prefs.defaultLevel} onValueChange={(v) => setPrefs((p) => ({ ...p, defaultLevel: v as any }))}>
                    <SelectTrigger>
                      <SelectValue />
                    </SelectTrigger>
                    <SelectContent>
                      <SelectItem value="EASY">Facile</SelectItem>
                      <SelectItem value="MEDIUM">Moyen</SelectItem>
                      <SelectItem value="HARD">Difficile</SelectItem>
                    </SelectContent>
                  </Select>
                </div>
              </div>

              <div className="space-y-2">
                <Label>Thèmes favoris</Label>
                <div className="flex flex-wrap gap-2">
                  {['Histoire','Sciences','Géographie','Culture Pop','Sport'].map((t) => {
                    const selected = prefs.favoriteTopics.includes(t);
                    return (
                      <Badge
                        key={t}
                        variant={selected ? 'default' : 'outline'}
                        className="cursor-pointer"
                        onClick={() => setPrefs((p) => {
                          let fav = [...p.favoriteTopics];
                          if (selected) fav = fav.filter(x => x !== t);
                          else if (fav.length < 2) fav.push(t);
                          return { ...p, favoriteTopics: fav };
                        })}
                      >
                        {t}
                      </Badge>
                    );
                  })}
                </div>
              </div>

              <div className="flex items-center justify-between">
                <div className="space-y-0.5">
                  <Label>Activer les réponses pièges</Label>
                  <p className="text-sm text-muted-foreground">Questions avec propositions ambiguës (15-20%)</p>
                </div>
                <Switch checked={prefs.trickyAnswersEnabled} onCheckedChange={(v) => setPrefs((p) => ({ ...p, trickyAnswersEnabled: v }))} />
              </div>

              <div className="flex gap-2">
                <Button
                  variant="secondary"
                  onClick={async () => {
                    if (!user) return;
                    try {
                      const updated = await usersService.updatePreferences(user.id, prefs);
                      setUser(updated);
                      toast({ title: 'Préférences enregistrées', description: 'Vos choix ont été mis à jour.' });
                    } catch (e) {
                      toast({ title: 'Échec', description: e instanceof Error ? e.message : 'Erreur inconnue', variant: 'destructive' });
                    }
                  }}
                >
                  Enregistrer les préférences
                </Button>
              </div>
            </CardContent>
          </Card>

          {/* Danger Zone */}
          <Card className="border-destructive">
            <CardHeader>
              <CardTitle className="flex items-center gap-2 text-destructive">
                <Trash2 className="h-5 w-5" />
                Zone dangereuse
              </CardTitle>
              <CardDescription>Actions irréversibles</CardDescription>
            </CardHeader>
            <CardContent>
              <div className="space-y-4">
                <p className="text-sm text-muted-foreground">
                  La suppression de votre compte est définitive. Toutes vos données seront perdues.
                </p>
                <Button
                  variant="destructive"
                  onClick={async () => {
                    if (!user) return;
                    try {
                      await usersService.delete(user.id);
                      toast({ title: 'Compte supprimé', description: 'Votre compte a été marqué comme supprimé.' });
                      localStorage.removeItem('auth_token');
                      window.location.href = '/';
                    } catch (e) {
                      toast({ title: 'Échec', description: e instanceof Error ? e.message : 'Erreur inconnue', variant: 'destructive' });
                    }
                  }}
                >
                  Supprimer mon compte
                </Button>
              </div>
            </CardContent>
          </Card>
        </div>
      </main>

      <Footer />
    </div>
  );
};

export default Profile;
