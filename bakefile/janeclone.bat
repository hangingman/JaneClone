powershell Set-ExecutionPolicy RemoteSigned
powershell %~dp0janeclone.ps1

@rem
@rem wxWidgetsに移動
@rem
cd %~dp0wxWidgets-WX_3_0_2\build\msw

@rem Remove the existing destination folders if the exist so that a complete rebuild occurs.

@rmdir vc120_mswuddll /s /q
@rmdir vc120_mswuddll_x64 /s /q
@rmdir vc120_mswudll /s /q
@rmdir vc120_mswudll_x64 /s /q

@rmdir ..\..\lib\vc120_dll /s /q
@rmdir ..\..\lib\vc120_x64_dll /s /q

@rem Delete the build output files from the last run, if any.

@del 2013x86_Debug.txt
@del 2013x86_Release.txt
@del 2013x64_Debug.txt
@del 2013x64_Release.txt

call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\x86_amd64\vcvarsx86_amd64.bat"

@echo "wxWidgetsをビルドします。ちょっと時間がかかります。"

@rem 64 bit release build
@rem ------------------
nmake -f makefile.vc BUILD=release SHARED=1 COMPILER_VERSION=120 OFFICIAL_BUILD=1 TARGET_CPU=AMD64 >> 2013x64_Release.txt

@rem 64 bit debug build
@rem ------------------
nmake -f makefile.vc BUILD=debug SHARED=1 COMPILER_VERSION=120 OFFICIAL_BUILD=1 TARGET_CPU=AMD64 >> 2013x64_Debug.txt

call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\vcvars32.bat"

@rem 32 bit release build
@rem ------------------
nmake -f makefile.vc BUILD=release SHARED=1 COMPILER_VERSION=120 OFFICIAL_BUILD=1 >> 2013x86_Release.txt

@rem 32 bit debug build
@rem ------------------
nmake -f makefile.vc BUILD=debug SHARED=1 COMPILER_VERSION=120 OFFICIAL_BUILD=1 >> 2013x86_Debug.txt

powershell Set-ExecutionPolicy Restricted