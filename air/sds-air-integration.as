package tools
{
	import flash.events.Event;
	import flash.events.IOErrorEvent;
	import flash.events.SecurityErrorEvent;
	import flash.filesystem.File;
	import flash.filesystem.FileMode;
	import flash.filesystem.FileStream;
	import flash.net.URLLoader;
	import flash.net.URLLoaderDataFormat;
	import flash.net.URLRequest;
	import flash.net.URLRequestMethod;
	import flash.net.URLVariables;
	import flash.utils.ByteArray;
	
	public class AttributionTool
	{
		//Object var to store milestones in
		static private var AttrEvents:Object;
		
		//url to send data to
		static private var url:String; //http://steam.gs/ca/[key]
		
		//variables for saving (complete directory = basePath + subPath + saveName)
		static private var basePath:File = File.cacheDirectory; //e.g: C:\Users\Gebruiker\AppData\Local\Temp (change var depending on coding language and preference)
		static private var subPath:String;
		static private var saveName:String;
		
		static public function init(urlTarget:String):void 
		{
			url = urlTarget;
			
			var splitAr:Array = url.split("/");
			saveName = splitAr[splitAr.length - 1];
			subPath = "attributionTool/" + saveName;
			
			//saved file already exists?
			if (fileExists(basePath, subPath))
			{
				//load AttrEvents from previous run
				AttrEvents = loadFromFile(basePath, subPath); 
				
				//loop through the AttrEvents and resend the ones that are set to false
				for (var name:String in AttrEvents) 
				{
					if (!AttrEvents[name])
					{
						SendAttrEvent(name);
					}
				}
			}
			else
			{
				//start with blank slate and send firstrun
				AttrEvents = new Object();
				SendAttrEvent("firstrun");
			}
		}
		
		//sends a url request with the milestone as variable m attached
		static public function SendAttrEvent(milestone:String):void
		{
			//add the milestone if it doesn't exist and save that (in case the url request fails)
			if (!milestoneExists(milestone))
			{
				addMilestone(milestone);
				saveToFile(basePath, subPath, AttrEvents);
			}
			
			//setup and send the url request
			var request:URLRequest = new URLRequest(url);
			request.method = URLRequestMethod.GET;
			
			var variables:URLVariables = new URLVariables();
			variables.m = milestone;
			request.data = variables;
			
			var loader:URLLoader = new URLLoader();
			loader.addEventListener(Event.COMPLETE, loaderCompleteHandler, false, 0, true); //last arg sets useWeakReference to true so it this eventlistener garbage-collected
			loader.addEventListener(SecurityErrorEvent.SECURITY_ERROR, securityErrorHandler, false, 0, true); //catch url fail because of security
			loader.addEventListener(IOErrorEvent.IO_ERROR, ioErrorHandler, false, 0, true); //catch wrong url
			loader.dataFormat = URLLoaderDataFormat.TEXT;
			loader.load(request);
		}
		
		//helper functions for url request catching
		static private function loaderCompleteHandler(e:Event):void
		{
			var jsonData:Object = JSON.parse(e.target.data);
			setMilestone(jsonData.milestone, true);
			
			saveToFile(basePath, subPath, AttrEvents);
		}
		
		static private function securityErrorHandler(e:SecurityErrorEvent):void
		{
			//something wrong with security
			trace("Something went wrong. Probably something with security or permission");
		}
		
		static private function ioErrorHandler(e:IOErrorEvent):void
		{
			//nonexisting milestone sent, no internet connection, wrong url
			trace("Something went wrong. An non-existing milestone was sent, the urlTarget is wrong or there's no internet connection");
		}
		
		//helper functions for milestones
		static private function getMilestone(milestone:String):Boolean 
		{
			return AttrEvents[milestone];
		}
		
		static private function addMilestone(milestone:String):void 
		{
			AttrEvents[milestone] = false;
		}
		
		static private function setMilestone(milestone:String, value:Boolean):void 
		{
			AttrEvents[milestone] = value;
		}
		
		static private function milestoneExists(milestone:String):Boolean 
		{
			return AttrEvents[milestone] != undefined;
		}
		
		//helper functions for saving
		static private function saveToFile(basePath:File, path:String, data:Object):void
		{
			var myFile:File = basePath;
			myFile = myFile.resolvePath(path); 
			var myFileStream:FileStream = new FileStream(); 
			myFileStream.open(myFile, FileMode.WRITE); 
			
			var ba:ByteArray = new ByteArray();
			ba.writeObject(data);
			myFileStream.writeBytes(ba);
			
			myFileStream.close();
		}
		
		static private function loadFromFile(basePath:File, path:String):*
		{
			var myFile:File = basePath; 
			myFile = myFile.resolvePath(path); 
			var myFileStream:FileStream = new FileStream(); 
			myFileStream.open(myFile, FileMode.UPDATE);
			
			var obj:*;
			
			if(myFileStream.bytesAvailable > 0){
				obj = myFileStream.readObject();
			}
			
			myFileStream.close();
			
			return obj;
		}
		
		static private function fileExists(basePath:File, path:String):Boolean
		{
			var myFile:File = basePath; 
			myFile = myFile.resolvePath(path); 
			
			return myFile.exists;
		}
	}
}