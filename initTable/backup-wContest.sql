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
-- Table structure for table `wContest`
--

DROP TABLE IF EXISTS `wContest`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wContest` (
  `contestid` varchar(24) NOT NULL DEFAULT '',
  `ctype` varchar(6) NOT NULL,
  `name` varchar(64) NOT NULL,
  `expLogbook` varchar(24) DEFAULT NULL,
  `expFile` varchar(24) DEFAULT NULL,
  `startday` varchar(10) NOT NULL,
  `stime` varchar(10) NOT NULL,
  `runtime` varchar(8) DEFAULT NULL,
  `view` varchar(4) NOT NULL,
  `freqband` varchar(8) DEFAULT NULL,
  `mode` varchar(8) DEFAULT NULL,
  `qsoid` int(11) NOT NULL,
  `srst` varchar(8) NOT NULL,
  `sent` varchar(18) NOT NULL,
  `rrst` varchar(8) NOT NULL,
  `mycall` varchar(16) DEFAULT NULL,
  `category` varchar(36) DEFAULT NULL,
  `specify` varchar(24) DEFAULT NULL,
  `user` varchar(24) DEFAULT NULL,
  `offtime1` varchar(64) DEFAULT NULL,
  `offtime2` varchar(64) DEFAULT NULL,
  `club` varchar(24) DEFAULT NULL,
  `soapbox` varchar(127) DEFAULT NULL,
  PRIMARY KEY (`contestid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `wContest`
--

LOCK TABLES `wContest` WRITE;
/*!40000 ALTER TABLE `wContest` DISABLE KEYS */;
/*!40000 ALTER TABLE `wContest` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2011-03-04 12:35:41
