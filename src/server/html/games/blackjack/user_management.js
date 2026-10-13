const placeBet = async () => {
    const userID = document.getElementById("user-id-input").value;
    const tableID = document.getElementById("table-id-input").value;
    const bet = document.getElementById("bet-value-input").value;

    const data = {
        userID: userID,
        tableID: tableID,
        bet: Number(bet),
    };

    const res = (await submitData("http://localhost:3000/routes/games/blackjack/bet", data))[0];

    document.getElementById("table-id").innerText = res.tableID;
    document.getElementById("user-id").innerText = res.userID;
    document.getElementById("bet-value").innerText = res.bet;
}
