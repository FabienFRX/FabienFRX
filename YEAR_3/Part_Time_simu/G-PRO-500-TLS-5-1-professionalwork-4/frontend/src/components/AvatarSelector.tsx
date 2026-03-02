import { useState } from 'react';
import { Card, CardContent } from '@/components/ui/card';
import { Button } from '@/components/ui/button';
import { Label } from '@/components/ui/label';

const AVAILABLE_AVATARS = [
  '/avatars/pig.png',
  '/avatars/dog.png',
  '/avatars/fox.png',
  '/avatars/chicken.png',
];

type AvatarSelectorProps = {
  currentAvatar?: string;
  onSelect: (avatarUrl: string) => void;
  onCancel?: () => void;
};

export const AvatarSelector = ({ currentAvatar, onSelect, onCancel }: AvatarSelectorProps) => {
  const [selected, setSelected] = useState<string>(currentAvatar || AVAILABLE_AVATARS[0]);

  const handleConfirm = () => {
    onSelect(selected);
  };

  return (
    <Card>
      <CardContent className="pt-6 space-y-4">
        <div>
          <Label className="text-base font-semibold">Choisir un avatar</Label>
          <p className="text-sm text-muted-foreground mt-1">
            Sélectionnez un avatar parmi les options disponibles
          </p>
        </div>

        <div className="grid grid-cols-4 sm:grid-cols-6 gap-3">
          {AVAILABLE_AVATARS.map((avatar) => (
            <button
              key={avatar}
              type="button"
              onClick={() => setSelected(avatar)}
              className={`
                relative rounded-lg overflow-hidden border-2 transition-all
                hover:scale-105 hover:shadow-md
                ${selected === avatar ? 'border-primary ring-2 ring-primary ring-offset-2' : 'border-border'}
              `}
            >
              <img
                src={avatar}
                alt={`Avatar ${avatar}`}
                className="w-full h-full object-cover"
              />
            </button>
          ))}
        </div>

        <div className="flex gap-3 justify-end">
          {onCancel && (
            <Button variant="outline" onClick={onCancel}>
              Annuler
            </Button>
          )}
          <Button onClick={handleConfirm}>
            Confirmer
          </Button>
        </div>
      </CardContent>
    </Card>
  );
};
