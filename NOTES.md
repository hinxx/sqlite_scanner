
# How ATTACH works

The ATTACH statement adds a new database file to the catalog that can be read from and written to.

ATTACH allows DuckDB to operate on multiple database files, and allows for transfer of data between different database files.

The DETACH statement allows previously attached database files to be closed and detached, releasing any locks held on the database file.


https://duckdb.org/docs/sql/statements/attach


## Example with sqlite3 extension

Using ATTACH and *not* calling "use sakila;" the call to "PRAGMA show_tables;" results in an error?!


D install 'build/debug/extension/sqlite_scanner/sqlite_scanner.duckdb_extension';
D load 'build/debug/extension/sqlite_scanner/sqlite_scanner.duckdb_extension';


D SHOW databases;
┌───────────────┐
│ database_name │
│    varchar    │
├───────────────┤
│ memory        │
└───────────────┘
D PRAGMA show_tables;
┌─────────┐
│  name   │
│ varchar │
├─────────┤
│ 0 rows  │
└─────────┘


D ATTACH 'sakila.db' (TYPE sqlite);
D SHOW databases;
┌───────────────┐
│ database_name │
│    varchar    │
├───────────────┤
│ memory        │
│ sakila        │
└───────────────┘
D PRAGMA show_tables;
Error: Catalog Error: Table with name customer does not exist!
Did you mean "sakila.customer"?

D use sakila;
D PRAGMA show_tables;
┌──────────────────────────────────┐
│               name               │
│             varchar              │
├──────────────────────────────────┤
│ actor                            │
│ address                          │
│ category                         │
│ city                             │
│ country                          │
│ customer                         │
│ customer_list                    │
│ film                             │
│ film_actor                       │
│ film_category                    │
│ film_list                        │
│ film_text                        │
│ idx_actor_last_name              │
│ idx_customer_fk_address_id       │
│ idx_customer_fk_store_id         │
│ idx_customer_last_name           │
│ idx_fk_city_id                   │
│ idx_fk_country_id                │
│ idx_fk_customer_id               │
│ idx_fk_film_actor_actor          │
│       ·                          │
│       ·                          │
│       ·                          │
│ sales_by_store                   │
│ sqlite_autoindex_actor_1         │
│ sqlite_autoindex_address_1       │
│ sqlite_autoindex_category_1      │
│ sqlite_autoindex_city_1          │
│ sqlite_autoindex_country_1       │
│ sqlite_autoindex_customer_1      │
│ sqlite_autoindex_film_1          │
│ sqlite_autoindex_film_actor_1    │
│ sqlite_autoindex_film_category_1 │
│ sqlite_autoindex_film_text_1     │
│ sqlite_autoindex_inventory_1     │
│ sqlite_autoindex_language_1      │
│ sqlite_autoindex_payment_1       │
│ sqlite_autoindex_rental_1        │
│ sqlite_autoindex_staff_1         │
│ sqlite_autoindex_store_1         │
│ staff                            │
│ staff_list                       │
│ store                            │
├──────────────────────────────────┤
│        61 rows (40 shown)        │
└──────────────────────────────────┘


If we instead do "CALL sqlite_attach('sakila.db');", "use sakila;" in not required and
we can do "PRAGMA show_tables;" right away:

D install 'build/debug/extension/sqlite_scanner/sqlite_scanner.duckdb_extension';
D load 'build/debug/extension/sqlite_scanner/sqlite_scanner.duckdb_extension';
D CALL sqlite_attach('sakila.db');
┌─────────┐
│ Success │
│ boolean │
├─────────┤
│ 0 rows  │
└─────────┘
D SHOW databases;
┌───────────────┐
│ database_name │
│    varchar    │
├───────────────┤
│ memory        │
└───────────────┘
D PRAGMA show_tables;
┌────────────────────────┐
│          name          │
│        varchar         │
├────────────────────────┤
│ actor                  │
│ address                │
│ category               │
│ city                   │
│ country                │
│ customer               │
│ customer_list          │
│ film                   │
│ film_actor             │
│ film_category          │
│ film_list              │
│ film_text              │
│ inventory              │
│ language               │
│ payment                │
│ rental                 │
│ sales_by_film_category │
│ sales_by_store         │
│ staff                  │
│ staff_list             │
│ store                  │
├────────────────────────┤
│        21 rows         │
└────────────────────────┘

## Execution of ATTACH

D install 'build/debug/extension/sqlite_scanner/sqlite_scanner.duckdb_extension';
D load 'build/debug/extension/sqlite_scanner/sqlite_scanner.duckdb_extension';
>>> SqliteScanFunction
>>> SqliteAttachFunction
>>> SQLiteStorageExtension

D ATTACH 'sakila.db' (TYPE sqlite);
>>> SQLiteAttach
>>> SQLiteCreateTransactionManager


## Execution of sqlite_attach

What happens when the "CALL sqlite_attach('sakila.db');" is called?

D install 'build/debug/extension/sqlite_scanner/sqlite_scanner.duckdb_extension';
D load 'build/debug/extension/sqlite_scanner/sqlite_scanner.duckdb_extension';
>>> SqliteScanFunction
>>> SqliteAttachFunction
D CALL sqlite_attach('sakila.db');


First the attach bind is called once.
>>> AttachBind

The the attach function is called once..

>>> AttachFunction

This is before the call to Sqlite "db.GetTables();" call.
Now we iterate over the table names returned by the SQL:  SELECT name FROM sqlite_master WHERE type='table'
The views are created and later used in part2.

>>> AttachFunction part1

SqliteBind == sqlite_scan table function
Not sure why the SqliteBind is called three times for each table here?!

