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
-- Table structure for table `tApplist`
--

DROP TABLE IF EXISTS `tApplist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tApplist` (
  `appid` varchar(5) NOT NULL,
  `appV` tinyint(4) NOT NULL,
  `application` varchar(24) NOT NULL,
  KEY `idexApp` (`appid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tApplist`
--

LOCK TABLES `tApplist` WRITE;
/*!40000 ALTER TABLE `tApplist` DISABLE KEYS */;
INSERT INTO `tApplist` VALUES ('app1',0,'qtlogDiag'),('app2',0,'addQso'),('app3',0,'addClog'),('app4',0,'addDlog'),('app5',0,'qslDiag'),('app6',0,'qslCardDiag'),('app7',0,'qslDruck'),('app8',0,'laDiag'),('app9',0,'laPref'),('app10',0,'awdStatus'),('app11',0,'ruAwd'),('app12',0,'iotaDiag'),('app13',0,'doc'),('app14',0,'dokDiag'),('app15',0,'dowLoadMgr'),('app16',0,'admindiag'),('app17',0,'cwkeyerDiag'),('app18',0,'dxSpot'),('app19',0,'getitMgr'),('app20',0,'uploadMgrEqsl'),('app22',0,''),('app21',0,'setupDiag');
/*!40000 ALTER TABLE `tApplist` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2011-03-04 12:28:26
