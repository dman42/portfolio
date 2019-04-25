CREATE DATABASE IF NOT EXISTS n9939768;
USE n9939768;

CREATE TABLE IF NOT EXISTS Members( 
	username VARCHAR(40),
	password TEXT,
	email TEXT,
	DOB DATE,
	gender ENUM('M', 'F'),
    status ENUM('On', 'Off') DEFAULT 'Off',
    memberID INT NOT NULL AUTO_INCREMENT,
    salt TEXT,
    PRIMARY Key (memberID)
);

CREATE TABLE IF NOT EXISTS Items( 	
	name VARCHAR(50),
	address TINYTEXT,
	suburb TINYTEXT,
	latitude DOUBLE,
	longitude DOUBLE,
	rating TINYINT(1),
	itemID INT NOT NULL AUTO_INCREMENT,
	PRIMARY Key (itemID)
);

CREATE TABLE IF NOT EXISTS Reviews( 
	commentID INT NOT NULL AUTO_INCREMENT,
	commentText TEXT,
	rating TINYINT(1),
	datePosted DATE,
	memberID INT,
	itemID INT,
	PRIMARY Key (commentID),
	FOREIGN Key (memberID) references Members(memberID),
	FOREIGN Key (itemID) references Items(itemID)
);


/*Used for hotspots near "own location"-------------------------------------------------------------------------------*/
DELIMITER //
DROP FUNCTION IF EXISTS calcDistance//
CREATE FUNCTION calcDistance ( origLat DOUBLE, origLng DOUBLE, targetLat DOUBLE, targetLng DOUBLE)
RETURNS DOUBLE
BEGIN
	DECLARE r, phi1, phi2, deltaPhi, deltaLambda, a, c, d DOUBLE;
	SET r = 6371e3;
	SET phi1 = radians(targetLat);
	SET phi2 = radians(origLat);
	SET deltaPhi = radians(origLat - targetLat);
	SET deltaLambda = radians(origLng - targetLng);
	SET a = SIN(deltaPhi/2)*SIN(deltaPhi/2)+COS(phi1)*COS(phi2)*SIN(deltaLambda/2)*SIN(deltaLambda/2);
	SET c = 2*ATAN2(SQRT(a), SQRT(1-a));
	SET d = r * c;
  RETURN d;
END//

DELIMITER ;
/*--------------------------------------------------------------------------------------------------------------------*/

LOAD DATA INFILE 'hotspot.csv' 
INTO TABLE Items 
FIELDS TERMINATED BY ',' 
ENCLOSED BY '"'
LINES TERMINATED BY '\n'
IGNORE 1 ROWS;