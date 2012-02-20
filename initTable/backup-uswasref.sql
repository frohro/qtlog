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
-- Table structure for table `uswasref`
--

DROP TABLE IF EXISTS `uswasref`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `uswasref` (
  `spref` varchar(4) COLLATE utf8_unicode_ci NOT NULL,
  `stateref` varchar(4) COLLATE utf8_unicode_ci NOT NULL,
  KEY `usrefidex` (`spref`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `uswasref`
--

LOCK TABLES `uswasref` WRITE;
/*!40000 ALTER TABLE `uswasref` DISABLE KEYS */;
INSERT INTO `uswasref` VALUES ('W1','W1'),('K1','W1'),('N1','W1'),('W2','W2'),('K2','W2'),('N2','W2'),('W3','W3'),('K3','W3'),('N3','W3'),('W4','W4'),('K4','W4'),('N4','W4'),('W5','W5'),('K5','W5'),('N5','W5'),('W6','W6'),('K6','W6'),('N6','W6'),('W7','W7'),('K7','W7'),('N7','W7'),('W8','W8'),('K8','W8'),('N8','W8'),('W9','W9'),('K9','W9'),('N9','W9'),('W0','W0'),('K0','W0'),('N0','W0'),('KH7','KH7'),('VE1','VE1'),('VE9','VE9'),('VE2','VE2'),('VE3','VE3'),('VE4','VE4'),('VE5','VE5'),('VE6','VE6'),('VE7','VE7'),('VE8','VE8'),('VO1','VO1'),('VO2','VO2'),('VY1','VY1'),('VY0','VY0'),('VY2','VY2');
/*!40000 ALTER TABLE `uswasref` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2011-03-04 12:29:16