>>> SqliteBind file_name: sakila.db table_name: actor
>>> SqliteBind name: actor_id type: DOUBLE
>>> SqliteBind name: first_name type: VARCHAR
>>> SqliteBind name: last_name type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: actor
>>> SqliteBind name: actor_id type: DOUBLE
>>> SqliteBind name: first_name type: VARCHAR
>>> SqliteBind name: last_name type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: actor
>>> SqliteBind name: actor_id type: DOUBLE
>>> SqliteBind name: first_name type: VARCHAR
>>> SqliteBind name: last_name type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: country
>>> SqliteBind name: country_id type: BIGINT
>>> SqliteBind name: country type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: country
>>> SqliteBind name: country_id type: BIGINT
>>> SqliteBind name: country type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: country
>>> SqliteBind name: country_id type: BIGINT
>>> SqliteBind name: country type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: city
>>> SqliteBind name: city_id type: BIGINT
>>> SqliteBind name: city type: VARCHAR
>>> SqliteBind name: country_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: city
>>> SqliteBind name: city_id type: BIGINT
>>> SqliteBind name: city type: VARCHAR
>>> SqliteBind name: country_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: city
>>> SqliteBind name: city_id type: BIGINT
>>> SqliteBind name: city type: VARCHAR
>>> SqliteBind name: country_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: address
>>> SqliteBind name: address_id type: BIGINT
>>> SqliteBind name: address type: VARCHAR
>>> SqliteBind name: address2 type: VARCHAR
>>> SqliteBind name: district type: VARCHAR
>>> SqliteBind name: city_id type: BIGINT
>>> SqliteBind name: postal_code type: VARCHAR
>>> SqliteBind name: phone type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: address
>>> SqliteBind name: address_id type: BIGINT
>>> SqliteBind name: address type: VARCHAR
>>> SqliteBind name: address2 type: VARCHAR
>>> SqliteBind name: district type: VARCHAR
>>> SqliteBind name: city_id type: BIGINT
>>> SqliteBind name: postal_code type: VARCHAR
>>> SqliteBind name: phone type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: address
>>> SqliteBind name: address_id type: BIGINT
>>> SqliteBind name: address type: VARCHAR
>>> SqliteBind name: address2 type: VARCHAR
>>> SqliteBind name: district type: VARCHAR
>>> SqliteBind name: city_id type: BIGINT
>>> SqliteBind name: postal_code type: VARCHAR
>>> SqliteBind name: phone type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: language
>>> SqliteBind name: language_id type: BIGINT
>>> SqliteBind name: name type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: language
>>> SqliteBind name: language_id type: BIGINT
>>> SqliteBind name: name type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: language
>>> SqliteBind name: language_id type: BIGINT
>>> SqliteBind name: name type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: category
>>> SqliteBind name: category_id type: BIGINT
>>> SqliteBind name: name type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: category
>>> SqliteBind name: category_id type: BIGINT
>>> SqliteBind name: name type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: category
>>> SqliteBind name: category_id type: BIGINT
>>> SqliteBind name: name type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: customer
>>> SqliteBind name: customer_id type: BIGINT
>>> SqliteBind name: store_id type: BIGINT
>>> SqliteBind name: first_name type: VARCHAR
>>> SqliteBind name: last_name type: VARCHAR
>>> SqliteBind name: email type: VARCHAR
>>> SqliteBind name: address_id type: BIGINT
>>> SqliteBind name: active type: VARCHAR
>>> SqliteBind name: create_date type: TIMESTAMP
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: customer
>>> SqliteBind name: customer_id type: BIGINT
>>> SqliteBind name: store_id type: BIGINT
>>> SqliteBind name: first_name type: VARCHAR
>>> SqliteBind name: last_name type: VARCHAR
>>> SqliteBind name: email type: VARCHAR
>>> SqliteBind name: address_id type: BIGINT
>>> SqliteBind name: active type: VARCHAR
>>> SqliteBind name: create_date type: TIMESTAMP
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: customer
>>> SqliteBind name: customer_id type: BIGINT
>>> SqliteBind name: store_id type: BIGINT
>>> SqliteBind name: first_name type: VARCHAR
>>> SqliteBind name: last_name type: VARCHAR
>>> SqliteBind name: email type: VARCHAR
>>> SqliteBind name: address_id type: BIGINT
>>> SqliteBind name: active type: VARCHAR
>>> SqliteBind name: create_date type: TIMESTAMP
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: film
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: title type: VARCHAR
>>> SqliteBind name: description type: VARCHAR
>>> SqliteBind name: release_year type: VARCHAR
>>> SqliteBind name: language_id type: BIGINT
>>> SqliteBind name: original_language_id type: BIGINT
>>> SqliteBind name: rental_duration type: BIGINT
>>> SqliteBind name: rental_rate type: DOUBLE
>>> SqliteBind name: length type: BIGINT
>>> SqliteBind name: replacement_cost type: DOUBLE
>>> SqliteBind name: rating type: VARCHAR
>>> SqliteBind name: special_features type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: film
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: title type: VARCHAR
>>> SqliteBind name: description type: VARCHAR
>>> SqliteBind name: release_year type: VARCHAR
>>> SqliteBind name: language_id type: BIGINT
>>> SqliteBind name: original_language_id type: BIGINT
>>> SqliteBind name: rental_duration type: BIGINT
>>> SqliteBind name: rental_rate type: DOUBLE
>>> SqliteBind name: length type: BIGINT
>>> SqliteBind name: replacement_cost type: DOUBLE
>>> SqliteBind name: rating type: VARCHAR
>>> SqliteBind name: special_features type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: film
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: title type: VARCHAR
>>> SqliteBind name: description type: VARCHAR
>>> SqliteBind name: release_year type: VARCHAR
>>> SqliteBind name: language_id type: BIGINT
>>> SqliteBind name: original_language_id type: BIGINT
>>> SqliteBind name: rental_duration type: BIGINT
>>> SqliteBind name: rental_rate type: DOUBLE
>>> SqliteBind name: length type: BIGINT
>>> SqliteBind name: replacement_cost type: DOUBLE
>>> SqliteBind name: rating type: VARCHAR
>>> SqliteBind name: special_features type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: film_actor
>>> SqliteBind name: actor_id type: BIGINT
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: film_actor
>>> SqliteBind name: actor_id type: BIGINT
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: film_actor
>>> SqliteBind name: actor_id type: BIGINT
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: film_category
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: category_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: film_category
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: category_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: film_category
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: category_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: film_text
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: title type: VARCHAR
>>> SqliteBind name: description type: VARCHAR
>>> SqliteBind file_name: sakila.db table_name: film_text
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: title type: VARCHAR
>>> SqliteBind name: description type: VARCHAR
>>> SqliteBind file_name: sakila.db table_name: film_text
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: title type: VARCHAR
>>> SqliteBind name: description type: VARCHAR
>>> SqliteBind file_name: sakila.db table_name: inventory
>>> SqliteBind name: inventory_id type: BIGINT
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: store_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: inventory
>>> SqliteBind name: inventory_id type: BIGINT
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: store_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: inventory
>>> SqliteBind name: inventory_id type: BIGINT
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: store_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: staff
>>> SqliteBind name: staff_id type: BIGINT
>>> SqliteBind name: first_name type: VARCHAR
>>> SqliteBind name: last_name type: VARCHAR
>>> SqliteBind name: address_id type: BIGINT
>>> SqliteBind name: picture type: BLOB
>>> SqliteBind name: email type: VARCHAR
>>> SqliteBind name: store_id type: BIGINT
>>> SqliteBind name: active type: BIGINT
>>> SqliteBind name: username type: VARCHAR
>>> SqliteBind name: password type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: staff
>>> SqliteBind name: staff_id type: BIGINT
>>> SqliteBind name: first_name type: VARCHAR
>>> SqliteBind name: last_name type: VARCHAR
>>> SqliteBind name: address_id type: BIGINT
>>> SqliteBind name: picture type: BLOB
>>> SqliteBind name: email type: VARCHAR
>>> SqliteBind name: store_id type: BIGINT
>>> SqliteBind name: active type: BIGINT
>>> SqliteBind name: username type: VARCHAR
>>> SqliteBind name: password type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: staff
>>> SqliteBind name: staff_id type: BIGINT
>>> SqliteBind name: first_name type: VARCHAR
>>> SqliteBind name: last_name type: VARCHAR
>>> SqliteBind name: address_id type: BIGINT
>>> SqliteBind name: picture type: BLOB
>>> SqliteBind name: email type: VARCHAR
>>> SqliteBind name: store_id type: BIGINT
>>> SqliteBind name: active type: BIGINT
>>> SqliteBind name: username type: VARCHAR
>>> SqliteBind name: password type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: store
>>> SqliteBind name: store_id type: BIGINT
>>> SqliteBind name: manager_staff_id type: BIGINT
>>> SqliteBind name: address_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: store
>>> SqliteBind name: store_id type: BIGINT
>>> SqliteBind name: manager_staff_id type: BIGINT
>>> SqliteBind name: address_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: store
>>> SqliteBind name: store_id type: BIGINT
>>> SqliteBind name: manager_staff_id type: BIGINT
>>> SqliteBind name: address_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: payment
>>> SqliteBind name: payment_id type: BIGINT
>>> SqliteBind name: customer_id type: BIGINT
>>> SqliteBind name: staff_id type: BIGINT
>>> SqliteBind name: rental_id type: BIGINT
>>> SqliteBind name: amount type: DOUBLE
>>> SqliteBind name: payment_date type: TIMESTAMP
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: payment
>>> SqliteBind name: payment_id type: BIGINT
>>> SqliteBind name: customer_id type: BIGINT
>>> SqliteBind name: staff_id type: BIGINT
>>> SqliteBind name: rental_id type: BIGINT
>>> SqliteBind name: amount type: DOUBLE
>>> SqliteBind name: payment_date type: TIMESTAMP
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: payment
>>> SqliteBind name: payment_id type: BIGINT
>>> SqliteBind name: customer_id type: BIGINT
>>> SqliteBind name: staff_id type: BIGINT
>>> SqliteBind name: rental_id type: BIGINT
>>> SqliteBind name: amount type: DOUBLE
>>> SqliteBind name: payment_date type: TIMESTAMP
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: rental
>>> SqliteBind name: rental_id type: BIGINT
>>> SqliteBind name: rental_date type: TIMESTAMP
>>> SqliteBind name: inventory_id type: BIGINT
>>> SqliteBind name: customer_id type: BIGINT
>>> SqliteBind name: return_date type: TIMESTAMP
>>> SqliteBind name: staff_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: rental
>>> SqliteBind name: rental_id type: BIGINT
>>> SqliteBind name: rental_date type: TIMESTAMP
>>> SqliteBind name: inventory_id type: BIGINT
>>> SqliteBind name: customer_id type: BIGINT
>>> SqliteBind name: return_date type: TIMESTAMP
>>> SqliteBind name: staff_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: rental
>>> SqliteBind name: rental_id type: BIGINT
>>> SqliteBind name: rental_date type: TIMESTAMP
>>> SqliteBind name: inventory_id type: BIGINT
>>> SqliteBind name: customer_id type: BIGINT
>>> SqliteBind name: return_date type: TIMESTAMP
>>> SqliteBind name: staff_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP


Here we are executing SQL: SELECT sql FROM sqlite_master WHERE type='view'
These are the views that were created in the part1 above.
We can see the SQL of the views; this is the same SQL as seen in sqlite3 cli
by executing the same select statement..

>>> AttachFunction part2
>>> AttachFunction view SQL CREATE VIEW customer_list
AS
SELECT cu.customer_id AS ID,
       cu.first_name||' '||cu.last_name AS name,
       a.address AS address,
       a.postal_code AS zip_code,
       a.phone AS phone,
       city.city AS city,
       country.country AS country,
       case when cu.active=1 then 'active' else '' end AS notes,
       cu.store_id AS SID
FROM customer AS cu JOIN address AS a ON cu.address_id = a.address_id JOIN city ON a.city_id = city.city_id
    JOIN country ON city.country_id = country.country_id

The above SQL is executed!
At this point we seem to be calling sqlite_scan bind function for each of the tables
involved in the view (in that order).

>>> SqliteBind file_name: sakila.db table_name: customer
>>> SqliteBind name: customer_id type: BIGINT
>>> SqliteBind name: store_id type: BIGINT
>>> SqliteBind name: first_name type: VARCHAR
>>> SqliteBind name: last_name type: VARCHAR
>>> SqliteBind name: email type: VARCHAR
>>> SqliteBind name: address_id type: BIGINT
>>> SqliteBind name: active type: VARCHAR
>>> SqliteBind name: create_date type: TIMESTAMP
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: address
>>> SqliteBind name: address_id type: BIGINT
>>> SqliteBind name: address type: VARCHAR
>>> SqliteBind name: address2 type: VARCHAR
>>> SqliteBind name: district type: VARCHAR
>>> SqliteBind name: city_id type: BIGINT
>>> SqliteBind name: postal_code type: VARCHAR
>>> SqliteBind name: phone type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: city
>>> SqliteBind name: city_id type: BIGINT
>>> SqliteBind name: city type: VARCHAR
>>> SqliteBind name: country_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: country
>>> SqliteBind name: country_id type: BIGINT
>>> SqliteBind name: country type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP


>>> AttachFunction view SQL CREATE VIEW film_list
AS
SELECT film.film_id AS FID,
       film.title AS title,
       film.description AS description,
       category.name AS category,
       film.rental_rate AS price,
       film.length AS length,
       film.rating AS rating,
       actor.first_name||' '||actor.last_name AS actors
FROM category LEFT JOIN film_category ON category.category_id = film_category.category_id LEFT JOIN film ON film_category.film_id = film.film_id
        JOIN film_actor ON film.film_id = film_actor.film_id
    JOIN actor ON film_actor.actor_id = actor.actor_id
