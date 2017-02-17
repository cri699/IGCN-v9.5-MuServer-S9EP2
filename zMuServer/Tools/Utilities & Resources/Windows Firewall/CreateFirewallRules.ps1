####################################################################################
# Dedicated for users who can manage it by following provided instructions.
# Excluded from official support - rev. 15, updated: 31-03-2015
#
# 				International Gaming Center, copyright 2010-2015
# 							www.igc-network.com
# Script for automatic creation of Windows Firewall Rules for IGCN Server Needs
#															
#  Instruction - Follow carefully:
#-----------------------------------------------------------------------------------
# 	STEP 1:
#-----------------------------------------------------------------------------------
# 		
#		Configure below values for own. Ports are configured for default
#		configuration of IGCN Server, do not change unless you want to set own ones.
#
#-----------------------------------------------------------------------------------
# 	STEP 2: 
#-----------------------------------------------------------------------------------
#		Run PowerShell Console as Administrator and execute following command:
#
#			PS:		Set-ExecutionPolicy unrestricted
# 	
# 		Confirm it by pressing 'Y' and ENTER	
#-----------------------------------------------------------------------------------
#	STEP 3:
#-----------------------------------------------------------------------------------
#		Being in PowerShell console access location of the script then execute it
#		by typing:
#
#			PS:		./CreateFirewallRules.ps1 
#
#		Press Enter
#
#-----------------------------------------------------------------------------------
#	STEP 4:
#-----------------------------------------------------------------------------------
#		Execute following command in PowerShell (run as Administrator):
#
#			PS:		Set-ExecutionPolicy restricted
# 	
# 		Confirm it by pressing 'Y' and ENTER
####################################################################################

