# total is 52 subdirs
SUBDIR +=_GSMStatus
SUBDIR +=_HelperReceiver
SUBDIR +=_HelperSender
SUBDIR +=_HTTPLibTest
SUBDIR +=_Keyguard
SUBDIR +=_LaunchCommands
SUBDIR +=_MMSCounter
SUBDIR +=_MMSReceiver
SUBDIR +=_PhoneEventReceiver
SUBDIR +=_PhoneInfo

SUBDIR +=_RadioSample
SUBDIR +=_REMTimer
SUBDIR +=_RingTone
SUBDIR +=_SimpleSMS
SUBDIR +=_STETest
SUBDIR +=AddEmailtoVersaMail
SUBDIR +=BluetoothMgr
SUBDIR +=BtClientServer
SUBDIR +=CameraV2
SUBDIR +=CameraV3

SUBDIR +=Conduit
SUBDIR +=CustomFav
SUBDIR +=ExtendedSound
SUBDIR +=FileBrowserSample
SUBDIR +=FiveWayV2
SUBDIR +=FotoFinish
SUBDIR +=FotoFinishV2
SUBDIR +=FullScreenWriting
SUBDIR +=HtmlLibTest68K
SUBDIR +=IRCommunication

SUBDIR +=MemoryInfo
SUBDIR +=MiniADPCM
SUBDIR +=MiniBuzzer
SUBDIR +=MiniGIF
SUBDIR +=MiniImage
SUBDIR +=MiniMP3
SUBDIR +=MiniMPEG4
SUBDIR +=NavExample
SUBDIR +=NetSelector
SUBDIR +=NVFSTest

SUBDIR +=PalmPhotoCapturePreview
SUBDIR +=PmConnectorTest
SUBDIR +=PushDemo
SUBDIR +=SerialCommunication
SUBDIR +=StatusBar
SUBDIR +=TickStock
SUBDIR +=Treo650PowerOut
SUBDIR +=TungstenT5FlowControl
SUBDIR +=TungstenT5SwitchAudio
SUBDIR +=TungstenT5SwitchAudioAlt
SUBDIR +=VersaMailAccConfig

SUBDIR +=VersaMailAttchmntPlugin
SUBDIR +=VersaMailFontApp

all:
	make clean
	make sample

clean::
	$(foreach DIR, $(SUBDIR), \
		cd $(DIR); \
		make clean; \
		cd ..; \
	)
	
sample:
	$(foreach DIR, $(SUBDIR), \
		cd $(DIR); \
		make; \
		cd ..; \
	)
	