>>> SqliteBind file_name: sakila.db table_name: category
>>> SqliteBind name: category_id type: BIGINT
>>> SqliteBind name: name type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: film_category
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: category_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: film
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: title type: VARCHAR
>>> SqliteBind name: description type: VARCHAR
>>> SqliteBind name: release_year type: VARCHAR
>>> SqliteBind name: language_id type: BIGINT
>>> SqliteBind name: original_language_id type: BIGINT
>>> SqliteBind name: rental_duration type: BIGINT
>>> SqliteBind name: rental_rate type: DOUBLE
>>> SqliteBind name: length type: BIGINT
>>> SqliteBind name: replacement_cost type: DOUBLE
>>> SqliteBind name: rating type: VARCHAR
>>> SqliteBind name: special_features type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: film_actor
>>> SqliteBind name: actor_id type: BIGINT
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: actor
>>> SqliteBind name: actor_id type: DOUBLE
>>> SqliteBind name: first_name type: VARCHAR
>>> SqliteBind name: last_name type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> AttachFunction view SQL CREATE VIEW staff_list
AS
SELECT s.staff_id AS ID,
       s.first_name||' '||s.last_name AS name,
       a.address AS address,
       a.postal_code AS zip_code,
       a.phone AS phone,
       city.city AS city,
       country.country AS country,
       s.store_id AS SID
FROM staff AS s JOIN address AS a ON s.address_id = a.address_id JOIN city ON a.city_id = city.city_id
    JOIN country ON city.country_id = country.country_id
>>> SqliteBind file_name: sakila.db table_name: staff
>>> SqliteBind name: staff_id type: BIGINT
>>> SqliteBind name: first_name type: VARCHAR
>>> SqliteBind name: last_name type: VARCHAR
>>> SqliteBind name: address_id type: BIGINT
>>> SqliteBind name: picture type: BLOB
>>> SqliteBind name: email type: VARCHAR
>>> SqliteBind name: store_id type: BIGINT
>>> SqliteBind name: active type: BIGINT
>>> SqliteBind name: username type: VARCHAR
>>> SqliteBind name: password type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: address
>>> SqliteBind name: address_id type: BIGINT
>>> SqliteBind name: address type: VARCHAR
>>> SqliteBind name: address2 type: VARCHAR
>>> SqliteBind name: district type: VARCHAR
>>> SqliteBind name: city_id type: BIGINT
>>> SqliteBind name: postal_code type: VARCHAR
>>> SqliteBind name: phone type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: city
>>> SqliteBind name: city_id type: BIGINT
>>> SqliteBind name: city type: VARCHAR
>>> SqliteBind name: country_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: country
>>> SqliteBind name: country_id type: BIGINT
>>> SqliteBind name: country type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> AttachFunction view SQL CREATE VIEW sales_by_store
AS
SELECT
  s.store_id
 ,c.city||','||cy.country AS store
 ,m.first_name||' '||m.last_name AS manager
 ,SUM(p.amount) AS total_sales
FROM payment AS p
INNER JOIN rental AS r ON p.rental_id = r.rental_id
INNER JOIN inventory AS i ON r.inventory_id = i.inventory_id
INNER JOIN store AS s ON i.store_id = s.store_id
INNER JOIN address AS a ON s.address_id = a.address_id
INNER JOIN city AS c ON a.city_id = c.city_id
INNER JOIN country AS cy ON c.country_id = cy.country_id
INNER JOIN staff AS m ON s.manager_staff_id = m.staff_id
GROUP BY
  s.store_id
, c.city||','||cy.country
, m.first_name||' '||m.last_name
>>> SqliteBind file_name: sakila.db table_name: payment
>>> SqliteBind name: payment_id type: BIGINT
>>> SqliteBind name: customer_id type: BIGINT
>>> SqliteBind name: staff_id type: BIGINT
>>> SqliteBind name: rental_id type: BIGINT
>>> SqliteBind name: amount type: DOUBLE
>>> SqliteBind name: payment_date type: TIMESTAMP
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: rental
>>> SqliteBind name: rental_id type: BIGINT
>>> SqliteBind name: rental_date type: TIMESTAMP
>>> SqliteBind name: inventory_id type: BIGINT
>>> SqliteBind name: customer_id type: BIGINT
>>> SqliteBind name: return_date type: TIMESTAMP
>>> SqliteBind name: staff_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: inventory
>>> SqliteBind name: inventory_id type: BIGINT
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: store_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: store
>>> SqliteBind name: store_id type: BIGINT
>>> SqliteBind name: manager_staff_id type: BIGINT
>>> SqliteBind name: address_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: address
>>> SqliteBind name: address_id type: BIGINT
>>> SqliteBind name: address type: VARCHAR
>>> SqliteBind name: address2 type: VARCHAR
>>> SqliteBind name: district type: VARCHAR
>>> SqliteBind name: city_id type: BIGINT
>>> SqliteBind name: postal_code type: VARCHAR
>>> SqliteBind name: phone type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: city
>>> SqliteBind name: city_id type: BIGINT
>>> SqliteBind name: city type: VARCHAR
>>> SqliteBind name: country_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: country
>>> SqliteBind name: country_id type: BIGINT
>>> SqliteBind name: country type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: staff
>>> SqliteBind name: staff_id type: BIGINT
>>> SqliteBind name: first_name type: VARCHAR
>>> SqliteBind name: last_name type: VARCHAR
>>> SqliteBind name: address_id type: BIGINT
>>> SqliteBind name: picture type: BLOB
>>> SqliteBind name: email type: VARCHAR
>>> SqliteBind name: store_id type: BIGINT
>>> SqliteBind name: active type: BIGINT
>>> SqliteBind name: username type: VARCHAR
>>> SqliteBind name: password type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> AttachFunction view SQL CREATE VIEW sales_by_film_category
AS
SELECT
c.name AS category
, SUM(p.amount) AS total_sales
FROM payment AS p
INNER JOIN rental AS r ON p.rental_id = r.rental_id
INNER JOIN inventory AS i ON r.inventory_id = i.inventory_id
INNER JOIN film AS f ON i.film_id = f.film_id
INNER JOIN film_category AS fc ON f.film_id = fc.film_id
INNER JOIN category AS c ON fc.category_id = c.category_id
GROUP BY c.name
>>> SqliteBind file_name: sakila.db table_name: payment
>>> SqliteBind name: payment_id type: BIGINT
>>> SqliteBind name: customer_id type: BIGINT
>>> SqliteBind name: staff_id type: BIGINT
>>> SqliteBind name: rental_id type: BIGINT
>>> SqliteBind name: amount type: DOUBLE
>>> SqliteBind name: payment_date type: TIMESTAMP
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: rental
>>> SqliteBind name: rental_id type: BIGINT
>>> SqliteBind name: rental_date type: TIMESTAMP
>>> SqliteBind name: inventory_id type: BIGINT
>>> SqliteBind name: customer_id type: BIGINT
>>> SqliteBind name: return_date type: TIMESTAMP
>>> SqliteBind name: staff_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: inventory
>>> SqliteBind name: inventory_id type: BIGINT
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: store_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: film
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: title type: VARCHAR
>>> SqliteBind name: description type: VARCHAR
>>> SqliteBind name: release_year type: VARCHAR
>>> SqliteBind name: language_id type: BIGINT
>>> SqliteBind name: original_language_id type: BIGINT
>>> SqliteBind name: rental_duration type: BIGINT
>>> SqliteBind name: rental_rate type: DOUBLE
>>> SqliteBind name: length type: BIGINT
>>> SqliteBind name: replacement_cost type: DOUBLE
>>> SqliteBind name: rating type: VARCHAR
>>> SqliteBind name: special_features type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: film_category
>>> SqliteBind name: film_id type: BIGINT
>>> SqliteBind name: category_id type: BIGINT
>>> SqliteBind name: last_update type: TIMESTAMP
>>> SqliteBind file_name: sakila.db table_name: category
>>> SqliteBind name: category_id type: BIGINT
>>> SqliteBind name: name type: VARCHAR
>>> SqliteBind name: last_update type: TIMESTAMP

┌─────────┐
│ Success │
│ boolean │
├─────────┤
│ 0 rows  │
└─────────┘

## selecting data from a table / view

Added more printfs to other functions and removed some noisy one compared to above.
Big number in front of some lines is the thread id.

