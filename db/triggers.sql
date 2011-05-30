CREATE TRIGGER IF NOT EXISTS ON_TBL_SESSION_DELETE AFTER DELETE ON session
  BEGIN
      DELETE FROM session_exercise WHERE id_session=OLD.id_session;
  END;
  
CREATE TRIGGER IF NOT EXISTS ON_TBL_EXERCISE_DELETE AFTER DELETE ON exercise
  BEGIN
      DELETE FROM exercise_muscle_group WHERE id_exercise=OLD.id_exercise;
      DELETE FROM session_exercise WHERE id_exercise=OLD.id_exercise;
  END;
  
