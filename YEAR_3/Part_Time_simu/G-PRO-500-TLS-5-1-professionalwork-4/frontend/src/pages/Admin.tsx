import { useEffect, useMemo, useState } from 'react';
import { Header } from '@/components/Header';
import { Footer } from '@/components/Footer';
import { Card, CardContent, CardHeader, CardTitle } from '@/components/ui/card';
import { Input } from '@/components/ui/input';
import { Button } from '@/components/ui/button';
import { Label } from '@/components/ui/label';
import { toast } from '@/hooks/use-toast';
import { usersService, type User } from '@/services/users.service';

const Admin = () => {
  const [users, setUsers] = useState<User[]>([]);
  const [loading, setLoading] = useState(false);
  const [query, setQuery] = useState('');
  const [pwMap, setPwMap] = useState<Record<string, string>>({});
  const [pwLoading, setPwLoading] = useState<Record<string, boolean>>({});

  useEffect(() => {
    setLoading(true);
    usersService.list()
      .then(setUsers)
      .catch(() => toast({ title: 'Erreur', description: 'Chargement des utilisateurs impossible', variant: 'destructive' }))
      .finally(() => setLoading(false));
  }, []);

  const filtered = useMemo(() => {
    const q = query.trim().toLowerCase();
    if (!q) return users;
    return users.filter(u =>
      (u.email?.toLowerCase().includes(q)) ||
      (u.username?.toLowerCase().includes(q))
    );
  }, [query, users]);

  return (
    <div className="min-h-screen flex flex-col bg-background">
      <Header />
      <main className="flex-1 px-4 py-8">
        <div className="max-w-6xl mx-auto space-y-6">
          <Card>
            <CardHeader>
              <CardTitle>Admin Panel — Utilisateurs</CardTitle>
            </CardHeader>
            <CardContent className="space-y-4">
              <div className="flex gap-3 items-end">
                <div className="flex-1 space-y-2">
                  <Label htmlFor="search">Rechercher (email / pseudo)</Label>
                  <Input id="search" value={query} onChange={(e) => setQuery(e.target.value)} placeholder="john@doe.com ou johndoe" />
                </div>
                <Button variant="outline" onClick={() => setQuery('')}>Réinitialiser</Button>
              </div>

              <div className="overflow-x-auto">
                <table className="w-full text-sm">
                  <thead>
                    <tr className="text-left border-b">
                      <th className="py-2 px-2">Email</th>
                      <th className="py-2 px-2">Pseudo</th>
                      <th className="py-2 px-2">Nom</th>
                      <th className="py-2 px-2">Actions</th>
                    </tr>
                  </thead>
                  <tbody>
                    {filtered.map(u => {
                      const draft = pwMap[u.id] || '';
                      const isRowLoading = pwLoading[u.id] || false;
                      return (
                        <tr key={u.id} className="border-b">
                          <td className="py-2 px-2">{u.email}</td>
                          <td className="py-2 px-2">{u.username}</td>
                          <td className="py-2 px-2">{u.displayName || ''}</td>
                          <td className="py-2 px-2 flex flex-wrap gap-2 items-center">
                            <Button
                              variant="destructive"
                              disabled={isRowLoading}
                              onClick={async () => {
                                try {
                                  await usersService.delete(u.id);
                                  toast({ title: 'Supprimé', description: u.email });
                                  setUsers(prev => prev.filter(x => x.id !== u.id));
                                  setPwMap(m => { const { [u.id]: _, ...rest } = m; return rest; });
                                } catch (e) {
                                  toast({ title: 'Échec suppression', description: e instanceof Error ? e.message : 'Erreur', variant: 'destructive' });
                                }
                              }}
                            >
                              Supprimer
                            </Button>
                            <Input
                              placeholder="Nouveau mot de passe"
                              aria-label={`Nouveau mot de passe pour ${u.email}`}
                              type="password"
                              value={draft}
                              onChange={(e) => setPwMap(m => ({ ...m, [u.id]: e.target.value }))}
                              className="w-48"
                              disabled={isRowLoading}
                            />
                            <Button
                              variant="secondary"
                              disabled={isRowLoading || !draft || draft.length < 8}
                              onClick={async () => {
                                setPwLoading(l => ({ ...l, [u.id]: true }));
                                try {
                                  await usersService.adminSetPassword(u.id, draft);
                                  toast({ title: 'Mot de passe changé', description: u.email });
                                  setPwMap(m => ({ ...m, [u.id]: '' }));
                                } catch (e) {
                                  toast({ title: 'Échec', description: e instanceof Error ? e.message : 'Erreur', variant: 'destructive' });
                                } finally {
                                  setPwLoading(l => ({ ...l, [u.id]: false }));
                                }
                              }}
                            >
                              {isRowLoading ? '...' : 'Changer MDP'}
                            </Button>
                          </td>
                        </tr>
                      );
                    })}
                    {!loading && filtered.length === 0 && (
                      <tr><td className="py-4 px-2 text-muted-foreground" colSpan={4}>Aucun utilisateur</td></tr>
                    )}
                  </tbody>
                </table>
              </div>
            </CardContent>
          </Card>
        </div>
      </main>
      <Footer />
    </div>
  );
};

export default Admin;