D install 'build/debug/extension/sqlite_scanner/sqlite_scanner.duckdb_extension';
D load 'build/debug/extension/sqlite_scanner/sqlite_scanner.duckdb_extension';
>>> SqliteScanFunction
>>> SqliteAttachFunction
>>> SQLiteStorageExtension
D CALL sqlite_attach('sakila.db');
>>> AttachBind
>>> AttachFunction
>>> AttachFunction part1
139975414130688 >>> SqliteBind file_name: sakila.db table_name: actor
139975414130688 >>> SqliteBind max_rowid: 200 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: actor
139975414130688 >>> SqliteBind max_rowid: 200 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: actor
139975414130688 >>> SqliteBind max_rowid: 200 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: country
139975414130688 >>> SqliteBind max_rowid: 109 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: country
139975414130688 >>> SqliteBind max_rowid: 109 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: country
139975414130688 >>> SqliteBind max_rowid: 109 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: city
139975414130688 >>> SqliteBind max_rowid: 600 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: city
139975414130688 >>> SqliteBind max_rowid: 600 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: city
139975414130688 >>> SqliteBind max_rowid: 600 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: address
139975414130688 >>> SqliteBind max_rowid: 603 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: address
139975414130688 >>> SqliteBind max_rowid: 603 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: address
139975414130688 >>> SqliteBind max_rowid: 603 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: language
139975414130688 >>> SqliteBind max_rowid: 6 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: language
139975414130688 >>> SqliteBind max_rowid: 6 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: language
139975414130688 >>> SqliteBind max_rowid: 6 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: category
139975414130688 >>> SqliteBind max_rowid: 16 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: category
139975414130688 >>> SqliteBind max_rowid: 16 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: category
139975414130688 >>> SqliteBind max_rowid: 16 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: customer
139975414130688 >>> SqliteBind max_rowid: 599 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: customer
139975414130688 >>> SqliteBind max_rowid: 599 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: customer
139975414130688 >>> SqliteBind max_rowid: 599 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: film
139975414130688 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: film
139975414130688 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: film
139975414130688 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: film_actor
139975414130688 >>> SqliteBind max_rowid: 5462 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: film_actor
139975414130688 >>> SqliteBind max_rowid: 5462 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: film_actor
139975414130688 >>> SqliteBind max_rowid: 5462 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: film_category
139975414130688 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: film_category
139975414130688 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: film_category
139975414130688 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: film_text
139975414130688 >>> SqliteBind max_rowid: 0 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: film_text
139975414130688 >>> SqliteBind max_rowid: 0 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: film_text
139975414130688 >>> SqliteBind max_rowid: 0 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: inventory
139975414130688 >>> SqliteBind max_rowid: 4581 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: inventory
139975414130688 >>> SqliteBind max_rowid: 4581 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: inventory
139975414130688 >>> SqliteBind max_rowid: 4581 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: staff
139975414130688 >>> SqliteBind max_rowid: 2 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: staff
139975414130688 >>> SqliteBind max_rowid: 2 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: staff
139975414130688 >>> SqliteBind max_rowid: 2 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: store
139975414130688 >>> SqliteBind max_rowid: 2 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: store
139975414130688 >>> SqliteBind max_rowid: 2 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: store
139975414130688 >>> SqliteBind max_rowid: 2 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: payment
139975414130688 >>> SqliteBind max_rowid: 16049 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: payment
139975414130688 >>> SqliteBind max_rowid: 16049 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: payment
139975414130688 >>> SqliteBind max_rowid: 16049 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: rental
139975414130688 >>> SqliteBind max_rowid: 16044 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: rental
139975414130688 >>> SqliteBind max_rowid: 16044 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: rental
139975414130688 >>> SqliteBind max_rowid: 16044 rows_per_group: 122880
>>> AttachFunction part2
>>> AttachFunction view SQL:
CREATE VIEW customer_list
AS
SELECT cu.customer_id AS ID,
       cu.first_name||' '||cu.last_name AS name,
       a.address AS address,
       a.postal_code AS zip_code,
       a.phone AS phone,
       city.city AS city,
       country.country AS country,
       case when cu.active=1 then 'active' else '' end AS notes,
       cu.store_id AS SID
FROM customer AS cu JOIN address AS a ON cu.address_id = a.address_id JOIN city ON a.city_id = city.city_id
    JOIN country ON city.country_id = country.country_id
139975414130688 >>> SqliteBind file_name: sakila.db table_name: customer
139975414130688 >>> SqliteBind max_rowid: 599 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: address
139975414130688 >>> SqliteBind max_rowid: 603 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: city
139975414130688 >>> SqliteBind max_rowid: 600 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: country
139975414130688 >>> SqliteBind max_rowid: 109 rows_per_group: 122880
>>> AttachFunction view SQL:
CREATE VIEW film_list
AS
SELECT film.film_id AS FID,
       film.title AS title,
       film.description AS description,
       category.name AS category,
       film.rental_rate AS price,
       film.length AS length,
       film.rating AS rating,
       actor.first_name||' '||actor.last_name AS actors
FROM category LEFT JOIN film_category ON category.category_id = film_category.category_id LEFT JOIN film ON film_category.film_id = film.film_id
        JOIN film_actor ON film.film_id = film_actor.film_id
    JOIN actor ON film_actor.actor_id = actor.actor_id
139975414130688 >>> SqliteBind file_name: sakila.db table_name: category
139975414130688 >>> SqliteBind max_rowid: 16 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: film_category
139975414130688 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: film
139975414130688 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: film_actor
139975414130688 >>> SqliteBind max_rowid: 5462 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: actor
139975414130688 >>> SqliteBind max_rowid: 200 rows_per_group: 122880
>>> AttachFunction view SQL:
CREATE VIEW staff_list
AS
SELECT s.staff_id AS ID,
       s.first_name||' '||s.last_name AS name,
       a.address AS address,
       a.postal_code AS zip_code,
       a.phone AS phone,
       city.city AS city,
       country.country AS country,
       s.store_id AS SID
FROM staff AS s JOIN address AS a ON s.address_id = a.address_id JOIN city ON a.city_id = city.city_id
    JOIN country ON city.country_id = country.country_id
139975414130688 >>> SqliteBind file_name: sakila.db table_name: staff
139975414130688 >>> SqliteBind max_rowid: 2 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: address
139975414130688 >>> SqliteBind max_rowid: 603 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: city
139975414130688 >>> SqliteBind max_rowid: 600 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: country
139975414130688 >>> SqliteBind max_rowid: 109 rows_per_group: 122880
>>> AttachFunction view SQL:
CREATE VIEW sales_by_store
AS
SELECT
  s.store_id
 ,c.city||','||cy.country AS store
 ,m.first_name||' '||m.last_name AS manager
 ,SUM(p.amount) AS total_sales
FROM payment AS p
INNER JOIN rental AS r ON p.rental_id = r.rental_id
INNER JOIN inventory AS i ON r.inventory_id = i.inventory_id
INNER JOIN store AS s ON i.store_id = s.store_id
INNER JOIN address AS a ON s.address_id = a.address_id
INNER JOIN city AS c ON a.city_id = c.city_id
INNER JOIN country AS cy ON c.country_id = cy.country_id
INNER JOIN staff AS m ON s.manager_staff_id = m.staff_id
GROUP BY
  s.store_id
, c.city||','||cy.country
, m.first_name||' '||m.last_name
139975414130688 >>> SqliteBind file_name: sakila.db table_name: payment
139975414130688 >>> SqliteBind max_rowid: 16049 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: rental
139975414130688 >>> SqliteBind max_rowid: 16044 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: inventory
139975414130688 >>> SqliteBind max_rowid: 4581 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: store
139975414130688 >>> SqliteBind max_rowid: 2 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: address
139975414130688 >>> SqliteBind max_rowid: 603 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: city
139975414130688 >>> SqliteBind max_rowid: 600 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: country
139975414130688 >>> SqliteBind max_rowid: 109 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: staff
139975414130688 >>> SqliteBind max_rowid: 2 rows_per_group: 122880
>>> AttachFunction view SQL:
CREATE VIEW sales_by_film_category
AS
SELECT
c.name AS category
, SUM(p.amount) AS total_sales
FROM payment AS p
INNER JOIN rental AS r ON p.rental_id = r.rental_id
INNER JOIN inventory AS i ON r.inventory_id = i.inventory_id
INNER JOIN film AS f ON i.film_id = f.film_id
INNER JOIN film_category AS fc ON f.film_id = fc.film_id
INNER JOIN category AS c ON fc.category_id = c.category_id
GROUP BY c.name
139975414130688 >>> SqliteBind file_name: sakila.db table_name: payment
139975414130688 >>> SqliteBind max_rowid: 16049 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: rental
139975414130688 >>> SqliteBind max_rowid: 16044 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: inventory
139975414130688 >>> SqliteBind max_rowid: 4581 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: film
139975414130688 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: film_category
139975414130688 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139975414130688 >>> SqliteBind file_name: sakila.db table_name: category
139975414130688 >>> SqliteBind max_rowid: 16 rows_per_group: 122880
┌─────────┐
│ Success │
│ boolean │
├─────────┤
│ 0 rows  │
└─────────┘

Select some data from one of the tables, limit the output for clarity.
We can see how the sqlite_scanner.cpp methods are being invoked in order.

D SELECT * FROM film where length > 100 limit 10;
139656391624704 >>> SqliteBind file_name: sakila.db table_name: film
139656391624704 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139656391624704 >>> SqliteInitGlobalState
139656391624704 >>> SqliteInitLocalState global db =>> result->db 0
139656391624704 >>> SqliteParallelStateNext gstate.position 0 bind_data->max_rowid 1000 status 1
139656391624704 >>> SqliteInitInternal
139656391624704 >>> SqliteInitInternal created local_state.db 0x60700033fc10

This is the prepared thread - local SQL statement that will be executed by the SqliteScan.
If there were more rows there would be more threads; here we see a single thread in action.

139656391624704 >>> SqliteInitInternal prepared SQL:
SELECT "film_id", "title", "description", "release_year", "language_id", "original_language_id", "rental_duration", "rental_rate", "length", "replacement_cost", "rating", "special_features", "last_update" FROM "film" WHERE ROWID BETWEEN 0 AND 122879


After the preparations and setup of the thread SQL statement it is executed by the SqliteScan.
SqliteScan will parse the statement result row-by-row in steps and fill in the values for each
column according to its data type.

If maximum output vector row count is reached (STANDARD_VECTOR_SIZE=2048) we exit the SqliteScan.
Other duckdb code will in this case notice that not all the input rows in the column were scanned and
it will repeat the above process of initializing local state with parameters for handling next batch of
rows..
Otherwise (all input rows were handled) the local state done flag is set to true and we finish processing
this table scan. Other duckdb code now has all the rows/columns of the query and they get printed..


