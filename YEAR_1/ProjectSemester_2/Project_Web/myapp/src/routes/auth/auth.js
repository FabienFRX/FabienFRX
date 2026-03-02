const { add_user, user_already_exist, authentificate_user } = require('./auth.query');
const bcrypt = require('bcryptjs');
const jwt = require('jsonwebtoken');
const db = require('../../config/db');

module.exports = function(app) {
    app.post('/register', (req, res) => {
        const { email, password, name, firstname } = req.body;

        if (email === undefined || password === undefined || name === undefined || firstname === undefined) {
            res.status(400).json({ "msg": "Bad parameter" });
            return;
        }

        user_already_exist(email, (isUserExistent) => {
            if (isUserExistent) {
                res.status(400).json({ "msg": "Account already exists" });
            } else {
                add_user(res, email, bcrypt.hashSync(password, 10), name, firstname);
            }
        });
    });

    app.post('/login', (req, res) => {
        const { email, password } = req.body;

        if (email === undefined || password === undefined) {
            res.status(400).json({ "msg": "Bad parameter" });
            return;
        }

        authentificate_user(res, email, password);
    });
};
