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
-- Table structure for table `wproz`
--

DROP TABLE IF EXISTS `wproz`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wproz` (
  `proz` varchar(12) NOT NULL,
  `owner` varchar(12) NOT NULL,
  `rig` varchar(24) NOT NULL,
  `rigpid` varchar(8) NOT NULL,
  `retry` varchar(4) NOT NULL,
  `rigdev` varchar(16) NOT NULL,
  `baud` varchar(8) NOT NULL,
  `iface` varchar(16) NOT NULL,
  `pttdev` varchar(16) NOT NULL,
  `rts` varchar(4) NOT NULL,
  `dtr` varchar(4) NOT NULL,
  `cts` varchar(4) NOT NULL,
  `host` varchar(16) NOT NULL,
  `port` varchar(8) NOT NULL,
  `runing` tinyint(4) NOT NULL,
  `id` int(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=58 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `wproz`
--

LOCK TABLES `wproz` WRITE;
/*!40000 ALTER TABLE `wproz` DISABLE KEYS */;
INSERT INTO `wproz` VALUES ('HamLib','hamlib','Icom IC-7600 (0.7 Beta)','363','1000','/dev/ttyUSB0','19200','Funkamateur ONE','/dev/ttyUSB0','OFF','OFF','OFF','localhost','4532',0,57);
/*!40000 ALTER TABLE `wproz` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2011-03-04 12:31:02