param (
# .TCP or .UDP is being added automatically to all rule names
#-----------------------------------------------------------------------------------
# Rule Name & Path to DataServer
#-----------------------------------------------------------------------------------
$DataServerRuleName				= "1. IGC.DataServer",
$PathToDataServer 				= "X:\PATH_TO\1. DataServer\IGC.DataServer.exe",
#-----------------------------------------------------------------------------------
# Data Server Ports (\DataServer\IGCDS.ini), no spaces
#-----------------------------------------------------------------------------------
$DataServerTCPPorts 			= "56960,56970,55906",
#-----------------------------------------------------------------------------------
# Rule Name & Path to DataServer_B
#-----------------------------------------------------------------------------------
$DataServerBRuleName			= "1. IGC.DataServer_B",
$PathToDataServerB 				= "X:\PATH_TO\1. DataServer_B\IGC.DataServer_B.exe",
#-----------------------------------------------------------------------------------
# Data Server_B Ports (\DataServer_B\IGCDS.ini), no spaces
#-----------------------------------------------------------------------------------
$DataServerBTCPPorts 			= "56770,56760",
#-----------------------------------------------------------------------------------
# Rule Name & Path to ConnectServer
#-----------------------------------------------------------------------------------
$ConnectServerRuleName			= "2. IGC.ConnectServer",
$PathToConnectServer 			= "X:\PATH_TO\2. ConnectServer\IGC.ConnectServer.exe",
#-----------------------------------------------------------------------------------
# Connect ServerTCP Port (\ConnectServer\IGCCS.ini), no spaces
#-----------------------------------------------------------------------------------
$ConnectServerTCPPorts 			= "44405",
#-----------------------------------------------------------------------------------
# Rule Name & Path to GameServer_R
#-----------------------------------------------------------------------------------
$GameServerRRuleName			= "3. IGC.GameServer_R",
$PathToGameServerR 				= "X:\PATH_TO\3. GameServer_R\IGC.GameServer_R.exe",
#-----------------------------------------------------------------------------------
# GameServer_R TCP Port (\GameServer_R\GameServer.ini), no spaces
#-----------------------------------------------------------------------------------
$GameServerRTCPPort 			= "56900",
#-----------------------------------------------------------------------------------
# Rule Name & Path to GameServer_A
#-----------------------------------------------------------------------------------
$GameServerARuleName			= "4. IGC.GameServer_A",
$PathToGameServerA 				= "X:\PATH_TO\4. GameServer_A\IGC.GameServer_A.exe",
#-----------------------------------------------------------------------------------
# GameServer_A TCP Port (\GameServer_A\GameServer.ini), no spaces
#-----------------------------------------------------------------------------------
$GameServerATCPPort 			= "56913",
#-----------------------------------------------------------------------------------
# Rule Name & Path to GameServer_C
#-----------------------------------------------------------------------------------
$GameServerCRuleName			= "5. IGC.GameServer_C",
$PathToGameServerC				= "X:\PATH_TO\5. GameServer_C\IGC.GameServer_C.exe",
#-----------------------------------------------------------------------------------
# GameServer_C TCP Port (\GameServer_C\GameServer.ini), no spaces
#-----------------------------------------------------------------------------------
$GameServerCTCPPort 			= "56914",
#-----------------------------------------------------------------------------------
# Rule Name & Path to GameServer_M
#-----------------------------------------------------------------------------------
$GameServerMRuleName			= "6. IGC.GameServer_M",
$PathToGameServerM				= "X:\PATH_TO\6. GameServer_M\IGC.GameServer_M.exe",
#-----------------------------------------------------------------------------------
# GameServer_M TCP Port (\GameServer_M\GameServer.ini), no spaces
#-----------------------------------------------------------------------------------
$GameServerMTCPPort 			= "56912",
#-----------------------------------------------------------------------------------
# Rule Name & Path to GameServer_B
#-----------------------------------------------------------------------------------
$GameServerBRuleName			= "6. IGC.GameServer_B",
$PathToGameServerB				= "X:\PATH_TO\6. GameServer_B\IGC.GameServer_B.exe",
#-----------------------------------------------------------------------------------
# GameServer_B TCP Port (\GameServer_B\GameServer.ini), no spaces
#-----------------------------------------------------------------------------------
$GameServerBTCPPort 			= "56120",
#-----------------------------------------------------------------------------------
# Rule Name & Path to ChatServer
#-----------------------------------------------------------------------------------
$ChatServerRuleName				= "7. IGC.ChatServer",
$PathToChatServer				= "X:\PATH_TO\7. ChatServer\IGC.ChatServer.exe",
#-----------------------------------------------------------------------------------
# ChatServer TCP Port (\ChatServer\config.ini), no spaces
#-----------------------------------------------------------------------------------
$ChatServerTCPPort 				= "56980",
#-----------------------------------------------------------------------------------
# Game Servers Auth Port, DO NOT CHANGE UNLESS YOU KNOW WHAT YOU DO, no spaces
#-----------------------------------------------------------------------------------
$GameServerAuthServerPort 		= "52854",
#-----------------------------------------------------------------------------------
# Connect Server Auth Port, DO NOT CHANGE UNLESS YOU KNOW WHAT YOU DO, no spaces
#-----------------------------------------------------------------------------------
$ConnectServerAuthServerPort 	= "52458",
#-----------------------------------------------------------------------------------
# UDP Port used by DataServer, ConnectServer and Game Server for internal Communication
# Configured in IGCCS.ini and GameServer.ini, no spaces
#-----------------------------------------------------------------------------------
$DSCSGSUDPPort 					= "55667",
#-----------------------------------------------------------------------------------
# Interface type, leave by default if do not need to change
#-----------------------------------------------------------------------------------
$InterfaceType 					= "any",
#-----------------------------------------------------------------------------------
# Private profile value, DO NOT CHANGE
#-----------------------------------------------------------------------------------
$profilePrivate 				= "private",
#-----------------------------------------------------------------------------------
# Public Profile value, DO NOT CHANGE
#-----------------------------------------------------------------------------------
$ProfilePublic 					= "public",
#-----------------------------------------------------------------------------------
#Private and Public Profiles together value, DO NOT CHANGE
#-----------------------------------------------------------------------------------
$ProfilePrivatePublic 			= "private,public",
#-----------------------------------------------------------------------------------
# Local IPs with access to SQL Port, no spaces, separate by comma
# Recommended to leave it ON, unless known what you do
#-----------------------------------------------------------------------------------
$CreateSQLProtectRule			= "1",
$SQLRuleName					= "0. IGC.SQLServer Protect",
$SQLLocalIPAccess 				= "127.0.0.1",
#-----------------------------------------------------------------------------------
# SQL Port, default 1433
#-----------------------------------------------------------------------------------
$SQLPort 						= "1433",
#-----------------------------------------------------------------------------------
# Remote IPs with access to SQL Port, no spaces, separate by comma
# To allow remote access from selected IPs first set $SQLLocalIPAccess to "any"
#-----------------------------------------------------------------------------------
$SQLRemoteIPAccess 				= "any",
#
####################################################################################
####################################################################################
#-----------------------------------------------------------------------------------
# If running more servers set their count below, maximum: 4, otherwise leave 0
#-----------------------------------------------------------------------------------
$AdditionalServersCount			= "0",
#
# Configure below sections corresponding to value set in 'AdditionalServersCount'
#-----------------------------------------------------------------------------------
# Rule Name & Path to Additional GameServer (8) & Port
#---------------------------------------------------------------------------------#1
$GameServer8RuleName			= "8. IGC.GameServer_8",
$PathToGameServer8 				= "X:\PATH_TO\8. GameServer_8\IGC.GameServer_8.exe",
$GameServer8TCPPort 			= "56901",
#-----------------------------------------------------------------------------------
# Rule Name & Path to Additional GameServer (9) & Port
#---------------------------------------------------------------------------------#2
$GameServer9RuleName			= "9. IGC.GameServer_9",
$PathToGameServer9 				= "X:\PATH_TO\9. GameServer_9\IGC.GameServer_9.exe",
$GameServer9TCPPort 			= "56902",
#-----------------------------------------------------------------------------------
# Rule Name & Path to Additional GameServer (10) & Port
#---------------------------------------------------------------------------------#3
$GameServer10RuleName			= "10. IGC.GameServer_10",
$PathToGameServer10 			= "X:\PATH_TO\10. GameServer_10\IGC.GameServer_10.exe",
$GameServer10TCPPort 			= "56903",
#-----------------------------------------------------------------------------------
# Rule Name & Path to Additional GameServer (11) & Port
#---------------------------------------------------------------------------------#4
$GameServer11RuleName			= "11. IGC.GameServer_11",
$PathToGameServer11 			= "X:\PATH_TO\11. GameServer_11\IGC.GameServer_11.exe",
$GameServer11TCPPort 			= "56904"
#-----------------------------------------------------------------------------------
) #DO NOT REMOVE

