import sqlite3
import sys

from argparse import ArgumentParser
from code import InteractiveConsole
from textwrap import dedent


class SqliteInteractiveConsole(InteractiveConsole):

    def __init__(self, connection):
        super().__init__()
        self._con = connection
        self._cur = connection.cursor()

    def runsql(self, sql):
        if not sqlite3.complete_statement(sql):
            return True
        try:
            for row in self._cur.execute(sql):
                print(row)
        except sqlite3.Error as e:
            print(f"{e.sqlite_errorname}: {e}")
        return False

    def runpy(self, source, filename="<input>", symbol="single"):
        code = self.compile(source, filename, symbol)
        self.runcode(code)
        return False

    def printhelp(self, ignored):
        print("Enter SQL code and press enter.")

    def runsource(self, source, filename="<input>", symbol="single"):
        keywords = {
            ".version": lambda x: print(f"{sqlite3.sqlite_version}"),
            ".help": self.printhelp,
            ".quit": lambda x: sys.exit(0),
        }
        return keywords.get(source, self.runsql)(source)


def main():
    parser = ArgumentParser(
        description="Python sqlite3 REPL",
        prog="python -m sqlite3",
    )
    parser.add_argument(
        "-f", "--filename",
        type=str, dest="database", action="store", default=":memory:",
        help="Database to open (defaults to ':memory:')",
    )
    parser.add_argument(
        "-v", "--version", action="version",
        version=f"SQLite version {sqlite3.sqlite_version}",
        help="Print underlying SQLite library version",
    )
    args = parser.parse_args()

    if args.database == ":memory:":
        db_name = "a transient in-memory database"
    else:
        db_name = repr(args.database)

    banner = dedent(f"""
        sqlite3 shell, running on SQLite version {sqlite3.sqlite_version}
        Connected to {db_name}

        Each command will be run using execute() on the cursor.
        Type ".help" for more information; type ".quit" or CTRL-D to quit.
    """).strip()
    sys.ps1 = "sqlite> "
    sys.ps2 = "    ... "

    con = sqlite3.connect(args.database, isolation_level=None)
    console = SqliteInteractiveConsole(con)
    console.interact(banner, exitmsg="")
    con.close()


main()
