const mysql = require('mysql2');

// Create the connection to database
const connection = mysql.createConnection({
    host: process.env.MYSQL_HOST,
    user: process.env.MYSQL_USER,
    password: process.env.MYSQL_ROOT_PASSWORD,
    database: process.env.MYSQL_DATABASE,
});

connection.connect((err) => {
    if (err) {
        console.log(`can't connect to db :: ${err}`);
        return;
    }
    console.log("connection to db ok");
});
module.exports = connection;