# -------------- DO NOT MODIFY BELOW THIS LINE UNLESS YOU KNOW WHAT YOU DO -------------- #
###########################################################################################

# Data Server TCP Rule
"`nCreating $DataServerRuleName TCP Rule"
netsh.exe advfirewall firewall add rule name="$DataServerRuleName.TCP" dir=in action=allow program="$PathToDataServer" protocol="TCP" localip="127.0.0.1" remoteip=any description="IGC.DataServer TCP Rule" localport="$DataServerTCPPorts"  profile="$profilePrivate" interfacetype="$InterfaceType"
#############################

# DataServer UDP Rule
"`nCreating $DataServerRuleName UDP Rule"
netsh.exe advfirewall firewall add rule name="$DataServerRuleName.UDP" dir=in action=allow program="$PathToDataServer" protocol="UDP" localip="127.0.0.1" remoteip=any description="IGC.DataServer UDP Rule" localport="$DSCSGSUDPPort"  profile="$profilePrivate" interfacetype="$InterfaceType"
#############################

# Data Server_B TCP Rule
"`nCreating $DataServerBRuleName TCP Rule"
netsh.exe advfirewall firewall add rule name="$DataServerBRuleName.TCP" dir=in action=allow program="$PathToDataServerB" protocol="TCP" localip="127.0.0.1" remoteip=any description="IGC.DataServer B TCP Rule" localport="$DataServerBTCPPorts"  profile="$profilePrivate" interfacetype="$InterfaceType"
#############################

