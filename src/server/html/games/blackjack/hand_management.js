const progressHand = async () => {
    const userID = document.getElementById("user-id-input").value;
    const tableID = document.getElementById("table-id-input").value;

    const data = {
        userID: userID,
        tableID: tableID,
    };

    const res = (await submitData("http://localhost:3000/routes/games/blackjack/progress-hand", data))[0];

    redealCards(res);
}

const hit = async () => {
    const userID = document.getElementById("user-id-input").value;
    const tableID = document.getElementById("table-id-input").value;

    const data = {
        userID: userID,
        tableID: tableID,
    };

    const res = (await submitData("http://localhost:3000/routes/games/blackjack/hit", data))[0];

    redealCards(res);
}

const stand = async () => {
    const userID = document.getElementById("user-id-input").value;
    const tableID = document.getElementById("table-id-input").value;

    const data = {
        userID: userID,
        tableID: tableID,
    };

    const res = (await submitData("http://localhost:3000/routes/games/blackjack/stand", data))[0];

    redealCards(res);
}

const statusCheck = () => {
    setInterval(async function () {
        const userID = document.getElementById("user-id-input").value;
        const tableID = document.getElementById("table-id-input").value;

        const data = {
            tableID: tableID,
        };

        const res = (await submitData("http://localhost:3000/routes/games/blackjack/check-status", data))[0];

        const progressHandButton = document.getElementById("progress-hand");
        if (res.handEnded) {
            progressHandButton.innerText = "End Hand";
        } else {
            progressHandButton.innerText = "Start Hand";
        }

        redealCards(res);

        if (res.activeUser.id == userID) {
            document.getElementById("hit-button").disabled = false;
            document.getElementById("stand-button").disabled = false;
        } else {
            document.getElementById("hit-button").disabled = true;
            document.getElementById("stand-button").disabled = true;
        }
    }, 1000);
}