139656391624704 >>> SqliteScan SELECT "film_id", "title", "description", "release_year", "language_id", "original_language_id", "rental_duration", "rental_rate", "length", "replacement_cost", "rating", "special_features", "last_update" FROM "film" WHERE ROWID BETWEEN 0 AND 122879
139656391624704 >>> SqliteScan out_idx 1000 !has_more
139656391624704 >>> SqliteScan SELECT "film_id", "title", "description", "release_year", "language_id", "original_language_id", "rental_duration", "rental_rate", "length", "replacement_cost", "rating", "special_features", "last_update" FROM "film" WHERE ROWID BETWEEN 0 AND 122879
139656391624704 >>> SqliteParallelStateNext gstate.position 122880 bind_data->max_rowid 1000 status 0

We can see above that there were only 1000 rows to be inspected and we did not have to execute the SqliteScan
more than once.

┌─────────┬──────────────────┬──────────────────────┬──────────────┬───┬────────┬──────────────────┬─────────┬──────────────────────┬─────────────────────┐
│ film_id │      title       │     description      │ release_year │ … │ length │ replacement_cost │ rating  │   special_features   │     last_update     │
│  int64  │     varchar      │       varchar        │   varchar    │   │ int64  │      double      │ varchar │       varchar        │      timestamp      │
├─────────┼──────────────────┼──────────────────────┼──────────────┼───┼────────┼──────────────────┼─────────┼──────────────────────┼─────────────────────┤
│       4 │ AFFAIR PREJUDICE │ A Fanciful Documen…  │ 2006         │ … │    117 │            26.99 │ G       │ Commentaries,Behin…  │ 2021-03-06 15:52:00 │
│       5 │ AFRICAN EGG      │ A Fast-Paced Docum…  │ 2006         │ … │    130 │            22.99 │ G       │ Deleted Scenes       │ 2021-03-06 15:52:00 │
│       6 │ AGENT TRUMAN     │ A Intrepid Panoram…  │ 2006         │ … │    169 │            17.99 │ PG      │ Deleted Scenes       │ 2021-03-06 15:52:00 │
│       9 │ ALABAMA DEVIL    │ A Thoughtful Panor…  │ 2006         │ … │    114 │            21.99 │ PG-13   │ Trailers,Deleted S…  │ 2021-03-06 15:52:00 │
│      11 │ ALAMO VIDEOTAPE  │ A Boring Epistle o…  │ 2006         │ … │    126 │            16.99 │ G       │ Commentaries,Behin…  │ 2021-03-06 15:52:00 │
│      12 │ ALASKA PHANTOM   │ A Fanciful Saga of…  │ 2006         │ … │    136 │            22.99 │ PG      │ Commentaries,Delet…  │ 2021-03-06 15:52:00 │
│      13 │ ALI FOREVER      │ A Action-Packed Dr…  │ 2006         │ … │    150 │            21.99 │ PG      │ Deleted Scenes,Beh…  │ 2021-03-06 15:52:00 │
│      16 │ ALLEY EVOLUTION  │ A Fast-Paced Drama…  │ 2006         │ … │    180 │            23.99 │ NC-17   │ Trailers,Commentar…  │ 2021-03-06 15:52:00 │
│      19 │ AMADEUS HOLY     │ A Emotional Displa…  │ 2006         │ … │    113 │            20.99 │ PG      │ Commentaries,Delet…  │ 2021-03-06 15:52:00 │
│      21 │ AMERICAN CIRCUS  │ A Insightful Drama…  │ 2006         │ … │    129 │            17.99 │ R       │ Commentaries,Behin…  │ 2021-03-06 15:52:00 │
├─────────┴──────────────────┴──────────────────────┴──────────────┴───┴────────┴──────────────────┴─────────┴──────────────────────┴─────────────────────┤
│ 10 rows                                                                                                                            13 columns (9 shown) │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘


Select some data from one of the views that contains several tables, limit the output for clarity.
Initial part repeats as with a single table, but now there are more tables to scan.

D SELECT * FROM sales_by_store limit 10;
139656391624704 >>> SqliteBind file_name: sakila.db table_name: payment
139656391624704 >>> SqliteBind max_rowid: 16049 rows_per_group: 122880
139656391624704 >>> SqliteBind file_name: sakila.db table_name: rental
139656391624704 >>> SqliteBind max_rowid: 16044 rows_per_group: 122880
139656391624704 >>> SqliteBind file_name: sakila.db table_name: inventory
139656391624704 >>> SqliteBind max_rowid: 4581 rows_per_group: 122880
139656391624704 >>> SqliteBind file_name: sakila.db table_name: store
139656391624704 >>> SqliteBind max_rowid: 2 rows_per_group: 122880
139656391624704 >>> SqliteBind file_name: sakila.db table_name: address
139656391624704 >>> SqliteBind max_rowid: 603 rows_per_group: 122880
139656391624704 >>> SqliteBind file_name: sakila.db table_name: city
139656391624704 >>> SqliteBind max_rowid: 600 rows_per_group: 122880
139656391624704 >>> SqliteBind file_name: sakila.db table_name: country
139656391624704 >>> SqliteBind max_rowid: 109 rows_per_group: 122880
139656391624704 >>> SqliteBind file_name: sakila.db table_name: staff
139656391624704 >>> SqliteBind max_rowid: 2 rows_per_group: 122880

For each table above a separate global state is created.


139656391624704 >>> SqliteInitGlobalState
139656391624704 >>> SqliteInitGlobalState
139656391624704 >>> SqliteInitGlobalState
139656391624704 >>> SqliteInitGlobalState
139656391624704 >>> SqliteInitGlobalState
139656391624704 >>> SqliteInitGlobalState
139656391624704 >>> SqliteInitGlobalState
139656391624704 >>> SqliteInitGlobalState
139656391624704 >>> SqliteInitLocalState global db =>> result->db 0
139656391624704 >>> SqliteParallelStateNext gstate.position 0 bind_data->max_rowid 109 status 1
139656391624704 >>> SqliteInitInternal
139656260830976 >>> SqliteInitLocalState global db =>> result->db 1396562523481600 >>> SqliteInitLocalState
139656260830976 >>> SqliteParallelStateNext global db =>> result->db  gstate.position 0
139656252348160 >>> SqliteParallelStateNext gstate.position 0 bind_data->max_rowid 4581 status 1
139656252348160 >>> SqliteInitInternal
0 bind_data->max_rowid 2 status 1
139656260830976 >>> SqliteInitInternal
139656260830976 >>> SqliteInitInternal created local_state.db 0x60700032a820
139656252348160 >>> SqliteInitInternal created local_state.db 0x6070003138a0
139656391624704 >>> SqliteInitInternal created local_state.db 0x607000313130
139656260830976139656252348160 >>> SqliteInitInternal prepared SQL:
SELECT "staff_id", "first_name", "last_name" FROM "staff" WHERE ROWID BETWEEN 0 AND 122879
 >>> SqliteInitInternal prepared SQL:
SELECT "inventory_id", "store_id" FROM "inventory" WHERE ROWID BETWEEN 0 AND 122879
139656391624704 >>> SqliteInitInternal prepared SQL:
SELECT "country_id", "country" FROM "country" WHERE ROWID BETWEEN 0 AND 122879
139656391624704139656260830976 >>> SqliteScan >>> SqliteScanSELECT "country_id", "country" FROM "country" WHERE ROWID BETWEEN 0 AND 122879SELECT "staff_id", "first_name", "last_name" FROM "staff" WHERE ROWID BETWEEN 0 AND 122879

