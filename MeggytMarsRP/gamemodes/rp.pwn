#include <a_samp>
#include <a_mysql>
#include <Pawn.Regex>
//Defines

//Colors
#define COLOR_RED 0xFF0000FF
#define COLOR_GREEN 0x66FF00FF

//Functions
#define SCM SendClientMessage

//New
new MySQL:q_Sql;

//Regex


main(){
	print("Hello, world!");
}

public OnGameModeInit()
{
	SetGameModeText("RolePlay");
	AddPlayerClass(0,1774.8048,-1950.6984,14.1096,358.1338,0,0,0,0,0,0);
	return 1;
}

public OnGameModeExit()
{
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	SCM(playerid, COLOR_RED, "Добро пожаловать на сервер, друг!");
	SCM(playerid, COLOR_RED, "Тебе надо пройти регистрацию, она займёт пару минут!");
	ShowPlayerDialog(playerid, 0,  DIALOG_STYLE_PASSWORD, "Регистрация", "Для того чтобы зарегестрироваться, введите ваш пароль:\nКоличество символов не должно быть меньше 6 и больше 15", "Вперёд", "Выйти");
	return 1;
}

public OnPlayerConnect(playerid)
{
	q_Sql = mysql_connect("localhost", "root", "" , "players");
	if(q_Sql){
		SCM(playerid, COLOR_GREEN, "Соединение с базой данных установленно!");
	}
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
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

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(response){
		switch(dialogid){
			case 0:
			{
				if(strlen(inputtext)<6||strlen(inputtext)>15){
				    ShowPlayerDialog(playerid, 0,  DIALOG_STYLE_PASSWORD, "Регистрация", "Для того чтобы зарегестрироваться, введите ваш пароль:\n{F81414}Количество символов в пароле не должно быть меньше 6 и больше 15", "Вперёд", "Выйти");
				}
				else{
					ShowPlayerDialog(playerid, 1, DIALOG_STYLE_INPUT, "Регистрация", "Теперь введите ваш Email, для того чтобы вы могли восстановить свой пароль, в случае необходимости\nВаш Email:", "Вперёд", "Выйти");
				}
			}
			case 1:
   			{
   			    new regex:email = regex_new("[a-zA-Z0-9_\\.]+@([a-zA-Z0-9\\-]+\\.)+[a-zA-Z]{2,4}");
      			if(regex_check(inputtext, email)){
					SCM(playerid, COLOR_RED, "Регистрация успешно завершена! В путь!");
					SetPlayerPos(playerid, 1774.8048,-1950.6984,14.1096);
				}
				else{
                    ShowPlayerDialog(playerid, 1, DIALOG_STYLE_INPUT, "Регистрация", "Теперь введите ваш Email, для того чтобы вы могли восстановить свой пароль, в случае необходимости\nВаш Email:\n{F81414}Email введён некорректно!", "Вперёд", "Выйти");
				}
   			}
		}
	}
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	return 1;
}
