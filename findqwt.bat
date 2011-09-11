@echo off

set DRIVES=A:\*, B:\*, C:\*, D:\*, E:\*, F:\*, G:\*, H:\*, I:\*, J:\*, K:\*, L:\*, M:\*, N:\*, O:\*, P:\*, Q:\*, R:\*, S:\*, T:\*, U:\*, V:\*, W:\*, X:\*, Y:\*, Z:\*
for /D %%G in (%1*, %DRIVES%) DO (
    echo %%G | findstr "KDAB" && GOTO END
)

:END