139656260830976 >>> SqliteScan out_idx 2 !has_more
139656252348160 >>> SqliteScanSELECT "inventory_id", "store_id" FROM "inventory" WHERE ROWID BETWEEN 0 AND 122879
139656391624704 >>> SqliteScan out_idx 109 !has_more
139656260830976 >>> SqliteScanSELECT "staff_id", "first_name", "last_name" FROM "staff" WHERE ROWID BETWEEN 0 AND 122879
139656260830976 >>> SqliteParallelStateNext gstate.position 122880 bind_data->max_rowid 2 status 0
139656391624704 >>> SqliteScanSELECT "country_id", "country" FROM "country" WHERE ROWID BETWEEN 0 AND 122879
139656391624704 >>> SqliteParallelStateNext gstate.position 122880 bind_data->max_rowid 109 status 0
139656235382528 >>> SqliteInitLocalState global db =>> result->db 0
139656235382528 >>> SqliteParallelStateNext gstate.position 0 bind_data->max_rowid 2 status 1
139656235382528 >>> SqliteInitInternal
139656269313792 >>> SqliteInitLocalState global db =>> result->db 0
139656269313792 >>> SqliteParallelStateNext gstate.position 0 bind_data->max_rowid 600 status 1
139656269313792 >>> SqliteInitInternal
139656235382528 >>> SqliteInitInternal created local_state.db 0x60700033a810
139656235382528 >>> SqliteInitInternal prepared SQL:
SELECT "store_id", "manager_staff_id", "address_id" FROM "store" WHERE ROWID BETWEEN 0 AND 122879
139656269313792 >>> SqliteInitInternal created local_state.db 0x60700006e040
139656269313792 >>> SqliteInitInternal prepared SQL:
SELECT "city_id", "city", "country_id" FROM "city" WHERE ROWID BETWEEN 0 AND 122879
139656252348160 >>> SqliteScan out_idx 2048 == STANDARD_VECTOR_SIZE
139656252348160 >>> SqliteScanSELECT "inventory_id", "store_id" FROM "inventory" WHERE ROWID BETWEEN 0 AND 122879
139656235382528 >>> SqliteScanSELECT "store_id", "manager_staff_id", "address_id" FROM "store" WHERE ROWID BETWEEN 0 AND 122879
139656235382528 >>> SqliteScan out_idx 2 !has_more
139656235382528 >>> SqliteScanSELECT "store_id", "manager_staff_id", "address_id" FROM "store" WHERE ROWID BETWEEN 0 AND 122879
139656235382528 >>> SqliteParallelStateNext gstate.position 122880 bind_data->max_rowid 2 status 0
139656252348160 >>> SqliteScan out_idx 2048 == STANDARD_VECTOR_SIZE
139656269313792 >>> SqliteScanSELECT "city_id", "city", "country_id" FROM "city" WHERE ROWID BETWEEN 0 AND 122879
139656252348160 >>> SqliteScanSELECT "inventory_id", "store_id" FROM "inventory" WHERE ROWID BETWEEN 0 AND 122879
139656252348160 >>> SqliteScan out_idx 485 !has_more
139656252348160 >>> SqliteScanSELECT "inventory_id", "store_id" FROM "inventory" WHERE ROWID BETWEEN 0 AND 122879
139656252348160 >>> SqliteParallelStateNext gstate.position 122880 bind_data->max_rowid 4581 status 0
139656303212288 >>> SqliteInitLocalState global db =>> result->db 0
139656303212288 >>> SqliteParallelStateNext gstate.position 0 bind_data->max_rowid 16044 status 1
139656303212288 >>> SqliteInitInternal
139656303212288 >>> SqliteInitInternal created local_state.db 0x6070000538c0
139656303212288 >>> SqliteInitInternal prepared SQL:
SELECT "rental_id", "inventory_id" FROM "rental" WHERE ROWID BETWEEN 0 AND 122879
139656269313792 >>> SqliteScan out_idx 600 !has_more
139656269313792 >>> SqliteScanSELECT "city_id", "city", "country_id" FROM "city" WHERE ROWID BETWEEN 0 AND 122879
139656269313792 >>> SqliteParallelStateNext gstate.position 122880 bind_data->max_rowid 600 status 0
139656391624704 >>> SqliteInitLocalState global db =>> result->db 0
139656391624704 >>> SqliteParallelStateNext gstate.position 0 bind_data->max_rowid 603 status 1
139656391624704 >>> SqliteInitInternal
139656391624704 >>> SqliteInitInternal created local_state.db 0x60700033a1f0
139656391624704 >>> SqliteInitInternal prepared SQL:
SELECT "address_id", "city_id" FROM "address" WHERE ROWID BETWEEN 0 AND 122879
139656303212288 >>> SqliteScanSELECT "rental_id", "inventory_id" FROM "rental" WHERE ROWID BETWEEN 0 AND 122879
139656391624704 >>> SqliteScanSELECT "address_id", "city_id" FROM "address" WHERE ROWID BETWEEN 0 AND 122879
139656391624704 >>> SqliteScan out_idx 603 !has_more
139656303212288 >>> SqliteScan out_idx 2048 == STANDARD_VECTOR_SIZE
139656391624704 >>> SqliteScanSELECT "address_id", "city_id" FROM "address" WHERE ROWID BETWEEN 0 AND 122879
139656391624704 >>> SqliteParallelStateNext gstate.position 122880 bind_data->max_rowid 603 status 0
139656303212288 >>> SqliteScanSELECT "rental_id", "inventory_id" FROM "rental" WHERE ROWID BETWEEN 0 AND 122879
139656303212288 >>> SqliteScan out_idx 2048 == STANDARD_VECTOR_SIZE
139656303212288 >>> SqliteScanSELECT "rental_id", "inventory_id" FROM "rental" WHERE ROWID BETWEEN 0 AND 122879
139656303212288 >>> SqliteScan out_idx 2048 == STANDARD_VECTOR_SIZE
139656303212288 >>> SqliteScanSELECT "rental_id", "inventory_id" FROM "rental" WHERE ROWID BETWEEN 0 AND 122879
139656303212288 >>> SqliteScan out_idx 2048 == STANDARD_VECTOR_SIZE
139656303212288 >>> SqliteScanSELECT "rental_id", "inventory_id" FROM "rental" WHERE ROWID BETWEEN 0 AND 122879
139656303212288 >>> SqliteScan out_idx 2048 == STANDARD_VECTOR_SIZE
139656303212288 >>> SqliteScanSELECT "rental_id", "inventory_id" FROM "rental" WHERE ROWID BETWEEN 0 AND 122879
139656303212288 >>> SqliteScan out_idx 2048 == STANDARD_VECTOR_SIZE
139656303212288 >>> SqliteScanSELECT "rental_id", "inventory_id" FROM "rental" WHERE ROWID BETWEEN 0 AND 122879
139656303212288 >>> SqliteScan out_idx 2048 == STANDARD_VECTOR_SIZE
139656303212288 >>> SqliteScanSELECT "rental_id", "inventory_id" FROM "rental" WHERE ROWID BETWEEN 0 AND 122879
139656303212288 >>> SqliteScan out_idx 1708 !has_more
139656303212288 >>> SqliteScanSELECT "rental_id", "inventory_id" FROM "rental" WHERE ROWID BETWEEN 0 AND 122879
139656303212288 >>> SqliteParallelStateNext gstate.position 122880 bind_data->max_rowid 16044 status 0
139656391624704 >>> SqliteInitLocalState global db =>> result->db 0
139656391624704 >>> SqliteParallelStateNext gstate.position 0 bind_data->max_rowid 16049 status 1
139656391624704 >>> SqliteInitInternal
139656391624704 >>> SqliteInitInternal created local_state.db 0x60700035ffe0
139656391624704 >>> SqliteInitInternal prepared SQL:
SELECT "rental_id", "amount" FROM "payment" WHERE ROWID BETWEEN 0 AND 122879
139656391624704 >>> SqliteScanSELECT "rental_id", "amount" FROM "payment" WHERE ROWID BETWEEN 0 AND 122879
139656391624704 >>> SqliteScan out_idx 2048 == STANDARD_VECTOR_SIZE
139656391624704 >>> SqliteScanSELECT "rental_id", "amount" FROM "payment" WHERE ROWID BETWEEN 0 AND 122879
139656391624704 >>> SqliteScan out_idx 2048 == STANDARD_VECTOR_SIZE
139656391624704 >>> SqliteScanSELECT "rental_id", "amount" FROM "payment" WHERE ROWID BETWEEN 0 AND 122879
139656391624704 >>> SqliteScan out_idx 2048 == STANDARD_VECTOR_SIZE
139656391624704 >>> SqliteScanSELECT "rental_id", "amount" FROM "payment" WHERE ROWID BETWEEN 0 AND 122879
139656391624704 >>> SqliteScan out_idx 2048 == STANDARD_VECTOR_SIZE
139656391624704 >>> SqliteScanSELECT "rental_id", "amount" FROM "payment" WHERE ROWID BETWEEN 0 AND 122879
139656391624704 >>> SqliteScan out_idx 2048 == STANDARD_VECTOR_SIZE
139656391624704 >>> SqliteScanSELECT "rental_id", "amount" FROM "payment" WHERE ROWID BETWEEN 0 AND 122879
139656391624704 >>> SqliteScan out_idx 2048 == STANDARD_VECTOR_SIZE
139656391624704 >>> SqliteScanSELECT "rental_id", "amount" FROM "payment" WHERE ROWID BETWEEN 0 AND 122879
139656391624704 >>> SqliteScan out_idx 2048 == STANDARD_VECTOR_SIZE
139656391624704 >>> SqliteScanSELECT "rental_id", "amount" FROM "payment" WHERE ROWID BETWEEN 0 AND 122879
139656391624704 >>> SqliteScan out_idx 1713 !has_more
139656391624704 >>> SqliteScanSELECT "rental_id", "amount" FROM "payment" WHERE ROWID BETWEEN 0 AND 122879
139656391624704 >>> SqliteParallelStateNext gstate.position 122880 bind_data->max_rowid 16049 status 0
┌──────────┬─────────────────────┬──────────────┬────────────────────┐
│ store_id │        store        │   manager    │    total_sales     │
│  int64   │       varchar       │   varchar    │       double       │
├──────────┼─────────────────────┼──────────────┼────────────────────┤
│        2 │ Woodridge,Australia │ Jon Stephens │  33726.77000000506 │
│        1 │ Lethbridge,Canada   │ Mike Hillyer │ 33679.790000004934 │
└──────────┴─────────────────────┴──────────────┴────────────────────┘


# not using attach or sqlite_attach

https://duckdb.org/docs/extensions/sqlite_scanner#querying-individual-tables

This means that we do not need to have support for the 'storage' part of the extension.
The sqlite_scan and sqlite_attach can be used without the 'SQLiteStorageExtension'.
See below.

This means that we can not change the database, delete, add or update the contents.

SHOULD BE ENOUGH FOR READING THE ROCKSDB ; WE DO NOT WANT TO HAVE THE ABILITY TO CHANGE IT CONTENTS, ONLY READ IT!!!!
This way the implementation in duckdb is limited to the 'scanner' part..



