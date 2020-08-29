###############################################################
#
# Makefile for MLDesktopApp
#
# Targets:
#		all		-	build
#		clean		-	clean everything
#		
#
###################################################################


CC = "/cygdrive/c/Program Files/Microsoft Visual Studio 8/Common7/IDE/devenv"
APP = MusicLibrary


SETENV:
#	echo Setting environment for using Microsoft Visual Studio 2005 x86 tools.
	export VSINSTALLDIR="/cygdrive/c/Program Files/Microsoft Visual Studio 8"
	export VCINSTALLDIR="/cygdrive/c/Program Files/Microsoft Visual Studio 8/VC"
	export FrameworkDir="/cygdrive/c/WINDOWS/Microsoft.NET/Framework"
	export FrameworkVersion=v2.0.50727
	export FrameworkSDKDir="/cygdrive/c/Program Files/Microsoft Visual Studio 8/SDK/v2.0"
	export DevEnvDir="/cygdrive/c/Program Files/Microsoft Visual Studio 8/Common7/IDE"
	export PATH="/cygdrive/c/Program Files/Microsoft Visual Studio 8/Common7/IDE:/cygrive/c/Program Files/Microsoft Visual Studio 8/VC/BIN:/cygrive/c/Program Files/Microsoft Visual Studio 8/Common7/Tools:/cygrive/c/Program Files/Microsoft Visual Studio 8/Common7/Tools/bin:/cygrive/c/Program Files/Microsoft Visual Studio 8/VC/PlatformSDK/bin:/cygrive/c/Program Files/Microsoft Visual Studio 8/SDK/v2.0/bin:/cygdrive/c/WINDOWS/Microsoft.NET/Framework/v2.0.50727:/cygrive/c/Program Files/Microsoft Visual Studio 8/VC/VCPackages":"$(PATH)"
	export INCLUDE="/cygdrive/c/Program Files/Microsoft Visual Studio 8/VC/ATLMFC/INCLUDE:/cygdrive/c/Program Files/Microsoft Visual Studio 8/VC/INCLUDE:/cygdrive/c/Program Files/Microsoft Visual Studio 8/VC/PlatformSDK/include:/cygdrive/c/Program Files/Microsoft Visual Studio 8/SDK/v2.0/include":"$(INCLUDE)"
	export LIB="/cygdrive/c/Program Files/Microsoft Visual Studio 8/VC/ATLMFC/LIB:/cygdrive/c/Program Files/Microsoft Visual Studio 8/VC/LIB:/cygdrive/c/Program Files/Microsoft Visual Studio 8/VC/PlatformSDK/lib:/cygdrive/c/Program Files/Microsoft Visual Studio 8/SDK/v2.0/lib":"$(LIB)"
	export LIBPATH="/cygdrive/c/WINDOWS/Microsoft.NET/Framework/v2.0.50727:/cygdrive/c/Program Files/Microsoft Visual Studio 8/VC/ATLMFC/LIB":"$(LIBPATH)"

$(APP): SETENV
	$(CC) $(APP).sln /Build



samples: ${APP}

clean: SETENV
	$(CC) $(APP).sln /Clean
