import { Injectable } from '@nestjs/common';

export interface SimpleQuestion {
  id: number;
  question: string;
  options: string[];
  correctAnswer: number;
  explanation: string;
}

@Injectable()
export class SimpleQuizService {
  // Quiz basique avec 3 questions - Facile à tester
  getSimpleQuiz(): SimpleQuestion[] {
    return [
      {
        id: 1,
        question: "Quelle est la couleur du ciel par temps clair ?",
        options: ["Bleu", "Vert", "Rouge", "Jaune"],
        correctAnswer: 0,
        explanation: "Le ciel est bleu à cause de la diffusion de la lumière du soleil par l'atmosphère terrestre."
      },
      {
        id: 2,
        question: "Combien font 2 + 2 ?",
        options: ["3", "4", "5", "6"],
        correctAnswer: 1,
        explanation: "2 + 2 = 4. C'est une addition simple."
      },
      {
        id: 3,
        question: "Quel animal est connu pour produire du miel ?",
        options: ["Le chien", "Le chat", "L'abeille", "La vache"],
        correctAnswer: 2,
        explanation: "Les abeilles produisent du miel en récoltant le nectar des fleurs."
      }
    ];
  }

  // Quiz avec 3 questions de culture générale
  getCultureQuiz(): SimpleQuestion[] {
    return [
      {
        id: 1,
        question: "Qui a peint la Joconde ?",
        options: ["Pablo Picasso", "Léonard de Vinci", "Vincent Van Gogh", "Claude Monet"],
        correctAnswer: 1,
        explanation: "Léonard de Vinci a peint la Joconde (Mona Lisa) entre 1503 et 1519."
      },
      {
        id: 2,
        question: "Quelle est la capitale de l'Italie ?",
        options: ["Milan", "Venise", "Rome", "Florence"],
        correctAnswer: 2,
        explanation: "Rome est la capitale de l'Italie et une des plus anciennes villes d'Europe."
      },
      {
        id: 3,
        question: "Combien de continents y a-t-il sur Terre ?",
        options: ["5", "6", "7", "8"],
        correctAnswer: 2,
        explanation: "Il y a 7 continents : Afrique, Antarctique, Asie, Europe, Amérique du Nord, Océanie et Amérique du Sud."
      }
    ];
  }

  // Quiz science avec 3 questions
  getScienceQuiz(): SimpleQuestion[] {
    return [
      {
        id: 1,
        question: "Quelle est la planète la plus proche du Soleil ?",
        options: ["Vénus", "Terre", "Mercure", "Mars"],
        correctAnswer: 2,
        explanation: "Mercure est la planète la plus proche du Soleil, à environ 58 millions de km."
      },
      {
        id: 2,
        question: "Quel est le symbole chimique de l'eau ?",
        options: ["H2O", "O2", "CO2", "H2"],
        correctAnswer: 0,
        explanation: "L'eau a pour formule chimique H2O (2 atomes d'hydrogène et 1 atome d'oxygène)."
      },
      {
        id: 3,
        question: "Combien d'os compte le corps humain adulte ?",
        options: ["186", "206", "226", "246"],
        correctAnswer: 1,
        explanation: "Le squelette humain adulte compte 206 os."
      }
    ];
  }
}
