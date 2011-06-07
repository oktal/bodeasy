CREATE VIEW `session_made_result_view` AS
SELECT er.*, e.*, sm.id_session_made FROM
exercise_result er
INNER JOIN session_made sm
INNER JOIN session_made_result smr
ON sm.id_session_made=smr.id_session_made
INNER JOIN exercise e ON e.id_exercise=er.id_exercise
WHERE er.id_result=smr.id_result