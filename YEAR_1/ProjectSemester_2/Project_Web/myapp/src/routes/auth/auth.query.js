const db = require('../../config/db');
const jwt = require("jsonwebtoken");
const bcrypt = require('bcryptjs');

/*
 In JS:
 - an object looks like { key: value }
 - create an object using { token } is the same thing as { token: token }
*/

function add_user(res, email, password, name, firstname) {
    db.query("INSERT INTO user (email, password, name, firstname) VALUES (?, ?, ?, ?)",[email, password, name, firstname], (err, results, fields) => {
        console.log(`user insert :: ${JSON.stringify(results)}`);
        const token = jwt.sign({ user: email }, process.env.SECRET, { expiresIn: '1h' });
        res.status(200).json({ token });
    });
}

function user_already_exist(email, callback) {
    db.query("SELECT * FROM user WHERE email = ?", [email], (err, results, fields) => {
        if (results.length === 0) {
            callback(false);
        } else {
            callback(true);
        }
    })
}

function authentificate_user(res, email, password) {
    db.query("SELECT * FROM user WHERE email = ?", [email], (err, results, fields) => {
        if (err) {
            res.status(500).json({ "msg": "Database error", "error": err });
            return;
        }

        if (results.length === 0) {
            res.status(400).json({ "msg": "Invalid Credentials" });
            return;
        }

        const user = results[0];
        if (bcrypt.compareSync(password, user.password)) {
            const token = jwt.sign({ user: email }, process.env.SECRET, { expiresIn: '1h' });
            res.status(200).json({ token });
        } else {
            res.status(400).json({ "msg": "Invalid Credentials" });
        }
    });
}

module.exports = {
    add_user,
    user_already_exist,
    authentificate_user,
    authentificate_user
};