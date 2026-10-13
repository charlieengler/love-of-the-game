const joinTable = async () => {
    const userID = document.getElementById("user-id-input").value;
    const tableID = document.getElementById("table-id-input").value;

    const data = {
        userID: userID,
        tableID: tableID,
    };

    const res = (await submitData("http://localhost:3000/routes/games/blackjack/join-table", data))[0];

    document.getElementById("table-id").innerText = res.tableID;
    document.getElementById("user-id").innerText = res.userID;

    document.getElementById("user-id-input").style.display = "none";
    document.getElementById("table-id-input").style.display = "none";
    document.getElementById("join-table-button").style.display = "none";

    statusCheck();
}
