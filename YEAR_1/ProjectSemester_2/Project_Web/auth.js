const { add_user, user_already_exist, add_login } = require('./auth.query');
const bcrypt = require("bcryptjs");

module.exports = function(app) {
    app.post('/register', (req, res) => {
        const {email, password, name, firstname} = req.body;

        if (email === undefined || password === undefined || name === undefined || firstname === undefined) res.status(400).json({ "msg": "Bad parameter"});
        user_already_exist(email, (isUserExistent) => {
            if (isUserExistent) {
                res.status(400).json({ "msg": "Account already exists" });
            } else {
                add_user(res, email, bcrypt.hashSync(password, 10), name, firstname);
            }
        })
    });

    app.post('/login', (req, res) => {
        const {email, password} = req.body;

        user_already_exist(email, (isUserExistent) => {
            if (isUserExistent) {
                res.status(400).json({ "msg": " Invalid Credentials " });
            } else {
                add_login(res, email, bcrypt.hashSync(password, 10));
            }
        // TODO: check user information + send token
        })
    });
}