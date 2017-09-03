//������ ����� ��� ����� ���� �������� �� ��� ������ � ��� ����, � ����� ����������� � ������� locahost ������ localhost axuet
#include <a_samp>
#include <a_mysql>
#include <mxINI>
#include <Pawn.Regex>
#include <streamer>
#include <sscanf2>
#include <foreach>
#include <mxdate>
#include <dc_cmd>
#include <strlib>
//Defines

#define MySqlHost   "localhost"
#define MySqlUser   "root"
#define MySqlPass   ""
#define MySqlDB     "players"

#define PN(%1) PlayerInfo[%1][pName]
#define logged(%1) PlayerInfo[%1][pLogged]
#define kick(%1) SetTimerEx("KickFromServer", 300, false, "i", %1);

//Colors
#define COLOR_RED 0xFF0000FF
#define COLOR_GREEN 0x66FF00FF

//Functions
#define SCM SendClientMessage
#define ownpublic%0(%1) forward%0(%1); public%0(%1)// %0 - ��� ��������, %1 - ��� ��������(Example: public Hello == public %0, public Hello(playerid) == public %0(%1)). ����� ����������� ����� ����� ���� ������� ���������

//Variables
new MySQL:database;// ���������� ���� ��

enum pInfo{// ��� ���� � ������
	pID,
	pName[MAX_PLAYER_NAME], //MAX_PLAYER_NAME ���������� ��������
	pPass[21],
	pEmail[51],
	pSex,
	pSkin,
	pLevel,
	pMoney,
	pLogged,
}

new PlayerInfo[MAX_PLAYERS][pInfo];//Max Players - ����� ��� ������������ �������� ������� ����

main(){
	print("Hello, world!");
}

GivePlayerCash(playerid, money){
	PlayerInfo[playerid][pMoney] += money;
    ResetPlayerMoney(playerid);//�������� ��� ���������� �����
    GivePlayerMoney(playerid, PlayerInfo[playerid][pMoney]);//������ ���������� �����
	return PlayerInfo[playerid][pMoney];
}

SetPlayerCash(playerid, money)// �������, ���� ���������� ����� ���������� �����
{
       PlayerInfo[playerid][pMoney] = money;
       ResetPlayerMoney(playerid);//�������� ��� ���������� �����
       GivePlayerMoney(playerid,PlayerInfo[playerid][pMoney]);///������ ���������� �����
       return PlayerInfo[playerid][pMoney];
}


ResetPlayerCash(playerid)// ������� ��� ������ � ������������
{
   PlayerInfo[playerid][pMoney] = 0;
   ResetPlayerMoney(playerid);//�������� ��� ���������� �����
   GivePlayerMoney(playerid,PlayerInfo[playerid][pMoney]);//������ ���������� �����
   return PlayerInfo[playerid][pMoney];
}

public OnGameModeInit()
{
	mysql_log(ALL);
	SetGameModeText("RolePlay");
	EnableStuntBonusForAll(0);
	DisableInteriorEnterExits();
	database = mysql_connect(MySqlHost,MySqlUser,MySqlPass,MySqlDB);// ����������� � ��
	return 1;
}

public OnGameModeExit()
{
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	return 1;
}

public OnPlayerConnect(playerid)
{
	GetPlayerName(playerid, PN(playerid), MAX_PLAYER_NAME);// �������� ���
	static const checkplayer[] = "SELECT * FROM `players` WHERE `Name` = '%s'";//��� ���� ������ �������, ����� ����� ���������� �������� ������ ������
	new query_string[sizeof(checkplayer)+MAX_PLAYER_NAME-2];//��������� ����.�������� �������. -2 ��� %s(������ ������ ������).
	format(query_string, sizeof(query_string), checkplayer, PN(playerid));//������������� ��� ���� �������, ������������ �����, ��� � ����� �� ��������
 	mysql_tquery(database, query_string, "FindPlayerInTable", "i", playerid);//������ � �� � ������ � �������(����� ��� ����))
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	static const saveplayerdata[] = "UPDATE players SET Sex='%i', Skin='%i', Level='%i', Money='%i' WHERE Name = '%s'";
	new query_string[sizeof(saveplayerdata)+MAX_PLAYER_NAME+100];
	format(query_string, sizeof(query_string), saveplayerdata, PlayerInfo[playerid][pSex],PlayerInfo[playerid][pSkin], PlayerInfo[playerid][pLevel],PlayerInfo[playerid][pMoney],PN(playerid));
	mysql_tquery(database, query_string);
    PlayerInfo[playerid][pLogged] = 0;
	return 1;
}

public OnPlayerSpawn(playerid)
{
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	return 1;
}

public OnPlayerText(playerid, text[])
{
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	if (strcmp("/mycommand", cmdtext, true, 10) == 0)
	{
		SendClientMessage(playerid, COLOR_RED, "Hello, world!");
		return 1;
	}
	return 0;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return 1;
}

public OnRconCommand(cmd[])
{
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	return 1;
}

public OnObjectMoved(objectid)
{
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	return 1;
}

public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}



