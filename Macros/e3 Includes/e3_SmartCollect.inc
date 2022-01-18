|--------------------------------------------------------------------------------|
| Smart Collect
|
| Author: Fibbon
|
| Performs a more intelligent version of /collect that is driven by your bot ini file.
|
| Example config:
| [SmartCollect]
| CollectItem=Excellent Silk
| CollectItem=Fine Silk
| CollectFromBot=Fibbon
| CollectFromBot=Arrno
| CollectFromBot=Sprinter
|
| Usage to collect all items from all bots:
|    /bc SmartCollect
|
| Installation:
| 1) Put e3_SmartCollect.inc in the e3 Includes folder
| 2) In e3_Setup.inc, add: #include e3 Includes\e3_SmartCollect.inc
| 3) In Advanced Settings.ini, add: Include#22=smartCollect under [Included Setups]
|--------------------------------------------------------------------------------|

|--------------------------------------------------------------------------------|
| Declare variables used in the macros
|--------------------------------------------------------------------------------|
SUB smartCollect_Setup
	/declare SmartCollectDebug bool outer TRUE
  /if (${SmartCollectDebug}) /echo |- Setup SmartCollect ==>

  /if (${Bool[${Ini[${Character_Ini},SmartCollect]}]}) {
    /call IniToArrayV "${Character_Ini},SmartCollect,CollectItem"  CollectItem
    /if (${SmartCollectDebug}) /echo Loaded SmartCollect CollectItems: ${CollectItem.Size}
    /call IniToArrayV "${Character_Ini},SmartCollect,CollectFromBot"  CollectFromBot
    /if (${SmartCollectDebug}) /echo Loaded SmartCollect CollectFromBot: ${CollectFromBot.Size}
  }
  
  /if (${SmartCollectDebug}) /echo <== Setup -|
/RETURN

|--------------------------------------------------------------------------------|
| Call background events and other subs required for checking the macros
|--------------------------------------------------------------------------------|
SUB smartCollect_Background_Events
  |/if (${SmartCollectDebug}) /echo |- smartCollect_Background_Events ==>
  /doevents smartCollect
  |/if (${SmartCollectDebug}) /echo <== smartCollect_Background_Events -|
/RETURN

SUB smartCollect_MacroSettings
/RETURN

SUB smartCollect_CharacterSettings
/RETURN

SUB smartCollect_Aliases
  /if (${SmartCollectDebug}) /echo |- smartCollect_Aliases ==>
  /squelch /alias /SmartCollect /bc SmartCollect
  /if (${SmartCollectDebug}) /echo <== smartCollect_Aliases -|
/RETURN

#event smartCollect "<#1#> SmartCollect"
SUB event_smartCollect(line, ChatSender)
  /if (!${ChatSender.Equal[${Me.Name}]}) /return

  /declare botIndex int local
  /declare itemIndex int local

  /for botIndex 1 to ${CollectFromBot.Size}
    | Don't attempt to collect items from myself
    /if (!${CollectFromBot[${botIndex}].Equal[${Me.Name}]}) {
      /for itemIndex 1 to ${CollectItem.Size}
        /if (${SmartCollectDebug}) /echo [SmartCollect] /bct ${CollectFromBot[${botIndex}]} gimme ${CollectItem[${itemIndex}]}
        /bct ${CollectFromBot[${botIndex}]} gimme ${CollectItem[${itemIndex}]}
        | Wait for trade window to open, then close, or wait 4s max
        /if (${SmartCollectDebug}) /echo /delay 2s ${Window[TradeWnd].Open}
        /delay 2s ${Window[TradeWnd].Open}
        /if (${SmartCollectDebug}) /echo /delay 2s !${Window[TradeWnd].Open}
        /delay 2s !${Window[TradeWnd].Open}
      /next itemIndex
    }
  /next botIndex
/RETURN