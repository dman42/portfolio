CREATE DATABASE  IF NOT EXISTS `2016_Quiz2_Dominos` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `2016_Quiz2_Dominos`;
-- MySQL dump 10.13  Distrib 5.6.17, for osx10.6 (i386)
--
-- Host: localhost    Database: Dominos
-- ------------------------------------------------------
-- Server version	5.6.19

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `CUSTOMERS`
--

DROP TABLE IF EXISTS `CUSTOMERS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `CUSTOMERS` (
  `customerID` int(11) NOT NULL DEFAULT '0',
  `name` varchar(50) DEFAULT NULL,
  `address` varchar(100) DEFAULT NULL,
  `phoneNumber` varchar(10) NOT NULL,
  `email` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`customerID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `CUSTOMERS`
--

LOCK TABLES `CUSTOMERS` WRITE;
/*!40000 ALTER TABLE `CUSTOMERS` DISABLE KEYS */;
INSERT INTO `CUSTOMERS` VALUES (1,'Vance Vince','11 Byth St South Brisbane','0735287283','Vanc@gmail.com\r'),(2,'Juliane Jarnigan','82 Alpha St Taringa','0462379273','Juli@hotmail.com\r'),(3,'Lakeesha Little','88 Indooroopilly Rd Taringa','0472836455','Lake@gmail.com\r'),(4,'Lucina Langham','88 Milton Rd Milton','0477283934','Luci@hotmail.com\r'),(5,'Laronda Lachance','78 Sir Fred Schonell Dr St Lucia','0482947834','Laro@hotmail.com\r'),(6,'Clinton Ciesielski','12 Carmody Rd St Lucia','0727647629','Clin@gmail.com\r'),(7,'Robbi Ruckman','23 Rue Montaigne Petrie','0711122233','Juli@hotmail.com\r'),(8,'Claudine Cepeda','77 Raymont Tce South Brisbane','0472867386','Clau@hotmail.com\r'),(9,'Evelina Eisner','87 Main St Hamilton','0476276722','Evel@gmail.com\r'),(10,'Sharika Segalla','729 Bowen Bridge Rd Herston','0487262900','Shar@yahoo.com\r'),(11,'Fe Facio','88 Gregory Tce Spring Hill','0409152383','FiFi@hotmail.com\r'),(12,'Christian Clopton','23 Rue Montaigne Petrie','0711122233','Chri@hotmail.com\r'),(13,'Eda Eckstein','23 Butterfeild St Herston','0487287927','Eda @gmail.com\r'),(14,'Chanel Claybrooks','16 Beta St South Brisbane','0737625828','Chan@yahoo.com\r'),(15,'Cleotilde Chavis','446 McBeth St West End','0478267823','Cleo@gmail.com\r'),(16,'Leo Lesser','11 Byth St South Brisbane','0735287283','Leo @gmail.com\r'),(17,'Robbi Ruckman','1262/77 George St Brisbane City','0486281000','Robb@yahoo.com\r'),(18,'Garland Gilstrap','67 Duncan St West End','0422682900','Garl@hotmail.com\r'),(19,'Lucy Learn','22 Vulture St West End','0482730202','Lucy@gmail.com');
/*!40000 ALTER TABLE `CUSTOMERS` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ORDERCONTENTS`
--

DROP TABLE IF EXISTS `ORDERCONTENTS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ORDERCONTENTS` (
  `orderID` int(11) NOT NULL DEFAULT '0',
  `name` varchar(50) NOT NULL DEFAULT '',
  `quantity` int(2) DEFAULT NULL,
  PRIMARY KEY (`orderID`,`name`),
  KEY `name` (`name`),
  CONSTRAINT `ordercontents_ibfk_1` FOREIGN KEY (`orderID`) REFERENCES `Orders` (`orderID`),
  CONSTRAINT `ordercontents_ibfk_2` FOREIGN KEY (`name`) REFERENCES `Products` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ORDERCONTENTS`
--

LOCK TABLES `ORDERCONTENTS` WRITE;
/*!40000 ALTER TABLE `ORDERCONTENTS` DISABLE KEYS */;
INSERT INTO `ORDERCONTENTS` VALUES (1,'Cheese',2),(1,'Supreme',1),(2,'Hawaiian',1),(3,'Cheese',2),(4,'Cheese',1),(4,'Meat Lovers',4),(4,'Supreme',2),(5,'Cheese',1);
/*!40000 ALTER TABLE `ORDERCONTENTS` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ORDERS`
--

DROP TABLE IF EXISTS `ORDERS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ORDERS` (
  `orderID` int(11) NOT NULL DEFAULT '0',
  `customerID` int(11) DEFAULT NULL,
  `postcode` int(4) DEFAULT NULL,
  `date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`orderID`),
  KEY `customerID` (`customerID`),
  KEY `postcode` (`postcode`),
  CONSTRAINT `orders_ibfk_1` FOREIGN KEY (`customerID`) REFERENCES `Customers` (`customerID`),
  CONSTRAINT `orders_ibfk_2` FOREIGN KEY (`postcode`) REFERENCES `Stores` (`postcode`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ORDERS`
--

LOCK TABLES `ORDERS` WRITE;
/*!40000 ALTER TABLE `ORDERS` DISABLE KEYS */;
INSERT INTO `ORDERS` VALUES (1,11,4001,'2015-08-04 14:00:00'),(2,13,4051,'2015-08-04 14:00:01'),(3,1,4000,'2015-08-05 14:00:02'),(4,2,4000,'2015-08-05 14:00:03'),(5,3,4001,'2015-08-05 14:00:04');
/*!40000 ALTER TABLE `ORDERS` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `PRODUCTS`
--

DROP TABLE IF EXISTS `PRODUCTS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `PRODUCTS` (
  `name` varchar(50) NOT NULL DEFAULT '',
  `price` decimal(5,2) DEFAULT NULL,
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `PRODUCTS`
--

LOCK TABLES `PRODUCTS` WRITE;
/*!40000 ALTER TABLE `PRODUCTS` DISABLE KEYS */;
INSERT INTO `PRODUCTS` VALUES ('Cheese',8.95),('Hawaiian',8.95),('Margareta',8.95),('Meat Lovers',9.95),('Smoked Salmon',14.95),('Smokey Bacon',12.95),('Supreme',9.95),('Vegetarian',9.95);
/*!40000 ALTER TABLE `PRODUCTS` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `STORES`
--

DROP TABLE IF EXISTS `STORES`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `STORES` (
  `postcode` int(4) NOT NULL DEFAULT '0',
  `address` varchar(100) DEFAULT NULL,
  `phoneNumber` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`postcode`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `STORES`
--

LOCK TABLES `STORES` WRITE;
/*!40000 ALTER TABLE `STORES` DISABLE KEYS */;
INSERT INTO `STORES` VALUES (4000,'105 ALBERT STREET BRISBANE QLD ','730064910\r'),(4001,'CNR BOUNDARY & LITTLE EDWARD STREETS SPRING HILL QLD','730007300'),(4005,'672 BRUNSWICK ST NEW FARM QLD','730004644\r'),(4006,'330 BRUNSWICK ST FORTITUDE VALLEY QLD','732574579\r'),(4007,'485 KINGSFORD SMITH DRIVE HAMILTON QLD','731310090\r'),(4011,'2/805 SANDGATE RD CLAYFIELD QLD ','736247177\r'),(4051,'104-110 ENOGGERA ROAD NEWMARKET QLD','735133222\r'),(4064,'1/232 GIVEN TCE PADDINGTON QLD','731095777\r');
/*!40000 ALTER TABLE `STORES` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-09-06 20:49:28