public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) // �������
{
		switch(dialogid){
			case 0://�����������
			{
				if(response){
			        if(strlen(inputtext)<6||strlen(inputtext)>20){
				    ShowPlayerDialog(playerid, 0,  DIALOG_STYLE_PASSWORD, "�����������", "��� ���� ����� ������������������, ������� ��� ������:\n{F81414}���������� �������� � ������ �� ������ ���� ������ 6 � ������ 20!", "�����", "�����");
					}
					else{
					    strmid(PlayerInfo[playerid][pPass], inputtext, 0, strlen(inputtext), 21);
						ShowPlayerDialog(playerid, 1, DIALOG_STYLE_INPUT, "�����������", "������ ������� ��� Email, ��� ���� ����� �� ����� ������������ ���� ������, � ������ �������������\n��� Email:", "�����", "�����");
					}
				}else{
					SCM(playerid, COLOR_RED, "������� ����� ESC ��� ��������� ������� /q");
					kick(playerid)
				}
			}
			case 1:
			{
			   if(response){
					new regex:email = regex_new("[a-zA-Z0-9_\\.]+@([a-zA-Z0-9\\-]+\\.)+[a-zA-Z]{2,4}");
					if(regex_check(inputtext, email)){
					    strmid(PlayerInfo[playerid][pEmail], inputtext, 0, strlen(inputtext), 51);
				 		ShowPlayerDialog(playerid, 2, DIALOG_STYLE_LIST, "����� ����", "�������\n�������", "�����!", "�����");
					}
					else{
			            ShowPlayerDialog(playerid, 1, DIALOG_STYLE_INPUT, "�����������", "������ ������� ��� Email, ��� ���� ����� �� ����� ������������ ���� ������, � ������ �������������\n��� Email:\n{F81414}Email ����� �����������!", "�����", "�����");
					}
			   }else{
			    	SCM(playerid, COLOR_RED, "������� ����� ESC ��� ��������� ������� /q");
					kick(playerid)
			   }
			}
			case 2:
			{
			    switch(listitem){
					case 0:
					{
				 		PlayerInfo[playerid][pSex] = 0;
						saveaccount(playerid);
					}
					case 1:
					{
					    PlayerInfo[playerid][pSex] = 1;
						saveaccount(playerid);
					}
				}
			}
			case 3://�����������
			{
				if(response){
                    new query[] = "SELECT * FROM `players` WHERE Name = '%s'";
					new query_set[sizeof(query)+MAX_PLAYER_NAME+2];
					format(query_set, sizeof(query_set), query, PN(playerid), inputtext);
			    	mysql_tquery(database, query_set, "Login","is",playerid, inputtext);
				}else{
				    SCM(playerid, COLOR_RED, "������� ����� ESC ��� ��������� ������� /q");
					kick(playerid)
				}
			}
		}
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	return 1;
}

//Stocks
stock spawnplayer(playerid){
    SpawnPlayer(playerid);
	SetPlayerPos(playerid, 1774.8048,-1950.6984,14.1096);
}

stock saveaccount(playerid){
	static const query[] = "INSERT INTO `players`(Name, Password, Email, Sex, Skin) VALUES('%s', '%s', '%s', '%i', '0')";
	new query_string[sizeof(query)+MAX_PLAYER_NAME+71];
	format(query_string, sizeof(query_string), query, PN(playerid), PlayerInfo[playerid][pPass],PlayerInfo[playerid][pEmail],PlayerInfo[playerid][pSex]);
	mysql_tquery(database, query_string);
    SCM(playerid, COLOR_RED, "����������� ������� ���������! � ����!");
	SpawnPlayer(playerid);
	SetPlayerPos(playerid, 1774.8048,-1950.6984,14.1096);
}

stock ShowPlayerLoginDialog(playerid, dialogid = 0, login = 0){// stock - ���������� �������
	if(!login){
        ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_PASSWORD, "�����������", "��� ����������� ���������� � ������� ������.\n� ���������� �� ����� �������������� ��� ����� �� ������!", "�����", "�����");
	}
	else{
		ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_PASSWORD, "���� �� ������", "��� ����� ������� ��� ������:", "������!", "�����");
	}
}

//Own publics
ownpublic FindPlayerInTable(playerid){ // ����
	new rows;
	cache_get_row_count(rows);
	if(rows){
	   ShowPlayerLoginDialog(playerid, 3, 1);//�����������
	}else{
		ShowPlayerLoginDialog(playerid, 0, 0);//�����������
	}
	return 1;
}

ownpublic Login(playerid, inputtext[]){
	new pass[128];
	cache_get_value_name(0, "Password", pass);
	cache_get_value_name_int(0, "Level", PlayerInfo[playerid][pLevel]);
	cache_get_value_name_int(0,"Money", PlayerInfo[playerid][pMoney]);
	if(isequal(pass, inputtext)){
	    SetPlayerScore(playerid, PlayerInfo[playerid][pLevel]);
		SetPlayerCash(playerid, PlayerInfo[playerid][pMoney]);
		GivePlayerCash(playerid, 500);
	    SCM(playerid, COLOR_RED, "�� ������� ����������������!");
        PlayerInfo[playerid][pLogged] = 1;
		spawnplayer(playerid);
	}else{
		ShowPlayerDialog(playerid, 3, DIALOG_STYLE_PASSWORD, "���� �� ������", "��� ����� ������� ��� ������:", "������!", "�����");
	}
}

ownpublic KickFromServer(playerid){
	Kick(playerid);
	return 1;
}


