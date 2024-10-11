const { get_all, get_user_by_email, update_user, delete_user, get_all_todos } = require('./user.query');
const authentification = require("../../middleware/auth");

module.exports = function(app) {
    app.get('/user', authentification, (req, res) => {
        get_user_by_email(res, res.user);
    });

    app.get('/user/todos', authentification, (req, res) => {
        get_all_todos(res);
    });

    app.get('/users/:data', authentification, (req, res) => {
        get_user_by_email(res, req.params.data)
    });

    app.put('/users/:id', authentification, (req, res) => {
        const {email, password, name, firstname} = req.body;
        const {id} = req.params;
        update_user(res, email, password, name, firstname, id)
    });

    app.delete('/users/:id', authentification, (req, res) => {
        delete_user(res, req.params.id);
    });
}