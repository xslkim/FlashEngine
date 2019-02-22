var folderPath = './'; //fl.browseForFolderURL("保存文件的路径")
var folderPath = fl.getDocumentDOM().pathURI
fl.trace(folderPath)

folderPath = folderPath.substring(0, folderPath.lastIndexOf("/"))
fl.trace(folderPath)

folderPath = folderPath + '/LIBRARY'
fl.trace(folderPath)

fl.trace('start');

var lib = fl.getDocumentDOM().library;

var items = lib.items;

fl.getDocumentDOM().getTimeline().setSelectedLayers(0); 
var fcNum = fl.getDocumentDOM().getTimeline().layers[0].frameCount; 
fl.getDocumentDOM().getTimeline().insertBlankKeyframe(fcNum);



for(var i =0; i < items.length; i++)
{
	if(items[i].itemType == "movie clip" || items[i].itemType == "graphic")
	{
		fl.getDocumentDOM().getTimeline().setSelectedLayers(0); 
		var item = items[i];
		var itemName = item.name;
		fl.trace('itemName ' + itemName);
		lib.selectItem(itemName);
	 　 lib.addItemToDocument({x:0, y:0});
　　	fl.getDocumentDOM().selectAll(); //选中场景中位图
		fl.getDocumentDOM().enterEditMode();
		var timeline = fl.getDocumentDOM().getTimeline();
		
		var firstSelect = true
		if(timeline.layerCount > 9)
		{
			for(var layerId =0; layerId < timeline.layerCount; layerId++)
			{
				var layer = timeline.layers[layerId];
				var needChange = false;
				//timeline.setSelectedLayers(layerId)
				for(var frameId = 0; frameId < layer.frames.length; ++frameId)
				{
					var frame = layer.frames[frameId];
					if (frameId==frame.startFrame) {
						
						//fl.trace(' layer:'+layer.name+'  frameId:' + frameId + 'frame duration:'+frame.duration)
						//fl.trace('motionTweenOrientToPath:' + frame.motionTweenOrientToPath)
						//fl.trace('motionTweenRotate:' + frame.motionTweenRotate)
						//fl.trace('motionTweenRotateTimes:' + frame.motionTweenRotateTimes)
						//fl.trace('motionTweenScale:' + frame.motionTweenScale)
						//fl.trace('motionTweenSnap:' + frame.motionTweenSnap)
						//fl.trace('motionTweenSync:' + frame.motionTweenSync)
						//fl.trace('tweenEasing:' + frame.tweenEasing)
						//fl.trace('tweenInstanceName:' + frame.tweenInstanceName)
						//fl.trace('tweenType:' + frame.tweenType)
						
						
						if(frame.motionTweenSnap && frame.tweenType == 'motion')
						{
							//fl.trace(' layer:'+layer.name+'  frameId:' + frameId + 'frame duration:'+frame.duration)
							if(firstSelect)
							{
								timeline.setSelectedFrames([layerId, frameId, frameId + frame.duration], true);
								firstSelect = false
							}
							else
							{
								timeline.setSelectedFrames([layerId, frameId, frameId + frame.duration], false);
							}
							//var selectFrames = timeline.getSelectedFrames();
							//fl.trace(selectFrames);
							//fl.trace('---------is motion---------');
						}
						
					}
					
				}
				//break;
				
			}
			
			//timeline.selectAllFrames()
			timeline.convertToKeyframes()
			firstSelect = true
			//break;
		}
		fl.getDocumentDOM().exitEditMode();
		fl.getDocumentDOM().selectAll(); 
		fl.getDocumentDOM().deleteSelection()
	}
}
fl.getDocumentDOM().getTimeline().removeFrames(fcNum);
fl.trace('export finished')
