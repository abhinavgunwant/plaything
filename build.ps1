$buildEXE = "C:\Program Files\Microsoft Visual Studio\2022\Community\Msbuild\Current\Bin\amd64\MSBuild.exe"

Start-Process -FilePath "$buildEXE" -ArgumentList "Plaything.sln","/t:Build","/m"

.\x64\Debug\Plaything.exe

