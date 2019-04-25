USE `n9939768`;

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