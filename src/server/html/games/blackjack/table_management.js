const joinTable = async () => {
    const userID = getCookie("userID");

    if(!userID) {
        window.location.href = "http://localhost:3000/users/auth.html";
    }

    const tableID = document.getElementById("table-id-input").value;

    const data = {
        userID: userID,
        tableID: tableID,
    };

    const res = (await submitData("http://localhost:3000/routes/games/blackjack/join-table", data))[0];

    document.getElementById("table-id").innerText = res.tableID;
    document.getElementById("user-id").innerText = res.userID;

    document.getElementById("table-id-input").style.display = "none";
    document.getElementById("table-id-setup").innerText = res.tableID;
    document.getElementById("join-table-button").style.display = "none";

    statusCheck();
}
