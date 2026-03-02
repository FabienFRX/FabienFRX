import { ReactNode } from "react";
import { Card, CardContent } from "@/components/ui/card";
import { LucideIcon } from "lucide-react";

interface ModeCardProps {
  title: string;
  description: string;
  icon: LucideIcon;
  gradient: string;
  onClick?: () => void;
}

export const ModeCard = ({
  title,
  description,
  icon: Icon,
  gradient,
  onClick,
}: ModeCardProps) => {
  return (
    <Card
      className="hover-lift cursor-pointer border-2 border-border overflow-hidden group"
      onClick={onClick}
    >
      <CardContent className="p-0">
        <div className={`${gradient} p-8 text-center`}>
          <div className="mb-4 flex justify-center">
            <div className="rounded-full bg-white/20 p-6 backdrop-blur-sm group-hover:scale-110 transition-transform duration-300">
              <Icon className="h-12 w-12 text-white" />
            </div>
          </div>
          <h3 className="text-2xl md:text-3xl font-bold text-white mb-2">
            {title}
          </h3>
          <p className="text-white/90 text-sm md:text-base">{description}</p>
        </div>
      </CardContent>
    </Card>
  );
};
