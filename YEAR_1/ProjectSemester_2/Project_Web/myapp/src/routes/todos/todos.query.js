const db = require('../../config/db');

function get_todo(res) {
    db.query("SELECT * FROM todo", (err, results, fields) => {
        res.status(201).json(results);
    });
}

function get_todo_by_id(res, data) {
    db.query("SELECT * FROM todo WHERE id = ?", [data], (err, results, fields) => {
        res.status(200).json(results[0]);
    });
}

function update_todo(res, title, description, due_time, user_id, status, id) {
    db.query("UPDATE todo SET title = ?, description = ?, due_time = ?, user_id = ?, status = ? WHERE id = ?", [title, description, due_time, user_id, status, id], (err, results, fields) => {
        get_todo_by_id(res, id);
    });
}

function delete_todo(res, id) {
    db.query("DELETE FROM todo WHERE id = ?", [id], (err, results, fields) => {
        res.status(200).json({ "msg" : `Successfully deleted record number : ${id}` });
    });
}

function add_todo(res, title, description, due_time, user_id, status) {
    console.log(`Try to insert into todo :: ${JSON.stringify({ title, description, due_time, user_id, status })}`);
    db.query("INSERT INTO `todo` (`title`, `description`, `due_time`, `status`, `user_id`) VALUES (?, ?, ?, ?, ?)",[title, description, due_time, status, user_id], (err, results, fields) => {
        console.log(`todo insert :: ${JSON.stringify(results)}`);
        if (err) {
            console.log("ERROR WHEN INSERT INTO todo");
            console.log(err);
        }
        get_todo_by_id(res, results["insertId"]);
    });
}
module.exports = {
    get_todo,
    get_todo_by_id,
    update_todo,
    delete_todo,
    add_todo
};