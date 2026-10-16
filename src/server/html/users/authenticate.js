const login = async () => {
    const userID = document.getElementById("login-username-input").value;
    const password = document.getElementById("login-password-input").value;

    // TODO: Sending password over network as plaintext lmao
    const data = {
        userID: userID,
        password: password,
    };

    const res = (await submitData("http://localhost:3000/routes/users/auth/login", data))[0];

    if(res.logged_in) {
        setCookie("userID", userID, 365);

        window.location.href = "http://localhost:3000/users/profile.html";
    }
}

const find = async () => {
    const userID = document.getElementById("login-username-input").value;

    const data = {
        userID: userID,
    };

    const res = (await submitData("http://localhost:3000/routes/users/auth/find", data))[0];

    if(!res.foundUser) {
        document.getElementById("login-form").style.display = 'none';
        document.getElementById("signup-form").style.display = 'block';
        document.getElementById("signup-username-input").value = userID;

        return;
    }

    document.getElementById("find-button").style.display = 'none';
    document.getElementById("login-password-input").style.display = 'block';
    document.getElementById("login-button").style.display = 'block';

    return res;
}

const signup = async () => {
    const userID = document.getElementById("signup-username-input").value;
    const password = document.getElementById("signup-password-input").value;

    // TODO: Sending password over network as plaintext lmao
    const data = {
        userID: userID,
        password: password,
    };

    const res = (await submitData("http://localhost:3000/routes/users/auth/signup", data))[0];

    if(!res.createdUser) {
        // TODO: Fail
    }

    document.getElementById("login-username-input").value = userID;
    document.getElementById("login-password-input").value = password;

    // TODO: Error checking
    login();
}

const logout = async () => {
    // TODO: Check to make sure the cookie is actually set
    const userID = getCookie("userID");

    const data = {
        userID: userID,
    };

    const res = (await submitData("http://localhost:3000/routes/users/auth/logout", data))[0];

    if(!res.loggedOut) {
        // TODO: Fail
        console.log("Failed to log out");
    }

    setCookie("userID", null, -1);

    window.location.href = "http://localhost:3000/index.html";
}
