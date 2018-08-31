test_run = require('test_run').new()
engine = test_run:get_cfg('engine')
box.sql.execute('pragma sql_default_engine=\''..engine..'\'')

-- Initializing some things.
box.sql.execute("CREATE TABLE t1(id PRIMARY KEY, a);")
box.sql.execute("CREATE TABLE t2(id PRIMARY KEY, a);")
box.sql.execute("CREATE INDEX i1 ON t1(a);")
box.sql.execute("CREATE INDEX i1 ON t2(a);")
box.sql.execute("INSERT INTO t1 VALUES(1, 2);")
box.sql.execute("INSERT INTO t2 VALUES(1, 2);")

-- Analyze.
box.sql.execute("ANALYZE;")

-- Checking the data.
box.sql.execute('SELECT "name","index_id","neq","nlt","ndlt","sample" FROM "_sql_stat4" join "_space" on "_sql_stat4"."space_id" = "_space"."id";')
box.sql.execute('SELECT "name","index_id","stat" FROM "_sql_stat1" join "_space" on "_sql_stat1"."space_id" = "_space"."id";')

-- Dropping an index.
box.sql.execute("DROP INDEX i1 ON t1;")

-- Checking the DROP INDEX results.
box.sql.execute('SELECT "name","index_id","neq","nlt","ndlt","sample" FROM "_sql_stat4" join "_space" on "_sql_stat4"."space_id" = "_space"."id";')
box.sql.execute('SELECT "name","index_id","stat" FROM "_sql_stat1" join "_space" on "_sql_stat1"."space_id" = "_space"."id";')

--Cleaning up.
box.sql.execute("DROP TABLE t1;")
box.sql.execute("DROP TABLE t2;")

-- Same test but dropping an INDEX ON t2.

box.sql.execute("CREATE TABLE t1(id PRIMARY KEY, a);")
box.sql.execute("CREATE TABLE t2(id PRIMARY KEY, a);")
box.sql.execute("CREATE INDEX i1 ON t1(a);")
box.sql.execute("CREATE INDEX i1 ON t2(a);")
box.sql.execute("INSERT INTO t1 VALUES(1, 2);")
box.sql.execute("INSERT INTO t2 VALUES(1, 2);")

-- Analyze.
box.sql.execute("ANALYZE;")

-- Checking the data.
box.sql.execute('SELECT "name","index_id","neq","nlt","ndlt","sample" FROM "_sql_stat4" join "_space" on "_sql_stat4"."space_id" = "_space"."id";')
box.sql.execute('SELECT "name","index_id","stat" FROM "_sql_stat1" join "_space" on "_sql_stat1"."space_id" = "_space"."id";')

-- Dropping an index.
box.sql.execute("DROP INDEX i1 ON t2;")

-- Checking the DROP INDEX results.
box.sql.execute('SELECT "name","index_id","neq","nlt","ndlt","sample" FROM "_sql_stat4" join "_space" on "_sql_stat4"."space_id" = "_space"."id";')
box.sql.execute('SELECT "name","index_id","stat" FROM "_sql_stat1" join "_space" on "_sql_stat1"."space_id" = "_space"."id";')

--Cleaning up.
box.sql.execute("DROP TABLE t1;")
box.sql.execute("DROP TABLE t2;")
