import sqlite3 as sql
import sys


def create_tables(cursor):
    with open('create.sql', 'r') as content_file:
        content = content_file.read()
        cursor.executescript(content)


def drop_tables(cursor):
    with open('drop.sql', 'r') as content_file:
        content = content_file.read()
        cursor.executescript(content)


def fill_tables(cursor):
    with open('fill_db.sql', 'r') as content_file:
        content = content_file.read()
        cursor.executescript(content)


def find_dish_by_ingrs(cursor, need_ingrs):
    count = len(need_ingrs)
    questions = ", ?" * (count - 1)
    need_ingrs.append(count)
    res = cursor.execute("SELECT Dishes.name FROM "
                          "Dishes INNER JOIN Composition ON Dishes.id = Composition.dish_id "
                          "INNER JOIN Ingridients AS I1 ON Composition.ingr_id = I1.id "
                          "WHERE I1.name in (?" + questions +
                          ") GROUP BY dish_id HAVING Count(I1.name "
                          ")=?;", need_ingrs)
    ans = []
    for i in res:
        ans.append(i[0])
    return ans


def read_ingrs_from_stdin():
    lines = sys.stdin.readlines()
    for i in range(len(lines)):
        lines[i] = lines[i].strip()
    return lines
    # list(map(lambda x: x.strip(), sys.stdin.readlines()))


def main():
    conn = sql.connect('restaurant.db')
    cursor = conn.cursor()
    cursor.execute("PRAGMA foreign_keys = 1")
    drop_tables(cursor)
    create_tables(cursor)
    fill_tables(cursor)
    products = read_ingrs_from_stdin()
    answer = find_dish_by_ingrs(cursor, products)
    print(len(answer))
    for row in answer:
        print(row)
main()
