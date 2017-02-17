# MU SERVER - MU ONLINE  
  
##################################################################################
#### iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - Dao Van Trong

    
##################################################################################
## System Requirement 

### Minimum Hardware: 2GB RAM - 2 CORE

+ Server: Windows NT >=6.0 X64  
+ Client: Windows NT >=6.0 x32 or x64  
+ Microsoft Visual C++ Runtime Libraries 2010/2013/2015 is required!


##########################################################################  
## Server setup  
+ List file for edit IP/Password infomation:  
 - zMuServer\1. DataServer_BattleCore\DataServer.ini  
 - zMuServer\1. DataServer\DataServer.ini  
 - zMuServer\2. ConnectServer\IGC_ServerList.xml  
 - zMuServer\IGCData\IGC_MapServerInfo.xml  
 - zMuServer\Tools\IGC.EssentialTool\config.ini  
   
+ Database need restore or Create: MuOnline, Events, Ranking  
   
 + Default INFO on Server:  
  - IP: 192.168.0.168  
  - Serial: PoweredByIGCN800  
  - Version: 1.05.25 (1.5y) 2286: (1.05.24 - 22869)  
  - MsSQLServer: (local)  
  - MsSQL User: sa  
  - MsSQL Password: SQLpassw0rd  
  - ConnectServer Ports: 44405  
  - GameServer Ports: 56900, 56912, 56913, 56914, 56120  
   
### Client: 
+ Connect Info: edit ServerInfo.bmd with any text editor!  
+ Requirement font: Arial Unicode MS (ARIALUNI.TTF)

	 
##########################################################################  
## Server Start
+ Basic RUN:  
  - 1. DataServer
  - 2. ConnectServer
  - 3. GameServer_Regular



  
###########################################################################
## Game Server Types  
  
1) GameServer_R - Regular Game server for Common Game Play  
2) GameSevrer_A - Arca Server, handles Arca Battle Event (only for Season 8 and later)  
3) GameServer_C - Siege Server, handles Castle Siege Event  
4) GameServer_M - Market Server, handles Loren Market Map, Chaos Castle Survival and Illusion Temple Renewal Events (only for Season 8 and later)  
5) GameServer_B - BattleCore Server, handles all Battle Core events, requires dedicated Data and Join Servers (only for Season 9 and later)  
  
   

##########################################################################  
## Source build   
IGC.dll :: Release::Win32 :: Visual Studio 2015 v140  
GameServer/DataServer/ConnectServer :: Release::x64 :: Visual Studio 2015 v140  
  
 
#### iDev.Games - MuOnline S9EP2 IGCN9.5 - TRONG.WIN - Dao Van Trong  
##################################################################################  
