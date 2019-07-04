-- Task 2
-- Q1
SELECT firstName, lastName, gender, DOB, email FROM player WHERE suburb LIKE 'Sunnybank%';
-- Q2
SELECT username, SUM(cost) AS totalCost FROM purchase GROUP BY username ORDER BY username DESC;
-- Q3
SELECT Plyr.username, Phn.phoneNumber FROM Player Plyr, PhoneNumber Phn WHERE Plyr.DOB = (SELECT MIN(DOB) FROM Player) GROUP BY username;
-- Q4 
SELECT b.badgeName, buyers.firstName, buyers.lastName, buyers.email FROM badge b LEFT JOIN (SELECT * FROM player NATURAL JOIN purchase pu) 
AS buyers ON b.badgeID = buyers.badgeID ORDER BY b.badgeName ASC, buyers.firstName ASC, buyers.lastName ASC;
-- Q5 
SELECT username, COUNT(progress) AS completedQuests FROM playerprogress NATURAL JOIN (SELECT questID, type FROM quest NATURAL JOIN treasure WHERE type = 'common' GROUP BY questID) AS commonQuest WHERE progress = 'complete' GROUP BY username;
-- Task 3
-- Insert
INSERT INTO badge VALUES (18, "Fools Gold", "Trickiest trickster in all the seas");
-- Delete
DELETE FROM playerprogress WHERE progress = "inactive";
-- Update
UPDATE player SET streetNumber = 72, streetName = "Evergreen Terrace", suburb = "Springfield" WHERE lastName = "Smith" AND streetNumber = 180 AND streetName = "Zelda Street" AND suburb = "Linkburb";
-- Task 4
-- Index 
CREATE INDEX webPage On treasure(webpage);
-- View 
CREATE VIEW task4View AS SELECT firstName, lastName, creationDateTime FROM player LEFT JOIN (SELECT username, COUNT(progress) AS completeProgress FROM playerprogress where progress = 'complete' GROUP BY username) AS completeProgress 
ON player.username = completeProgress.username WHERE completeProgress.completeProgress IS NULL;
-- Task 5 
GRANT INSERT ON treasurehunters.player TO 'nikki'@'localhost';
GRANT DELETE ON treasurehunters.player TO 'nikki'@'localhost';
REVOKE INSERT ON treasurehunters.player FROM 'phil'@'localhost';
REVOKE DELETE ON treasurehunters.player FROM 'phil'@'localhost';

