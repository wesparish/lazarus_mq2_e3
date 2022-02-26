|--------------------------------------------------------------------------------|
| Patrol
|
| Author: Fibbon
|
| Patrols, not for AFK use!
|
| Example config:
| [Patrol]
| PullCommand=/casting "cease"
|
| Usage:
|   /bc Patrol <MQ2AdvPath Path Name>
|
| Installation:
| 1) Put e3_Patrol.inc in the e3 Includes folder
| 2) In e3_Setup.inc, add: #include e3 Includes\e3_Patrol.inc
| 3) In Advanced Settings.ini, add: Include#22=patrol under [Included Setups]
|
| State Diagram
|                                          ┌─────────┐
|     ┌────────────────────────────────────┤ Resting ◄────────┐
|     │                                    └─────────┘        │
|     │                                                       │
|┌────▼────┐   ┌──────────┐     ┌───────┐   ┌────────┐    ┌───┴───┐
|│ Started ├──►│Patrolling├────►│Pulling├──►│ Killing├───►│Looting│
|└────▲────┘   └──────────┘     └───────┘   └────────┘    └───┬───┘
|     │                                                       │
|     └───────────────────────────────────────────────────────┘
|--------------------------------------------------------------------------------|

|--------------------------------------------------------------------------------|
| Declare variables used in the macros
|--------------------------------------------------------------------------------|
SUB patrol_Setup
	/declare PatrolDebug bool outer TRUE
  /if (${PatrolDebug}) /echo |- Setup Patrol ==>

  /declare PatrolState string outer started
  /if (${PatrolDebug}) /echo PatrolState: ${PatrolState}
  /declare PatrolPathName string outer

  /if (${Bool[${Ini[${Character_Ini},Patrol]}]}) {
    /call iniToVarV "${Character_Ini},Patrol,PullCommand"  pullCommand string outer
    /if (${PatrolDebug}) /echo Loaded Patrol PullCommand: ${pullCommand}
  }
  
  /if (${PatrolDebug}) /echo <== Setup -|
/RETURN

|--------------------------------------------------------------------------------|
| Call background events and other subs required for checking the macros
|--------------------------------------------------------------------------------|
SUB patrol_Background_Events
  |/if (${PatrolDebug}) /echo |- patrol_Background_Events ==>
  /doevents patrol
  |/if (${PatrolDebug}) /echo <== patrol_Background_Events -|
/RETURN

SUB patrol_MacroSettings
/RETURN

SUB patrol_CharacterSettings
/RETURN

SUB patrol_Aliases
  /if (${PatrolDebug}) /echo |- patrol_Aliases ==>
  /squelch /alias /Patrol /bc Patrol
  /if (${PatrolDebug}) /echo <== patrol_Aliases -|
/RETURN

#event patrol "<#1#> Patrol #2#"
SUB event_patrol(line, ChatSender, PathName)
  /if (${PatrolDebug}) /echo |- event_patrol ==>
  /if (!${ChatSender.Equal[${Me.Name}]}) /return

  /if (${PathName.Length} > 0) {
    /varset PatrolPathName = ${PathName}
    /if (${PatrolDebug}) /echo PatrolPathName: ${PatrolPathName}
  }

  /if (${PatrolState.Equal[started]}) {
    /bc "Starting to patrol with MQ2AdvPath: ${PatrolPathName}..."
  }

  /if (${PatrolDebug}) /echo <== event_patrol -|
/RETURN