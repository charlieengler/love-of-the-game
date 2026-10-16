const login = async () => {
    const userID = document.getElementById("login-username-input").value;
    const password = document.getElementById("login-password-input").value;

    // TODO: Sending password over network as plaintext lmao
    const data = {
        userID: userID,
        password: password,
    };

    const res = (await submitData("http://localhost:3000/routes/users/auth/login", data))[0];

    console.log(res);
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

    // TODO: Set cookies with result

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
