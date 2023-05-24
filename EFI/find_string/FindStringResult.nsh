#FindString.efi BIOSUpdateResult.log "Process completed"
FindString.efi BIOSUpdateResult.log "Testing..."

if %lasterror% == 0 then
  @echo "ScriptPass"
else
  @echo "ScriptFail"
endif