const progressHand = async () => {
    const userID = getCookie("userID");

    if(!userID) {
        window.location.href = "http://localhost:3000/users/auth.html";
    }

    const tableID = document.getElementById("table-id").innerText;

    const data = {
        userID: userID,
        tableID: tableID,
    };

    const res = (await submitData("http://localhost:3000/routes/games/blackjack/progress-hand", data))[0];

    if(document.getElementById("progress-hand").className == "start-hand-button") {
        document.getElementById("progress-hand").style.display = "none";
    }
}

const createHitButton = () => {
    const hit = document.createElement("button");

    hit.id = "hit-button";
    hit.className = "hit-button";

    hit.onclick = async function () {
        const userID = getCookie("userID");

        if(!userID) {
            window.location.href = "http://localhost:3000/users/auth.html";
        }

        const tableID = document.getElementById("table-id").innerText;

        const data = {
            userID: userID,
            tableID: tableID,
        };

        const res = (await submitData("http://localhost:3000/routes/games/blackjack/hit", data))[0];

        // TODO: Needed logic?
    };

    hit.innerText = "Hit";

    return hit;
}

const createStandButton = () => {
    const stand = document.createElement("button");

    stand.id = "stand-button";
    stand.className = "stand-button";

    stand.onclick = async function () {
        const userID = getCookie("userID");

        if(!userID) {
            window.location.href = "http://localhost:3000/users/auth.html";
        }

        const tableID = document.getElementById("table-id").innerText;

        const data = {
            userID: userID,
            tableID: tableID,
        };

        const res = (await submitData("http://localhost:3000/routes/games/blackjack/stand", data))[0];

        // TODO: Needed logic?
    };

    stand.innerText = "Stand";

    return stand;
}

const redealCards = (activeUser, currentUser, data) => {
    if(!data.users) {
        return;
    }

    const users = data.users;

    if(!data.dealerCards) {
        return;
    }

    const dealerCards = data.dealerCards;

    document.getElementById("dealer-card-table").innerHTML = '';
    document.getElementById("user-card-table").innerHTML = '';

    for (let i = 0; i < users.length; i++) {
        const newHandLabel = document.createElement("div");

        newHandLabel.innerText = users[i].id;

        const newHand = document.createElement("div");

        newHand.appendChild(newHandLabel);

        newHand.id = "player-" + users[i].id + "-cards";

        newHand.className = "hand";

        if(!users[i].hand) {
            continue;
        }

        for (let j = 0; j < users[i].hand.length; j++) {
            generateCardGraphic(users[i].hand[j], newHand);
        }

        newHand.appendChild(document.createElement("br"));

        if(users[i].id == activeUser.id) {
            if(users[i].id == currentUser) {
                newHand.appendChild(createHitButton());
                newHand.appendChild(createStandButton());
            } else {
                const otherUserAction = document.createElement("p");

                otherUserAction.innerText = users[i].id + "'s Turn";

                newHand.appendChild(otherUserAction);
            }
        }

        document.getElementById("user-card-table").appendChild(newHand);
    }

    for (let i = 0; i < dealerCards.length; i++) {
        generateCardGraphic(dealerCards[i], document.getElementById("dealer-card-table"));
    }
}

const statusCheck = () => {
    setInterval(async function () {
        const userID = getCookie("userID");

        if(!userID) {
            window.location.href = "http://localhost:3000/users/auth.html";
        }

        const tableID = document.getElementById("table-id").innerText;

        const data = {
            tableID: tableID,
        };

        const res = (await submitData("http://localhost:3000/routes/games/blackjack/check-status", data))[0];

        const progressHandButton = document.getElementById("progress-hand");

        if (res.handEnded) {
            progressHandButton.style.display = "block";
            progressHandButton.innerText = "End Hand";
            progressHandButton.className = "end-hand-button";
        } else {
            progressHandButton.innerText = "Start Hand";
            progressHandButton.className = "start-hand-button";
        }

        if(res.newHand) {
            const userBet = res.users.find(x => x.id === userID).bet;

            document.getElementById("setup-window").style.display = "block";

            if(!userBet) {
                document.getElementById("bet-value-setup").innerText = '';
                document.getElementById("bet-value-input").style.display = "block";
                document.getElementById("place-bet-button").style.display = "block";
            }
        } else {
            document.getElementById("setup-window").style.display = "none";
        }

        if(!res.activeUser) {
            return;
        }

        redealCards(res.activeUser, userID, res);
    }, 1000);
}
