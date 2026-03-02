import React from 'react';
import ApiImage from '../components/ApiImage';

// Types for the components
interface News {
  id: number;
  title: string;
  description: string;
}

interface Event {
  id: number;
  name: string;
  location: string;
}

interface Startup {
  id: number;
  name: string;
  sector: string;
}

interface User {
  id: number;
  name: string;
}

interface Founder {
  id: number;
  name: string;
}

// Exemple d'utilisation dans différents composants

// 1. Dans NewsCard
export const NewsCardExample = ({ news }: { news: News }) => (
  <div className="news-card">
    <ApiImage
      endpoint="news"
      id={news.id}
      alt={news.title}
      className="w-full h-48 object-cover rounded-t-lg"
      fallbackSrc="/logo.png"
    />
    <div className="p-4">
      <h3>{news.title}</h3>
      <p>{news.description}</p>
    </div>
  </div>
);

// 2. Dans EventCard
export const EventCardExample = ({ event }: { event: Event }) => (
  <div className="event-card">
    <ApiImage
      endpoint="events"
      id={event.id}
      alt={event.name}
      className="w-full h-32 object-cover"
      fallbackSrc="/logo.png"
    />
    <div className="p-3">
      <h3>{event.name}</h3>
      <p>{event.location}</p>
    </div>
  </div>
);

// 3. Dans StartupCard
export const StartupCardExample = ({ startup }: { startup: Startup }) => (
  <div className="startup-card">
    <ApiImage
      endpoint="startups"
      id={startup.id}
      alt={startup.name}
      className="w-16 h-16 rounded-full object-cover"
      fallbackSrc="/logo.png"
    />
    <div>
      <h3>{startup.name}</h3>
      <p>{startup.sector}</p>
    </div>
  </div>
);

// 4. Dans UserCard (pour les fondateurs)
export const UserCardExample = ({ user }: { user: User }) => (
  <div className="user-card">
    <ApiImage
      endpoint="users"
      id={user.id}
      alt={user.name}
      className="w-12 h-12 rounded-full object-cover"
      fallbackSrc="/logo.png"
    />
    <span>{user.name}</span>
  </div>
);

// 5. Pour les fondateurs de startups
export const FounderCardExample = ({ founder, startupId }: { founder: Founder, startupId: number }) => (
  <div className="founder-card">
    <ApiImage
      endpoint="founders"
      id={startupId}
      subId={founder.id}
      alt={founder.name}
      className="w-20 h-20 rounded-full object-cover"
      fallbackSrc="/logo.png"
    />
    <div>
      <h4>{founder.name}</h4>
      <p>Fondateur</p>
    </div>
  </div>
);