D install 'build/release/extension/sqlite_scanner-hinxx/sqlite_scanner.duckdb_extension';
D load 'build/release/extension/sqlite_scanner-hinxx/sqlite_scanner.duckdb_extension';
>>> SqliteScanFunction
>>> SqliteAttachFunction
D
D SELECT * FROM sqlite_scan('sakila.db', 'film');
140417704974208 >>> SqliteBind file_name: sakila.db table_name: film
140417704974208 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
140417704974208 >>> SqliteInitGlobalState
140417704974208 >>> SqliteInitLocalState global db =>> result->db 0
140417704974208 >>> SqliteParallelStateNext gstate.position 0 bind_data->max_rowid 1000 status 1
140417704974208 >>> SqliteInitInternal
140417704974208 >>> SqliteInitInternal created local_state.db 0x55b911accae0
140417704974208 >>> SqliteInitInternal prepared SQL:
SELECT "film_id", "title", "description", "release_year", "language_id", "original_language_id", "rental_duration", "rental_rate", "length", "replacement_cost", "rating", "special_features", "last_update" FROM "film" WHERE ROWID BETWEEN 0 AND 122879
140417704974208 >>> SqliteScan SELECT "film_id", "title", "description", "release_year", "language_id", "original_language_id", "rental_duration", "rental_rate", "length", "replacement_cost", "rating", "special_features", "last_update" FROM "film" WHERE ROWID BETWEEN 0 AND 122879
140417704974208 >>> SqliteScan out_idx 1000 !has_more
140417704974208 >>> SqliteScan SELECT "film_id", "title", "description", "release_year", "language_id", "original_language_id", "rental_duration", "rental_rate", "length", "replacement_cost", "rating", "special_features", "last_update" FROM "film" WHERE ROWID BETWEEN 0 AND 122879
140417704974208 >>> SqliteParallelStateNext gstate.position 122880 bind_data->max_rowid 1000 status 0
┌─────────┬──────────────────────┬──────────────────────┬──────────────┬───┬────────┬──────────────────┬─────────┬──────────────────────┬─────────────────────┐
│ film_id │        title         │     description      │ release_year │ … │ length │ replacement_cost │ rating  │   special_features   │     last_update     │
│  int64  │       varchar        │       varchar        │   varchar    │   │ int64  │      double      │ varchar │       varchar        │      timestamp      │
├─────────┼──────────────────────┼──────────────────────┼──────────────┼───┼────────┼──────────────────┼─────────┼──────────────────────┼─────────────────────┤
│       1 │ ACADEMY DINOSAUR     │ A Epic Drama of a …  │ 2006         │ … │     86 │            20.99 │ PG      │ Deleted Scenes,Beh…  │ 2021-03-06 15:52:00 │
│       2 │ ACE GOLDFINGER       │ A Astounding Epist…  │ 2006         │ … │     48 │            12.99 │ G       │ Trailers,Deleted S…  │ 2021-03-06 15:52:00 │
│       3 │ ADAPTATION HOLES     │ A Astounding Refle…  │ 2006         │ … │     50 │            18.99 │ NC-17   │ Trailers,Deleted S…  │ 2021-03-06 15:52:00 │
│       4 │ AFFAIR PREJUDICE     │ A Fanciful Documen…  │ 2006         │ … │    117 │            26.99 │ G       │ Commentaries,Behin…  │ 2021-03-06 15:52:00 │
│       5 │ AFRICAN EGG          │ A Fast-Paced Docum…  │ 2006         │ … │    130 │            22.99 │ G       │ Deleted Scenes       │ 2021-03-06 15:52:00 │
│       6 │ AGENT TRUMAN         │ A Intrepid Panoram…  │ 2006         │ … │    169 │            17.99 │ PG      │ Deleted Scenes       │ 2021-03-06 15:52:00 │
│       7 │ AIRPLANE SIERRA      │ A Touching Saga of…  │ 2006         │ … │     62 │            28.99 │ PG-13   │ Trailers,Deleted S…  │ 2021-03-06 15:52:00 │
│       8 │ AIRPORT POLLOCK      │ A Epic Tale of a M…  │ 2006         │ … │     54 │            15.99 │ R       │ Trailers             │ 2021-03-06 15:52:00 │
│       9 │ ALABAMA DEVIL        │ A Thoughtful Panor…  │ 2006         │ … │    114 │            21.99 │ PG-13   │ Trailers,Deleted S…  │ 2021-03-06 15:52:00 │
│      10 │ ALADDIN CALENDAR     │ A Action-Packed Ta…  │ 2006         │ … │     63 │            24.99 │ NC-17   │ Trailers,Deleted S…  │ 2021-03-06 15:52:00 │
│      11 │ ALAMO VIDEOTAPE      │ A Boring Epistle o…  │ 2006         │ … │    126 │            16.99 │ G       │ Commentaries,Behin…  │ 2021-03-06 15:52:00 │
│      12 │ ALASKA PHANTOM       │ A Fanciful Saga of…  │ 2006         │ … │    136 │            22.99 │ PG      │ Commentaries,Delet…  │ 2021-03-06 15:52:00 │
│      13 │ ALI FOREVER          │ A Action-Packed Dr…  │ 2006         │ … │    150 │            21.99 │ PG      │ Deleted Scenes,Beh…  │ 2021-03-06 15:52:00 │
│      14 │ ALICE FANTASIA       │ A Emotional Drama …  │ 2006         │ … │     94 │            23.99 │ NC-17   │ Trailers,Deleted S…  │ 2021-03-06 15:52:00 │
│      15 │ ALIEN CENTER         │ A Brilliant Drama …  │ 2006         │ … │     46 │            10.99 │ NC-17   │ Trailers,Commentar…  │ 2021-03-06 15:52:00 │
│      16 │ ALLEY EVOLUTION      │ A Fast-Paced Drama…  │ 2006         │ … │    180 │            23.99 │ NC-17   │ Trailers,Commentar…  │ 2021-03-06 15:52:00 │
│      17 │ ALONE TRIP           │ A Fast-Paced Chara…  │ 2006         │ … │     82 │            14.99 │ R       │ Trailers,Behind th…  │ 2021-03-06 15:52:00 │
│      18 │ ALTER VICTORY        │ A Thoughtful Drama…  │ 2006         │ … │     57 │            27.99 │ PG-13   │ Trailers,Behind th…  │ 2021-03-06 15:52:00 │
│      19 │ AMADEUS HOLY         │ A Emotional Displa…  │ 2006         │ … │    113 │            20.99 │ PG      │ Commentaries,Delet…  │ 2021-03-06 15:52:00 │
│      20 │ AMELIE HELLFIGHTERS  │ A Boring Drama of …  │ 2006         │ … │     79 │            23.99 │ R       │ Commentaries,Delet…  │ 2021-03-06 15:52:00 │
│       · │       ·              │          ·           │  ·           │ · │      · │              ·   │ ·       │         ·            │          ·          │
│       · │       ·              │          ·           │  ·           │ · │      · │              ·   │ ·       │         ·            │          ·          │
│       · │       ·              │          ·           │  ·           │ · │      · │              ·   │ ·       │         ·            │          ·          │
│     981 │ WOLVES DESIRE        │ A Fast-Paced Drama…  │ 2006         │ … │     55 │            13.99 │ NC-17   │ Behind the Scenes    │ 2021-03-06 15:52:08 │
│     982 │ WOMEN DORADO         │ A Insightful Docum…  │ 2006         │ … │    126 │            23.99 │ R       │ Deleted Scenes,Beh…  │ 2021-03-06 15:52:08 │
│     983 │ WON DARES            │ A Unbelieveable Do…  │ 2006         │ … │    105 │            18.99 │ PG      │ Behind the Scenes    │ 2021-03-06 15:52:08 │
│     984 │ WONDERFUL DROP       │ A Boring Panorama …  │ 2006         │ … │    126 │            20.99 │ NC-17   │ Commentaries         │ 2021-03-06 15:52:08 │
│     985 │ WONDERLAND CHRISTMAS │ A Awe-Inspiring Ch…  │ 2006         │ … │    111 │            19.99 │ PG      │ Commentaries         │ 2021-03-06 15:52:08 │
│     986 │ WONKA SEA            │ A Brilliant Saga o…  │ 2006         │ … │     85 │            24.99 │ NC-17   │ Trailers,Commentar…  │ 2021-03-06 15:52:08 │
│     987 │ WORDS HUNTER         │ A Action-Packed Re…  │ 2006         │ … │    116 │            13.99 │ PG      │ Trailers,Commentar…  │ 2021-03-06 15:52:08 │
│     988 │ WORKER TARZAN        │ A Action-Packed Ya…  │ 2006         │ … │    139 │            26.99 │ R       │ Trailers,Commentar…  │ 2021-03-06 15:52:08 │
│     989 │ WORKING MICROCOSMOS  │ A Stunning Epistle…  │ 2006         │ … │     74 │            22.99 │ R       │ Commentaries,Delet…  │ 2021-03-06 15:52:08 │
│     990 │ WORLD LEATHERNECKS   │ A Unbelieveable Ta…  │ 2006         │ … │    171 │            13.99 │ PG-13   │ Trailers,Behind th…  │ 2021-03-06 15:52:08 │
│     991 │ WORST BANGER         │ A Thrilling Drama …  │ 2006         │ … │    185 │            26.99 │ PG      │ Deleted Scenes,Beh…  │ 2021-03-06 15:52:08 │
│     992 │ WRATH MILE           │ A Intrepid Reflect…  │ 2006         │ … │    176 │            17.99 │ NC-17   │ Trailers,Commentar…  │ 2021-03-06 15:52:08 │
│     993 │ WRONG BEHAVIOR       │ A Emotional Saga o…  │ 2006         │ … │    178 │            10.99 │ PG-13   │ Trailers,Behind th…  │ 2021-03-06 15:52:08 │
│     994 │ WYOMING STORM        │ A Awe-Inspiring Pa…  │ 2006         │ … │    100 │            29.99 │ PG-13   │ Deleted Scenes       │ 2021-03-06 15:52:08 │
│     995 │ YENTL IDAHO          │ A Amazing Display …  │ 2006         │ … │     86 │            11.99 │ R       │ Trailers,Commentar…  │ 2021-03-06 15:52:08 │
│     996 │ YOUNG LANGUAGE       │ A Unbelieveable Ya…  │ 2006         │ … │    183 │             9.99 │ G       │ Trailers,Behind th…  │ 2021-03-06 15:52:08 │
│     997 │ YOUTH KICK           │ A Touching Drama o…  │ 2006         │ … │    179 │            14.99 │ NC-17   │ Trailers,Behind th…  │ 2021-03-06 15:52:08 │
│     998 │ ZHIVAGO CORE         │ A Fateful Yarn of …  │ 2006         │ … │    105 │            10.99 │ NC-17   │ Deleted Scenes       │ 2021-03-06 15:52:08 │
│     999 │ ZOOLANDER FICTION    │ A Fateful Reflecti…  │ 2006         │ … │    101 │            28.99 │ R       │ Trailers,Deleted S…  │ 2021-03-06 15:52:08 │
│    1000 │ ZORRO ARK            │ A Intrepid Panoram…  │ 2006         │ … │     50 │            18.99 │ NC-17   │ Trailers,Commentar…  │ 2021-03-06 15:52:08 │
├─────────┴──────────────────────┴──────────────────────┴──────────────┴───┴────────┴──────────────────┴─────────┴──────────────────────┴─────────────────────┤
│ 1000 rows (40 shown)                                                                                                                   13 columns (9 shown) │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

