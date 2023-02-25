cd "C:\Games\Steam\steamapps\common\Sven Co-op\svencoop\addons\metamod\dlls"

if exist Metrics_old.dll (
    del Metrics_old.dll
)
if exist Metrics.dll (
    rename Metrics.dll Metrics_old.dll 
)

exit /b 0