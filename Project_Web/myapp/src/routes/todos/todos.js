const { get_todo, get_todo_by_id, update_todo, delete_todo, add_todo } = require('./todos.query');
const authentification = require("../../middleware/auth");

module.exports = function(app) {
    app.get('/todo', authentification, (req, res) => {
        get_todo(res, res.user);
    });

    app.get('/todos/:id', authentification, (req, res) => {
        const {id} = req.params;
        get_todo_by_id(res, id);
    });

    app.put('/todos/:id', authentification, (req, res) => {
        const {title, description, due_time, user_id, status} = req.body;
        const {id} = req.params;
        update_todo(res, title, description, due_time, user_id, status, id);
    });

    app.delete('/todos/:id', authentification, (req, res) => {
        delete_todo(res, req.params.id);
    });

    app.post('/todos', authentification, (req, res) => {
        const {title, description, due_time, user_id, status} = req.body;
        if (title === undefined || description === undefined || due_time === undefined || user_id === undefined || status === undefined) res.status(400).json({ "msg": "Bad parameter"});
        add_todo(res, title, description, due_time, user_id, status);
    });
}