#!nsh

#FindString.efi BIOSUpdateResult.log "Process completed"
FindString.efi BIOSUpdateResult.log "Testing..."

IF EXIST ResultPass.log THEN
@echo "ResultPass.log EXIST : Fail"
ELSE
@echo "ResultPass.log NO EXIST : Pass"
ENDIF

#delete *.log