# DataServer_B UDP Rule
"`nCreating $DataServerBRuleName UDP Rule"
netsh.exe advfirewall firewall add rule name="$DataServerBRuleName.UDP" dir=in action=allow program="$PathToDataServerB" protocol="UDP" localip="127.0.0.1" remoteip=any description="IGC.DataServer B UDP Rule" localport="$DSCSGSUDPPort"  profile="$profilePrivate" interfacetype="$InterfaceType"
#############################

# ConnectServer TCP Rule
"`nCreating $ConnectServerRuleName TCP Rule"
netsh.exe advfirewall firewall add rule name="$ConnectServerRuleName.TCP" dir=in action=allow program="$PathToConnectServer" protocol="TCP" localip=any remoteip=any description="IGC.ConnectServer TCP Rule" localport="$ConnectServerTCPPorts,$ConnectServerAuthServerPort"  profile="$ProfilePrivatePublic" interfacetype="$InterfaceType"
#############################

# ConnectServer UDP Rule
"`nCreating $ConnectServerRuleName UDP Rule"
netsh.exe advfirewall firewall add rule name="$ConnectServerRuleName.UDP" dir=in action=allow program="$PathToConnectServer" protocol="UDP" localip="127.0.0.1" remoteip=any description="IGC.ConnectServer UDP Rule" localport="$DSCSGSUDPPort"  profile="$profilePrivate" interfacetype="$InterfaceType"
#############################

# GameServer_R TCP Rule
"`nCreating $GameServerRRuleName TCP Rule"
netsh.exe advfirewall firewall add rule name="$GameServerRRuleName.TCP" dir=in action=allow program="$PathToGameServerR" protocol="TCP" localip=any remoteip=any description="IGC.GameServer R TCP Rule" localport="$GameServerRTCPPort,$GameServerAuthServerPort"  profile="$ProfilePrivatePublic" interfacetype="$InterfaceType"
#############################

# GameServer_R UDP Rule
"`nCreating $GameServerRRuleName UDP Rule"
netsh.exe advfirewall firewall add rule name="$GameServerRRuleName.UDP" dir=in action=allow program="$PathToGameServerR" protocol="UDP" localip=any remoteip="127.0.0.1" description="IGC.GameServer R UDP Rule" localport="$DSCSGSUDPPort"  profile="$profilePrivate" interfacetype="$InterfaceType"
#############################

# GameServer_A TCP Rule
"`nCreating $GameServerARuleName TCP Rule"
netsh.exe advfirewall firewall add rule name="$GameServerARuleName.TCP" dir=in action=allow program="$PathToGameServerA" protocol="TCP" localip=any remoteip=any description="IGC.GameServer A TCP Rule" localport="$GameServerATCPPort,$GameServerAuthServerPort"  profile="$ProfilePrivatePublic" interfacetype="$InterfaceType"
#############################

# GameServer_A UDP Rule
"`nCreating $GameServerARuleName UDP Rule"
netsh.exe advfirewall firewall add rule name="$GameServerARuleName.UDP" dir=in action=allow program="$PathToGameServerA" protocol="UDP" localip=any remoteip="127.0.0.1" description="IGC.GameServer A UDP Rule" localport="$DSCSGSUDPPort"  profile="$profilePrivate" interfacetype="$InterfaceType"
#############################

# GameServerC TCP Rule
"`nCreating $GameServerCRuleName TCP Rule"
netsh.exe advfirewall firewall add rule name="$GameServerCRuleName.TCP" dir=in action=allow program="$PathToGameServerC" protocol="TCP" localip=any remoteip=any description="IGC.GameServer C TCP Rule" localport="$GameServerCTCPPort,$GameServerAuthServerPort"  profile="$ProfilePrivatePublic" interfacetype="$InterfaceType"

