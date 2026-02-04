# SQLite from Scratch

A simple SQLite database implementation written in C, following the tutorial from [cstack/db_tutorial](https://cstack.github.io/db_tutorial/).

## Building

Compile the database executable:

```bash
make
```

This creates the `db` binary from `main.c`.

## Running Tests

Install dependencies and run the RSpec tests:

```bash
bundle install
bundle exec rspec
```

The tests interact with the compiled `db` binary to verify database functionality including inserting and selecting rows.

## Usage

After building, you can interact with the database directly:

```bash
./db
```

The database supports basic SQL-like commands for inserting and selecting data.

