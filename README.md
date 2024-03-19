# XGXunFeiLink  
Plugin Version:4.1
## Brief  

This is XGXunFeiLink project,which it helps to connect iFlyTek with  UnrealEngine 5.  
At pressent,It contains a lot of functions :   
Spark Big Model  
Automatic Speech Recognition  
Speech Synthesis  
Face Recognition  
Character Recognition  
Image Recognition  


It contains Four parts:   

"DocumentPictures":There are some pictures used by this document.  

"XGKeDaXunFeiSound-v1.0-Test":It is just study and test code.lease do not use it in a formal situation.  

"XGXunFeiLink-v2.4-FreeTrial":The part of code is only for learning.You can try it for free, but it's not commercially available.If you have some commercial use,you can purchase it through Unreal Engine Marketplace.  

"XunFeiLinDemo-v3.6-UE5.3":The project shows you how to quickly use this plugin.Before you start it,you should purchase the plugin and install it to UnealEngine 5.Then,you should prepare some necessary resources(text,auido,img...).You must register your App info from iFlyTek Web by yourself.

Demo Video URL:  

[PluginShowVideo-2.4](https://youtu.be/6NQ4PO2RpOU)  

UE Marketplace URL:  

[XGXunFeiLink](https://www.unrealengine.com/marketplace/en-US/product/xgxunfeilink)  

Chinese Instruction URL:
[中文说明书](./README_CN.md)  

iFlyTek Global Web URL:  

[IFlyTekWebUrl-en](https://global.xfyun.cn/)

iFlyTek Chinese Web URL:  

[IFlyTekWebUrl-cn](https://www.xfyun.cn/)   

## Important Things

CategoryList:  

![image](DocumentPictures/Product/G_CategoryList.png)  

FunctionList:  

![image](DocumentPictures/Product/G_FunctionList_1.png)  

![image](DocumentPictures/Product/G_FunctionList_2.png)  

All Method List:  

![image](DocumentPictures/Product/G_AllMethod.png)  

iFlyTek provides a lot of Web API both in China or in the global region.

This plugin supports these WebAPI in different region. 

Most of WebAPI seem to be similar.

However,they are really different.  
You need different accounts,AppID,APIKey,APISecret,especially when you are in China and out of China.
Keep in mind that right App information correspond with right Web API.

## Get App Information

If you don't create the iFlyTek App,you should create it.

iFlyTek Web(Chinese site,No Global):

[IFlyTekWebUrl-cn](https://www.xfyun.cn/)   

iFlyTek Web(English site,Global):

[IFlyTekWebUrl-en](https://global.xfyun.cn/)

Although the code is similar,Only when iFlyTek supports Web API in these regions,this plugin can work .

You have to register your account and manage your App information through different sites.

It is not very difficult to do . You can translate this web page directly through the browser tool.

I have translated some important interface parameters in my codes and documents.

After you create the App, you should enable corresponding function in your iFlyTek control panel,such as STT ,TTS,Spark.

Be careful that some functions are not free.Howover,You can try it for free in a short time.
  
## Browse The Plugin From UE

You can quickly learn which method and documentation each API corresponds to.

![image](DocumentPictures/DocImg/D_ProjectSettings.png)  

## Functional detail

### Spark Big Model  


#### Spark(V1.5,V2.0,V3.0)   

URL:https://www.xfyun.cn/doc/spark/Web.html  
  

![image](DocumentPictures/Product/G_Spark_Code.png)  

#### SparkV3.0FuncionCall 

URL:https://www.xfyun.cn/doc/spark/Web.html  
  
![image](DocumentPictures/Product/G_SparkV3FunctionCall_Code.png)  

#### SparkHelper  

URL:  
https://www.xfyun.cn/doc/spark/SparkAssistantAPI.html
  
  
![image](DocumentPictures/Product/G_SparkHelper_Code.png)  

#### SparkHint  

URL:  
https://www.xfyun.cn/doc/spark/%E6%98%9F%E7%81%AB%E5%BE%AE%E8%B0%83%E6%9C%8D%E5%8A%A1Web%E6%96%87%E6%A1%A3.html
  
  
![image](DocumentPictures/Product/G_SparkHint_Code.png)  

#### ImgUnderstand  

URL:  
https://www.xfyun.cn/doc/spark/ImageUnderstanding.html  
  
  

![image](DocumentPictures/Product/G_ImgUnderstand_Code.png)    

### Automatic Speech Recognition  

You can call XunFeiBeginRealTimeSpeechToText to begin STT.

You can call XunFeiStopRealTimeSpeechToText to end STT.

It captures audio data from audio input device,and then send it to iFLyTek Web API.

It will call you to recive results.  

It calls you to recive results ,src text,dst text.  

Defualt audio input device must in

[48000 kHz,16 bits, 2 channle],

[48000 kHz,16 bits, 1 channle],

[16000 kHz,16 bits, 1 channle].

If your audio input device is not right,you wil not get src text or dst text.  

#### ASR(Global)  

URL:  
https://global.xfyun.cn/doc/rtasr/rtasr/API.html
  
  

![image](DocumentPictures/Product/G_ASRG_Code.png)  

#### STT  

URL:  
https://www.xfyun.cn/doc/asr/rtasr/API.html  
  
  
![image](DocumentPictures/Product/G_STT_Code.png)  

#### VoiceDictation  

URL:  
https://www.xfyun.cn/doc/asr/voicedictation/API.html
  
  
![image](DocumentPictures/Product/G_VD_Code.png)  


### Speech Synthesis  

You can call XunFeiTextToSpeech to begin TTS.

It is a async blueprint node.

It converts text to USoundWave and wav file.

You have to input the text to convert it.

If you enable bSaveToLocal,you must input the right and absolute file path.

The directory must exist.

The file name must end with ".wav"

You can keep it default for test at first.  
Different languages choose different pronunciations to have the best pronunciation.  
Advanced speakers cost extra.
The Picture Shows where to add a new speeker.  
The Picture after it was translated by the browsing tools is as follow:  
![image](DocumentPictures/TTS_VoiceManage.png)      
  
The Picture Shows where to add a new speeker.
![image](DocumentPictures/TTSG_Speaker.png)  

#### TTS(Global) 

URL:  
https://global.xfyun.cn/doc/tts/online_tts/API.html
  
  
![image](DocumentPictures/Product/G_TTSG_Code.png)  

#### TTS  

URL:  
https://www.xfyun.cn/doc/tts/online_tts/API.html  

![image](DocumentPictures/Product/G_TTS_Code.png)  

### Face Recognition  

#### FaceComparison  

URL:  
https://www.xfyun.cn/doc/face/xffaceComparisonRecg/API.html  
  
  
![image](DocumentPictures/Product/G_FaceCompare_Code.png)  


### Character Recognition  


#### Universal Character Recognition  

URL:  
https://www.xfyun.cn/doc/words/universal_character_recognition/API.html  

  
  
![image](DocumentPictures/Product/G_UCR_Code.png)  

#### Universal Character Recognition(intsig)  

URL:  
https://www.xfyun.cn/doc/words/universal-character-recognition/API.html  

  
  
![image](DocumentPictures/Product/G_UCRintsig_Code.png)  


### Image Recognition


#### Scene Recognition  

URL:  
https://www.xfyun.cn/doc/image/scene-recg/API.html

  
![image](DocumentPictures/Product/G_SceneRecognition_Code.png)  

#### Object Recognition  

URL:  
https://www.xfyun.cn/doc/image/object-recg/API.html  

  
![image](DocumentPictures/Product/G_ObjectRecognition_Code.png)  

#### Place Recoginition  

URL:  
https://www.xfyun.cn/doc/image/place-recg/API.html  

  
![image](DocumentPictures/Product/G_PlaceRecognition.png)  


## XGXunFeiLinkDemo


1.App info shoulud be input "Content/GameFrame/BP_XunFeiGameInstance"

![image](DocumentPictures/DocImg/D_GameInstance.png)  


2.All BP methods are in "Content/UI/BP_ShowCode"

![image](DocumentPictures/DocImg/B_ShowCode.png)  


3.All buttons are in "Content/UI/WBP_MainUI"

![image](DocumentPictures/DocImg/D_MainUI_Designer.png)  


![image](DocumentPictures/DocImg/D_MainUI_Graph.png)  


4.Some nodes need to temporarily prepare pictures, audio and other resources in the Saved directory of the project for testing, and specify the file location in the blueprint node. If the test is packaged, you also need to copy the same resource in the packaged path.  

## Framwork  

XGXunFeiBase    
Process open/ssl->HmacSha256
    
XGXunFeiCore    
Common API Exposed to OtherModule

XGXunFeiSTT    
STT Module

XGXunFeiTTS    
TTS Module

XGXunFeiSpark    
Spark Module

XGXunFeiASRG    
ASRG Module

XGXunFeiTTSG    
TTSG Module

XGXunFeiTR    
TTSG Module

XGXunFeiFR    
TTSG Module

XGXunFeiIR    
TTSG Module

XGXunFeiOther
......(to be added)

XGXunFeiLink  
It contains UE C++ API .

......\Plugins\XGXunFeiLink\Source\XGXunFeiLink\Public\XGXunFeiLinkBPLibrary.h  

Other API is highly customized.
It may be not good for the general design.

![image](DocumentPictures/All_Frame.png)  

Remember it：

If you use C++ API,you should add the modules to your "*.build.cs".
```
public class XGXunFeiDemo : ModuleRules
{
	public XGXunFeiDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore","
			 XGXunFeiLink",
			"XGXunFeiCore",
			"XGXunFeiSTT",
			"XGXunFeiTTS",
			"XGXunFeiSpark",
			"XGXunFeiTTSG",
			"XGXunFeiASRG",
			"XGXunFeiOther......"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });


	}
}

```

## Package
Windows:[5.0-5.3]
Andriod:Only version 5.3 has been tested.
## Other
This plugin does not contain any iFlyTek's files(\*.h,\*.cpp,\*.lib,......)

This plugin does not contain AI code or AI Model code.

This plugin uses "OpenSSL" to encrypt information in the network,but it will not send data to the Creator("OpenSSL").

This plugin will not send any data to the plugin author.

This plugin was not developed by iFlyTek.

This plugin will only send the interactive data to iFLyTek and recieve the message from iFlyTek . This plugin will not generate any data directly.You must comply with the relevant user and data usage agreements aboutt iFlyTek.These agreements can be found in the iFLyTek website.

The part of plugin code was uploaded to GitHub just for learning.

If the plugin is useful for you. Please purchase it in Unreal Engine Marketplace.

Thanks a lot.

## Contact Information    

Please contact me if you have any questions about [XGXunFeiLink](https://www.unrealengine.com/marketplace/en-US/product/xgxunfeilink).

QQ:
709777172  

Email:
709777172@qq.com  

bilibili:
[虚幻小刚](https://space.bilibili.com/8383085)  