#############################

# GameServerC UDP Rule
"`nCreating $GameServerCRuleName UDP Rule"
netsh.exe advfirewall firewall add rule name="$GameServerCRuleName.UDP" dir=in action=allow program="$PathToGameServerC" protocol="UDP" localip="127.0.0.1" remoteip=any description="IGC.GameServer C UDP Rule" localport="$DSCSGSUDPPort"  profile="$profilePrivate" interfacetype="$InterfaceType"
############################

# GameServerM TCP Rule
"`nCreating $GameServerMRuleName TCP Rule"
netsh.exe advfirewall firewall add rule name="$GameServerMRuleName.TCP" dir=in action=allow program="$PathToGameServerM" protocol="TCP" localip=any remoteip=any description="IGC.GameServer M TCP Rule" localport="$GameServerMTCPPort,$GameServerAuthServerPort"  profile="$ProfilePrivatePublic" interfacetype="$InterfaceType"

#############################

# GameServerM UDP Rule
"`nCreating $GameServerMRuleName UDP Rule"
netsh.exe advfirewall firewall add rule name="$GameServerMRuleName.UDP" dir=in action=allow program="$PathToGameServerM" protocol="UDP" localip="127.0.0.1" remoteip=any description="IGC.GameServer M UDP Rule" localport="$DSCSGSUDPPort"  profile="$profilePrivate" interfacetype="$InterfaceType"

############################

# GameServerB TCP Rule
"`nCreating $GameServerBRuleName TCP Rule"
netsh.exe advfirewall firewall add rule name="$GameServerBRuleName.TCP" dir=in action=allow program="$PathToGameServerB" protocol="TCP" localip=any remoteip=any description="IGC.GameServer B TCP Rule" localport="$GameServerBTCPPort,$GameServerAuthServerPort"  profile="$ProfilePrivatePublic" interfacetype="$InterfaceType"

#############################

# GameServerB UDP Rule
"`nCreating $GameServerBRuleName UDP Rule"
netsh.exe advfirewall firewall add rule name="$GameServerBRuleName.UDP" dir=in action=allow program="$PathToGameServerB" protocol="UDP" localip="127.0.0.1" remoteip=any description="IGC.GameServer B UDP Rule" localport="$DSCSGSUDPPort"  profile="$profilePrivate" interfacetype="$InterfaceType"
############################

# ChatServer TCP Rule
"`nCreating $ChatServerRuleName TCP Rule"
netsh.exe advfirewall firewall add rule name="$ChatServerRuleName.TCP" dir=in action=allow program="$PathToChatServer" protocol="TCP" localip=any remoteip=any description="IGC.ChatServer TCP Rule" localport="$ChatServerTCPPort"  profile="$ProfilePrivatePublic" interfacetype="$InterfaceType"

#############################

if ($CreateSQLProtectRule -eq "1")
{
    # SQL Port Rule
    "`nCreating $SQLRuleName Rule"
    netsh.exe advfirewall firewall add rule name="$SQLRuleName" dir=in action=allow protocol="TCP" localip="$SQLLocalIPAccess" remoteip="$SQLRemoteIPAccess" description="IGC.SQLServerPort Rule" localport="$SQLPort"  profile="$ProfilePrivatePublic" interfacetype="$InterfaceType"
}



