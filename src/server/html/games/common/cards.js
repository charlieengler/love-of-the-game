const cards = [
    {
        name: "Joker",
        graphic: "🤡"
    },

    {
        name: "Ace of Diamonds",
        graphic: "A♦️"
    },
    {
        name: "Two of Diamonds",
        graphic: "2♦️"
    },
    {
        name: "Three of Diamonds",
        graphic: "3♦️"
    },
    {
        name: "Four of Diamonds",
        graphic: "4♦️"
    },
    {
        name: "Five of Diamonds",
        graphic: "5♦️"
    },
    {
        name: "Six of Diamonds",
        graphic: "6♦️"
    },
    {
        name: "Seven of Diamonds",
        graphic: "7♦️"
    },
    {
        name: "Eight of Diamonds",
        graphic: "8♦️"
    },
    {
        name: "Nine of Diamonds",
        graphic: "9♦️"
    },
    {
        name: "Ten of Diamonds",
        graphic: "10♦️"
    },
    {
        name: "Jack of Diamonds",
        graphic: "J♦️"
    },
    {
        name: "Queen of Diamonds",
        graphic: "Q♦️"
    },
    {
        name: "King of Diamonds",
        graphic: "K♦️"
    },

    "One of Clubs",
    "Two of Clubs",
    "Three of Clubs",
    "Four of Clubs",
    "Five of Clubs",
    "Six of Clubs",
    "Seven of Clubs",
    "Eight of Clubs",
    "Nine of Clubs",
    "Ten of Clubs",
    "Jack of Clubs",
    "Queen of Clubs",
    "King of Clubs",
    "Ace of Clubs",

    "One of Hearts",
    "Two of Hearts",
    "Three of Hearts",
    "Four of Hearts",
    "Five of Hearts",
    "Six of Hearts",
    "Seven of Hearts",
    "Eight of Hearts",
    "Nine of Hearts",
    "Ten of Hearts",
    "Jack of Hearts",
    "Queen of Hearts",
    "King of Hearts",
    "Ace of Hearts",

    "One of Spades",
    "Two of Spades",
    "Three of Spades",
    "Four of Spades",
    "Five of Spades",
    "Six of Spades",
    "Seven of Spades",
    "Eight of Spades",
    "Nine of Spades",
    "Ten of Spades",
    "Jack of Spades",
    "Queen of Spades",
    "King of Spades",
    "Ace of Spades",
];

const parseCardIndexString = cardsString => {
    const cardsArray = [];

    const splitString = cardsString.split(",");

    for(let i = 0; i < splitString.length; i++) {
        cardsArray.push(cards[Number(splitString[i])]);
    }

    return cardsArray;
}

const generateCardGraphic = card => {

}
