-- MySQL dump 10.13  Distrib 5.1.49, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: logdb
-- ------------------------------------------------------
-- Server version	5.1.49-1ubuntu8.1

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
-- Table structure for table `uswas`
--

DROP TABLE IF EXISTS `uswas`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `uswas` (
  `pcode` varchar(6) COLLATE utf8_unicode_ci NOT NULL,
  `state` varchar(30) COLLATE utf8_unicode_ci NOT NULL,
  `capital` varchar(30) COLLATE utf8_unicode_ci NOT NULL,
  `cqzone` varchar(6) COLLATE utf8_unicode_ci NOT NULL,
  `ituzone` varchar(6) COLLATE utf8_unicode_ci NOT NULL,
  `cqarea` varchar(4) COLLATE utf8_unicode_ci NOT NULL,
  `sel` varchar(4) COLLATE utf8_unicode_ci NOT NULL,
  KEY `pcidx` (`pcode`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `uswas`
--

LOCK TABLES `uswas` WRITE;
/*!40000 ALTER TABLE `uswas` DISABLE KEYS */;
INSERT INTO `uswas` VALUES ('CT','Connecticut','Hardford','05','08','W1','1'),('ME','Maine','Augusta','05','08','W1','1'),('MA','Massachusetts','Boston','05','08','W1','1'),('NH','New Hampshire','Concorde','05','08','W1','1'),('RI','Rhode Island','Providence','05','08','W1','1'),('VT','Vermont','Montpelier','05','08','W1','1'),('NJ','New Jersey','New Jersey','05','08','W2','1'),('NY','New York','Alabany','05','08','W2','1'),('DE','Delaware','Dover','05','08','W3','1'),('DC','District of Columbia','Washington','05','08','W3','1'),('MD','Maryland','Annapolis','05','08','W3','1'),('PA','Pennsylvania','Harrisburg','05','08','W3','1'),('AL','Alabama','Montgomery','04','08','W4','1'),('FL','Florida','Tallahassee','05','08','W4','1'),('GA','Georgia','Atlanta','05','08','W4','1'),('KY','Kentucky','Frankfort','04','08','W4','1'),('NC','North Carolina','Raleigh','04','08','W4','1'),('SC','South Carolina','Columbia','05','08','W4','1'),('TN','Tennessee','Nashville','04','07/08','W4','1'),('VA','Virginia','Richmond','05','08','W4','1'),('AR','Arkansas','Little Rock','04','07/08','W5','1'),('LA','Louisiana','Baton Rouge','04','07/08','W5','1'),('MS','Mississippi','Jackson','04','07/08','W5','1'),('NM','New Mexico','Santa Fe','04','07','W5','1'),('OK','Oklahoma','Oklahoma City','04','07','W5','1'),('TX','Texas','Austin','04','07','W5','1'),('CA','California','Sacramento','03','06','W6','1'),('AZ','Arizona','Phoenix','03','06/07','W7','1'),('ID','Idaho','Boise','03','06','W7','1'),('MT','Montana','Helena','04','06/07','W7','1'),('NV','Nevada','Carson City','03','06','W7','1'),('OR','Oregon','Salem','03','06','W7','1'),('UT','Utah','Salt Lake City','03','06/07','W7','1'),('WA','Washington','Olympia','03','06','W7','1'),('WY','Wyoming','Cheyenne','04','07','W7','1'),('MI','Michigan','Lansing','04','07/08','W8','1'),('OH','Ohio','Columbus','04','08','W8','1'),('WV','West Virginia','Charleston','05','08','W8','1'),('IL','Illinois','Springfield','04','07/08','W9','1'),('IN','Indiana','Indianapolis','04','08','W9','1'),('WI','Wisconsin','Madison','04','07/08','W9','1'),('CO','Colorado','Denver','04','07','W0','1'),('IA','Iowa','Des Moines','04','07','W0','1'),('KS','Kansas','Topeka','04','07','W0','1'),('MN','Minnesota','St.Paul','04','07','W0','1'),('MO','Missouri','Jefferson City','04','07/08','W0','1'),('NE','Nebraska','Lincoln','04','07','W0','1'),('ND','North Dakota','Bismark','04','07','W0','1'),('SD','South Dakota','Pierre','04','07','W0','1'),('HI','Hawaii','Honolulu','','','KH6','0'),('HI','Hawaii','Honolulu','','','KH7','0'),('AK','Alaska','Alaska','','','KL','0'),('NS','Nova Scotia','Halifax','','','VE1','2'),('NB','New Brunswick','Fredricton','','','VE9','2'),('QUE','Province of Quebec','Quebec','','','VE2','2'),('ON','Ontario','Toronto','','','VE3','2'),('MB','Manitoba','Winnipeg','','','VE4','2'),('SK','Saskatchewan','Regina','','','VE5','2'),('AB','Alberta','Edmonton','','','VE6','2'),('BC','British Columbia','Victoria','','','VE7','2'),('NWT','North West Territories','Yellowknife','','','VE8','2'),('NFL','Newfoundland','St.Jones','','','VO1','2'),('LAB','Labrador','( zu NFL )','','','VO2','2'),('YT','Yukon','Whitehorse','','','VY1','2'),('NU','Nunavut','Iqaluit','','','VY0','2'),('PEI','Prince Edward Island','Charlottetown','','','VY2','2');
/*!40000 ALTER TABLE `uswas` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2011-03-04 12:29:08
