# XGXunFeiLink  
Plugin Version:3.6
## 简介
连接虚幻引擎5和科大讯飞的WebAPI的引擎插件  
英文文档:
[English Instruction](./README.md)  
中文文档:
[Chinese Instruction](./Instruction_cn.md)
开发日志视频(中文):
[虚幻科大讯飞插件开发实战](https://www.bilibili.com/video/BV13W4y1w7MV/)
油管视频(英文):
[PluginVersion-2.4](https://youtu.be/6NQ4PO2RpOU)
商城链接:
[XGXunFeiLink购买链接](https://www.unrealengine.com/marketplace/en-US/product/xgxunfeilink)  
注意:
1.你需要自行去科大讯飞官网进行注册应用信息,获取AppID,APISecret,APIKey等,大部分功能均可免费试用,部分功能可以长期试用.  
科大讯飞官网分为中文站和全球站,两套网站的信息功能不互通.只有全球站的功能才支持全球调用.
[IFlyTekWebUrl-en](https://global.xfyun.cn/)
[IFlyTekWebUrl-cn](https://www.xfyun.cn/)   
2.本插件并非由官方人员开发,也不包含任何来自科大讯飞的代码,图片等,也不会存储你的应用信息等,功能仅由虚幻引擎的提供代码进行实现通讯交互.
## 代码库介绍
### 文件夹 DocumentPictures:  

文档中的各类图片  

### 文件夹 XGKeDaXunFeiSound-v1.0-Test:  

插件1.0版本开发中的测试代码,请勿使用  

### 文件夹 XGXunFeiLink-v2.4-FreeTrial:  

插件2.4版本,仅包含STT和TTS,免费体验试用版,不允许商用  

### 文件夹 XunFeiLinDemo-v3.6-EngineVersion5.3: 

在插件3.6版本和引擎5.3版本下的测试Demo,主要为了方便蓝图节点拷贝测试使用.  
需要在商城购置插件,引擎安装后,Demo方可运行.   
Demo本身不包含插件.


## 功能简介  

![image](DocumentPictures/Product/G_CategoryList.png)  

星火大模型,语音识别,语音合成,人脸识别,文字识别,图像识别......

支持多引擎版本Windos打包(5.0-5.3),UE5.3已测试可安卓打包使用.其余未测试.  

内置功能跳转链接:

![image](DocumentPictures/DocImg/D_ProjectSettings.png)  

所有API预览:  

![image](DocumentPictures/Product/G_AllMethod.png)  

## 功能详解  
![image](DocumentPictures/Product/G_FunctionList_1.png)  
![image](DocumentPictures/Product/G_FunctionList_2.png)  

### Spark Big Model-星火大模型  

#### Spark(V1.5,V2.0,V3.0)   

星火大模型  

URL:https://www.xfyun.cn/doc/spark/Web.html  

![image](DocumentPictures/Product/G_Spark_Code.png)  

#### SparkV3.0FuncionCall 

星火大模型  

URL:  
https://www.xfyun.cn/doc/spark/Web.html  
  
![image](DocumentPictures/Product/G_SparkV3FunctionCall_Code.png)  

#### SparkHelper  

星火助手  

URL:  
https://www.xfyun.cn/doc/spark/SparkAssistantAPI.html
  

![image](DocumentPictures/Product/G_SparkHelper_Code.png)  

#### SparkHint  

星火微调      

URL:  
https://www.xfyun.cn/doc/spark/%E6%98%9F%E7%81%AB%E5%BE%AE%E8%B0%83%E6%9C%8D%E5%8A%A1Web%E6%96%87%E6%A1%A3.html
  
  
![image](DocumentPictures/Product/G_SparkHint_Code.png)  

#### ImgUnderstand  

图片理解 

URL:  
https://www.xfyun.cn/doc/spark/ImageUnderstanding.html  
  

  
![image](DocumentPictures/Product/G_ImgUnderstand_Code.png)    

### Automatic Speech Recognition-语音识别
#### ASR(Global)  

实时语音识别 (全球)  
  

URL:  
https://global.xfyun.cn/doc/rtasr/rtasr/API.html
  
  
![image](DocumentPictures/Product/G_ASRG_Code.png)  
#### STT  

实时语音识别  
  URL:  
  https://www.xfyun.cn/doc/asr/rtasr/API.html  
  
  
![image](DocumentPictures/Product/G_STT_Code.png)  
#### VoiceDictation  

语音听写(1min)  

URL:  
https://www.xfyun.cn/doc/asr/voicedictation/API.html
  
  
![image](DocumentPictures/Product/G_VD_Code.png)  

### Speech Synthesis-语音合成
#### TTS(Global) 
  
语音合成(全球)  

URL:  
https://global.xfyun.cn/doc/tts/online_tts/API.html
  
  
![image](DocumentPictures/Product/G_TTSG_Code.png)  
#### TTS  

语音合成  

URL:  
https://www.xfyun.cn/doc/tts/online_tts/API.html
  
  
![image](DocumentPictures/Product/G_TTS_Code.png)  

### Face Recognition-人脸识别
#### FaceComparison  

人脸比较  

URL:  
https://www.xfyun.cn/doc/face/xffaceComparisonRecg/API.html  
  
  
![image](DocumentPictures/Product/G_FaceCompare_Code.png)  

### Character Recognition-文字识别
#### Universal Character Recognition  

通用文字识别  

URL:  
https://www.xfyun.cn/doc/words/universal_character_recognition/API.html  

  
![image](DocumentPictures/Product/G_UCR_Code.png)  
#### Universal Character Recognition(intsig)  

通用文字识别(多语种)  

URL:  
https://www.xfyun.cn/doc/words/universal-character-recognition/API.html  

  
![image](DocumentPictures/Product/G_UCRintsig_Code.png)  


### Image Recognition-图像识别
#### Scene Recognition  

场景识别  

URL:  
https://www.xfyun.cn/doc/image/scene-recg/API.html

  
![image](DocumentPictures/Product/G_SceneRecognition_Code.png)  

#### Object Recognition  

物体识别  

URL:  
https://www.xfyun.cn/doc/image/object-recg/API.html  

  
![image](DocumentPictures/Product/G_ObjectRecognition_Code.png)  

#### Place Recoginition  

场所识别  

URL:  
https://www.xfyun.cn/doc/image/place-recg/API.html  

  
![image](DocumentPictures/Product/G_PlaceRecognition.png)  


## XGXunFeiLinkDemo
1.请在Content/GameFrame/BP_XunFeiGameInstance中对于App信息进行配置

![image](DocumentPictures/DocImg/D_GameInstance.png)  


2.所有可以使用的蓝图节点在Content/UI/BP_ShowCode中

![image](DocumentPictures/DocImg/B_ShowCode.png)  


3.所有UI按钮均在Content/UI/WBP_MainUI中

![image](DocumentPictures/DocImg/D_MainUI_Designer.png)  


![image](DocumentPictures/DocImg/D_MainUI_Graph.png)  


4.部分节点需要临时自行准备图片,音频等资源放在项目的Saved目录下进行测试,并在蓝图节点中指定文件位置.如果是测试打包后,也需要拷贝同样的资源在打包后的路径.

## 联系方式  
如果有任何[XGXunFeiLink](https://www.unrealengine.com/marketplace/en-US/product/xgxunfeilink)插件产品问题,请联系我.  

QQ:
709777172  

Email:
709777172@qq.com  

bilibili:
[虚幻小刚](https://space.bilibili.com/8383085)  

