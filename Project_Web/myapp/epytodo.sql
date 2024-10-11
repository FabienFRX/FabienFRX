-- avoid datetime error -> https://stackoverflow.com/questions/44304777/er-truncated-wrong-value-incorrect-datetime-value
SET SESSION sql_mode = 'NO_ENGINE_SUBSTITUTION';
SET GLOBAL sql_mode = 'NO_ENGINE_SUBSTITUTION';

CREATE DATABASE epytodo;

use epytodo;

CREATE TABLE user (
    id INT NOT NULL AUTO_INCREMENT,
    email varchar(255) NOT NULL,
    password varchar(255) NOT NULL,
    name varchar(255) NOT NULL,
    firstname varchar(255) NOT NULL,
    created_at DATETIME DEFAULT NOW(),
    PRIMARY KEY (id)
);

CREATE TABLE todo (
    id INT NOT NULL AUTO_INCREMENT,
    title varchar(255) NOT NULL,
    description varchar(255) NOT NULL,
    created_at DATETIME DEFAULT NOW(),
    due_time DATETIME NOT NULL,
    status varchar(255) NOT NULL,
    user_id INT NOT NULL,
    CONSTRAINT fk_user FOREIGN KEY (user_id) REFERENCES user(id),
    PRIMARY KEY (id)
);

-- status ENUM("not started", "todo", "in progress", "done") DEFAULT "not started", 
-- foreign key = realtion between 2 tables