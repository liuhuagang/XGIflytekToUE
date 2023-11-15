# XGXunFeiLink  

## Brief  

This is XGXunFeiLink Project，Which Connects iFlyTek To UnrealEngine 5.  
At Pressent,It contains two functions : RealTime SpeechToText ，TextToSpeechStream.
It Contains two parts:   
The First  is a XGXunFeiLink plugin, which is commercial.You can buy it through UE-MarketSpace.  
The Sencod is a XGKeDaXunFeiSoud plugin,which is just study demo code. There are many video to show you how to code it. Please do not use this plugin in Formal occasions.In fact, there are some bugs,which it keeps the same to video.

Commercial use is prohibited without permission!
If you need Commercial use,Please purchase it In UE Marketspace

封装了科大讯飞的WebApi能力连接到UE5
目前主要是：STT和TTS.
第一个插件XGXunFeiLink完全是商业化封装的插件,推荐使用，商城可以买。
第二个插件XGKeDaXunFeiSoud是一款学习插件，包含了大量代码视频，可以帮助你理解每一行代码字是如何写出来的。或许，有一些bug，但是没必要去修复。  

未经许可,不允许商业化使用~
如有需要,请到UE商城进行购买~

## XGXunFeiLink  
Hi,the plugin helps you connect to iFLyTek WebApi.

At Present,It supports two main functions:RealTime SpeechToText ,TextToSpeech Sream.

It includes follows steps:

### 1.Get App Information

DemoVideoUrl:todo  

If you don't create the iFlyTek App,you should create it.

iFlyTek Web:

https://www.xfyun.cn/

After you create the app,you should open Corresponding function in your iFlyTek web control panel,such as STT or TTS.

Be careful,some functions is not free.Howover,you can get free experience in a short time.  

This free web path  :  

https://www.xfyun.cn/free  

  
STT:  

![image](DocumentPictures/STTFreeEntry.png)    
  
TTS:  

![image](DocumentPictures/TTS_Free.png)    
  


### 2.Set App Information To Plugin

RealTime STT needs: AppID,AppKey;  

![image](DocumentPictures/STTCenterWeb.png)  

TTSStream needs:AppID,AppKey,APPSecret;  

![image](DocumentPictures/TTS_Center.png)      


You can set it in Project Settins->XGPlugins->XGXunFeiCoreSettings.

Remember Enable Plugins XGXunFeiLink
  

STT:  
![image](DocumentPictures/STTSettins.png)  


TTS  :

![image](DocumentPictures/TTS_Settings.png)    



### 3.Begin Or Stop RealTime STT
  
![image](DocumentPictures/STTBegin.png)      

Call XunFeiBeginRealTimeSpeechToText.

Call XunFeiStopRealTimeSpeechToText.

It captures audio data from audio input device,and then send it to iFLyTek Web Api.

It calls you to recive result ,src text,dst text.

#### 3.1 Be Careful:

Defualt Audio InputDevice must in

[48000 kHz,16 bits, 2 channle],

[48000 kHz,16 bits, 1 channle],

[16000 kHz,16 bits, 1 channle].

If your Audio Device is not right,you wil not get src text.  

![image](DocumentPictures/STT_Audio_48000_2_16.png)  

#### 3.2 Delegate:  

![image](DocumentPictures/STTResult.png)  

InitRealTimeSTTDelegate: Tell you whethe to Create connect to iFlyTek and reasons.

RealTimeSTTNoTranslateDelegate:After Create connect successfully,you can get every sentence src text.

RealTimeSTTTranslateDelegate:After Create connect successfully, if plugin and Web Contol Panel enable translation

,you will get everysentence src text and dst text.

#### 3.3 ReqInfo:
  
![image](DocumentPictures/STTReqInfo.png)  

Lang: Speech lanaguge,Chinese is "cn",Engish is "en";

Enable Translte: if it is true ,Web Contol Panel must enable translation. TransType,TransStrategy,TargetLang will be useful.Dst Text will be gived to you.

TargetLang:Convet the speech to Target lanauage.such as "cn","en","cn_cantonese"

......

More Req info:

https://www.xfyun.cn/doc/asr/rtasr/API.html

You can keep it default for test at first.

#### 3.4 C++Api  

Same to blueprint~  

![image](DocumentPictures/STTC++.png)  

### 4.Begin TTS Sream  
  
![image](DocumentPictures/TTS_Blueprint.png)    

DemoVideoUrl:  TODO  

Call XunFeiTextToSpeech.

It is async blueprint node.

It converts text to USoundWave and wav file

#### 4.1 Be Careful:

You mush input the text to convert.

If you enable SaveToLocal,you must input the right th Absolute file path.

The directory must exist.

The file name must end with ".wav"

#### 4.2 Delegate:

On Success: you will get the USoundWave

On Fail:you will get nullptr and reason.

#### 4.3 ReqInfo:

Vcn: you can choose the speaker to say text.Such as :"xiaoyan","aisbabyxu"......
Speed,Volume,Pitch:0~100  
......

More Req info:

https://www.xfyun.cn/doc/tts/online_tts/API.html

You can keep it default for test at first.
Part of Vcn List:  

![image](DocumentPictures/TTS_VoiceManage.png)    

#### 4.4 C++Api  

![image](DocumentPictures/TTS_C++.png)      

### Framwork  

XGXunFeiBase    
Process open/ssl->HmacSha256
    
XGXunFeiCore    
Common Api Exposed to OtherModule

XGXunFeiSTT    
STT Module

XGXunFeiTTS    
TTS Module

XGXunFeiLink  
Anything Exposed to you for use.
Anything C++ Api you shoule use from  

......\Plugins\XGXunFeiLink\Source\XGXunFeiLink\Public\XGXunFeiLinkBPLibrary.h  

Other Api is custom use ,may be not good for common use~~

![image](DocumentPictures/All_Frame.png)

### Package
No problem~

## XGKeDaXunFeiSoud  

StudyVideoUrl:  
https://www.bilibili.com/video/BV11h41137bq/

