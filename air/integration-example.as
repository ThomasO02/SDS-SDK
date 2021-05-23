package
{
	import flash.display.Sprite;
	
	/**
	 * ...
	 * @author jeroen wimmers
	 */
	public class Main extends Sprite
	{
		public function Main()
		{
			//initialize tool and specify custom url
			AttributionTool.init("https://steam.gs/ca/85f33160e4f6445b9c2fd7a852d11bc4");
			
			//custom events example
			//AttributionTool.SendAttrEvent('level10');
		}
	}
}