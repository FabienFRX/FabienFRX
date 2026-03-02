require('dotenv').config();
const express = require('express');
const app = express();
const bcrypt = require('bcryptjs');

app.use(express.urlencoded({ extended:true })); //get params
app.use(express.json()); //parse body
app.use(express.raw()); //get body as object

// error handling
app.use((err, _, res, __) => {
    console.error(err.stack);
    res.status(500).json({ "msg": "Internal server error" });
});

// load routes
require('./routes/user/user')(app);
require('./routes/auth/auth')(app);
require('./routes/todos/todos')(app);

// health of server (to know if it is up)
app.get('/', function (req, res) {
  res.send('System up\n');
});

// start server
app.listen(process.env.PORT, () => {
    console.log(`Serveur listening at http://localhost:${process.env.PORT}`);
});