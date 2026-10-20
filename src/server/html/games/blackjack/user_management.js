const placeBet = async () => {
    const userID = getCookie("userID");

    if(!userID) {
        window.location.href = "http://localhost:3000/users/auth.html";
    }

    const tableID = document.getElementById("table-id").innerText;
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

    document.getElementById("setup-window").style.display = "none";
}
