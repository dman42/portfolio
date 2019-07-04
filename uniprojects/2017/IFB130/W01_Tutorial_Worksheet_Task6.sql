-- MySQL dump 10.13  Distrib 5.7.9, for osx10.9 (x86_64)
--
-- Host: localhost    Database: TIMETABLE
-- ------------------------------------------------------
-- Server version	5.7.13

CREATE SCHEMA `timetable` ;

SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT;
SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS;
SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION;
SET NAMES utf8;
SET @OLD_TIME_ZONE=@@TIME_ZONE;
SET TIME_ZONE='+00:00';
SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO';
SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0;


--
-- Table structure for table `Allocations`
--

DROP TABLE IF EXISTS `Allocations`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `Allocations` (
  `unitCode` varchar(6) NOT NULL,
  `year` int(4) NOT NULL,
  `semester` int(1) NOT NULL,
  `lecturerID` int(11) DEFAULT NULL,
  PRIMARY KEY (`unitCode`,`year`,`semester`),
  KEY `lecturerID` (`lecturerID`),
  CONSTRAINT `allocations_ibfk_1` FOREIGN KEY (`unitCode`) REFERENCES `Units` (`unitCode`),
  CONSTRAINT `allocations_ibfk_2` FOREIGN KEY (`lecturerID`) REFERENCES `Lecturers` (`lecturerID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `Allocations`
--

LOCK TABLES `Allocations` WRITE;
ALTER TABLE `Allocations` DISABLE KEYS;
ALTER TABLE `Allocations` ENABLE KEYS;
UNLOCK TABLES;

--
-- Table structure for table `Lecturers`
--

DROP TABLE IF EXISTS `Lecturers`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `Lecturers` (
  `lecturerID` int(11) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `school` varchar(100) DEFAULT NULL,
  `email` varchar(100) DEFAULT NULL,
  `phoneNumber` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`lecturerID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `Lecturers`
--

LOCK TABLES `Lecturers` WRITE;
ALTER TABLE `Lecturers` DISABLE KEYS;
ALTER TABLE `Lecturers` ENABLE KEYS;
UNLOCK TABLES;

--
-- Table structure for table `Units`
--

DROP TABLE IF EXISTS `Units`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `Units` (
  `unitCode` varchar(6) NOT NULL,
  `unitTitle` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`unitCode`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `Units`
--

LOCK TABLES `Units` WRITE;
ALTER TABLE `Units` DISABLE KEYS;
ALTER TABLE `Units` ENABLE KEYS;
UNLOCK TABLES;
SET TIME_ZONE=@OLD_TIME_ZONE;

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT;
SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS;
SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION;
SET SQL_NOTES=@OLD_SQL_NOTES;

-- Dump completed on 2016-07-01 17:22:35
