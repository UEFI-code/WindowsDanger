sc delete WinDangerDrv
copy x64\Debug\WindowsDanger.sys C:\
sc create WinDangerDrv binPath= "C:\WindowsDanger.sys" type= kernel start= demand