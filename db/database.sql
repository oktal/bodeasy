-- Creator:       MySQL Workbench 5.2.38/ExportSQLite plugin 2009.12.02
-- Author:        Disk1
-- Caption:       New Model
-- Project:       Name of the project
-- Changed:       2012-11-03 18:06
-- Created:       2010-12-11 20:09
PRAGMA foreign_keys = OFF;

-- Schema: mydb
BEGIN;
CREATE TABLE "user"(
  "id_user" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL CHECK("id_user">=0),
  "name" TEXT NOT NULL,
  CONSTRAINT "id_user_UNIQUE"
    UNIQUE("id_user"),
  CONSTRAINT "name_UNIQUE"
    UNIQUE("name")
);
CREATE TABLE "exercise"(
  "id_exercise" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL CHECK("id_exercise">=0),
  "name" TEXT NOT NULL,
  "type" INTEGER NOT NULL,
  "weight" INTEGER NOT NULL,
  "difficulty" INTEGER NOT NULL,
  "description" TEXT,
  CONSTRAINT "id_exercice_UNIQUE"
    UNIQUE("id_exercise"),
  CONSTRAINT "name_UNIQUE"
    UNIQUE("name")
);
CREATE TABLE "mensuration"(
  "id_mensuration" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL CHECK("id_mensuration">=0),
  "neck" DOUBLE,
  "shoulder" DOUBLE,
  "chest" DOUBLE,
  "waist_size" DOUBLE,
  "forearm" DOUBLE,
  "thigh" DOUBLE,
  "calf" DOUBLE,
  "biceps" DOUBLE,
  "wrist" DOUBLE,
  "ankle" DOUBLE,
  "weight" DOUBLE,
  "size" DOUBLE,
  "date" DATETIME,
  "id_user" INTEGER NOT NULL CHECK("id_user">=0),
  CONSTRAINT "id_mensuration_UNIQUE"
    UNIQUE("id_mensuration"),
  CONSTRAINT "fk_id_user"
    FOREIGN KEY("id_user")
    REFERENCES "user"("id_user")
);
CREATE INDEX "mensuration.id_user_INDEX" ON "mensuration"("id_user");
CREATE TABLE "muscle_group"(
  "id_muscle_group" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL CHECK("id_muscle_group">=0),
  "name" TEXT NOT NULL,
  CONSTRAINT "id_muscle_group_UNIQUE"
    UNIQUE("id_muscle_group"),
  CONSTRAINT "name_UNIQUE"
    UNIQUE("name")
);
CREATE TABLE "session"(
  "id_session" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL CHECK("id_session">=0),
  "name" TEXT NOT NULL,
  "objective" TEXT,
  CONSTRAINT "id_session_UNIQUE"
    UNIQUE("id_session"),
  CONSTRAINT "name_UNIQUE"
    UNIQUE("name")
);
CREATE TABLE "exercise_muscle_group"(
  "id_exercise" INTEGER NOT NULL CHECK("id_exercise">=0),
  "id_muscle_group" INTEGER NOT NULL CHECK("id_muscle_group">=0),
  PRIMARY KEY("id_exercise","id_muscle_group"),
  CONSTRAINT "fk_id_exercise"
    FOREIGN KEY("id_exercise")
    REFERENCES "exercise"("id_exercise"),
  CONSTRAINT "fk_id_muscle_group"
    FOREIGN KEY("id_muscle_group")
    REFERENCES "muscle_group"("id_muscle_group")
);
CREATE INDEX "exercise_muscle_group.id_exercise_INDEX" ON "exercise_muscle_group"("id_exercise");
CREATE INDEX "exercise_muscle_group.id_muscle_group_INDEX" ON "exercise_muscle_group"("id_muscle_group");
CREATE TABLE "session_exercise"(
  "id_session_exercise" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
  "id_session" INTEGER NOT NULL CHECK("id_session">=0),
  "id_exercise" INTEGER NOT NULL CHECK("id_exercise">=0),
  "series" INTEGER NOT NULL,
  "repetitions" INTEGER,
  "rest" INTEGER,
  CONSTRAINT "id_session_exercise_UNIQUE"
    UNIQUE("id_session_exercise"),
  CONSTRAINT "fk_id_session"
    FOREIGN KEY("id_session")
    REFERENCES "session"("id_session"),
  CONSTRAINT "fk_id_exercise"
    FOREIGN KEY("id_exercise")
    REFERENCES "exercise"("id_exercise")
);
CREATE INDEX "session_exercise.id_session_INDEX" ON "session_exercise"("id_session");
CREATE INDEX "session_exercise.id_exercise_INDEX" ON "session_exercise"("id_exercise");
CREATE TABLE "exercise_result"(
  "id_result" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL CHECK("id_result">=0),
  "result" INTEGER NOT NULL,
  "load" INTEGER,
  "date" DATETIME NOT NULL,
  "serie_number" INTEGER NOT NULL,
  "id_exercise" INTEGER NOT NULL CHECK("id_exercise">=0),
  "id_session" INTEGER NOT NULL CHECK("id_session">=0),
  "id_user" INTEGER NOT NULL CHECK("id_user">=0),
  "id_session_exercise" INTEGER NOT NULL,
  CONSTRAINT "id_result_UNIQUE"
    UNIQUE("id_result"),
  CONSTRAINT "fk_id_session"
    FOREIGN KEY("id_session")
    REFERENCES "session"("id_session"),
  CONSTRAINT "fk_id_exercise"
    FOREIGN KEY("id_exercise")
    REFERENCES "exercise"("id_exercise"),
  CONSTRAINT "fk_id_user"
    FOREIGN KEY("id_user")
    REFERENCES "user"("id_user"),
  CONSTRAINT "fk_exercise_result_session_exercise1"
    FOREIGN KEY("id_session_exercise")
    REFERENCES "session_exercise"("id_session_exercise")
);
CREATE INDEX "exercise_result.id_session_INDEX" ON "exercise_result"("id_session");
CREATE INDEX "exercise_result.id_exercise_INDEX" ON "exercise_result"("id_exercise");
CREATE INDEX "exercise_result.id_user_INDEX" ON "exercise_result"("id_user");
CREATE INDEX "exercise_result.fk_exercise_result_session_exercise1" ON "exercise_result"("id_session_exercise");
CREATE TABLE "session_made"(
  "id_session_made" INTEGER PRIMARY KEY NOT NULL CHECK("id_session_made">=0),
  "objective_achieved" BOOL NOT NULL,
  "date" DATETIME NOT NULL,
  "comment" TEXT,
  "id_session" INTEGER NOT NULL CHECK("id_session">=0),
  "id_user" INTEGER NOT NULL CHECK("id_user">=0),
  CONSTRAINT "id_session_made_UNIQUE"
    UNIQUE("id_session_made"),
  CONSTRAINT "fk_id_session"
    FOREIGN KEY("id_session")
    REFERENCES "session"("id_session"),
  CONSTRAINT "fk_id_user"
    FOREIGN KEY("id_user")
    REFERENCES "user"("id_user")
);
CREATE INDEX "session_made.id_session_INDEX" ON "session_made"("id_session");
CREATE INDEX "session_made.fk_id_user" ON "session_made"("id_user");
CREATE TABLE "session_made_result"(
  "id_session_made" INTEGER NOT NULL CHECK("id_session_made">=0),
  "id_result" INTEGER NOT NULL CHECK("id_result">=0),
  PRIMARY KEY("id_session_made","id_result"),
  CONSTRAINT "fk_session_made_result_session_made1"
    FOREIGN KEY("id_session_made")
    REFERENCES "session_made"("id_session_made"),
  CONSTRAINT "fk_session_made_result_exercise_result1"
    FOREIGN KEY("id_result")
    REFERENCES "exercise_result"("id_result")
);
CREATE INDEX "session_made_result.fk_session_made_result_exercise_result1" ON "session_made_result"("id_result");
CREATE TABLE "session_planned"(
  "id_session_planned" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
  "id_session" INTEGER NOT NULL CHECK("id_session">=0),
  "date" DATE,
  "start_time" TIME,
  "end_time" TIME,
  CONSTRAINT "id_session_planned_UNIQUE"
    UNIQUE("id_session_planned"),
  CONSTRAINT "fk_session_planned_session1"
    FOREIGN KEY("id_session")
    REFERENCES "session"("id_session")
);
CREATE INDEX "session_planned.fk_session_planned_session1" ON "session_planned"("id_session");
COMMIT;
