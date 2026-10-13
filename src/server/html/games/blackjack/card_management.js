const redealCards = data => {
    const users = data.users;
    const dealerCards = data.dealerCards;

    document.getElementById("dealer-card-table").innerHTML = '';
    document.getElementById("user-card-table").innerHTML = '';

    for (let i = 0; i < users.length; i++) {
        const newHandLabel = document.createElement("div");

        newHandLabel.innerText = users[i].id + "'s Hand:";

        document.getElementById("user-card-table").appendChild(newHandLabel);

        const newHand = document.createElement("div");

        newHand.id = "player-" + users[i].id + "-cards";

        document.getElementById("user-card-table").appendChild(newHand);
    }

    for (let i = 0; i < dealerCards.length; i++) {
        generateCardGraphic(dealerCards[i], "dealer-card-table");
    }

    let k = 0;
    for (let i = 0; i < users.length; i++) {
        for (let j = 0; j < users[i].hand.length; j++) {
            generateCardGraphic(users[i].hand[j], "player-" + users[i].id + "-cards");

            k++;
        }
    }
}
