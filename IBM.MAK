
# MAKE file for IBM.MAK

# Revised:  1996-06-02

# Macro definitions

C_SW=-G5e- -O+ -W3 -C 


BarChart.DLL: BarChart.Obj
 ILink /FREE /ALIGN:4 /BASE:0x12000000 /EXEPACK BarChart.Obj /OUT:BarChart.Dll BarChart.Def
 Rc BarChart.Res BarChart.Dll

BarChart.Obj: BarChart.C
 Icc $(C_SW) -Fo$*.Obj BarChart.C

