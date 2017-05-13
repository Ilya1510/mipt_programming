CREATE TABLE Clients (
    id INTEGER,
    first_name VARCHAR(30) NOT NULL,
    second_name VARCHAR(30) NOT NULL,
    age INTEGER,
    PRIMARY KEY (id)
);

CREATE TABLE Dishes (
    id INTEGER,
    name VARCHAR(30) NOT NULL,
    caloricity INTEGER,
    cost INTEGER,
    weight INTEGER,
    PRIMARY KEY (id)
);

CREATE TABLE Ingridients (
    id INTEGER,
    name VARCHAR(30) NOT NULL,
    remainder INTEGER,
    PRIMARY KEY (id)
);

CREATE TABLE Composition (
    dish_id INTEGER,
    ingr_id INTEGER,
    quantity INTEGER,
    PRIMARY KEY (dish_id, ingr_id),
    FOREIGN KEY (dish_id) REFERENCES Dishes(id),
    FOREIGN KEY (ingr_id) REFERENCES Ingridients(id)
);

CREATE TABLE Marks (
    mark_id INTEGER,
    client_id INTEGER,
    dish_id INTEGER,
    mark INTEGER,
    details VARCHAR(30),
    PRIMARY KEY (mark_id),
    FOREIGN KEY (client_id) REFERENCES Clients(id),
    FOREIGN KEY (dish_id) REFERENCES Dishes(id),
    CONSTRAINT mark_check CHECK (mark BETWEEN 0 AND 10)
);

