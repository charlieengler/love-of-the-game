const cards = [
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

    {
        name: "Ace of Clubs",
        graphic: "A♣️"
    },
    {
        name: "Two of Clubs",
        graphic: "2♣️"
    },
    {
        name: "Three of Clubs",
        graphic: "3♣️"
    },
    {
        name: "Four of Clubs",
        graphic: "4♣️"
    },
    {
        name: "Five of Clubs",
        graphic: "5♣️"
    },
    {
        name: "Six of Clubs",
        graphic: "6♣️"
    },
    {
        name: "Seven of Clubs",
        graphic: "7♣️"
    },
    {
        name: "Eight of Clubs",
        graphic: "8♣️"
    },
    {
        name: "Nine of Clubs",
        graphic: "9♣️"
    },
    {
        name: "Ten of Clubs",
        graphic: "10♣️"
    },
    {
        name: "Jack of Clubs",
        graphic: "J♣️"
    },
    {
        name: "Queen of Clubs",
        graphic: "Q♣️"
    },
    {
        name: "King of Clubs",
        graphic: "K♣️"
    },

    {
        name: "Ace of Hearts",
        graphic: "A♥️"
    },
    {
        name: "Two of Hearts",
        graphic: "2♥️"
    },
    {
        name: "Three of Hearts",
        graphic: "3♥️"
    },
    {
        name: "Four of Hearts",
        graphic: "4♥️"
    },
    {
        name: "Five of Hearts",
        graphic: "5♥️"
    },
    {
        name: "Six of Hearts",
        graphic: "6♥️"
    },
    {
        name: "Seven of Hearts",
        graphic: "7♥️"
    },
    {
        name: "Eight of Hearts",
        graphic: "8♥️"
    },
    {
        name: "Nine of Hearts",
        graphic: "9♥️"
    },
    {
        name: "Ten of Hearts",
        graphic: "10♥️"
    },
    {
        name: "Jack of Hearts",
        graphic: "J♥️"
    },
    {
        name: "Queen of Hearts",
        graphic: "Q♥️"
    },
    {
        name: "King of Hearts",
        graphic: "K♥️"
    },

    {
        name: "Ace of Spades",
        graphic: "A♠️"
    },
    {
        name: "Two of Spades",
        graphic: "2♠️"
    },
    {
        name: "Three of Spades",
        graphic: "3♠️"
    },
    {
        name: "Four of Spades",
        graphic: "4♠️"
    },
    {
        name: "Five of Spades",
        graphic: "5♠️"
    },
    {
        name: "Six of Spades",
        graphic: "6♠️"
    },
    {
        name: "Seven of Spades",
        graphic: "7♠️"
    },
    {
        name: "Eight of Spades",
        graphic: "8♠️"
    },
    {
        name: "Nine of Spades",
        graphic: "9♠️"
    },
    {
        name: "Ten of Spades",
        graphic: "10♠️"
    },
    {
        name: "Jack of Spades",
        graphic: "J♠️"
    },
    {
        name: "Queen of Spades",
        graphic: "Q♠️"
    },
    {
        name: "King of Spades",
        graphic: "K♠️"
    },

    {
        name: "Joker",
        graphic: "🤡"
    },
];

const parseCardIndexString = cardsString => {
    const cardsArray = [];

    const splitString = cardsString.split(",");

    for(let i = 0; i < splitString.length; i++) {
        cardsArray.push(cards[Number(splitString[i])]);
    }

    return cardsArray;
}

const generateCardGraphic = (card, slot) => {
    const cardContainer = document.createElement("div");
    cardContainer.style = "border:1px solid white; display:inline-block; height:150px; position:relative; width:100px;";

    let cardGraphic = document.createElement("div");
    cardGraphic.innerText = card.graphic;
    cardGraphic.style = "left:0; position:absolute; top:0;"
    cardContainer.appendChild(cardGraphic);

    const br = document.createElement("br");
    cardContainer.appendChild(br);

    cardGraphic = document.createElement("div");
    cardGraphic.innerText = card.graphic;
    cardGraphic.style = "bottom:0; position:absolute; right:0;"
    cardContainer.appendChild(cardGraphic);

    const cardSlot = document.getElementById(slot);

    cardSlot.appendChild(cardContainer);
}
