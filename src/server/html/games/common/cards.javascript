const cards = [
    "Joker",

    "One of Diamonds",
    "Two of Diamonds",
    "Three of Diamonds",
    "Four of Diamonds",
    "Five of Diamonds",
    "Six of Diamonds",
    "Seven of Diamonds",
    "Eight of Diamonds",
    "Nine of Diamonds",
    "Ten of Diamonds",
    "Jack of Diamonds",
    "Queen of Diamonds",
    "King of Diamonds",
    "Ace of Diamonds",

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
