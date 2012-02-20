
/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.

   V 1.5.n : 30.10.09
********************************************************************** */
// 01.11.2010


#define ESC    0x1b

// Contest Klassen

#define ARRLDX    1        // ARRL international DX
#define HNY       2
#define CQWAEDC   3        // Work all Europe
#define WAG       4        // WAG Worked All Germany
#define DTC       5        // Deutscher Telegrafie Contest
#define CHECK     6        // Check Log HF
#define CQWWDX    7        // CQ WW DX Contest ( WAZ )                      - geändert -
#define XMAS      8        // DARC Weihnachtswettbewerb
#define CQWPX     9        // CQ World-Wide WPX Contest
#define DARC10M   10

#define HHCont    11       // Hamburger contest
#define PACC      12       // PACC Contest
#define AGCWHTP   13       // AGCW Handtastenparty
#define CQPTY     14       // DARC Februar-QSO-Party
#define RSGB160M  15       // RSGB 1,8 MHz CW Contest
#define RUSSDX    16       // Russian DX Contest

#define UBADX     45       // UBA Contest
#define UBASP     17       // UBS Spring Contest
#define UBALB     18       // UBA Low Band
#define ONCONT    19       // ON Contest
#define ARRL160M  20       // ARRL 160M Contest
#define ARRL10M   21       // ARRL 10M Contest
#define HADX      22       // HA DX Contest
#define REFC      23       // REF Contest - French HF Championship   
#define SPDX      24       // SP DX Contest


#define OCEANIADX 42       // Coceania DX Contest
#define NASPRINT  43       // North American Sprint
 
#define CQWW160M  46       // CQ World-Wide 160m-Contest

// ------------------
#define FREQZ     1        // hamlib qddQso
#define MODE      2        // hamlib addQso

#define STF       1        // 
#define CBR2      2        // Cabrillo V 2,0
#define CBR3      3        // Cabrillo V 3.0
#define CSV       4        // Textfile


#define CONTINUE  2
#define EDITLOG   3
#define SENTLOG   4
#define DELETELOG 5
#define CONVQTLOG 6
#define CWKEYER   7
#define NEWTABLE  8       // TEST - Funktion      

typedef int classPtr(int, QString);

  