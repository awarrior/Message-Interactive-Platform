package 
{
	import flash.net.Socket;
	import flash.errors.*;
	import flash.text.TextField;
    import flash.events.*;
    import flash.display.*;
	import flash.utils.*;
	import flash.system.*;
	import flash.utils.Timer;
	import flash.net.*;
	
	public class Main extends MovieClip
	{
		private var aSocket:Socket; 		// socket对象
		private var msg:String = ""; 		// 存储读取信息的字串
		private var num:String = "";		// 电话号码字串
		private var mes:String = "";		// 短信内容字串
		private var counter:int = 0;		// 短信记录计数器
		private var item:int = 0;			// 投票项目数
		private var hper:Number = 0.0;		// 一票的高度变化量
		private var max:int = 0;			// 票数最大值
		private var clk:Boolean = false;	// 抽奖标记
		private var lnum:Boolean = false;	// 最后一位数已读标记
		private var myTimer:Timer = new Timer(2000);	// 2s后重连
		private var loader:Loader = new Loader();		// 创建loader类对象
		
		private const IP:String = "127.0.0.1";	// 本地ip地址
		private const PORT:int = 6000;			// 端口号
		private static var conneTime:uint = 0;	// 统计连接次数
		
		/**
		 * 入口
		 */
		public function Main() 
		{			
			// 初始化
			Init();
			// socket连接
			setTimeout(Conn,100);
		}
		
		/**
		 * 初始化函数
		 */
		private function Init()
		{
			_Show.gotoAndStop(1);
			addEventListener(Event.ENTER_FRAME, enterFrameHandler);
		}
		
		/**
		 * 外部图片加载
		 */
		private function imgLoad(url:String)
		{
			var request:URLRequest = new URLRequest(url);
    		loader.contentLoaderInfo.addEventListener(Event.INIT, initHandler);
    		loader.load(request);
		}
		
		/**
		 * 图片加载处理
		 */
		private function initHandler(event:Event)
		{
            this.addChildAt(loader, 0);
			var scale:Number = this.stage.stageHeight/loader.height;
			loader.scaleX *= scale;
			loader.scaleY *= scale;
   			loader.x=this.stage.stageWidth/2-loader.width/2;
   			loader.y=this.stage.stageHeight/2-loader.height/2;
		}

		/**
		 * socket连接
		 */
 		private function Conn() 
        { 
			// 处理安全沙箱
			Security.loadPolicyFile("xmlsocket://"+ IP +":"+PORT);
			// 建立连接
			aSocket = new Socket();
			aSocket.connect(IP, PORT); 
			// 处理连接
			aSocket.addEventListener(Event.CONNECT, OnConnect); 
			// 异常处理
			aSocket.addEventListener(IOErrorEvent.IO_ERROR, ioErrorHandler);
			// 接收信息
            aSocket.addEventListener(ProgressEvent.SOCKET_DATA, ReceiveData);
			// 服务关闭
			aSocket.addEventListener(Event.CLOSE, closeHandler);
        }
		
		/**
		 * 发送数据函数
		 */
		private function Send(socket:Socket,str:String):void
		{
			socket.writeMultiByte(str,"utf-8"); 
            socket.flush();
		}
		
		/**
		 * 接收数据函数
		 */
		private function Receive(socket:Socket):String
		{
			var str:String = "";
			str = socket.readMultiByte(socket.bytesAvailable,"utf-8");
			return str;
		}
		
		/**
		 * 已连接
		 */
        private function OnConnect(e:Event):void 
        { 
            trace("连接成功");
			// 连接成功时停止计时器
			myTimer.stop();
			conneTime = 0;
        }
		
		/**
		 * 错误处理
		 */
        private function ioErrorHandler(e:IOErrorEvent):void 
        { 
            trace("连接失败(2s后重连)");
			// Re-Connect
			myTimer.addEventListener(TimerEvent.TIMER,timeHandler);
			myTimer.start();
        }
		
		private function timeHandler(event:TimerEvent):void
		{
			trace("重新连接");
			conneTime++;
			if(conneTime <900)
				aSocket.connect(IP, PORT);
			else
				trace("连接超时30分钟，请检查网络情况");
		}
		
		private function closeHandler(event:Event):void
		{
			trace("连接失败(2s后重连)");
        	// 重连
			myTimer.addEventListener(TimerEvent.TIMER,timeHandler);
			myTimer.start();
        }
		
		/**
		 * 接收数据
		 */
        private function ReceiveData(e:ProgressEvent):void 
        { 
            //trace("收到的字节数"+aSocket.bytesAvailable); 
			// 接收消息
			msg = "";
            while (aSocket.bytesAvailable) 
            { 
                msg += Receive(aSocket); 
            } 
            //trace(msg);
			// 筛选内容
			Filter(msg);
			return;
		}
		
		/**
		 * 信息过滤处理器
		 */
		private function Filter(str:String):void
		{
			var i:int = 0;
			// 索引为0的字符串值
			var ist:String = str.charAt();
			// 退出
			/*if(ist == "x")
			{
				fscommand("quit");
				return;
			}*/
			// 停止
			if(ist == "s")
			{
				_Show.gotoAndStop(1);
				return;
			}
			// 模式
			if(ist == "m")
			{
				// 索引为1的字符串值
				var sec:String = str.charAt(1);
				// 模式1--评论
				// e.g. m1
				if(sec == "1")
				{
					if(_Show.currentFrame == 2)
						return;
					_Show.gotoAndStop(2);
					imgLoad("Img/Bg1.jpg");
					_Show._Text.gotoAndStop(1);
					_Show._List.gotoAndStop(1);
					_Show._HeightLight.gotoAndStop(1);
					counter = 0;
					return;
				}
				// 模式2--投票
				// e.g. m23200+jacky+tom+potter
				if(sec == "2")
				{
					if(_Show.currentFrame == 3)
						return;
					_Show.gotoAndStop(3);
					imgLoad("Img/Bg2.jpg");
					counter = 0;
					// 参数读取
					// 字串索引值为2
					var thd:String = str.charAt(2);
					item = int(thd);
					switch(item)
					{
						case 0:
							_Show._Vote.gotoAndStop(5);
							_Show._Vote._Item1P.text = "0";
							_Show._Vote._Item2P.text = "0";
							_Show._Vote._Item3P.text = "0";
							_Show._Vote._Item4P.text = "0";
							_Show._Vote._Item5P.text = "0";
							_Show._Vote._Item6P.text = "0";
							_Show._Vote._Item7P.text = "0";
							_Show._Vote._Item8P.text = "0";
							_Show._Vote._Item9P.text = "0";
							_Show._Vote._Item0P.text = "0";
							_Show._Vote._Blue.height = 5.6;
							_Show._Vote._Green.height = 5.6;
							_Show._Vote._Yellow.height = 5.6;
							_Show._Vote._Orange.height = 5.6;
							_Show._Vote._LGreen.height = 5.6;
							_Show._Vote._Blue1.height = 5.6;
							_Show._Vote._Green1.height = 5.6;
							_Show._Vote._Yellow1.height = 5.6;
							_Show._Vote._Orange1.height = 5.6;
							_Show._Vote._LGreen1.height = 5.6;
							_Show._Vote._Blue.x = -400.3;
							_Show._Vote._Green.x = -311.4;
							_Show._Vote._Yellow.x = -222.45;
							_Show._Vote._Orange.x = -133.5;
							_Show._Vote._LGreen.x = -44.55;
							_Show._Vote._Blue1.x = 44.45;
							_Show._Vote._Green1.x = 133.35;
							_Show._Vote._Yellow1.x = 222.3;
							_Show._Vote._Orange1.x = 311.25;
							_Show._Vote._LGreen1.x = 400.2;
							break;
						case 2:
							_Show._Vote.gotoAndStop(1);
							_Show._Vote._Item1P.text = "0";
							_Show._Vote._Item2P.text = "0";
							_Show._Vote._Blue.height = 5.6;
							_Show._Vote._Green.height = 5.6;
							_Show._Vote._Blue.x = -123.3;
							_Show._Vote._Green.x = 123.95;
							break;
						case 3:
							_Show._Vote.gotoAndStop(2);
							_Show._Vote._Item1P.text = "0";
							_Show._Vote._Item2P.text = "0";
							_Show._Vote._Item3P.text = "0";
							_Show._Vote._Blue.height = 5.6;
							_Show._Vote._Green.height = 5.6;
							_Show._Vote._Yellow.height = 5.6;
							_Show._Vote._Blue.x = -216.95;
							_Show._Vote._Green.x = 0.0;
							_Show._Vote._Yellow.x = 217.9;
							break;
						case 4:
							_Show._Vote.gotoAndStop(3);
							_Show._Vote._Item1P.text = "0";
							_Show._Vote._Item2P.text = "0";
							_Show._Vote._Item3P.text = "0";
							_Show._Vote._Item4P.text = "0";
							_Show._Vote._Blue.height = 5.6;
							_Show._Vote._Green.height = 5.6;
							_Show._Vote._Yellow.height = 5.6;
							_Show._Vote._Orange.height = 5.6;
							_Show._Vote._Blue.x = -251;
							_Show._Vote._Green.x = -82.6;
							_Show._Vote._Yellow.x = 85.85;
							_Show._Vote._Orange.x = 254.3;
							break;
						case 5:
							_Show._Vote.gotoAndStop(4);
							_Show._Vote._Item1P.text = "0";
							_Show._Vote._Item2P.text = "0";
							_Show._Vote._Item3P.text = "0";
							_Show._Vote._Item4P.text = "0";
							_Show._Vote._Item5P.text = "0";
							_Show._Vote._Blue.height = 5.6;
							_Show._Vote._Green.height = 5.6;
							_Show._Vote._Yellow.height = 5.6;
							_Show._Vote._Orange.height = 5.6;
							_Show._Vote._LGreen.height = 5.6;
							_Show._Vote._Blue.x = -261.15;
							_Show._Vote._Green.x = -129.9;
							_Show._Vote._Yellow.x = 1.45;
							_Show._Vote._Orange.x = 132.75;
							_Show._Vote._LGreen.x = 266.2;
							break;
					}
					// 字串索引值为3及之后
					var tmp:String = "";
					i = 3;
					while(str.charAt(i) != "+")
					{
						tmp += str.charAt(i);
						i++;
					}
					// 获取票数最大值及每票位移量
					max = int(tmp);
					hper = 500.0/max;
					// 获取项目名字
					var calc:int = 0;
					while(i<str.length)
					{
						if(str.charAt(i) == "+")
						{
							if(_Show._Vote.currentFrame != 5)
							{
								if(calc == 1)
									_Show._Vote._Item1.text = tmp+"\n#1";
								if(calc == 2)
									_Show._Vote._Item2.text = tmp+"\n#2";
								if(calc == 3)
									_Show._Vote._Item3.text = tmp+"\n#3";
								if(calc == 4)
									_Show._Vote._Item4.text = tmp+"\n#4";
							}
							else
							{
								switch(calc)
								{
									case 1:
										_Show._Vote._Item1.text = tmp+"#1";
									case 2:
										_Show._Vote._Item2.text = tmp+"#2";
									case 3:
										_Show._Vote._Item3.text = tmp+"#3";
									case 4:
										_Show._Vote._Item4.text = tmp+"#4";
									case 5:
										_Show._Vote._Item5.text = tmp+"#5";
									case 6:
										_Show._Vote._Item6.text = tmp+"#6";
									case 7:
										_Show._Vote._Item7.text = tmp+"#7";
									case 8:
										_Show._Vote._Item8.text = tmp+"#8";
									case 9:
										_Show._Vote._Item9.text = tmp+"#9";
								}
							}
							calc++;
							tmp = "";
						}
						else
						{
							tmp += str.charAt(i);
						}
						i++;
					}
					if(calc == 2)
						_Show._Vote._Item2.text = tmp+"\n#2";
					if(calc == 3)
						_Show._Vote._Item3.text = tmp+"\n#3";
					if(calc == 4)
						_Show._Vote._Item4.text = tmp+"\n#4";
					if(calc == 5)
						_Show._Vote._Item5.text = tmp+"\n#5";
					if(calc == 10)
						_Show._Vote._Item0.text = tmp+"#0";
					return;
				}
				// 模式3--抽奖
				// e.g. m3
				if(sec == "3")
				{
					if(_Show.currentFrame == 4)
						return;
					_Show.gotoAndStop(4);
					imgLoad("Img/Bg3.jpg");
					counter = 0;
					_Show._Click.addEventListener(MouseEvent.CLICK,clickHandler);
					var nArr:Array = new Array(
						_Show._Lottery._N1, _Show._Lottery._N2,
						_Show._Lottery._N3, _Show._Lottery._N4,
						_Show._Lottery._N5, _Show._Lottery._N6,
						_Show._Lottery._N7, _Show._Lottery._N8,
						_Show._Lottery._N9, _Show._Lottery._N10,
						_Show._Lottery._N11);
					var rArr:Array = new Array(
						_Show._Lottery._R1, _Show._Lottery._R2,
						_Show._Lottery._R3, _Show._Lottery._R4,
						_Show._Lottery._R5, _Show._Lottery._R6,
						_Show._Lottery._R7, _Show._Lottery._R8,
						_Show._Lottery._R9, _Show._Lottery._R10,
						_Show._Lottery._R11);
					for(var m:int=0; m<11; m++)
					{
						nArr[m].gotoAndStop(1);
						rArr[m].gotoAndStop(1);
					}
					return;
				}
			}
			// 评论
			// e.g. c13824557640+hello!
			if(ist == "c")
			{
				// 高亮取消
				// e.g. cc
				if(str.charAt(1) == "c")
				{
					_Show._HeightLight._PhoNum.text = "";
					_Show._HeightLight._Message.text = "";
					_Show._HeightLight.gotoAndPlay(21);
					return;
				}
				// 高亮开始
				// e.g. cl13824557640+hello!
				var aStart:int = 1;
				if(str.charAt(1) != "l")
				{
					_Show._List.gotoAndStop(1);
				}
				else
				{
					aStart = 2;
					_Show._HeightLight.gotoAndPlay(1);
				}
				var flg:Boolean = false;
				num = "";
				mes = "";
				counter++;
				for(i=aStart; i<str.length; i++)
				{
					var s:String = str.charAt(i);
					// 字符串以+为分界
					if(s == "+" && !flg)
					{
						flg = true;
						continue;
					}
					if(!flg)
					{
						num += s;
					}
					else
					{
						mes += s;
					}
				}
				if(str.charAt(1) != "l" && str.charAt(1) != "c")
				{
					// 动画响应
					Respond();
				}
				return;
			}
			// 投票
			// e.g. v1
			if(ist == "v")
			{
				if(counter >= max)
				{
					return;
				}
				var per:String = str.charAt(1);
				var pern:int = int(per);
				switch(pern)
				{
					case 1:
						_Show._Vote._Blue.height += hper;
						var t1:int = int(_Show._Vote._Item1P.text);
						t1++;
						_Show._Vote._Item1P.text = String(t1);
						if(_Show._Vote.currentFrame != 5)
							_Show._Vote._Item1P.y -= hper;
						counter++;
						break;
					case 2:
						_Show._Vote._Green.height += hper;
						var t2:int = int(_Show._Vote._Item2P.text);
						t2++;
						_Show._Vote._Item2P.text = String(t2);
						if(_Show._Vote.currentFrame != 5)
							_Show._Vote._Item2P.y -= hper;
						counter++;
						break;
					case 3:
						_Show._Vote._Yellow.height += hper;
						var t3:int = int(_Show._Vote._Item3P.text);
						t3++;
						_Show._Vote._Item3P.text = String(t3);
						if(_Show._Vote.currentFrame != 5)
							_Show._Vote._Item3P.y -= hper;
						counter++;
						break;
					case 4:
						_Show._Vote._Orange.height += hper;
						var t4:int = int(_Show._Vote._Item4P.text);
						t4++;
						_Show._Vote._Item4P.text = String(t4);
						if(_Show._Vote.currentFrame != 5)
							_Show._Vote._Item4P.y -= hper;
						counter++;
						break;
					case 5:
						_Show._Vote._LGreen.height += hper;
						var t5:int = int(_Show._Vote._Item5P.text);
						t5++;
						_Show._Vote._Item5P.text = String(t5);
						if(_Show._Vote.currentFrame != 5)
							_Show._Vote._Item5P.y -= hper;
						counter++;
						break;
					case 6:
						_Show._Vote._Blue1.height += hper;
						var t6:int = int(_Show._Vote._Item6P.text);
						t6++;
						_Show._Vote._Item6P.text = String(t6);
						counter++;
						break;
					case 7:
						_Show._Vote._Green1.height += hper;
						var t7:int = int(_Show._Vote._Item7P.text);
						t7++;
						_Show._Vote._Item7P.text = String(t7);
						counter++;
						break;
					case 8:
						_Show._Vote._Yellow1.height += hper;
						var t8:int = int(_Show._Vote._Item8P.text);
						t8++;
						_Show._Vote._Item8P.text = String(t8);
						counter++;
						break;
					case 9:
						_Show._Vote._Orange1.height += hper;
						var t9:int = int(_Show._Vote._Item9P.text);
						t9++;
						_Show._Vote._Item9P.text = String(t9);
						counter++;
						break;
					case 0:
						_Show._Vote._LGreen1.height += hper;
						var t0:int = int(_Show._Vote._Item0P.text);
						t0++;
						_Show._Vote._Item0P.text = String(t0);
						counter++;
						break;
				}
				return;
			}
			// 抽奖
			// e.g. l13824557640
			if(ist == "l")
			{
				var reArr:Array = new Array(
					_Show._Lottery._R1, _Show._Lottery._R2,
					_Show._Lottery._R3, _Show._Lottery._R4,
					_Show._Lottery._R5, _Show._Lottery._R6,
					_Show._Lottery._R7, _Show._Lottery._R8,
					_Show._Lottery._R9, _Show._Lottery._R10,
					_Show._Lottery._R11);
				var nuArr:Array = new Array(
					_Show._Lottery._N1, _Show._Lottery._N2,
					_Show._Lottery._N3, _Show._Lottery._N4,
					_Show._Lottery._N5, _Show._Lottery._N6,
					_Show._Lottery._N7, _Show._Lottery._N8,
					_Show._Lottery._N9, _Show._Lottery._N10,
					_Show._Lottery._N11);
				var j:int = 0;
				LotResult(reArr, nuArr, str, j);
			}
		}
		
		/**
		 * 抽奖模式结果出现动画
		 */
		private function LotResult(reArr:Array, nuArr:Array, str:String, j:int):void
		{
			var r:String = str.charAt(j+1);
			reArr[j].alpha = 100;
			nuArr[j].alpha = 0;
			nuArr[j].gotoAndStop(1);
			reArr[j].gotoAndPlay(int(r+"1"));
			if(j==10)
			{
				lnum = true;
			}
			if(j<10)
			{
				j++;
				setTimeout(LotResult, 300, reArr, nuArr, str, j);
			}
		}
		
		/**
		 * 评论模式演示动画设定
		 */
		private function Respond():void
		{
			if(counter == 1)
			{
				_Show._Text._Text1._PhoNum.text = num;
				_Show._Text._Text1._Message.text = mes;
				_Show._Text.gotoAndPlay(1);
				_Show._List.gotoAndPlay(1);
				return;
			}
			else if(counter == 2)
			{
				_Show._Text._Text2._PhoNum.text = num;
				_Show._Text._Text2._Message.text = mes;
				_Show._Text.gotoAndPlay(31);
				_Show._List.gotoAndPlay(31);
				return;
			}
			else if(counter == 3)
			{
				_Show._Text._Text3._PhoNum.text = num;
				_Show._Text._Text3._Message.text = mes;
				_Show._Text.gotoAndPlay(61);
				_Show._List.gotoAndPlay(61);
				return;
			}
			else if(counter == 4)
			{
				_Show._Text._Text4._PhoNum.text = num;
				_Show._Text._Text4._Message.text = mes;
				_Show._Text.gotoAndPlay(91);
				_Show._List.gotoAndPlay(91);
				return;
			}
			else
			{
				_Show._Text._Text1._PhoNum.text = _Show._Text._Text2._PhoNum.text;
				_Show._Text._Text1._Message.text = _Show._Text._Text2._Message.text;
				_Show._Text._Text2._PhoNum.text = _Show._Text._Text3._PhoNum.text;
				_Show._Text._Text2._Message.text = _Show._Text._Text3._Message.text;
				_Show._Text._Text3._PhoNum.text = _Show._Text._Text4._PhoNum.text;
				_Show._Text._Text3._Message.text = _Show._Text._Text4._Message.text;
				_Show._Text._Text4._PhoNum.text = num;
				_Show._Text._Text4._Message.text = mes;
				_Show._Text.gotoAndPlay(91);
				_Show._List.gotoAndPlay(91);
				return;
			}
		}
		
		/**
		 * 帧位置监听处理
		 */
		private function enterFrameHandler(event:Event):void 
		{
			if(_Show.currentFrame == 2)
			{
				var cFrame:int = _Show._List.currentFrame;
				if(cFrame == 30 || cFrame == 60 || cFrame == 90 || cFrame == 120)
				{
					_Show._List.stop();
				}
				cFrame = _Show._Text.currentFrame;
				if(cFrame == 30 || cFrame == 60 || cFrame == 90 || cFrame == 120)
				{
					_Show._Text.stop();
				}
				cFrame = _Show._HeightLight.currentFrame;
				if(cFrame == 20 || cFrame == 40)
				{
					_Show._HeightLight.stop();
					if(cFrame == 20)
					{
						_Show._HeightLight._PhoNum.text = num;
						_Show._HeightLight._Message.text = mes;
					}
				}
				return;
			}
			if(_Show.currentFrame == 4)
			{
				var k:int;
				// 开始抽奖
				if(clk)
				{
					var numArr:Array = new Array(
						_Show._Lottery._N1, _Show._Lottery._N2,
						_Show._Lottery._N3, _Show._Lottery._N4,
						_Show._Lottery._N5, _Show._Lottery._N6,
						_Show._Lottery._N7, _Show._Lottery._N8,
						_Show._Lottery._N9, _Show._Lottery._N10,
						_Show._Lottery._N11);
					for(k=0; k<10; k++)
					{
						if(numArr[k].currentFrame == 5)
							numArr[k+1].gotoAndPlay(1);
					}
				}
				var reaArr:Array = new Array(
					_Show._Lottery._R1, _Show._Lottery._R2,
					_Show._Lottery._R3, _Show._Lottery._R4,
					_Show._Lottery._R5, _Show._Lottery._R6,
					_Show._Lottery._R7, _Show._Lottery._R8,
					_Show._Lottery._R9, _Show._Lottery._R10,
					_Show._Lottery._R11);
				for(k=0; k<11; k++)
				{
					var rtmp:int = reaArr[k].currentFrame;
					if(rtmp == 10 || rtmp == 20 || rtmp == 30 || rtmp == 40 || rtmp == 50 ||
				   	   rtmp == 60 || rtmp == 70 || rtmp == 80 || rtmp == 90 || rtmp == 100)
					{
						reaArr[k].stop();
						if(lnum && clk)
						{
							clk = false;
							var flg:Boolean = false;
							var u:int;
							for(u=1; u<12; u++)
							{
								if(msg.charAt(u) != "0")
								{
									flg = true;
									break;
								}
							}
							if(!flg)
							{
								return;
							}
							for(u=1; u<12; u++)
							{
								_Show._Number.text += msg.charAt(u);
							}
							if(counter%2 == 1)
							{
								_Show._Number.text += "\t\t";
							}
							else
							{
								_Show._Number.text += "\n";
							}
							if(counter > 16)
							{
								_Show._Next.text = "滑动鼠标滚轮更精彩！";
							}
						}
					}
				}
			}
    	}
		
		/**
		 * 抽奖按钮处理
		 */
		private function clickHandler(event:MouseEvent):void
		{
			if(!clk)
			{
				clk = true;
				lnum = false;
				counter++;
				Send(aSocket,"click");
				var reachArr:Array = new Array(
					_Show._Lottery._R1, _Show._Lottery._R2,
					_Show._Lottery._R3, _Show._Lottery._R4,
					_Show._Lottery._R5, _Show._Lottery._R6,
					_Show._Lottery._R7, _Show._Lottery._R8,
					_Show._Lottery._R9, _Show._Lottery._R10,
					_Show._Lottery._R11);
				var numberArr:Array = new Array(
					_Show._Lottery._N1, _Show._Lottery._N2,
					_Show._Lottery._N3, _Show._Lottery._N4,
					_Show._Lottery._N5, _Show._Lottery._N6,
					_Show._Lottery._N7, _Show._Lottery._N8,
					_Show._Lottery._N9, _Show._Lottery._N10,
					_Show._Lottery._N11);
				for(var q:int=0; q<11; q++)
				{
					reachArr[q].alpha = 0;
					numberArr[q].alpha = 100;
				}
				_Show._Lottery._N1.gotoAndPlay(1);
			}
		}
    }
}