D CALL sqlite_attach('sakila.db');
>>> AttachBind
>>> AttachFunction
>>> AttachFunction part1
139827031803776 >>> SqliteBind file_name: sakila.db table_name: actor
139827031803776 >>> SqliteBind max_rowid: 200 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: actor
139827031803776 >>> SqliteBind max_rowid: 200 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: actor
139827031803776 >>> SqliteBind max_rowid: 200 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: country
139827031803776 >>> SqliteBind max_rowid: 109 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: country
139827031803776 >>> SqliteBind max_rowid: 109 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: country
139827031803776 >>> SqliteBind max_rowid: 109 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: city
139827031803776 >>> SqliteBind max_rowid: 600 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: city
139827031803776 >>> SqliteBind max_rowid: 600 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: city
139827031803776 >>> SqliteBind max_rowid: 600 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: address
139827031803776 >>> SqliteBind max_rowid: 603 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: address
139827031803776 >>> SqliteBind max_rowid: 603 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: address
139827031803776 >>> SqliteBind max_rowid: 603 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: language
139827031803776 >>> SqliteBind max_rowid: 6 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: language
139827031803776 >>> SqliteBind max_rowid: 6 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: language
139827031803776 >>> SqliteBind max_rowid: 6 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: category
139827031803776 >>> SqliteBind max_rowid: 16 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: category
139827031803776 >>> SqliteBind max_rowid: 16 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: category
139827031803776 >>> SqliteBind max_rowid: 16 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: customer
139827031803776 >>> SqliteBind max_rowid: 599 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: customer
139827031803776 >>> SqliteBind max_rowid: 599 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: customer
139827031803776 >>> SqliteBind max_rowid: 599 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: film
139827031803776 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: film
139827031803776 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: film
139827031803776 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: film_actor
139827031803776 >>> SqliteBind max_rowid: 5462 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: film_actor
139827031803776 >>> SqliteBind max_rowid: 5462 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: film_actor
139827031803776 >>> SqliteBind max_rowid: 5462 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: film_category
139827031803776 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: film_category
139827031803776 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: film_category
139827031803776 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: film_text
139827031803776 >>> SqliteBind max_rowid: 0 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: film_text
139827031803776 >>> SqliteBind max_rowid: 0 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: film_text
139827031803776 >>> SqliteBind max_rowid: 0 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: inventory
139827031803776 >>> SqliteBind max_rowid: 4581 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: inventory
139827031803776 >>> SqliteBind max_rowid: 4581 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: inventory
139827031803776 >>> SqliteBind max_rowid: 4581 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: staff
139827031803776 >>> SqliteBind max_rowid: 2 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: staff
139827031803776 >>> SqliteBind max_rowid: 2 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: staff
139827031803776 >>> SqliteBind max_rowid: 2 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: store
139827031803776 >>> SqliteBind max_rowid: 2 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: store
139827031803776 >>> SqliteBind max_rowid: 2 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: store
139827031803776 >>> SqliteBind max_rowid: 2 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: payment
139827031803776 >>> SqliteBind max_rowid: 16049 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: payment
139827031803776 >>> SqliteBind max_rowid: 16049 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: payment
139827031803776 >>> SqliteBind max_rowid: 16049 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: rental
139827031803776 >>> SqliteBind max_rowid: 16044 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: rental
139827031803776 >>> SqliteBind max_rowid: 16044 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: rental
139827031803776 >>> SqliteBind max_rowid: 16044 rows_per_group: 122880
>>> AttachFunction part2
>>> AttachFunction view SQL:
CREATE VIEW customer_list
AS
SELECT cu.customer_id AS ID,
       cu.first_name||' '||cu.last_name AS name,
       a.address AS address,
       a.postal_code AS zip_code,
       a.phone AS phone,
       city.city AS city,
       country.country AS country,
       case when cu.active=1 then 'active' else '' end AS notes,
       cu.store_id AS SID
FROM customer AS cu JOIN address AS a ON cu.address_id = a.address_id JOIN city ON a.city_id = city.city_id
    JOIN country ON city.country_id = country.country_id
139827031803776 >>> SqliteBind file_name: sakila.db table_name: customer
139827031803776 >>> SqliteBind max_rowid: 599 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: address
139827031803776 >>> SqliteBind max_rowid: 603 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: city
139827031803776 >>> SqliteBind max_rowid: 600 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: country
139827031803776 >>> SqliteBind max_rowid: 109 rows_per_group: 122880
>>> AttachFunction view SQL:
CREATE VIEW film_list
AS
SELECT film.film_id AS FID,
       film.title AS title,
       film.description AS description,
       category.name AS category,
       film.rental_rate AS price,
       film.length AS length,
       film.rating AS rating,
       actor.first_name||' '||actor.last_name AS actors
FROM category LEFT JOIN film_category ON category.category_id = film_category.category_id LEFT JOIN film ON film_category.film_id = film.film_id
        JOIN film_actor ON film.film_id = film_actor.film_id
    JOIN actor ON film_actor.actor_id = actor.actor_id
139827031803776 >>> SqliteBind file_name: sakila.db table_name: category
139827031803776 >>> SqliteBind max_rowid: 16 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: film_category
139827031803776 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: film
139827031803776 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: film_actor
139827031803776 >>> SqliteBind max_rowid: 5462 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: actor
139827031803776 >>> SqliteBind max_rowid: 200 rows_per_group: 122880
>>> AttachFunction view SQL:
CREATE VIEW staff_list
AS
SELECT s.staff_id AS ID,
       s.first_name||' '||s.last_name AS name,
       a.address AS address,
       a.postal_code AS zip_code,
       a.phone AS phone,
       city.city AS city,
       country.country AS country,
       s.store_id AS SID
FROM staff AS s JOIN address AS a ON s.address_id = a.address_id JOIN city ON a.city_id = city.city_id
    JOIN country ON city.country_id = country.country_id
139827031803776 >>> SqliteBind file_name: sakila.db table_name: staff
139827031803776 >>> SqliteBind max_rowid: 2 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: address
139827031803776 >>> SqliteBind max_rowid: 603 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: city
139827031803776 >>> SqliteBind max_rowid: 600 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: country
139827031803776 >>> SqliteBind max_rowid: 109 rows_per_group: 122880
>>> AttachFunction view SQL:
CREATE VIEW sales_by_store
AS
SELECT
  s.store_id
 ,c.city||','||cy.country AS store
 ,m.first_name||' '||m.last_name AS manager
 ,SUM(p.amount) AS total_sales
FROM payment AS p
INNER JOIN rental AS r ON p.rental_id = r.rental_id
INNER JOIN inventory AS i ON r.inventory_id = i.inventory_id
INNER JOIN store AS s ON i.store_id = s.store_id
INNER JOIN address AS a ON s.address_id = a.address_id
INNER JOIN city AS c ON a.city_id = c.city_id
INNER JOIN country AS cy ON c.country_id = cy.country_id
INNER JOIN staff AS m ON s.manager_staff_id = m.staff_id
GROUP BY
  s.store_id
, c.city||','||cy.country
, m.first_name||' '||m.last_name
139827031803776 >>> SqliteBind file_name: sakila.db table_name: payment
139827031803776 >>> SqliteBind max_rowid: 16049 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: rental
139827031803776 >>> SqliteBind max_rowid: 16044 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: inventory
139827031803776 >>> SqliteBind max_rowid: 4581 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: store
139827031803776 >>> SqliteBind max_rowid: 2 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: address
139827031803776 >>> SqliteBind max_rowid: 603 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: city
139827031803776 >>> SqliteBind max_rowid: 600 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: country
139827031803776 >>> SqliteBind max_rowid: 109 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: staff
139827031803776 >>> SqliteBind max_rowid: 2 rows_per_group: 122880
>>> AttachFunction view SQL:
CREATE VIEW sales_by_film_category
AS
SELECT
c.name AS category
, SUM(p.amount) AS total_sales
FROM payment AS p
INNER JOIN rental AS r ON p.rental_id = r.rental_id
INNER JOIN inventory AS i ON r.inventory_id = i.inventory_id
INNER JOIN film AS f ON i.film_id = f.film_id
INNER JOIN film_category AS fc ON f.film_id = fc.film_id
INNER JOIN category AS c ON fc.category_id = c.category_id
GROUP BY c.name
139827031803776 >>> SqliteBind file_name: sakila.db table_name: payment
139827031803776 >>> SqliteBind max_rowid: 16049 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: rental
139827031803776 >>> SqliteBind max_rowid: 16044 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: inventory
139827031803776 >>> SqliteBind max_rowid: 4581 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: film
139827031803776 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: film_category
139827031803776 >>> SqliteBind max_rowid: 1000 rows_per_group: 122880
139827031803776 >>> SqliteBind file_name: sakila.db table_name: category
139827031803776 >>> SqliteBind max_rowid: 16 rows_per_group: 122880
┌─────────┐
│ Success │
│ boolean │
├─────────┤
│ 0 rows  │
└─────────┘


