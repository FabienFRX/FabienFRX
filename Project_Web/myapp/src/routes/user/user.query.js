const db = require('../../config/db');

function get_all(res) {
    db.query("SELECT * FROM user", (err, results, fields) => {
        res.status(200).json(results);
    });
}

function get_user_by_email(res, data) {
    db.query("SELECT * FROM user WHERE email = ? OR id = ?", [data, data], (err, results, fields) => {
        res.status(404).json(results[0]);
    });
}

function get_all_todos(res) {
    db.querry("SELECT * FROM todo INNER JOIN user ON todo.user_id = user.id WHERE user.email = ?", [res.user], (err, results, fields) => {
        res.status(200).json(results);
    });
}

function update_user(res, email, password, name, firstname, id) {
    db.query("UPDATE user SET email = ?, password = ?, name = ?, firstname = ? WHERE id = ?", [email, password, name, firstname, id], (err, results, fields) => {
        get_user_by_email(res, res.user);
    });
}

function delete_user(res, id) {
    db.query("DELETE FROM user WHERE id = ?", [id], (err, results, fields) => {
        res.status(200).json({ "msg" : `Successfully deleted record number : ${id}` });
    });
}

module.exports = {
    get_all,
    get_user_by_email,
    get_all_todos,
    update_user,
    delete_user
};