if ($AdditionalServersCount -eq "1" -or $AdditionalServersCount -eq "2" -or $AdditionalServersCount -eq "3" -or $AdditionalServersCount -eq "4")
{
    # GameServer 8 TCP Rule
    "`nCreating $GameServer8RuleName TCP Rule"
    netsh.exe advfirewall firewall add rule name="$GameServer8RuleName.TCP" dir=in action=allow program="$PathToGameServer8" protocol="TCP" localip=any remoteip=any description="IGC.GameServer 8 TCP Rule" localport="$GameServer8TCPPort,$GameServerAuthServerPort"  profile="$ProfilePrivatePublic" interfacetype="$InterfaceType"
    #############################
    # GameServer 8 UDP Rule
    "`nCreating $GameServer8RuleName UDP Rule"
    netsh.exe advfirewall firewall add rule name="$GameServer8RuleName.UDP" dir=in action=allow program="$PathToGameServer8" protocol="UDP" localip=any remoteip="127.0.0.1" description="IGC.GameServer 8 UDP Rule" localport="$DSCSGSUDPPort"  profile="$profilePrivate" interfacetype="$InterfaceType"
    #############################
}


if ($AdditionalServersCount -eq "2" -or $AdditionalServersCount -eq "3" -or $AdditionalServersCount -eq "4")
{
    # GameServer 9 TCP Rule
    "`nCreating $GameServer9RuleName TCP Rule"
    netsh.exe advfirewall firewall add rule name="$GameServer9RuleName.TCP" dir=in action=allow program="$PathToGameServer9" protocol="TCP" localip=any remoteip=any description="IGC.GameServer 9 TCP Rule" localport="$GameServer9TCPPort,$GameServerAuthServerPort"  profile="$ProfilePrivatePublic" interfacetype="$InterfaceType"
    #############################
    # GameServer 9 UDP Rule
    "`nCreating $GameServer9RuleName UDP Rule"
    netsh.exe advfirewall firewall add rule name="$GameServer9RuleName.UDP" dir=in action=allow program="$PathToGameServer9" protocol="UDP" localip=any remoteip="127.0.0.1" description="IGC.GameServer 9 UDP Rule" localport="$DSCSGSUDPPort"  profile="$profilePrivate" interfacetype="$InterfaceType"
    #############################
}


if ($AdditionalServersCount -eq "3" -or $AdditionalServersCount -eq "4")
{
    # GameServer 10 TCP Rule
    "`nCreating $GameServer10RuleName TCP Rule"
    netsh.exe advfirewall firewall add rule name="$GameServer10RuleName.TCP" dir=in action=allow program="$PathToGameServer10" protocol="TCP" localip=any remoteip=any description="IGC.GameServer 10 TCP Rule" localport="$GameServer10TCPPort,$GameServerAuthServerPort"  profile="$ProfilePrivatePublic" interfacetype="$InterfaceType"
    #############################
    # GameServer 10 UDP Rule
    "`nCreating $GameServer10RuleName UDP Rule"
    netsh.exe advfirewall firewall add rule name="$GameServer10RuleName.UDP" dir=in action=allow program="$PathToGameServer10" protocol="UDP" localip=any remoteip="127.0.0.1" description="IGC.GameServer 10 UDP Rule" localport="$DSCSGSUDPPort"  profile="$profilePrivate" interfacetype="$InterfaceType"
    #############################
}


if ($AdditionalServersCount -eq "4")
{
    # GameServer 11 TCP Rule
    "`nCreating $GameServer11RuleName TCP Rule"
    netsh.exe advfirewall firewall add rule name="$GameServer11RuleName.TCP" dir=in action=allow program="$PathToGameServer11" protocol="TCP" localip=any remoteip=any description="IGC.GameServer 11 TCP Rule" localport="$GameServer11TCPPort,$GameServerAuthServerPort"  profile="$ProfilePrivatePublic" interfacetype="$InterfaceType"
    #############################
    # GameServer 11 UDP Rule
    "`nCreating $GameServer11RuleName UDP Rule"
    netsh.exe advfirewall firewall add rule name="$GameServer11RuleName.UDP" dir=in action=allow program="$PathToGameServer11" protocol="UDP" localip=any remoteip="127.0.0.1" description="IGC.GameServer 11 UDP Rule" localport="$DSCSGSUDPPort"  profile="$profilePrivate" interfacetype="$InterfaceType"
    #############################
}

"`nNOTE: Remember to execute 'Set-ExecutionPolicy restricted' once finished`n"

"`nExiting -> DONE"
exit